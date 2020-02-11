const Player = require("./player");
const Table = require("./table");
const Strategies = require("./strategies");

const NumOfPlayers = 5;
const NumOfDecks = 8;
const BetSize = 10;
const MinCards = 40;

const Rounds = 100000;
const Verbose = false;

const t = new Table(NumOfPlayers,NumOfDecks,BetSize,MinCards,Verbose);
t.mCardPile.shuffle();

let x = 0;
while(x++ < Rounds) {
    if(Verbose) {
        console.log("Round " + x);
    }
    if(!Verbose && Rounds > 1000 && x % (Rounds/100) == 0) {
        process.stdout.write("\tProgress: " + parseInt((x/Rounds)*100) + "%\r");
    }

    t.startRound();
    t.checkEarnings();
}

t.clear();

for(let i = 0; i < t.mPlayers.length; i++) {
    console.log("Player " + t.mPlayers[i].mPlayerNum + " earnings: " + t.mPlayers[i].mEarnings + "\t\tWin Percentage: " + (50 + (t.mPlayers[i].mEarnings/(Rounds*BetSize)*50)) + "%");
}