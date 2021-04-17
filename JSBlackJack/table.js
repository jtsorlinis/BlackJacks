const process = require('process')
const CardPile = require('./cardpile')
const Dealer = require('./dealer')
const Player = require('./player')
const Strategies = require('./strategies')

module.exports = class Table {
  constructor (numplayers, numdecks, betsize, mincards, verbose) {
    this.mVerbose = verbose
    this.mBetSize = betsize
    this.mPlayers = []
    this.mNumOfDecks = numdecks
    this.mCardPile = new CardPile(numdecks)
    this.mMinCards = mincards
    this.mDealer = new Dealer()
    this.mCurrentPlayer = 0
    this.mCasinoEarnings = 0
    this.mRunningCount = 0
    this.mTrueCount = 0

    this.mStratHard = Strategies.array2dToMap(Strategies.stratHard)
    this.mStratSoft = Strategies.array2dToMap(Strategies.stratSoft)
    this.mStratSplit = Strategies.array2dToMap(Strategies.stratSplit)

    for (let i = 0; i < numplayers; i++) {
      this.mPlayers.push(new Player(this))
    }
  }

  dealRound () {
    for (let i = 0; i < this.mPlayers.length; i++) {
      this.deal()
      this.mCurrentPlayer++
    }
    this.mCurrentPlayer = 0
  }

  evaluateAll () {
    for (let i = 0; i < this.mPlayers.length; i++) {
      this.mPlayers[i].evaluate()
    }
  }

  deal () {
    const card = this.mCardPile.mCards.pop()
    this.mPlayers[this.mCurrentPlayer].mHand.push(card)
    this.mRunningCount += card.mCount
  }

  preDeal () {
    for (let i = 0; i < this.mPlayers.length; i++) {
      this.selectBet(this.mPlayers[i])
    }
  }

  selectBet (player) {
    if (this.mTrueCount >= 2) {
      player.mInitialBet = this.mBetSize * Math.floor(this.mTrueCount - 1)
    }
  }

  dealDealer (facedown = false) {
    const card = this.mCardPile.mCards.pop()
    card.mFaceDown = facedown
    this.mDealer.mHand.push(card)
    if (!facedown) {
      this.mRunningCount += card.mCount
    }
  }

  startRound () {
    this.clear()
    this.updateCount()
    if (this.mVerbose) {
      console.log(`${this.mCardPile.mCards.length} cards left`)
      console.log(`Running count is: ${this.mRunningCount}\tTrue count is: ${this.mTrueCount}`)
    }
    this.getNewCards()
    this.preDeal()
    this.dealRound()
    this.dealDealer()
    this.dealRound()
    this.dealDealer(true)
    this.evaluateAll()
    this.mCurrentPlayer = 0
    if (this.checkDealerNatural()) {
      this.finishRound()
    } else {
      this.checkPlayerNatural()
      if (this.mVerbose) {
        this.print()
      }
      this.autoPlay()
    }
  }

  getNewCards () {
    if (this.mCardPile.mCards.length < this.mMinCards) {
      this.mCardPile.refresh()
      this.mCardPile.shuffle()
      this.mTrueCount = 0
      this.mRunningCount = 0
      if (this.mVerbose) {
        console.log(`Got ${this.mNumOfDecks} new decks as number of cards left is below ${this.mMinCards}`)
      }
    }
  }

  clear () {
    for (let i = this.mPlayers.length - 1; i >= 0; i--) {
      if (this.mPlayers[i].mSplitFrom != null) {
        this.mPlayers[i - 1].mEarnings += this.mPlayers[i].mEarnings
        this.mPlayers.splice(i, 1)
      } else {
        this.mPlayers[i].resetHand()
      }
    }
    this.mDealer.resetHand()
    this.mCurrentPlayer = 0
  }

  updateCount () {
    if (this.mCardPile.mCards.length > 51) {
      this.mTrueCount = this.mRunningCount / Math.floor((this.mCardPile.mCards.length / 52))
    }
  }

  hit () {
    this.deal()
    this.mPlayers[this.mCurrentPlayer].evaluate()
    if (this.mVerbose) {
      console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} hits`)
    }
  }

  stand () {
    if (this.mVerbose && this.mPlayers[this.mCurrentPlayer].mValue <= 21) {
      console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} stands`)
      this.print()
    }
    this.mPlayers[this.mCurrentPlayer].mIsDone = true
  }

  split () {
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer])
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer)
    this.mPlayers[this.mCurrentPlayer].evaluate()
    this.mPlayers[this.mCurrentPlayer + 1].evaluate()
    if (this.mVerbose) {
      console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits`)
    }
  }

  splitAces () {
    if (this.mVerbose) {
      console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} splits Aces`)
    }
    const splitPlayer = new Player(this, this.mPlayers[this.mCurrentPlayer])
    this.mPlayers.splice(this.mCurrentPlayer + 1, 0, splitPlayer)
    this.deal()
    this.mPlayers[this.mCurrentPlayer].evaluate()
    this.stand()
    this.mCurrentPlayer++
    this.deal()
    this.mPlayers[this.mCurrentPlayer].evaluate()
    this.stand()
    if (this.mVerbose) {
      this.print()
    }
  }

  doubleBet () {
    if (this.mPlayers[this.mCurrentPlayer].mBetMult === 1 &&
      this.mPlayers[this.mCurrentPlayer].mHand.length === 2) {
      this.mPlayers[this.mCurrentPlayer].doubleBet()
      if (this.mVerbose) {
        console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} doubles`)
      }
      this.hit()
      this.stand()
    } else {
      this.hit()
    }
  }

  autoPlay () {
    while (!this.mPlayers[this.mCurrentPlayer].mIsDone) {
      if (this.mPlayers[this.mCurrentPlayer].mHand.length === 1) {
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[this.mCurrentPlayer].mPlayerNum} gets 2nd card after splitting`)
        }
        this.deal()
        this.mPlayers[this.mCurrentPlayer].evaluate()
      }
      if (this.mPlayers[this.mCurrentPlayer].mHand.length < 5 &&
        this.mPlayers[this.mCurrentPlayer].mValue < 21) {
        const splitCardVal = this.mPlayers[this.mCurrentPlayer].canSplit()
        if (splitCardVal === 11) {
          this.splitAces()
        } else if (splitCardVal !== 0 && (splitCardVal !== 5 && splitCardVal !== 10)) {
          this.action(Strategies.getAction(splitCardVal, this.mDealer.upCard(), this.mStratSplit))
        } else if (this.mPlayers[this.mCurrentPlayer].mIsSoft) {
          this.action(Strategies.getAction(this.mPlayers[this.mCurrentPlayer].mValue,
            this.mDealer.upCard(), this.mStratSoft))
        } else {
          this.action(Strategies.getAction(this.mPlayers[this.mCurrentPlayer].mValue,
            this.mDealer.upCard(), this.mStratHard))
        }
      } else {
        this.stand()
      }
    }
    this.nextPlayer()
  }

  action (action) {
    if (action === 'H') {
      this.hit()
    } else if (action === 'S') {
      this.stand()
    } else if (action === 'D') {
      this.doubleBet()
    } else if (action === 'P') {
      this.split()
    } else {
      console.log(`No action found. Action was: ${action}`)
      process.exit(1)
    }
  }

  dealerPlay () {
    let allBusted = true
    for (let i = 0; i < this.mPlayers.length; i++) {
      if (this.mPlayers[i].mValue < 22) {
        allBusted = false
        break
      }
    }
    this.mDealer.mHand[1].mFaceDown = false
    this.mRunningCount += this.mDealer.mHand[1].mCount
    this.mDealer.evaluate()
    if (this.mVerbose) {
      console.log('Dealer\'s turn')
      this.print()
    }
    if (allBusted) {
      if (this.mVerbose) {
        console.log('Dealer automatically wins cause all players busted')
      }
      this.finishRound()
    } else {
      while (this.mDealer.mValue < 17 && this.mDealer.mHand.length < 5) {
        this.dealDealer()
        this.mDealer.evaluate()
        if (this.mVerbose) {
          console.log('Dealer hits')
          this.print()
        }
      }
      this.finishRound()
    }
  }

  nextPlayer () {
    if (++this.mCurrentPlayer < this.mPlayers.length) {
      this.autoPlay()
    } else {
      this.dealerPlay()
    }
  }

  checkPlayerNatural () {
    for (let i = 0; i < this.mPlayers.length; i++) {
      if (this.mPlayers[i].mValue === 21 && this.mPlayers[i].mHand.length === 2 &&
        this.mPlayers[i].mSplitFrom == null) {
        this.mPlayers[i].mHasNatural = true
      }
    }
  }

  checkDealerNatural () {
    this.mDealer.evaluate()
    if (this.mDealer.mValue === 21) {
      this.mDealer.mHand[1].mFaceDown = false
      this.mRunningCount += this.mDealer.mHand[1].mCount
      if (this.mVerbose) {
        this.print()
        console.log('Dealer has natural 21')
      }
      return true
    }
    return false
  }

  checkEarnings () {
    let check = 0
    for (let i = 0; i < this.mPlayers.length; i++) {
      check += this.mPlayers[i].mEarnings
    }
    if (check * -1 !== this.mCasinoEarnings) {
      console.log('Earning\'s dont match')
      process.exit(1)
    }
  }

  finishRound () {
    if (this.mVerbose) {
      console.log('Scoring round')
    }
    for (let i = 0; i < this.mPlayers.length; i++) {
      if (this.mPlayers[i].mHasNatural) {
        this.mPlayers[i].win(1.5)
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Wins with a natural 21`)
        }
      } else if (this.mPlayers[i].mValue > 21) {
        this.mPlayers[i].lose()
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Busts and Loses`)
        }
      } else if (this.mDealer.mValue > 21) {
        this.mPlayers[i].win()
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Wins`)
        }
      } else if (this.mPlayers[i].mValue > this.mDealer.mValue) {
        this.mPlayers[i].win()
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Wins`)
        }
      } else if (this.mPlayers[i].mValue === this.mDealer.mValue) {
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Draws`)
        }
      } else {
        this.mPlayers[i].lose()
        if (this.mVerbose) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Loses`)
        }
      }
    }
    if (this.mVerbose) {
      for (let i = 0; i < this.mPlayers.length; i++) {
        if (this.mPlayers[i].mSplitFrom == null) {
          console.log(`Player ${this.mPlayers[i].mPlayerNum} Earnings: ${this.mPlayers[i].mEarnings}`)
        }
      }
      console.log()
    }
  }

  print () {
    for (let i = 0; i < this.mPlayers.length; i++) {
      console.log(this.mPlayers[i].print())
    }
    console.log(`${this.mDealer.print()}\n`)
  }
}
