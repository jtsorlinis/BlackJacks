package javablackjack;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

class Table {
    int mVerbose;
    int mBetSize;
    List<Player> mPlayers = new ArrayList<Player>();
    int mNumOfDecks;
    public CardPile mCardPile;
    int mMinCards;
    Dealer mDealer = new Dealer();
    int mCurrentPlayer = 0;
    float mCasinoEarnings = 0;
    int mRunningCount = 0;
    int mTrueCount = 0;
    HashMap<Integer,String> mStratHard = (HashMap<Integer, String>) Strategies.array2dToMap(Strategies.stratHard);
    HashMap<Integer,String> mStratSoft = (HashMap<Integer, String>) Strategies.array2dToMap(Strategies.stratSoft);
    HashMap<Integer,String> mStratSplit = (HashMap<Integer, String>) Strategies.array2dToMap(Strategies.stratSplit);

    public Table(int numPlayers, int numDecks, int betsize, int minCards, int verbose) {
        mCardPile = new CardPile(numDecks);
        mVerbose = verbose;
        mBetSize = betsize;
        mNumOfDecks = numDecks;
        mMinCards = minCards;

        for(int i = 0; i < numPlayers; i++) {
            mPlayers.add(new Player(this));
        }
    }

    void dealRound() {
        for(Player player : mPlayers){
            deal();
            mCurrentPlayer++;
        }
        mCurrentPlayer = 0;
    }

    void evaluateAll() {
        for (Player player : mPlayers) {
            player.evaluate();
        }
    }

    void deal() {
        Card card = mCardPile.mCards.get(mCardPile.mCards.size() - 1);
        mPlayers.get(mCurrentPlayer).mHand.add(card);
        mRunningCount += card.mCount;
        mCardPile.mCards.remove(mCardPile.mCards.size() - 1);
    }

    void predeal() {
        for (Player player : mPlayers) {
            selectBet(player);
        }
    }

    void selectBet(Player player) {
        if(mTrueCount >=2) {
            player.mInitialBet = mBetSize * (mTrueCount - 1);
        }
    }

    void dealDealer() {
        dealDealer(false);
    }

    void dealDealer(boolean faceDown) {
        Card card = mCardPile.mCards.get(mCardPile.mCards.size() - 1);
        mCardPile.mCards.remove(mCardPile.mCards.size() - 1);
        card.mFaceDown = faceDown;
        mDealer.mHand.add(card);
        if(!faceDown) {
            mRunningCount += card.mCount;
        }
    }

    public void startRound() {
        clear();
        updateCount();
        if(mVerbose > 0) {
            System.out.println(mCardPile.mCards.size() + " cards left");
            System.out.println("Running count is: " + mRunningCount + "\tTrue count is: " + mTrueCount);
        }
        getNewCards();
        predeal();
        dealRound();
        dealDealer();
        dealRound();
        dealDealer(true);
        evaluateAll();
        mCurrentPlayer = 0;
        if(checkDealerNatural()) {
            finishRound();
        } 
        else {
            checkPlayerNatural();
            if(mVerbose > 0) {
                print();
            }
            autoPlay();
        }
    }

    void getNewCards() {
        if(mCardPile.mCards.size() < mMinCards) {
            mCardPile.refresh();
            mCardPile.shuffle();
            mTrueCount = 0;
            mRunningCount = 0;
            if(mVerbose > 0) {
                System.out.println("Got " + mNumOfDecks + " new decks as number of cards left is below " + mMinCards);
            }
        }
    }

    public void clear() {
        for(int i = mPlayers.size() -1; i >= 0; i--) {
            if (mPlayers.get(i).mSplitFrom == null) {
                mPlayers.get(i).resetHand();
            } else {
                mPlayers.remove(i);
            }
        }
        mDealer.resetHand();
        mCurrentPlayer = 0;
    }

    void updateCount() {
        if(mCardPile.mCards.size() > 51) {
            mTrueCount = mRunningCount / (mCardPile.mCards.size()/52);
        }
    }

