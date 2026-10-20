import { exit } from "process";
import CardPile from "./cardpile.js";
import Dealer from "./dealer.js";
import Player from "./player.js";
import {
  array2dToMap,
  stratHard,
  stratSoft,
  stratSplit,
  getAction,
} from "./strategies.js";

export default class Table {
  constructor(numplayers, numdecks, betsize, mincards, verbose) {
    this.mVerbose = verbose;
    this.mBetSize = betsize;
    this.mPlayers = [];
    this.mNumOfDecks = numdecks;
    this.mCardPile = new CardPile(numdecks);
    this.mMinCards = mincards;
    this.mDealer = new Dealer();
    this.mCurrentPlayer = 0;
    this.mCasinoEarnings = 0;
    this.mRunningCount = 0;
    this.mTrueCount = 0;

    this.mStratHard = array2dToMap(stratHard);
    this.mStratSoft = array2dToMap(stratSoft);
    this.mStratSplit = array2dToMap(stratSplit);

    for (let i = 0; i < numplayers; i++) {
      this.mPlayers.push(new Player(this));
    }
  }

  dealRound() {
    let runningCount = this.mRunningCount;
    for (let i = 0; i < this.mPlayers.length; i++) {
      const card = this.mCardPile.mCards.pop();
      this.mPlayers[i].mHand.push(card);
      runningCount += card.mCount;
    }
    this.mRunningCount = runningCount;
    this.mCurrentPlayer = 0;
  }

  evaluateAll() {
    for (let i = 0; i < this.mPlayers.length; i++) {
      this.mPlayers[i].evaluate();
    }
  }

  deal() {
    const card = this.mCardPile.mCards.pop();
    this.mPlayers[this.mCurrentPlayer].mHand.push(card);
    this.mRunningCount += card.mCount;
  }

  preDeal() {
    for (let i = 0; i < this.mPlayers.length; i++) {
      this.selectBet(this.mPlayers[i]);
    }
  }

  selectBet(player) {
    if (this.mTrueCount >= 2) {
      player.mInitialBet = this.mBetSize * (this.mTrueCount - 1);
    }
  }

  dealDealer(facedown = false) {
    const card = this.mCardPile.mCards.pop();
    card.mFaceDown = facedown;
    this.mDealer.mHand.push(card);
    if (!facedown) {
      this.mRunningCount += card.mCount;
    }
  }

  startRound() {
    this.updateCount();
    if (this.mVerbose) {
      console.log(`${this.mCardPile.mCards.length} cards left`);
      console.log(
        `Running count is: ${this.mRunningCount}\tTrue count is: ${this.mTrueCount}`
      );
    }
    this.getNewCards();
    this.preDeal();
    this.dealRound();
    this.dealDealer();
    this.dealRound();
    this.dealDealer(true);
    this.evaluateAll();
    this.mCurrentPlayer = 0;
    if (this.checkDealerNatural()) {
      this.finishRound();
    } else {
      this.checkPlayerNatural();
      if (this.mVerbose) {
        this.print();
      }
      this.autoPlay();
    }
  }

  getNewCards() {
    if (this.mCardPile.mCards.length < this.mMinCards) {
      this.mCardPile.refresh();
      this.mCardPile.shuffle();
      this.mTrueCount = 0;
      this.mRunningCount = 0;
      if (this.mVerbose) {
        console.log(
          `Got ${this.mNumOfDecks} new decks as number of cards left is below ${this.mMinCards}`
        );
      }
    }
  }

  clear() {
    for (let i = this.mPlayers.length - 1; i >= 0; i--) {
      if (this.mPlayers[i].mSplitFrom != null) {
        this.mPlayers[i - 1].mEarnings += this.mPlayers[i].mEarnings;
        this.mPlayers.splice(i, 1);
      } else {
        this.mPlayers[i].resetHand();
      }
    }
    this.mDealer.resetHand();
    this.mCurrentPlayer = 0;
  }

  updateCount() {
    if (this.mCardPile.mCards.length > 51) {
      // | 0 truncates toward zero like the other ports' integer division;
      // Math.trunc can return -0, which kept Bun from treating it as an int
      this.mTrueCount =
        (this.mRunningCount / Math.floor(this.mCardPile.mCards.length / 52)) |
        0;
    }
  }

