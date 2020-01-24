namespace NETCoreBlackJack {
    class Dealer : Player {
        public bool mHideSecond = true;

        public Dealer() {
            mPlayerNum = "D";
            mValue = 0;
        }

        public override void ResetHand() {
            mHand.Clear();
            mValue = 0;
            mHideSecond = true;
        }

        public int UpCard() {
            return mHand[0].mValue;
        }
    }
}
