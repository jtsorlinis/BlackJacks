import type { Int32 } from "@scriptc/types";
import Card from "./card.ts";
import type Table from "./table.ts";

let PlayerNumCount = 0;
const MaxHands = 4;

export default class Player {
  mHand: Card[];
  mValue: Int32;
  mEarnings: number;
  mAces: Int32;
  mIsSoft: boolean;
  mSplitCount: { count: number };
  mIsDone: boolean;
  mIsSplitHand: boolean;
  mBetMult: number;
  mHasNatural: boolean;
  mOriginalBet: Int32;
  mInitialBet: Int32;
  mPlayerNum: string = "";

  constructor(table: Table | null = null, split: Player | null = null) {
    this.mHand = new Array();
    this.mValue = 0;
    this.mEarnings = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mIsDone = false;
    this.mIsSplitHand = false;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mOriginalBet = 0;
    this.mInitialBet = 0;
    if (split != null) {
      this.mSplitCount = split.mSplitCount;
    } else {
      this.mSplitCount = { count: 0 };
    }
    if (table != null) {
      this.mOriginalBet = table.mBetSize;
      this.mInitialBet = table.mBetSize;
      if (split != null) {
        this.mSplitCount.count++;
        this.mHand.push(split.mHand.pop()!);
        this.mPlayerNum = `${split.mPlayerNum}S`;
        this.mInitialBet = split.mInitialBet;
        this.mIsSplitHand = true;
      } else {
        PlayerNumCount++;
        this.mPlayerNum = `${PlayerNumCount}`;
      }
    }
  }

  doubleBet(): void {
    this.mBetMult = 2;
  }

  resetHand(): void {
    // popping is the cheapest way to empty the hand under scriptc (length = 0
    // is a runtime call) and is as fast as a fresh array under Node
    while (this.mHand.length > 0) {
      this.mHand.pop();
    }
    this.mValue = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mSplitCount.count = 0;
    this.mIsDone = false;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mInitialBet = this.mOriginalBet;
  }

  canSplit(): Int32 {
    if (
      this.mHand.length === 2 &&
      this.mHand[0].mRankId === this.mHand[1].mRankId &&
      this.mSplitCount.count < MaxHands - 1
    ) {
      return this.mHand[0].mValue;
    }
    return 0;
  }

  settle(mult: number): number {
    const x = this.mInitialBet * this.mBetMult * mult;
    this.mEarnings += x;
    return -x;
  }

  print(): string {
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
    output += `\tBet: ${Math.trunc(this.mInitialBet * this.mBetMult)}`;
    return output;
  }

  evaluate(): void {
    let value = 0;
    let aces = 0;
    for (let i = 0; i < this.mHand.length; i++) {
      value += this.mHand[i].mValue;
      if (this.mHand[i].mIsAce) {
        aces++;
      }
    }

    while (value > 21 && aces > 0) {
      value -= 10;
      aces--;
    }

    this.mValue = value | 0;
    this.mAces = aces | 0;
    this.mIsSoft = aces > 0;
  }
}