    void hit() {
        deal();
        mPlayers.get(mCurrentPlayer).evaluate();
        if(mVerbose > 0) {
            System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " hits");
        }
    }

    void stand() {
        if(mVerbose > 0 && mPlayers.get(mCurrentPlayer).mValue <= 21) {
            System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " stands");
            print();
        }
        mPlayers.get(mCurrentPlayer).mIsDone = true;
    }

    void split() {
        Player splitPlayer = new Player(this, mPlayers.get(mCurrentPlayer));
        mPlayers.get(mCurrentPlayer).mHand.remove(mPlayers.get(mCurrentPlayer).mHand.size()-1);
        mPlayers.add(mCurrentPlayer+1, splitPlayer);
        mPlayers.get(mCurrentPlayer).evaluate();
        mPlayers.get(mCurrentPlayer+1).evaluate();
        if( mVerbose > 0) {
            System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " splits");
        }
    }

    void splitAces() {
        if(mVerbose > 0) {
            System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " splits Aces");
        }
        Player splitPlayer = new Player(this, mPlayers.get(mCurrentPlayer));
        mPlayers.get(mCurrentPlayer).mHand.remove(mPlayers.get(mCurrentPlayer).mHand.size()-1);
        mPlayers.add(mCurrentPlayer+1, splitPlayer);
        deal();
        mPlayers.get(mCurrentPlayer).evaluate();
        stand();
        mCurrentPlayer++;
        deal();
        mPlayers.get(mCurrentPlayer).evaluate();
        stand();
        if(mVerbose > 0) {
            print();
        }

    }

    void doubleBet() {
        if(mPlayers.get(mCurrentPlayer).mBetMult == 1 && mPlayers.get(mCurrentPlayer).mHand.size() == 2) {
            mPlayers.get(mCurrentPlayer).doubleBet();
            if(mVerbose > 0) {
                System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " doubles");
            }
            hit();
            stand();
        }
        else {
            hit();
        }

    }

    void autoPlay() {
        while (!mPlayers.get(mCurrentPlayer).mIsDone) {
            // check if player just split
            if (mPlayers.get(mCurrentPlayer).mHand.size() == 1) {
                if (mVerbose > 0) {
                    System.out.println("Player " + mPlayers.get(mCurrentPlayer).mPlayerNum + " gets 2nd card after splitting");
                }
                deal();
                mPlayers.get(mCurrentPlayer).evaluate();
            }

            if (mPlayers.get(mCurrentPlayer).mHand.size() < 5 && mPlayers.get(mCurrentPlayer).mValue < 21) {
                int splitPlayerVal = mPlayers.get(mCurrentPlayer).canSplit();
                if (splitPlayerVal == 11) {
                    splitAces();
                }
                else if (splitPlayerVal != 0 && (splitPlayerVal != 5 && splitPlayerVal != 10)) {
                    action(Strategies.getAction(splitPlayerVal, mDealer.upCard(), mStratSplit));
                }
                else if (mPlayers.get(mCurrentPlayer).mIsSoft) {
                    action(Strategies.getAction(mPlayers.get(mCurrentPlayer).mValue, mDealer.upCard(), mStratSoft));
                }
                else {
                    action(Strategies.getAction(mPlayers.get(mCurrentPlayer).mValue, mDealer.upCard(), mStratHard));
                }
            }
            else {
                stand();
            }
        }
        nextPlayer();
    }

    void action(String action) {
        if (action == "H") {
            hit();
        }
        else if (action == "S") {
            stand();
        }
        else if (action == "D") {
            doubleBet();
        }
        else if (action == "P") {
            split();
        }
        else {
            System.out.println("No action found");
            System.exit(1);
        }
    }

    void dealerPlay() {
        boolean allBusted = true;
        for(Player player : mPlayers){
            if(player.mValue < 22) {
                allBusted = false;
                break;
            }
        }
        mDealer.mHand.get(1).mFaceDown = false;
        mRunningCount += mDealer.mHand.get(1).mCount;
        mDealer.evaluate();
        if(mVerbose > 0) {
            System.out.println("Dealer's turn");
            print();
        }
        if(allBusted) {
            if(mVerbose > 0) {
                System.out.println("Dealer automatically wins cause all players busted");
            }
            finishRound();
        } 
        else {
            while(mDealer.mValue < 17 && mDealer.mHand.size() < 5) {
                dealDealer();
                mDealer.evaluate();
                if(mVerbose > 0) {
                    System.out.println("Dealer hits");
                    print();
                }
            }
            finishRound();
        }
    }

    void nextPlayer() {
        if(++mCurrentPlayer < mPlayers.size()){
            autoPlay();
        } else {
            dealerPlay();
        }
        
    }

    void checkPlayerNatural() {
        for (Player player : mPlayers) {
            if(player.mValue == 21 && player.mHand.size() == 2 && player.mSplitFrom == null){
                player.mHasNatural = true;
            }
        }
    }

    boolean checkDealerNatural() {
        if(mDealer.evaluate() == 21) {
            mDealer.mHand.get(1).mFaceDown = false;
            mRunningCount += mDealer.mHand.get(1).mCount;
            if(mVerbose > 0) {
                print();
                System.out.println("Dealer has a natural 21");
            }
            return true;
        }
        return false;
    }

    public void checkEarnings() {
        float check = 0;
        for(Player player : mPlayers) {
            check += player.mEarnings;
        }
        if(check * -1 != mCasinoEarnings) {
            System.out.println("Earnings don't match");
            System.exit(1);
        }
    }

    void finishRound() {
        if (mVerbose > 0) {
            System.out.println("Scoring round");
        }
        for (Player player : mPlayers) {
            if (player.mHasNatural) {
                player.win(1.5f);
                if (mVerbose > 0) {
                    System.out.println("Player " + player.mPlayerNum + " Wins " + (1.5 * player.mBetMult * player.mInitialBet) + " with a natural 21");
                }
            }
            else if (player.mValue > 21) {
                player.lose();
                if (mVerbose > 0) {
                    System.out.println("Player " + player.mPlayerNum + " Busts and Loses " + (player.mBetMult * player.mInitialBet));
                }

            }
            else if (mDealer.mValue > 21 || player.mValue > mDealer.mValue) {
                player.win();
                if (mVerbose > 0) {
                    System.out.println("Player " + player.mPlayerNum + " Wins " + (player.mBetMult * player.mInitialBet));
                }
            }
            else if (player.mValue == mDealer.mValue) {
                if (mVerbose > 0) {
                    System.out.println("Player " + player.mPlayerNum + " Draws");
                }
            }
            else {
                player.lose();
                if (mVerbose > 0) {
                    System.out.println("Player " + player.mPlayerNum + " Loses " + (player.mBetMult * player.mInitialBet));
                }
            }
        }
        if (mVerbose > 0) {
            for(Player player : mPlayers) {
                if (player.mSplitFrom == null) {
                    System.out.println("Player " + player.mPlayerNum + " Earnings: " + player.mEarnings);
                }
            }
            System.out.println();
        }
    }

    void print() {
        for(Player player : mPlayers) {
            System.out.println(player.print());
        }
        System.out.println(mDealer.print()+"\n");
    }
}