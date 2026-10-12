import Table from "./table.js";

const NumOfPlayers = 5;
const NumOfDecks = 8;
const BetSize = 10;
const MinCards = 52;

let Rounds = 1000000;
const Verbose = false;

// toFixed rounds exact binary ties (e.g. 51.625) away from zero, while the
// other ports round them to even, so step those back a digit to match
const toFixed2 = (value) => {
  const fixed = value.toFixed(2);
  const lastDigit = Number(fixed.at(-1));
  const isTie = Number.isInteger(value * 8) && (value * 8) % 2 !== 0;
  return isTie && lastDigit % 2 === 1
    ? fixed.slice(0, -1) + (lastDigit - 1)
    : fixed;
};

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
  if (!Verbose && Rounds > 1000 && x % Math.floor(Rounds / 100) === 0) {
    process.stderr.write(`\tProgress: ${Math.floor((x * 100) / Rounds)}%\r`);
  }

  t.startRound();
  t.checkEarnings();
}

t.clear();

for (let i = 0; i < t.mPlayers.length; i++) {
  console.log(
    `Player ${t.mPlayers[i].mPlayerNum} earnings: ${
      t.mPlayers[i].mEarnings
    }\t\tWin Percentage: ${toFixed2(
      50 + (t.mPlayers[i].mEarnings / (Rounds * BetSize)) * 50
    )}%`
  );
}
console.log(`Casino earnings: ${t.mCasinoEarnings}`);
console.log(
  `Played ${Rounds} rounds in ${((Date.now() - start) / 1000).toFixed(3)} seconds`
);
