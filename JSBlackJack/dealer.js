import Player from "./player.js";

export default class Dealer extends Player {
  constructor() {
    super();
    this.mPlayerNum = "D";
    this.mValue = 0;
  }

  resetHand() {
    this.mHand = [];
    this.mValue = 0;
  }

  upCard() {
    return this.mHand[0].mValue;
  }
}
