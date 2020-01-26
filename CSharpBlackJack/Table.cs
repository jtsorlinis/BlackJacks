using System;
using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Table {
        int mVerbose;
        public int mBetSize;
        public LinkedList<Player> mPlayers = new LinkedList<Player>();
        int mNumOfDecks;
        public CardPile mCardPile;
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
            mCardPile.mCards.RemoveAt(mCardPile.mCards.Count - 1);
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
            mCardPile.mCards.RemoveAt(mCardPile.mCards.Count - 1);
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
                Console.WriteLine("Running count is: " + mRunningCount + "\tTrue count is: " + (int)mTrueCount);
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

        public void Clear() {
            var node = mPlayers.First;
            while(node != null) {
                var nextNode = node.Next;
                if(node.Value.mSplitFrom != null) {
                    mPlayers.Remove(node);
                }
                node.Value.ResetHand();
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
                Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " stands");
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
            if(mVerbose > 0) {
                Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " splits Aces");
            }
            Player splitPlayer = new Player(this, mCurrentPlayer.Value);
            mCurrentPlayer.Value.mHand.RemoveAt(mCurrentPlayer.Value.mHand.Count-1);
            mPlayers.AddAfter(mCurrentPlayer,splitPlayer);
            Deal();
            mCurrentPlayer.Value.Evaluate();
            Stand();
            mCurrentPlayer = mCurrentPlayer.Next;
            Deal();
            mCurrentPlayer.Value.Evaluate();
            Stand();
            if(mVerbose > 0) {
                Print();
            }

        }

        void DoubleBet() {
            if(mCurrentPlayer.Value.mBetMult == 1 && mCurrentPlayer.Value.mHand.Count == 2) {
                mCurrentPlayer.Value.Doublebet();
                if(mVerbose > 0) {
                    Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " doubles");
                }
                Hit();
                Stand();
            }
            else {
                Hit();
            }

        }

        void AutoPlay() {
            while (!mCurrentPlayer.Value.mIsDone) {
                // check if player just split
                if (mCurrentPlayer.Value.mHand.Count == 1) {
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + mCurrentPlayer.Value.mPlayerNum + " gets 2nd card after splitting");
                    }
                    Deal();
                    mCurrentPlayer.Value.Evaluate();
                }

                if (mCurrentPlayer.Value.mHand.Count < 5 && mCurrentPlayer.Value.mValue < 21) {
                    string canSplit = mCurrentPlayer.Value.CanSplit();
                    if (canSplit == "A") {
                        SplitAces();
                    }
                    else if (canSplit != null && (canSplit != "5" && canSplit != "10" && canSplit != "J" && canSplit != "Q" && canSplit != "K")) {
                        Action(Strategies.GetAction(int.Parse(canSplit), mDealer.UpCard(), mStratSplit));
                    }
                    else if (mCurrentPlayer.Value.mIsSoft) {
                        Action(Strategies.GetAction(mCurrentPlayer.Value.mValue, mDealer.UpCard(), mStratSoft));
                    }
                    else {
                        Action(Strategies.GetAction(mCurrentPlayer.Value.mValue, mDealer.UpCard(), mStratHard));
                    }
                }
                else {
                    Stand();
                }
            }
            NextPlayer();
        }

        void Action(string action) {
            if (action == "H") {
                Hit();
            }
            else if (action == "S") {
                Stand();
            }
            else if (action == "D") {
                DoubleBet();
            }
            else if (action == "P") {
                Split();
            }
            else {
                Console.WriteLine("No action found");
                Environment.Exit(1);
            }
        }

        void DealerPlay() {
            bool allBusted = true;
            foreach( var player in mPlayers) {
                if(player.mValue < 22) {
                    allBusted = false;
                }
            }
            mDealer.mHand[1].mFaceDown = false;
            UpdateCount(mDealer.mHand[1]);
            mDealer.Evaluate();
            if(mVerbose > 0) {
                Console.WriteLine("Dealer's turn");
                Print();
            }
            if(allBusted) {
                if(mVerbose > 0) {
                    Console.WriteLine("Dealer automatically wins cause all players busted");
                }
                FinishRound();
            } 
            else {
                while(mDealer.mValue < 17 && mDealer.mHand.Count < 5) {
                    DealDealer();
                    mDealer.Evaluate();
                    if(mVerbose > 0) {
                        Console.WriteLine("Dealer hits");
                        Print();
                    }
                }
                FinishRound();
            }
        }

        void NextPlayer() {
            if(mCurrentPlayer.Next != null){
                mCurrentPlayer = mCurrentPlayer.Next;
                AutoPlay();
            } else {
                DealerPlay();
            }
            
        }

        void CheckPlayerNatural() {
            foreach(var player in mPlayers){
                if(player.mValue == 21 && player.mHand.Count == 2 && player.mSplitFrom == null){
                    player.mHasNatural = true;
                }
            }
        }

        bool CheckDealerNatural() {
            if(mDealer.Evaluate() == 21) {
                mDealer.mHand[1].mFaceDown = false;
                UpdateCount(mDealer.mHand[1]);
                if(mVerbose > 0) {
                    Print();
                    Console.WriteLine("Dealer has a natural 21");
                }
                return true;
            } else {
                return false;
            }
            
        }

        public void CheckEarnings() {
            float check = 0;
            foreach(var player in mPlayers){
                check += player.mEarnings;
            }
            if(check * -1 != mCasinoEarnings) {
                Console.WriteLine("Earnings don't match");
                Environment.Exit(1);
            }
        }

        void FinishRound() {
            if (mVerbose > 0) {
                Console.WriteLine("Scoring round");
            }
            foreach (var player in mPlayers) {
                if (player.mHasNatural) {
                    player.Win(1.5f);
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Wins " + (1.5 * player.mBetMult * player.mInitialBet) + " with a natural 21");
                    }
                }
                else if (player.mValue > 21) {
                    player.Lose();
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Busts and Loses " + (player.mBetMult * player.mInitialBet));
                    }

                }
                else if (mDealer.mValue > 21) {
                    player.Win();
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Wins " + (player.mBetMult * player.mInitialBet));
                    }
                }
                else if (player.mValue > mDealer.mValue) {
                    player.Win();
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Wins " + (player.mBetMult * player.mInitialBet));
                    }
                }
                else if (player.mValue == mDealer.mValue) {
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Draws");
                    }
                }
                else {
                    player.Lose();
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Loses " + (player.mBetMult * player.mInitialBet));
                    }
                }
            }
            if (mVerbose > 0) {
                foreach (var player in mPlayers) {
                    if (player.mSplitFrom == null) {
                        Console.WriteLine("Player " + player.mPlayerNum + " Earnings: " + player.mEarnings);
                    }
                }
                Console.WriteLine();
            }
        }

        void Print() {
            foreach (Player player in mPlayers) {
                Console.WriteLine(player.Print());
            }
            Console.WriteLine(mDealer.Print()+"\n");
        }
    }
}