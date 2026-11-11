import type Card from "./card.ts";
import type Table from "./table.ts";

let PlayerNumCount = 0;
const MaxHands = 4;

export default class Player {
  declare mHand: Card[];
  declare mValue: number;
  declare mEarnings: number;
  declare mAces: number;
  declare mIsSoft: boolean;
  declare mSplitCount: { count: number };
  declare mIsDone: boolean;
  declare mSplitFrom: Player | null;
  declare mBetMult: number;
  declare mHasNatural: boolean;
  declare mTable: Table | null;
  declare mInitialBet: number;
  declare mPlayerNum: number | string;

  constructor(table: Table | null = null, split: Player | null = null) {
    this.mHand = new Array();
    this.mValue = 0;
    this.mEarnings = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mSplitCount = { count: 0 };
    this.mIsDone = false;
    this.mSplitFrom = null;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mTable = table;
    this.mInitialBet = 0;
    if (table != null) {
      this.mInitialBet = this.mTable!.mBetSize;
      if (split != null) {
        this.mSplitCount = split.mSplitCount;
        this.mSplitCount.count++;
        this.mHand.push(split.mHand.pop()!);
        this.mPlayerNum = `${split.mPlayerNum}S`;
        this.mInitialBet = split.mInitialBet;
        this.mSplitFrom = split;
      } else {
        PlayerNumCount++;
        this.mPlayerNum = PlayerNumCount;
      }
    }
  }

  doubleBet(): void {
    this.mBetMult = 2;
  }

  resetHand(): void {
    // a fresh array beats clearing in place (length = 0): +17% Node, +4% Bun
    this.mHand = new Array();
    this.mValue = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mSplitCount.count = 0;
    this.mIsDone = false;
    this.mBetMult = 1;
    this.mHasNatural = false;
    this.mInitialBet = this.mTable!.mBetSize;
  }

  canSplit(): number {
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
    if (this.mPlayerNum !== "D") {
      output += `\tBet: ${Math.trunc(this.mInitialBet * this.mBetMult)}`;
    }
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

    this.mValue = value;
    this.mAces = aces;
    this.mIsSoft = aces > 0;
  }
}
