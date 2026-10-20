#ifndef DEALER_H
#define DEALER_H

#include <stdbool.h>

#include "vector.h"

typedef struct Dealer {
  Vector m_hand;
  char* m_player_num;
  int m_value;
  int m_aces;
  bool m_is_soft;
  bool m_hide;
} Dealer;

void Dealer__init(Dealer* self);
void Dealer__reset_hand(Dealer* self);
int Dealer__up_card(Dealer* self);
void Dealer__evaluate(Dealer* self);
void Dealer__print(Dealer* self);

#endif
