#ifndef CARDPILE_H
#define CARDPILE_H

#include "vector.h"

typedef struct CardPile {
  Vector m_cards;
  Vector m_original_cards;
} CardPile;

void CardPile__init(CardPile* self, int numdecks);
void CardPile__refresh(CardPile* self);
void CardPile__print(CardPile* self);
void CardPile__shuffle(CardPile* self);

#endif
