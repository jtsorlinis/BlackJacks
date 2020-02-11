const CardPile = require("./cardpile");
const Dealer = require("./dealer");
const Player = require("./player");

module.exports = class Table {
    constructor(numplayers,numdecks,betsize,mincards,verbose) {
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

        // this.mStratHard = 
        // this.mStratSoft = 
        // this.mStratSplit = 

        for(let i = 0; i < numplayers; i++) {
            this.mPlayers.push(new Player(this));
        }
    }

    dealRound() {
        for(let i = 0; i < this.mPlayers.length; i++) { 
            this.deal();
            this.mPlayers[i].evaluate();
            this.mCurrentPlayer++;
        }
        this.mCurrentPlayer = 0;
    }

    deal() {
        let card = this.mCardPile.mCards.pop();
        this.mPlayers[this.mCurrentPlayer].mHand.push(card);
        this.mRunningCount += card.mCount;
    }

    preDeal() {
        for(let i = 0; i < this.mPlayers.length; i++) { 
            this.selectBet(this.mPlayers[i]);
        }
    }

    selectBet(player) {
        if(this.mTrueCount >= 2) {
            player.mInitialBet = this.mBetSize * (this.mTrueCount-1) * 1.25
        }
    }

    dealDealer(facedown = false) {
        let card = this.mCardPile.mCards.pop();
        card.mFaceDown = facedown;
        this.mDealer.mHand.push(card);
        if(!facedown) {
            this.mRunningCount += card.mCount;
        }
    }

    startRound() {
        this.clear();
        this.updateCount();
        if(this.mVerbose) {
            console.log(this.mCardPile.mCards.length + " cards left");
            console.log("Running count is: " + this.mRunningCount + "\tTrue count is: " + parseInt(this.mTrueCount));
        }
        this.getNewCards();
        this.preDeal();
        this.dealRound();
        this.dealDealer();
        this.dealRound();
        this.dealDealer();
        this.mCurrentPlayer = 0;
        if(this.checkDealerNatural()) {
            this.finishRound();
        } else {
            this.checkPlayerNatural();
            if(this.mVerbose) {
                this.print();
            }
            this.autoPlay();
        }
    }

    getNewCards() {
        //TODO
    }

    clear() {
        //TODO
    }

    updateCount() {
        //TODO
    }

    hit() {
        //TODO
    }

    stand() {
        //TODO
    }

    split() {
        //TODO
    }

    splitAces() {
        //TODO
    }

    doubleBet() {
        //TODO
    }

    autoPlay() {
        //TODO
    }

    action(action) {
        //TODO
    }

    dealerPlay() {
        //TODO
    }

    nextPlayer() {
        //TODO
    }

    checkPlayerNatural() {
        //TODO
    }

    checkDealerNatural() {
        //TODO
    }

    checkEarnings() {
        //TODO
    }

    finishRound() {
        //TODO
    }



    print() {
        for(let i = 0; i < this.mPlayers.length; i++) {
            console.log(this.mPlayers[i].print());
        }
        console.log(this.mDealer.print() + "\n");
    }


};