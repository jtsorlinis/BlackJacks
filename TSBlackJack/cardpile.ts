import Card from "./card.ts";
import Deck from "./deck.ts";

export default class CardPile {
  mCards: Card[];
  mOriginalCards: Card[];

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
    const cards = this.mCards;
    for (let i = cards.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      const temp = cards[i];
      cards[i] = cards[j];
      cards[j] = temp;
    }
  }
}
