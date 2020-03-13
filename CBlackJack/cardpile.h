#include "cardarray.h"

typedef struct {
    CardArray* m_cards;
    CardArray* m_original_cards;
} CardPile;

char ranks[13][3] = {"A", "2", "3",  "4", "5", "6", "7",
                     "8", "9", "10", "J", "Q", "K"};
char suits[4][10] = {"Clubs", "Hearts", "Spades", "Diamonds"};

CardPile* CardPile__new(int numdecks) {
  CardPile* cp = malloc(sizeof(CardPile));
  cp->m_cards = CardArray__new(8);
  for (int i = 0; i < numdecks; i++) {
    for (int suit = 0; suit < 4; suit++) {
      for (int rank = 0; rank < 13; rank++) {
        CardArray__push(cp->m_cards, Card__new(ranks[rank], suits[suit]));
      }
    }
  }
  cp->m_original_cards = CardArray__copy(cp->m_cards);
  return cp;
}

void CardPile__refresh(CardPile* self) {
    free(self->m_cards->array);
    free(self->m_cards);
    self->m_cards = CardArray__copy(self->m_original_cards);
}

void CardPile__print(CardPile* self) {
  CardArray__print(self->m_cards);
}

void CardPile__shuffle(CardPile* self) {
  CardArray__shuffle(self->m_cards);
}