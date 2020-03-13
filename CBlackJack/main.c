#include <stdio.h>
#include <time.h>
#include "cardpile.h"

int main(int argc, char const* argv[]) {

  clock_t start = clock();
  CardPile* cp = CardPile__new(8);

  for (int i = 0; i < 1000000; i++) {
    CardPile__shuffle(cp);
    CardPile__refresh(cp);
  }

  CardPile__print(cp);
  printf("Took %f seconds", (double)(clock()-start)/ CLOCKS_PER_SEC);

  return 0;
}
