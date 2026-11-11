import type { Int32, ProgramLifetime } from "@scriptc/types";
import Card from "./card.ts";

// A standalone class (as in the Rust port) rather than a Player subclass, so
// Player has no subclasses and scriptc can call its methods directly
export default class Dealer implements ProgramLifetime {
  mHand: Card[];
  mValue: Int32;
  mAces: Int32;
  mIsSoft: boolean;
  mPlayerNum: string;

  constructor() {
    this.mHand = new Array();
    this.mValue = 0;
    this.mAces = 0;
    this.mIsSoft = false;
    this.mPlayerNum = "D";
  }

  resetHand(): void {
    while (this.mHand.length > 0) {
      this.mHand.pop();
    }
    this.mValue = 0;
    this.mAces = 0;
    this.mIsSoft = false;
  }

  upCard(): Int32 {
    return this.mHand[0].mValue;
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
