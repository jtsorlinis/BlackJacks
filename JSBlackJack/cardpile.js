const Deck = require("./deck");

module.exports = class CardPile {
    constructor(numofdecks) {
        this.mCards = [];
        this.mOriginalCards = [];
        for (let x = 0; x < numofdecks; x++) {
            const temp = new Deck();
            this.mCards = [...this.mCards, ...temp.mCards];
        }
        this.mOriginalCards = this.mCards.slice();
    }

    refresh() {
        this.mCards = this.mOriginalCards.slice();
    }

    print() {
        var output = "";
        this.mCards.forEach(function(card) {
            output += card.print();
        });
        return output;
    }

    shuffle() {
        for(var i = this.mCards.length-1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i+1));
            [this.mCards[i], this.mCards[j]] = [this.mCards[j], this.mCards[i]];
        }
    }
};
