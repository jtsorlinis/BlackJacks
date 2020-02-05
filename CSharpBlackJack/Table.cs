using System;
using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Table {
        int mVerbose;
        public int mBetSize;
        public List<Player> mPlayers = new List<Player>();
        int mNumOfDecks;
        public CardPile mCardPile;
        int mMinCards;
        Dealer mDealer = new Dealer();
        int mCurrentPlayer = 0;
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
                mPlayers.Add(new Player(this));
            }
        }

        void DealRound() {
            for(int i = 0; i < mPlayers.Count; i++) { 
                Deal();
                mPlayers[i].Evaluate();
                mCurrentPlayer++;
            }
            mCurrentPlayer = 0;
        }

        void Deal() {
            Card card = mCardPile.mCards[mCardPile.mCards.Count - 1];
            mPlayers[mCurrentPlayer].mHand.Add(card);
            mRunningCount += card.mCount;
            mCardPile.mCards.RemoveAt(mCardPile.mCards.Count - 1);
        }

        void PreDeal() {
            for (int i = 0; i < mPlayers.Count; i++) {
                SelectBet(mPlayers[i]);
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
                mRunningCount += card.mCount;
            }
        }

        public void StartRound() {
            Clear();
            UpdateCount();
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
            mCurrentPlayer = 0;
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
            for(int i = mPlayers.Count -1; i >= 0; i--) {
                mPlayers[i].ResetHand();
                if (mPlayers[i].mSplitFrom != null) {
                    mPlayers.RemoveAt(i);
                }
            }
            mDealer.ResetHand();
            mCurrentPlayer = 0;
        }

        void UpdateCount() {
            mTrueCount = mRunningCount / (mCardPile.mCards.Count/52f);
        }

        void Hit() {
            Deal();
            mPlayers[mCurrentPlayer].Evaluate();
            if(mVerbose > 0) {
                Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " hits");
            }
        }

        void Stand() {
            if(mVerbose > 0 && mPlayers[mCurrentPlayer].mValue <= 21) {
                Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " stands");
                Print();
            }
            mPlayers[mCurrentPlayer].mIsDone = true;
        }

        void Split() {
            Player splitPlayer = new Player(this, mPlayers[mCurrentPlayer]);
            mPlayers[mCurrentPlayer].mHand.RemoveAt(mPlayers[mCurrentPlayer].mHand.Count-1);
            mPlayers.Insert(mCurrentPlayer+1, splitPlayer);
            mPlayers[mCurrentPlayer].Evaluate();
            mPlayers[mCurrentPlayer+1].Evaluate();
            if( mVerbose > 0) {
                Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " splits");
            }
        }

        void SplitAces() {
            if(mVerbose > 0) {
                Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " splits Aces");
            }
            Player splitPlayer = new Player(this, mPlayers[mCurrentPlayer]);
            mPlayers[mCurrentPlayer].mHand.RemoveAt(mPlayers[mCurrentPlayer].mHand.Count-1);
            mPlayers.Insert(mCurrentPlayer + 1, splitPlayer);
            Deal();
            mPlayers[mCurrentPlayer].Evaluate();
            Stand();
            mCurrentPlayer++;
            Deal();
            mPlayers[mCurrentPlayer].Evaluate();
            Stand();
            if(mVerbose > 0) {
                Print();
            }

        }

        void DoubleBet() {
            if(mPlayers[mCurrentPlayer].mBetMult == 1 && mPlayers[mCurrentPlayer].mHand.Count == 2) {
                mPlayers[mCurrentPlayer].Doublebet();
                if(mVerbose > 0) {
                    Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " doubles");
                }
                Hit();
                Stand();
            }
            else {
                Hit();
            }

        }

        void AutoPlay() {
            while (!mPlayers[mCurrentPlayer].mIsDone) {
                // check if player just split
                if (mPlayers[mCurrentPlayer].mHand.Count == 1) {
                    if (mVerbose > 0) {
                        Console.WriteLine("Player " + mPlayers[mCurrentPlayer].mPlayerNum + " gets 2nd card after splitting");
                    }
                    Deal();
                    mPlayers[mCurrentPlayer].Evaluate();
                }

                if (mPlayers[mCurrentPlayer].mHand.Count < 5 && mPlayers[mCurrentPlayer].mValue < 21) {
                    int splitCardVal = mPlayers[mCurrentPlayer].CanSplit();
                    if (splitCardVal == 11) {
                        SplitAces();
                    }
                    else if (splitCardVal != 0 && (splitCardVal != 5 && splitCardVal != 10)) {
                        Action(Strategies.GetAction(splitCardVal, mDealer.UpCard(), mStratSplit));
                    }
                    else if (mPlayers[mCurrentPlayer].mIsSoft) {
                        Action(Strategies.GetAction(mPlayers[mCurrentPlayer].mValue, mDealer.UpCard(), mStratSoft));
                    }
                    else {
                        Action(Strategies.GetAction(mPlayers[mCurrentPlayer].mValue, mDealer.UpCard(), mStratHard));
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
            for(int i = 0; i < mPlayers.Count; i++) {
                if(mPlayers[i].mValue < 22) {
                    allBusted = false;
                }
            }
            mDealer.mHand[1].mFaceDown = false;
            mRunningCount += mDealer.mHand[1].mCount;
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
            if(++mCurrentPlayer < mPlayers.Count){
                AutoPlay();
            } else {
                DealerPlay();
            }
            
        }

        void CheckPlayerNatural() {
            for (int i = 0; i < mPlayers.Count; i++) {
                if(mPlayers[i].mValue == 21 && mPlayers[i].mHand.Count == 2 && mPlayers[i].mSplitFrom == null){
                    mPlayers[i].mHasNatural = true;
                }
            }
        }

        bool CheckDealerNatural() {
            mDealer.Evaluate();
            if (mDealer.mValue == 21) {
                mDealer.mHand[1].mFaceDown = false;
                mRunningCount += mDealer.mHand[1].mCount;
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
            for(int i = 0; i < mPlayers.Count; i++) {
                check += mPlayers[i].mEarnings;
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