using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Table {
        int mVerbose;
        public int mBetSize;
        List<Player> mPlayers = new List<Player>();
        int mNumOfDecks;
        CardPile mCardPile;
        int mMinCards;
        Dealer mDealer;
        Player mCurrentPlayer;
        public float mCasinoEarnings;
        int mRunningCount;
        float mTrueCount;
        List<List<string>> mStratHard;
        List<List<string>> mStratSoft;
        List<List<string>> mStratSplit;

        public Table(int numPlayers, int numDecks, int minCards, int verbose = 0) {

        }

        public void DealRound() {
            
        }

        void Deal() {

        }

        void PreDeal() {

        }

        void SelectBet(Player player) {

        }
    }
}