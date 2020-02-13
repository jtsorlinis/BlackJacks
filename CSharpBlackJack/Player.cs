using System.Collections.Generic;

namespace CSharpBlackJack {
    internal class Player {
        private const int MaxSplits = 10;
        private static int _playerNumCount;
        private readonly Table _table;
        public readonly List<Card> mHand = new List<Card>();
        public readonly Player mSplitFrom;
        private int _aces;
        private int _splitCount;
        public float mBetMult = 1;
        public float mEarnings;
        public bool mHasNatural;
        public int mInitialBet;
        public bool mIsDone;
        public bool mIsSoft;

        public string mPlayerNum;
        public int mValue;

        public Player(Table table = null, Player split = null) {
            _table = table;
            if (table == null) return;
            mInitialBet = _table.mBetSize;
            if (split != null) {
                mHand.Add(split.mHand[1]);
                _splitCount++;
                mPlayerNum = split.mPlayerNum + "S";
                mSplitFrom = split;
            }
            else {
                _playerNumCount++;
                mPlayerNum = _playerNumCount.ToString();
            }
        }

        public void DoubleBet() {
            mBetMult = 2;
        }

        public virtual void ResetHand() {
            mHand.Clear();
            mValue = 0;
            _aces = 0;
            mIsSoft = false;
            _splitCount = 0;
            mIsDone = false;
            mBetMult = 1;
            mHasNatural = false;
            mInitialBet = _table.mBetSize;
        }

        public int CanSplit() {
            if (mHand.Count == 2 && mHand[0].mRank == mHand[1].mRank && _splitCount < MaxSplits)
                return mHand[0].mValue;
            return 0;
        }

        public void Win(float mult = 1) {
            if (mSplitFrom != null) {
                mSplitFrom.Win(mult);
            }
            else {
                mEarnings += mInitialBet * mBetMult * mult;
                _table.mCasinoEarnings -= mInitialBet * mBetMult * mult;
            }
        }

        public void Lose() {
            if (mSplitFrom != null) {
                mSplitFrom.Lose();
            }
            else {
                mEarnings -= mInitialBet * mBetMult;
                _table.mCasinoEarnings += mInitialBet * mBetMult;
            }
        }

        public string Print() {
            var output = "Player " + mPlayerNum + ": ";
            foreach (var i in mHand) output += i.Print() + " ";
            for (var i = mHand.Count; i < 5; i++) output += "  ";
            output += "\tScore: " + mValue;
            if (mValue > 21)
                output += " (Bust) ";
            else
                output += "        ";
            if (mPlayerNum != "D") output += "\tBet: " + mInitialBet * mBetMult;
            return output;
        }

        public void Evaluate() {
            _aces = 0;
            mValue = 0;
            for (var i = 0; i < mHand.Count; i++) {
                mValue += mHand[i].mValue;
                // check for ace
                if (!mHand[i].mIsAce) continue;
                _aces++;
                mIsSoft = true;
            }

            while (mValue > 21 && _aces > 0) {
                mValue -= 10;
                _aces--;
            }

            if (_aces == 0) mIsSoft = false;
        }
    }
}