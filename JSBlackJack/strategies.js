const stratHard = [
  ['0', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11'],
  ['2', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['3', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['4', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['5', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['6', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['7', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['8', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'],
  ['9', 'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['10', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H'],
  ['11', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H'],
  ['12', 'H', 'H', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'],
  ['13', 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'],
  ['14', 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'],
  ['15', 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'],
  ['16', 'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'],
  ['17', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['18', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['19', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['20', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['21', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
];

const stratSoft = [
  ['0', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11'],
  ['13', 'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['14', 'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['15', 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['16', 'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['17', 'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'],
  ['18', 'S', 'D', 'D', 'D', 'D', 'S', 'S', 'H', 'H', 'H'],
  ['19', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['20', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
  ['21', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'],
];

const stratSplit = [
  ['0', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11'],
  ['2', 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'],
  ['3', 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'],
  ['4', 'H', 'H', 'H', 'P', 'P', 'H', 'H', 'H', 'H', 'H'],
  ['6', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H', 'H'],
  ['7', 'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'],
  ['8', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'],
  ['9', 'P', 'P', 'P', 'P', 'P', 'S', 'P', 'P', 'S', 'S'],
  ['11', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'],
];

function getAction(playerval, dealerval, strategy) {
  const playerVal = parseInt(playerval);
  const dealerVal = parseInt(dealerval);
  const key = ((playerVal + dealerVal) *
    (playerVal + dealerVal + 1)) / 2 + dealerVal;
  return strategy.get(key);
}

function array2dToMap(array) {
  const temp = new Map();
  for (let row = 0; row < array.length; row++) {
    for (let col = 0; col < array[0].length; col++) {
      const playerVal = parseInt(array[row][0]);
      const dealerVal = parseInt(array[0][col]);
      const key = ((playerVal + dealerVal) *
        (playerVal + dealerVal + 1)) / 2 + dealerVal;
      temp.set(key, array[row][col]);
    }
  }
  return temp;
}

module.exports = {
  stratHard,
  stratSoft,
  stratSplit,
  getAction,
  array2dToMap,
};

