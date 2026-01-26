import Deck from "./deck.js";

export default class CardPile {
  constructor(numofdecks) {
    this.mCards = [];
    this.mOriginalCards = [];
    for (let x = 0; x < numofdecks; x++) {
      const temp = new Deck();
      this.mCards.push(...temp.mCards);
    }
    this.mOriginalCards = [...this.mCards];
  }

  refresh() {
    this.mCards = [...this.mOriginalCards];
  }

  print() {
    let output = "";
    this.mCards.forEach((card) => {
      output += card.print();
    });
    return output;
  }

  shuffle() {
    for (let i = this.mCards.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      // [this.mCards[i], this.mCards[j]] = [this.mCards[j], this.mCards[i]];
      const temp = this.mCards[i];
      this.mCards[i] = this.mCards[j];
      this.mCards[j] = temp;
    }
  }
}
