using System;
using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Table {
        int mVerbose;
        public int mBetSize;
        List<Player> mPlayers = new List<Player>();
        int mNumOfDecks;
        CardPile mCardPile;
        int mMinCards;
        Dealer mDealer = new Dealer();
        Player mCurrentPlayer = null;
        public float mCasinoEarnings = 0;
        int mRunningCount = 0;
        float mTrueCount = 0;
        Dictionary<int, string> mStratHard = Strategies.Array2dToMap(Strategies.stratHard);
        Dictionary<int, string> mStratSoft = Strategies.Array2dToMap(Strategies.stratSoft);
        Dictionary<int, string> mStratSplit = Strategies.Array2dToMap(Strategies.stratSplit);

        public Table(int numPlayers, int numDecks, int betsize, int minCards, int verbose = 0) {
            mVerbose = verbose;
            mBetSize = betsize;
            mNumOfDecks = numDecks;
            mMinCards = minCards;

            for(int i = 0; i < numPlayers; i++) {
                mPlayers.Add(new Player(this));
            }
        }

        void DealRound() {
            foreach(Player it in mPlayers) {
                mCurrentPlayer = it;
                Deal();
                mCurrentPlayer.Evaluate();

            }
        }

        void Deal() {

        }

        void PreDeal() {

        }

        void SelectBet(Player player) {

        }

        void DealDealer(bool faceDown) {

        }

        public void StartRound() {
            DealRound();
            Print();

        }

        void GetNewCards() {

        }

        void Clear() {

        }

        void UpdateCount() {

        }

        void Hit() {

        }

        void Stand() {

        }

        void Split() {

        }

        void SplitAces() {

        }

        void DoubleBet() {

        }

        void AutoPlay() {

        }

        void Action() {

        }

        void DealerPlay() {

        }

        void NextPlayer() {

        }

        void CheckPlayerNatural() {

        }

        bool CheckDealerNatural() {
            return true;
        }

        void CheckEarnings() {

        }

        void FinishRound() {

        }

        void Print() {
            foreach (Player player in mPlayers) {
                Console.WriteLine(player.Print());
            }
            Console.WriteLine(mDealer.Print() + "\n");
        }
    }
}