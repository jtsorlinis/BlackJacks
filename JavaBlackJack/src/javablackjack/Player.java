package javablackjack;

import java.util.ArrayList;
import java.util.List;

class Player {
    static int playerNumCount = 0;
    int maxsplits = 10;

    String mPlayerNum;
    List<Card> mHand = new ArrayList<>(5);
    int mValue = 0;
    float mEarnings = 0;
    int mAces = 0;
    boolean mIsSoft = false;
    int mSplitCount = 0;
    boolean mIsDone = false;
    Player mSplitFrom = null;
    float mBetMult = 1;
    boolean mHasNatural = false;
    Table mTable;
    int mInitialBet;

    Player() {
        this(null);
    }

    Player(Table table) {
        this(table, null);
    }

    Player(Table table, Player split) {
        mTable = table;
        if (table != null) {
            mInitialBet = mTable.mBetSize;
            if (split != null) {
                mHand.add(split.mHand.get(1));
                mSplitCount++;
                mPlayerNum = split.mPlayerNum + "S";
                mInitialBet = split.mInitialBet;
                mSplitFrom = split;
            } else {
                playerNumCount++;
                mPlayerNum = String.valueOf(playerNumCount);
            }
        }
    }

    void doubleBet() {
        mBetMult = 2;
    }

    void resetHand() {
        mHand.clear();
        mValue = 0;
        mAces = 0;
        mIsSoft = false;
        mSplitCount = 0;
        mIsDone = false;
        mBetMult = 1;
        mHasNatural = false;
        mInitialBet = mTable.mBetSize;
    }

    int canSplit() {
        if (mHand.size() == 2 && mHand.get(0).mRank == mHand.get(1).mRank && mSplitCount < maxsplits) {
            return mHand.get(0).mValue;
        } else {
            return 0;
        }
    }

    void win() {
        win(1);
    }

    void win(float mult) {
        float x = mInitialBet * mBetMult * mult;
        mEarnings += x;
        mTable.mCasinoEarnings -= x;
    }

    void lose() {
        float x = mInitialBet * mBetMult;
        mEarnings -= x;
        mTable.mCasinoEarnings += x;
    }

    String print() {
        String output = "Player " + mPlayerNum + ": ";
        for (Card i : mHand) {
            output += i.print() + " ";
        }
        for (int i = mHand.size(); i < 5; i++) {
            output += "  ";
        }
        output += "\tScore: " + String.valueOf(mValue);
        if (mValue > 21) {
            output += " (Bust) ";
        } else {
            output += "        ";
        }
        if (mPlayerNum != "D") {
            output += "\tBet: " + String.valueOf(mInitialBet * mBetMult);
        }
        return output;
    }

    int evaluate() {
        mAces = 0;
        mValue = 0;
        for (Card card : mHand) {
            mValue += card.mValue;
            // check for ace
            if (card.mIsAce) {
                mAces++;
                mIsSoft = true;
            }
        }

        while (mValue > 21 && mAces > 0) {
            mValue -= 10;
            mAces--;
        }

        if (mAces == 0) {
            mIsSoft = false;
        }

        return mValue;
    }

}