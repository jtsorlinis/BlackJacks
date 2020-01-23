using System.Collections.Generic;

namespace NETCoreBlackJack {
    class Player {
        static int playerNumCount = 0;
        int maxsplits = 10;

        public string mPlayerNum;
        public List<Card> mHand;
        public int mValue;
        public float mEarnings;
        public int mAces;
        public bool mIsSoft;
        public int mSplitCount;
        public bool mIsDone;
        public Player mSplitFrom = null;
        public float mBetMult;
        public bool mHasNatural;
        public Table mTable;
        public int mInitialBet;

        public Player(Table table, Player split = null) {

        }

        public void Doublebet() {

        }

        public virtual void ResetHand() {

        }

        public string CanSplit() {
            return "";
        }

        public void Win(float mult = 1) {

        }

        public void Lose() {

        }

        public string Print() {
            return "";
        }

        public int Evaluate() {
            return 0;
        }
    }
}