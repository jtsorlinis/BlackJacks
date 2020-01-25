namespace NETCoreBlackJack {
    class Card {
        public string mRank;
        string mSuit;
        public bool mFaceDown = false;
        public int mValue;
        public int mCount;

        public Card(string rank, string suit) {
            mRank = rank;
            mSuit = suit;
            mValue = Evaluate();
            mCount = Count();

        }

        public string Print() {
            if(mFaceDown) {
                return "X";
            } else {
                return mRank;
            }
        }

        public int Evaluate() {
            if(mRank == "J" || mRank == "Q" || mRank == "K") {
                return 10;
            } else if (mRank == "A") {
                return 11;
            } else {
                return int.Parse(mRank);
            }
        }

        public int Count() {
            if (mRank == "10" || mRank == "J" || mRank == "Q" || mRank == "K" || mRank == "A") {
                return -1;
            }
            else if (mRank == "7" || mRank == "8" || mRank == "9") {
                return 0;
            }
            else {
                return 1;
            }
        }
    }
}