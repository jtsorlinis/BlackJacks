export default class Card {
  declare mRank: string;
  declare mRankId: number;
  declare mSuit: string;
  declare mFaceDown: boolean;
  declare mValue: number;
  declare mCount: number;
  declare mIsAce: boolean;

  constructor(rank: string, suit: string) {
    this.mRank = rank;
    this.mRankId = rank.charCodeAt(0);
    this.mSuit = suit;
    this.mFaceDown = false;
    this.mValue = this.evaluate();
    this.mCount = this.count();
    this.mIsAce = this.mRank === "A";
  }

  print(): string {
    if (this.mFaceDown) {
      return "X";
    }
    return this.mRank;
  }

  evaluate(): number {
    if (this.mRank === "J" || this.mRank === "Q" || this.mRank === "K") {
      return 10;
    }
    if (this.mRank === "A") {
      return 11;
    }
    return parseInt(this.mRank, 10);
  }

  count(): number {
    if (
      this.mRank === "10" ||
      this.mRank === "J" ||
      this.mRank === "Q" ||
      this.mRank === "K" ||
      this.mRank === "A"
    ) {
      return -1;
    }
    if (this.mRank === "7" || this.mRank === "8" || this.mRank === "9") {
      return 0;
    }
    return 1;
  }
}
