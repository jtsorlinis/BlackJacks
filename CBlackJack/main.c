#include <stdio.h>
#include <time.h>
#include "cardpile.h"
#include "player.h"
#include "table.h"
#include "vector.h"
#include "card.h"

int main(int argc, char const* argv[]) {

  clock_t start = clock();
  CardPile* cp = CardPile__new(8);


  Table t;
  t.m_bet_size = 10;
  Player* p = Player__new(&t, NULL);
  
  Vector__push(p->m_hand, Vector__last(cp->m_cards));
  Vector__pop(cp->m_cards);
  
  Player__evaluate(p);

  // CardPile__print(cp);

  Player__print(p);
  printf("Took %f seconds", (double)(clock()-start)/ CLOCKS_PER_SEC);

  return 0;
}
