#include <stdio.h>
#include <time.h>
#include "cardpile.h"
#include "player.h"
#include "table.h"

int main(int argc, char const* argv[]) {

  clock_t start = clock();
  CardPile* cp = CardPile__new(8);

  for (int i = 0; i < 100000; i++) {
    CardPile__shuffle(cp);
    CardPile__refresh(cp);
  }

  CardPile__print(cp);

  Table t;
  t.m_bet_size = 10;
  Player* p = Player__new(&t, NULL);
  printf("Took %f seconds", (double)(clock()-start)/ CLOCKS_PER_SEC);

  return 0;
}