  hit() {
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    if (this.mVerbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} hits`
      );
    }
  }

  stand() {
    if (this.mVerbose && this.mPlayers[this.mCurrentPlayer].mValue <= 21) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} stands`
      );
      this.print();
    }
    this.mPlayers[this.mCurrentPlayer].mIsDone = true;
  }

  split() {
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer]);
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer);
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.mPlayers[this.mCurrentPlayer + 1].evaluate();
    if (this.mVerbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits`
      );
    }
  }

  splitAces() {
    if (this.mVerbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits aces`
      );
    }
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer]);
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer);
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.stand();
    this.mCurrentPlayer++;
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.stand();
    if (this.mVerbose) {
      this.print();
    }
  }

  doubleBet() {
    if (
      this.mPlayers[this.mCurrentPlayer].mBetMult < 1.1 &&
      this.mPlayers[this.mCurrentPlayer].mHand.length === 2
    ) {
      this.mPlayers[this.mCurrentPlayer].doubleBet();
      if (this.mVerbose) {
        console.log(
          `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} doubles`
        );
      }
      this.hit();
      this.stand();
    } else if (
      this.mPlayers[this.mCurrentPlayer].mIsSoft &&
      this.mPlayers[this.mCurrentPlayer].mValue === 18
    ) {
      this.stand();
    } else {
      this.hit();
    }
  }

  autoPlay() {
    const dealerUp = this.mDealer.upCard();
    while (this.mCurrentPlayer < this.mPlayers.length) {
      while (!this.mPlayers[this.mCurrentPlayer].mIsDone) {
        if (this.mPlayers[this.mCurrentPlayer].mHand.length === 1) {
          if (this.mVerbose) {
            console.log(
              `Player ${
                this.mPlayers[this.mCurrentPlayer].mPlayerNum
              } gets 2nd card after splitting`
            );
          }
          this.deal();
          this.mPlayers[this.mCurrentPlayer].evaluate();
        }
        if (this.mPlayers[this.mCurrentPlayer].mValue < 21) {
          const splitCardVal = this.mPlayers[this.mCurrentPlayer].canSplit();
          if (splitCardVal === 11) {
            this.splitAces();
          } else if (
            splitCardVal !== 0 &&
            splitCardVal !== 5 &&
            splitCardVal !== 10
          ) {
            this.action(getAction(splitCardVal, dealerUp, this.mStratSplit));
          } else if (this.mPlayers[this.mCurrentPlayer].mIsSoft) {
            this.action(
              getAction(
                this.mPlayers[this.mCurrentPlayer].mValue,
                dealerUp,
                this.mStratSoft
              )
            );
          } else {
            this.action(
              getAction(
                this.mPlayers[this.mCurrentPlayer].mValue,
                dealerUp,
                this.mStratHard
              )
            );
          }
        } else {
          this.stand();
        }
      }
      this.mCurrentPlayer++;
    }
    this.mCurrentPlayer = 0;
    this.dealerPlay();
  }

  action(action) {
    if (action === "H") {
      this.hit();
    } else if (action === "S") {
      this.stand();
    } else if (action === "D") {
      this.doubleBet();
    } else if (action === "P") {
      this.split();
    } else {
      console.log(`No action found. Action was: ${action}`);
      exit(1);
    }
  }

  dealerPlay() {
    let allSettled = true;
    for (let i = 0; i < this.mPlayers.length; i++) {
      if (this.mPlayers[i].mValue < 22 && !this.mPlayers[i].mHasNatural) {
        allSettled = false;
        break;
      }
    }
    this.mDealer.mHand[1].mFaceDown = false;
    this.mRunningCount += this.mDealer.mHand[1].mCount;
    if (this.mVerbose) {
      console.log("Dealer's turn");
      this.print();
    }
    if (allSettled) {
      if (this.mVerbose) {
        console.log("Dealer does not draw because all player hands are settled");
      }
      this.finishRound();
    } else {
      while (this.mDealer.mValue < 17) {
        this.dealDealer();
        this.mDealer.evaluate();
        if (this.mVerbose) {
          console.log("Dealer hits");
          this.print();
        }
      }
      this.finishRound();
    }
  }

  checkPlayerNatural() {
    for (let i = 0; i < this.mPlayers.length; i++) {
      if (
        this.mPlayers[i].mValue === 21 &&
        this.mPlayers[i].mHand.length === 2 &&
        this.mPlayers[i].mSplitCount.count === 0
      ) {
        this.mPlayers[i].mHasNatural = true;
      }
    }
  }

  checkDealerNatural() {
    this.mDealer.evaluate();
    if (this.mDealer.mValue === 21) {
      this.mDealer.mHand[1].mFaceDown = false;
      this.mRunningCount += this.mDealer.mHand[1].mCount;
      if (this.mVerbose) {
        this.print();
        console.log("Dealer has a natural 21");
      }
      return true;
    }
    return false;
  }

  checkEarnings() {
    let check = 0;
    for (let i = 0; i < this.mPlayers.length; i++) {
      check += this.mPlayers[i].mEarnings;
    }
    if (check + this.mCasinoEarnings !== 0) {
      console.log("Earning's dont match");
      exit(1);
    }
  }

  finishRound() {
    if (this.mVerbose) {
      console.log("Scoring round");
    }
    let casinoEarnings = this.mCasinoEarnings;
    for (let i = 0; i < this.mPlayers.length; i++) {
      const player = this.mPlayers[i];
      if (player.mHasNatural) {
        casinoEarnings += player.win(1.5);
        if (this.mVerbose) {
          console.log(
            `Player ${player.mPlayerNum} Wins ${Math.trunc(
              1.5 * player.mBetMult * player.mInitialBet
            )} with a natural 21`
          );
        }
      } else if (player.mValue > 21) {
        casinoEarnings += player.lose();
        if (this.mVerbose) {
          console.log(
            `Player ${player.mPlayerNum} Busts and Loses ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
      } else if (
        this.mDealer.mValue > 21 ||
        player.mValue > this.mDealer.mValue
      ) {
        casinoEarnings += player.win();
        if (this.mVerbose) {
          console.log(
            `Player ${player.mPlayerNum} Wins ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
      } else if (player.mValue === this.mDealer.mValue) {
        if (this.mVerbose) {
          console.log(`Player ${player.mPlayerNum} Draws`);
        }
      } else {
        casinoEarnings += player.lose();
        if (this.mVerbose) {
          console.log(
            `Player ${player.mPlayerNum} Loses ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
      }
    }
    this.mCasinoEarnings = casinoEarnings;
    if (this.mVerbose) {
      for (let i = 0; i < this.mPlayers.length; i++) {
        if (this.mPlayers[i].mSplitFrom == null) {
          console.log(
            `Player ${this.mPlayers[i].mPlayerNum} Earnings: ${Math.trunc(
              this.mPlayers[i].mEarnings
            )}`
          );
        }
      }
      console.log();
    }
    this.clear();
  }

  print() {
    for (let i = 0; i < this.mPlayers.length; i++) {
      console.log(this.mPlayers[i].print());
    }
    console.log(`${this.mDealer.print()}\n`);
  }
}
