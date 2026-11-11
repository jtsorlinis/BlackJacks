import Player from "./player.ts";

export default class Dealer extends Player {
  constructor() {
    super();
    this.mPlayerNum = "D";
    this.mValue = 0;
  }

  resetHand(): void {
    this.mHand = new Array();
    this.mValue = 0;
  }

  upCard(): number {
    return this.mHand[0].mValue;
  }
}
