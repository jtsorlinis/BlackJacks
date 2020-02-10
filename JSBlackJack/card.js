module.exports = class Card {
    constructor(rank, suit) {
        this.mRank = rank;
        this.mSuit = suit;
        this.mFaceDown = false;
        this.mValue = this.evaluate();
        this.mCount = this.count();
        this.mIsAce = false;
        if (this.mRank == "A") {
            this.mIsAce = true;
        }
        
    }

    print() {
        if(this.mFaceDown) {
            return "X";
        } else {
            return this.mRank;
        }
    }

    evaluate() {
        if(this.mRank == "J" || this.mRank == "Q" || this.mRank == "K") {
            return 10;
        } else if (this.mRank == "A") {
            return 11;
        } else {
            return this.mRank;
        }
    }

    count() {
        if (this.mRank == "10" || this.mRank == "J" || this.mRank == "Q" || this.mRank == "K" || this.mRank == "A") {
            return -1;
        }
        else if (this.mRank == "7" || this.mRank == "8" || this.mRank == "9") {
            return 0;
        }
        else {
            return 1;
        }
    }


}