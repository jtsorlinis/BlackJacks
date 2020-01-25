using System;
using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Table {
        int mVerbose;
        public int mBetSize;
        LinkedList<Player> mPlayers = new LinkedList<Player>();
        int mNumOfDecks;
        CardPile mCardPile;
        int mMinCards;
        Dealer mDealer = new Dealer();
        LinkedListNode<Player> mCurrentPlayer = null;
        public float mCasinoEarnings = 0;
        int mRunningCount = 0;
        float mTrueCount = 0;
        Dictionary<int, string> mStratHard = Strategies.Array2dToMap(Strategies.stratHard);
        Dictionary<int, string> mStratSoft = Strategies.Array2dToMap(Strategies.stratSoft);
        Dictionary<int, string> mStratSplit = Strategies.Array2dToMap(Strategies.stratSplit);

        public Table(int numPlayers, int numDecks, int betsize, int minCards, int verbose = 0) {
            mCardPile = new CardPile(numDecks);
            mVerbose = verbose;
            mBetSize = betsize;
            mNumOfDecks = numDecks;
            mMinCards = minCards;

            for(int i = 0; i < numPlayers; i++) {
                mPlayers.AddLast(new Player(this));
            }
        }

        void DealRound() {
            var node = mPlayers.First;
            while( node != null) {
                mCurrentPlayer = node;
                Deal();
                mCurrentPlayer.Value.Evaluate();
                node = node.Next;
            }
            mCurrentPlayer = mPlayers.First;
        }

        void Deal() {
            Card card = mCardPile.mCards[mCardPile.mCards.Count - 1];
            mCurrentPlayer.Value.mHand.Add(card);
            UpdateCount(card);
            mCardPile.mCards.Remove(card);
        }

        void PreDeal() {
            foreach(var player in mPlayers){
                SelectBet(player);
            }
        }

        void SelectBet(Player player) {
            if(mTrueCount >=2) {
                player.mInitialBet = (int)(mBetSize * (int)(mTrueCount - 1) * 1.25);
            }
        }

        void DealDealer(bool faceDown = false) {
            Card card = mCardPile.mCards[mCardPile.mCards.Count - 1];
            mCardPile.mCards.Remove(card);
            card.mFaceDown = faceDown;
            mDealer.mHand.Add(card);
            if(!faceDown) {
                UpdateCount(card);
            }
        }

        public void StartRound() {
            Clear();
            if(mVerbose > 0) {
                Console.WriteLine(mCardPile.mCards.Count + " cards left");
                Console.WriteLine("Running count is: " + mRunningCount);
            }
            GetNewCards();
            PreDeal();
            DealRound();
            DealDealer();
            DealRound();
            DealDealer(true);
            mCurrentPlayer = mPlayers.First;
            if(CheckDealerNatural()) {
                FinishRound();
            } 
            else {
                CheckPlayerNatural();
                if(mVerbose > 0) {
                    Print();
                }
                AutoPlay();
            }
        }

        void GetNewCards() {
            if(mCardPile.mCards.Count < mMinCards) {
                mCardPile.Refresh();
                mCardPile.Shuffle();
                mTrueCount = 0;
                mRunningCount = 0;
                if(mVerbose > 0) {
                    Console.WriteLine("Got " + mNumOfDecks + " new decks as number of cards left is below " + mMinCards);
                }
            }
        }

        void Clear() {
            var node = mPlayers.First;
            while(node != null) {
                var nextNode = node.Next;
                if(node.Value.mSplitFrom != null) {
                    node.Value.ResetHand();
                    mPlayers.Remove(node);
                }
                node = nextNode;
            }
            mDealer.ResetHand();
        }

        void UpdateCount(Card card) {
            mRunningCount += card.mCount;
            mTrueCount = mRunningCount / (mCardPile.mCards.Count/(float)52);
        }

        void Hit() {
            Deal();
            mCurrentPlayer.Value.Evaluate();
            if(mVerbose > 0) {
                Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " hits");
            }
        }

        void Stand() {
            if(mVerbose > 0 && mCurrentPlayer.Value.mValue <= 21) {
                Console.WriteLine("Player" + mCurrentPlayer.Value.mPlayerNum + " stands");
                Print();
            }
            mCurrentPlayer.Value.mIsDone = true;
        }

        void Split() {
            Player splitPlayer = new Player(this, mCurrentPlayer.Value);
            mCurrentPlayer.Value.mHand.RemoveAt(mCurrentPlayer.Value.mHand.Count-1);
            mPlayers.AddAfter(mCurrentPlayer,splitPlayer);
            mCurrentPlayer.Value.Evaluate();
            mCurrentPlayer.Next.Value.Evaluate();
            if( mVerbose > 0) {
                Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " splits");
            }
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
            return false;
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