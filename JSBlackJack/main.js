const Player = require("./player");
const Table = require("./table");

const NumOfPlayers = 5;
const NumOfDecks = 8;
const BetSize = 10;
const MinCards = 40;

const Rounds = 10;
const Verbose = true;

const t = new Table(NumOfPlayers,NumOfDecks,BetSize,MinCards,Verbose);
t.startRound();
