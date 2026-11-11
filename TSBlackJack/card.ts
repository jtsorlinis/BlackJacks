import type { Int32, ProgramLifetime } from "@scriptc/types";

export default class Card implements ProgramLifetime {
  mRank: string;
  mRankId: Int32;
  mSuit: string;
  mFaceDown: boolean;
  mValue: Int32;
  mCount: Int32;
  mIsAce: boolean;

  constructor(rank: string, suit: string) {
    this.mRank = rank;
    this.mRankId = rank.charCodeAt(0) | 0;
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

  evaluate(): Int32 {
    if (this.mRank === "J" || this.mRank === "Q" || this.mRank === "K") {
      return 10;
    }
    if (this.mRank === "A") {
      return 11;
    }
    return parseInt(this.mRank, 10) | 0;
  }

  count(): Int32 {
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
