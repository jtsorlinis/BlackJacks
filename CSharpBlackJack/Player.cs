using System;
using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Player {
        static int playerNumCount = 0;
        int maxsplits = 10;

        public string mPlayerNum;
        public List<Card> mHand = new List<Card>();
        public int mValue = 0;
        public float mEarnings = 0;
        public int mAces = 0;
        public bool mIsSoft = false;
        public int mSplitCount = 0;
        public bool mIsDone = false;
        public Player mSplitFrom = null;
        public float mBetMult = 1;
        public bool mHasNatural = false;
        public Table mTable;
        public int mInitialBet;

        public Player(Table table = null, Player split = null) {
            mTable = table;
            if(table != null) {
                mInitialBet = mTable.mBetSize;
                if(split != null) {
                    mHand.Add(split.mHand[1]);
                    mSplitCount++;
                    mPlayerNum = split.mPlayerNum + "S";
                    mSplitFrom = split;
                } else {
                    playerNumCount++;
                    mPlayerNum = playerNumCount.ToString();
                }
            }
        }

        public void Doublebet() {
            mBetMult = 2;
        }

        public virtual void ResetHand() {
            mHand.Clear();
            mValue = 0;
            mAces = 0;
            mIsSoft = false;
            mSplitCount = 0;
            mIsDone = false;
            mBetMult = 1;
            mHasNatural = false;
            mInitialBet = mTable.mBetSize;
        }

        public string CanSplit() {
            if(mHand.Count == 2 && mHand[0].mRank == mHand[1].mRank && mSplitCount < maxsplits) {
                return mHand[0].mRank;
            } else {
                return null;
            }
            
        }

        public void Win(float mult = 1) {
            if (mSplitFrom != null) {
                mSplitFrom.Win(mult);
            } else {
                mEarnings += (mInitialBet * mBetMult * mult);
                mTable.mCasinoEarnings -= (mInitialBet * mBetMult * mult);
            }
        }

        public void Lose() {
            if (mSplitFrom != null) {
                    mSplitFrom.Lose();
                } else {
                    mEarnings -= (mInitialBet * mBetMult);
                    mTable.mCasinoEarnings += (mInitialBet * mBetMult);
                }
        }

        public string Print() {
            string output = "Player " + mPlayerNum + ": ";
            foreach(Card i in mHand) {
                output += i.Print() + " ";
            }
            for (int i = mHand.Count; i < 5; i++) {
                output += "  ";
            }
            output += "\tScore: " + mValue.ToString();
            if (mValue > 21) {
                output += " (Bust) ";
            } else {
                output += "        ";
            }
            if (mPlayerNum != "D") {
                output += "\tBet: " + (mInitialBet * mBetMult).ToString();
            }
            return output;
        }

        public int Evaluate() {
            mAces = 0;
            mValue = 0;
            foreach(var card in mHand){
                mValue += card.mValue;
                // check for ace
                if(card.mRank == "A") {
                    mAces++;
                    mIsSoft = true;
                }
            }

            while(mValue > 21 && mAces > 0) {
                mValue -= 10;
                mAces--;
            }

            if(mAces == 0) {
                mIsSoft = false;
            }

            return mValue;
        }
    }
}