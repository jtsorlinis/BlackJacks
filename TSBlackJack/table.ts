import type { Int32 } from "@scriptc/types";
import CardPile from "./cardpile.ts";
import Dealer from "./dealer.ts";
import Player from "./player.ts";
import {
  array2dToMap,
  stratHard,
  stratSoft,
  stratSplit,
  getAction,
} from "./strategies.ts";

// a constant (as in the Rust port) rather than a Table field, so the
// verbose-only code compiles away
export const Verbose = false;

export default class Table {
  mBetSize: Int32;
  mPlayers: Player[];
  mNumOfDecks: Int32;
  mCardPile: CardPile;
  mMinCards: Int32;
  mDealer: Dealer;
  mCurrentPlayer: Int32;
  mCasinoEarnings: number;
  mRunningCount: Int32;
  mTrueCount: Int32;
  mStratHard: string[];
  mStratSoft: string[];
  mStratSplit: string[];

  constructor(
    numplayers: number,
    numdecks: Int32,
    betsize: Int32,
    mincards: Int32
  ) {
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

  dealRound(): void {
    let runningCount = this.mRunningCount;
    const cards = this.mCardPile.mCards;
    for (const player of this.mPlayers) {
      const card = cards.pop()!;
      player.mHand.push(card);
      runningCount += card.mCount;
    }
    this.mRunningCount = runningCount | 0;
    this.mCurrentPlayer = 0;
  }

  evaluateAll(): void {
    for (const player of this.mPlayers) {
      player.evaluate();
    }
  }

  deal(): void {
    const card = this.mCardPile.mCards.pop()!;
    this.mPlayers[this.mCurrentPlayer].mHand.push(card);
    this.mRunningCount = (this.mRunningCount + card.mCount) | 0;
  }

  preDeal(): void {
    for (const player of this.mPlayers) {
      this.selectBet(player);
    }
  }

  selectBet(player: Player): void {
    if (this.mTrueCount >= 2) {
      player.mInitialBet = (this.mBetSize * (this.mTrueCount - 1)) | 0;
    }
  }

  dealDealer(facedown: boolean = false): void {
    const card = this.mCardPile.mCards.pop()!;
    card.mFaceDown = facedown;
    this.mDealer.mHand.push(card);
    if (!facedown) {
      this.mRunningCount = (this.mRunningCount + card.mCount) | 0;
    }
  }

  startRound(): void {
    this.updateCount();
    if (Verbose) {
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
      if (Verbose) {
        this.print();
      }
      this.autoPlay();
    }
  }

  getNewCards(): void {
    if (this.mCardPile.mCards.length < this.mMinCards) {
      this.mCardPile.refresh();
      this.mCardPile.shuffle();
      this.mTrueCount = 0;
      this.mRunningCount = 0;
      if (Verbose) {
        console.log(
          `Got ${this.mNumOfDecks} new decks as number of cards left is below ${this.mMinCards}`
        );
      }
    }
  }

  clear(): void {
    for (let i = this.mPlayers.length - 1; i >= 0; i--) {
      const player = this.mPlayers[i];
      if (player.mIsSplitHand) {
        this.mPlayers[i - 1].mEarnings += player.mEarnings;
        this.mPlayers.splice(i, 1);
      } else {
        player.resetHand();
      }
    }
    this.mDealer.resetHand();
    this.mCurrentPlayer = 0;
  }

  updateCount(): void {
    if (this.mCardPile.mCards.length > 51) {
      // | 0 truncates toward zero like the other ports' integer division;
      // Math.trunc can return -0, which kept Bun from treating it as an int
      this.mTrueCount =
        (this.mRunningCount / Math.floor(this.mCardPile.mCards.length / 52)) |
        0;
    }
  }

  hit(): void {
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    if (Verbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} hits`
      );
    }
  }

  stand(): void {
    if (Verbose && this.mPlayers[this.mCurrentPlayer].mValue <= 21) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} stands`
      );
      this.print();
    }
    this.mPlayers[this.mCurrentPlayer].mIsDone = true;
  }

  split(): void {
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer]);
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer);
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.mPlayers[this.mCurrentPlayer + 1].evaluate();
    if (Verbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits`
      );
    }
  }

  splitAces(): void {
    if (Verbose) {
      console.log(
        `Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits aces`
      );
    }
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer]);
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer);
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.stand();
    this.mCurrentPlayer = (this.mCurrentPlayer + 1) | 0;
    this.deal();
    this.mPlayers[this.mCurrentPlayer].evaluate();
    this.stand();
    if (Verbose) {
      this.print();
    }
  }

  doubleBet(): void {
    const player = this.mPlayers[this.mCurrentPlayer];
    if (player.mBetMult < 1.1 && player.mHand.length === 2) {
      player.doubleBet();
      if (Verbose) {
        console.log(`Player ${player.mPlayerNum} doubles`);
      }
      this.hit();
      this.stand();
    } else if (player.mIsSoft && player.mValue === 18) {
      this.stand();
    } else {
      this.hit();
    }
  }

  autoPlay(): void {
    const dealerUp = this.mDealer.upCard();
    while (this.mCurrentPlayer < this.mPlayers.length) {
      while (!this.mPlayers[this.mCurrentPlayer].mIsDone) {
        const player = this.mPlayers[this.mCurrentPlayer];
        if (player.mHand.length === 1) {
          if (Verbose) {
            console.log(
              `Player ${player.mPlayerNum} gets 2nd card after splitting`
            );
          }
          this.deal();
          player.evaluate();
        }
        if (player.mValue < 21) {
          const splitCardVal = player.canSplit();
          if (splitCardVal === 11) {
            this.splitAces();
          } else if (
            splitCardVal !== 0 &&
            splitCardVal !== 5 &&
            splitCardVal !== 10
          ) {
            this.action(getAction(splitCardVal, dealerUp, this.mStratSplit));
          } else if (player.mIsSoft) {
            this.action(getAction(player.mValue, dealerUp, this.mStratSoft));
          } else {
            this.action(getAction(player.mValue, dealerUp, this.mStratHard));
          }
        } else {
          this.stand();
        }
      }
      this.mCurrentPlayer = (this.mCurrentPlayer + 1) | 0;
    }
    this.mCurrentPlayer = 0;
    this.dealerPlay();
  }

  action(action: string): void {
    if (action === "H") {
      this.hit();
    } else if (action === "S") {
      this.stand();
    } else if (action === "D") {
      this.doubleBet();
    } else if (action === "P") {
      this.split();
    } else {
      // scriptc can't lower process.exit yet, so throw instead
      throw new Error(`No action found. Action was: ${action}`);
    }
  }

  dealerPlay(): void {
    let allSettled = true;
    for (let i = 0; i < this.mPlayers.length; i++) {
      const player = this.mPlayers[i];
      if (player.mValue < 22 && !player.mHasNatural) {
        allSettled = false;
        break;
      }
    }
    this.mDealer.mHand[1].mFaceDown = false;
    this.mRunningCount =
        (this.mRunningCount + this.mDealer.mHand[1].mCount) | 0;
    if (Verbose) {
      console.log("Dealer's turn");
      this.print();
    }
    if (allSettled) {
      if (Verbose) {
        console.log("Dealer does not draw because all player hands are settled");
      }
      this.finishRound();
    } else {
      while (this.mDealer.mValue < 17) {
        this.dealDealer();
        this.mDealer.evaluate();
        if (Verbose) {
          console.log("Dealer hits");
          this.print();
        }
      }
      this.finishRound();
    }
  }

  checkPlayerNatural(): void {
    for (let i = 0; i < this.mPlayers.length; i++) {
      const player = this.mPlayers[i];
      if (
        player.mValue === 21 &&
        player.mHand.length === 2 &&
        player.mSplitCount.count === 0
      ) {
        player.mHasNatural = true;
      }
    }
  }

  checkDealerNatural(): boolean {
    this.mDealer.evaluate();
    if (this.mDealer.mValue === 21) {
      this.mDealer.mHand[1].mFaceDown = false;
      this.mRunningCount =
        (this.mRunningCount + this.mDealer.mHand[1].mCount) | 0;
      if (Verbose) {
        this.print();
        console.log("Dealer has a natural 21");
      }
      return true;
    }
    return false;
  }

  checkEarnings(): void {
    let check = 0;
    for (const player of this.mPlayers) {
      check += player.mEarnings;
    }
    if (check + this.mCasinoEarnings !== 0) {
      throw new Error("Earning's dont match");
    }
  }

  finishRound(): void {
    if (Verbose) {
      console.log("Scoring round");
    }
    let casinoEarnings = this.mCasinoEarnings;
    for (let i = 0; i < this.mPlayers.length; i++) {
      const player = this.mPlayers[i];
      let mult: number;
      if (player.mHasNatural) {
        if (Verbose) {
          console.log(
            `Player ${player.mPlayerNum} Wins ${Math.trunc(
              1.5 * player.mBetMult * player.mInitialBet
            )} with a natural 21`
          );
        }
        mult = 1.5;
      } else if (player.mValue > 21) {
        if (Verbose) {
          console.log(
            `Player ${player.mPlayerNum} Busts and Loses ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
        mult = -1;
      } else if (
        this.mDealer.mValue > 21 ||
        player.mValue > this.mDealer.mValue
      ) {
        if (Verbose) {
          console.log(
            `Player ${player.mPlayerNum} Wins ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
        mult = 1;
      } else if (player.mValue === this.mDealer.mValue) {
        if (Verbose) {
          console.log(`Player ${player.mPlayerNum} Draws`);
        }
        mult = 0;
      } else {
        if (Verbose) {
          console.log(
            `Player ${player.mPlayerNum} Loses ${Math.trunc(
              player.mBetMult * player.mInitialBet
            )}`
          );
        }
        mult = -1;
      }
      casinoEarnings += player.settle(mult);
    }
    this.mCasinoEarnings = casinoEarnings;
    if (Verbose) {
      for (let i = 0; i < this.mPlayers.length; i++) {
        if (!this.mPlayers[i].mIsSplitHand) {
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

  print(): void {
    for (let i = 0; i < this.mPlayers.length; i++) {
      console.log(this.mPlayers[i].print());
    }
    console.log(`${this.mDealer.print()}\n`);
  }
}
