const Deck = require("./deck");

var seed = Date.now();

function xorShift(){
    seed ^= seed >> 12;
    seed ^= seed << 25;
    seed ^= seed >> 27;
    return seed;
}

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
    //TODO: Optimize this
    shuffle() {
        for(var i = this.mCards.length-1; i > 0; i--) {
            xorShift();
            const j = seed  % (i+1);
            [this.mCards[i], this.mCards[j]] = [this.mCards[j], this.mCards[i]];
        }
    }
};
