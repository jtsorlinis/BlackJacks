#include <stdio.h>

#include "dealer.h"
#include "vector.h"
#include "card.h"

Dealer* Dealer__new() {
    Dealer* d = malloc(sizeof(Dealer));
    d->m_hand = Vector__new(5);
    d->m_player_num = "D";
    d->m_value = 0;
    d->m_aces = 0;
    d->m_is_soft = 0;
    return d;
}

void Dealer__reset_hand(Dealer* self) {
    Vector__clear(self->m_hand);
    self->m_value = 0;
}

int Dealer__up_card(Dealer* self) {
    return ((Card*)self->m_hand->items[0])->m_value;
}

void Dealer__evaluate(Dealer* self) {
  self->m_aces = 0;
  self->m_value = 0;
  for (int i = 0; i < self->m_hand->size; i++) {
    self->m_value += ((Card*)self->m_hand->items[i])->m_value;
    // Check for ace
    if (((Card*)self->m_hand->items[i])->m_is_ace) {
      self->m_aces++;
      self->m_is_soft = true;
    }
  }

  while (self->m_value > 21 && self->m_aces > 0) {
    self->m_value -= 10;
    self->m_aces--;
  }

  if (self->m_aces == 0) {
    self->m_is_soft = false;
  };
}

void Dealer__print(Dealer* self) {
  printf("Player %s: ", self->m_player_num);
  for (int i = 0; i < self->m_hand->size; i++) {
    printf("%s ", Card__print(self->m_hand->items[i]));
  }
  for (int i = self->m_hand->size; i < 5; i++) {
    printf("  ");
  }
  printf("\tScore: %d", self->m_value);
  if (self->m_value > 21) {
    printf(" (Bust) ");
  }
  printf("\n");
}