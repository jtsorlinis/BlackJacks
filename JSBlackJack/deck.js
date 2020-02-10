const Card = require('./card');

var mRanks = ["A", "2", "3", "4", "5", "6", "7", "8","9","10","J","Q","K"];
var mSuits = ["Clubs", "Hearts", "Spades", "Diamonds" ];

module.exports = class Deck {
    constructor() {
        this.mCards = [];
        for(var i = 0; i < mSuits.length; i++) {
            for(var j =0; j < mRanks.length; j++) {
                this.mCards.push(new Card(mRanks[j],mSuits[i]));
            }
        }
    }

    print() {
        var output = "";
        this.mCards.forEach(function(card) {
            output += card.print();
        })
        return output;
    }

    shuffle() {
        for(var i = this.mCards.length-1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i+1));
            [this.mCards[i], this.mCards[j]] = [this.mCards[j], this.mCards[i]];
        }
    }

}

