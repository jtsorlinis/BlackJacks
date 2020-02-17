const Card = require('./card');

const mRanks = ['A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K'];
const mSuits = ['Clubs', 'Hearts', 'Spades', 'Diamonds'];

module.exports = class Deck {
  constructor() {
    this.mCards = [];
    for (let i = 0; i < mSuits.length; i++) {
      for (let j = 0; j < mRanks.length; j++) {
        this.mCards.push(new Card(mRanks[j], mSuits[i]));
      }
    }
  }

  print() {
    let output = '';
    this.mCards.forEach((card) => {
      output += card.print();
    });
    return output;
  }

  shuffle() {
    for (let i = this.mCards.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [this.mCards[i], this.mCards[j]] = [this.mCards[j], this.mCards[i]];
    }
  }
};
