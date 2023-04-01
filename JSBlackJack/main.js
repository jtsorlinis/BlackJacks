import Table from "./table.js";

const NumOfPlayers = 5;
const NumOfDecks = 8;
const BetSize = 10;
const MinCards = 40;

let Rounds = 1000000;
const Verbose = false;

if (process.argv.length === 3) {
  Rounds = parseInt(process.argv[2], 10);
}

const t = new Table(NumOfPlayers, NumOfDecks, BetSize, MinCards, Verbose);
t.mCardPile.shuffle();

const start = Date.now();

let x = 0;
while (x++ < Rounds) {
  if (Verbose) {
    console.log(`Round ${x}`);
  }
  if (!Verbose && Rounds > 1000 && x % (Rounds / 100) === 0) {
    process.stdout.write(`\tProgress: ${(x * 100) / Rounds}%\r`);
  }

  t.startRound();
  t.checkEarnings();
}

t.clear();

for (let i = 0; i < t.mPlayers.length; i++) {
  console.log(
    `Player ${t.mPlayers[i].mPlayerNum} earnings: ${
      t.mPlayers[i].mEarnings
    }\t\tWin Percentage: ${
      50 + (t.mPlayers[i].mEarnings / (Rounds * BetSize)) * 50
    }%`
  );
}
console.log(`Casino earnings: ${t.mCasinoEarnings}`);
console.log(
  `Played ${Rounds} rounds in ${(Date.now() - start) / 1000} seconds`
);
