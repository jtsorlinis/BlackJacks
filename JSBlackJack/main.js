const Player = require("./player");
const Table = require("./table");

const t = new Table();
const p = new Player(t);
console.log(p.print());
