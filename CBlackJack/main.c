#include <stdio.h>

#include "cardpile.h"

int main(int argc, char const* argv[]) {
  CardPile* cp = CardPile__new(1);
  CardPile__print(cp);

  return 0;
}
