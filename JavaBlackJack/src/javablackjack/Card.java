package javablackjack;

class Card {
    public String mRank;
    String mSuit;
    public boolean mFaceDown = false;
    public int mValue;
    public int mCount;
    public boolean mIsAce = false;

    public Card(String rank, String suit) {
        mRank = rank;
        mSuit = suit;
        mValue = evaluate();
        mCount = count();
        if(mRank == "A") {
            mIsAce = true;
        }
    }

    public String print() {
        if(mFaceDown) {
            return "X";
        } else {
            return mRank;
        }
    }

    int evaluate() {
        if(mRank == "J" || mRank == "Q" || mRank == "K") {
            return 10;
        } else if (mRank == "A") {
            return 11;
        } else {
            return Integer.parseInt(mRank);
        }
    }

    int count() {
        if (mRank == "10" || mRank == "J" || mRank == "Q" || mRank == "K" || mRank == "A") {
            return -1;
        }
        else if (mRank == "7" || mRank == "8" || mRank == "9") {
            return 0;
        }
        else {
            return 1;
        }
    }
}