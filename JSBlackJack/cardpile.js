const Deck = require('./deck')

module.exports = class CardPile {
    constructor(numofdecks) {
        this.mCards = [];
        this.mOriginalCards = [];
        for(var x = 0; x < numofdecks; x++) {
            const temp = new Deck();
            this.mCards = [...this.mCards, ...temp.mCards];
        }
    }
}