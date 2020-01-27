package javablackjack;

class Dealer extends Player {
    boolean mHideSecond = true;

    public Dealer() {
        mPlayerNum = "D";
        mValue = 0;
    }
    
    @Override
    void resetHand() {
        mHand.clear();
        mValue = 0;
        mHideSecond = true;
    }

    int upCard() {
        return mHand.get(0).mValue;
    }
}