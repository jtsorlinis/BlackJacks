let PlayerNumCount = 0;
const MaxSplits = 10;

export default class Player {
  constructor(table = null, split = null) {
    this.mHand = [];
    this.mValue = 0;
    this.mEarnings = 0;
    this.mAces = 0;
    this.mIsSoft = 0;
    this.mSplitCount = 0;
    this.mIsDone = false;
    this.mSplitFrom = null;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mTable = table;
    this.mInitialBet = 0;
    if (table != null) {
      this.mInitialBet = this.mTable.mBetSize;
      if (split != null) {
        this.mHand.push(split.mHand.pop());
        this.mSplitCount++;
        this.mPlayerNum = `${split.mPlayerNum}S`;
        this.mInitialBet = split.mInitialBet;
        this.mSplitFrom = split;
      } else {
        PlayerNumCount++;
        this.mPlayerNum = PlayerNumCount;
      }
    }
  }

  doubleBet() {
    this.mBetMult = 2;
  }

  resetHand() {
    this.mHand = [];
    this.mValue = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mSplitCount = 0;
    this.mIsDone = false;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mInitialBet = this.mTable.mBetSize;
  }

  canSplit() {
    if (
      this.mHand.length === 2 &&
      this.mHand[0].mRank === this.mHand[1].mRank &&
      this.mSplitCount < MaxSplits
    ) {
      return this.mHand[0].mValue;
    }
    return 0;
  }

  win(mult = 1) {
    const x = this.mInitialBet * this.mBetMult * mult;
    this.mEarnings += x;
    this.mTable.mCasinoEarnings -= x;
  }

  lose() {
    const x = this.mInitialBet * this.mBetMult;
    this.mEarnings -= x;
    this.mTable.mCasinoEarnings += x;
  }

  print() {
    let output = `Player ${this.mPlayerNum}: `;
    this.mHand.forEach((card) => {
      output += `${card.print()} `;
    });
    for (let i = this.mHand.length; i < 5; i++) {
      output += "  ";
    }
    output += `\tScore: ${this.mValue}`;
    if (this.mValue > 21) {
      output += " (Bust) ";
    } else {
      output += "        ";
    }
    if (this.mPlayerNum !== "D") {
      output += `\tBet: ${this.mInitialBet * this.mBetMult}`;
    }
    return output;
  }

  evaluate() {
    this.mAces = 0;
    this.mValue = 0;
    for (let i = 0; i < this.mHand.length; i++) {
      this.mValue += this.mHand[i].mValue;
      if (this.mHand[i].mIsAce) {
        this.mAces++;
        this.mIsSoft = true;
      }
    }

    while (this.mValue > 21 && this.mAces > 0) {
      this.mValue -= 10;
      this.mAces--;
    }

    if (this.mAces === 0) {
      this.mIsSoft = false;
    }
  }
}
