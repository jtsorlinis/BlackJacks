const Player = require("./player");

module.exports = class Dealer extends Player {
    constructor() {
        super();
        this.mPlayerNum = "D";
        this.mValue = 0;
    }

    resetHand() {
        //TODO: Ensure this is best way to clear array
        this.mHand.length = 0;
        this.mValue = 0;
    }

    upCard() {
        return this.mHand[0].mValue;
    }
}