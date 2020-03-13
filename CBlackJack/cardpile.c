#include "cardpile.h"
#include "cardarray.h"

char ranks[13][3] = {"A", "2", "3",  "4", "5", "6", "7",
                     "8", "9", "10", "J", "Q", "K"};
char suits[4][10] = {"Clubs", "Hearts", "Spades", "Diamonds"};

CardPile* CardPile__new(int numdecks) {
  CardPile* cp = malloc(sizeof(CardPile));
  cp->m_cards = CardArray__new(5);
  for (int i = 0; i < numdecks; i++) {
    for (int suit = 0; suit < 4; suit++) {
      for (int rank = 0; rank < 13; rank++) {
        CardArray__push(cp->m_cards, Card__new(ranks[rank], suits[suit]));
      }
    }
  }
  return cp;
}

void CardPile__print(CardPile* self) {
  CardArray__print(self->m_cards);
}