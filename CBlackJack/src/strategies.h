#include <stdlib.h>

const char* strat_hard[][11] = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"2", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"3", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"4", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"5", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"6", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"7", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"8", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
    {"9", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"10", "D", "D", "D", "D", "D", "D", "D", "D", "H", "H"},
    {"11", "D", "D", "D", "D", "D", "D", "D", "D", "D", "H"},
    {"12", "H", "H", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"13", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"14", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"15", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"16", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"17", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"18", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"}};

const char* strat_soft[][11] = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"13", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
    {"14", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
    {"15", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"16", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"17", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"18", "S", "D", "D", "D", "D", "S", "S", "H", "H", "H"},
    {"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"}};

const char* strat_split[][11] = {
    {"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
    {"2", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"3", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"4", "H", "H", "H", "P", "P", "H", "H", "H", "H", "H"},
    {"6", "P", "P", "P", "P", "P", "H", "H", "H", "H", "H"},
    {"7", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
    {"8", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"},
    {"9", "P", "P", "P", "P", "P", "S", "P", "P", "S", "S"},
    {"11", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"}};

const char get_action(int player_val, int dealer_val, char* strategy) {
  int key = player_val * 12 + dealer_val;
  return strategy[key];
}

char* array_to_map(const char* array[][11], int rows) {
  char* temp = malloc(sizeof(char) * 1000);
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < 11; col++) {
      int player_val = atoi(array[row][0]);
      int dealer_val = atoi(array[0][col]);
      int key = player_val * 12 + dealer_val;
      temp[key] = array[row][col][0];
    }
  }
  return temp;
}