import type Card from "./card.ts";
import Deck from "./deck.ts";

export default class CardPile {
  declare mCards: Card[];
  declare mOriginalCards: Card[];

  constructor(numofdecks: number) {
    this.mCards = [];
    this.mOriginalCards = [];
    for (let x = 0; x < numofdecks; x++) {
      const temp = new Deck();
      this.mCards.push(...temp.mCards);
    }
    this.mOriginalCards = [...this.mCards];
  }

  refresh(): void {
    this.mCards = [...this.mOriginalCards];
  }

  print(): string {
    let output = "";
    this.mCards.forEach((card) => {
      output += card.print();
    });
    return output;
  }

  shuffle(): void {
    for (let i = this.mCards.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      const temp = this.mCards[i];
      this.mCards[i] = this.mCards[j];
      this.mCards[j] = temp;
    }
  }
}
