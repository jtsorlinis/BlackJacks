#include "cardpile.h"
#include "vector.h"
#include "card.h"

#include <time.h>
#include <stdio.h>

char ranks[13][3] = {"A", "2", "3",  "4", "5", "6", "7",
                     "8", "9", "10", "J", "Q", "K"};
char suits[4][10] = {"Clubs", "Hearts", "Spades", "Diamonds"};

unsigned int seed;

unsigned int xorshift() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

CardPile* CardPile__new(int numdecks) {
  seed = time(NULL);
  CardPile* cp = malloc(sizeof(CardPile));
  cp->m_cards = Vector__new(52*numdecks);
  for (int i = 0; i < numdecks; i++) {
    for (int suit = 0; suit < 4; suit++) {
      for (int rank = 0; rank < 13; rank++) {
        Vector__push(cp->m_cards, Card__new(ranks[rank], suits[suit]));
      }
    }
  }
  cp->m_original_cards = Vector__copy(cp->m_cards);
  return cp;
}

void CardPile__refresh(CardPile* self) {
    free(self->m_cards->items);
    free(self->m_cards);
    self->m_cards = Vector__copy(self->m_original_cards);
}

void CardPile__print(CardPile* self) {
  for (int i = 0; i < self->m_cards->size; i++) {
    printf("%s", ((Card*) self->m_cards->items[i])->mRank);
  }
  printf("\n");
}

void CardPile__shuffle(CardPile* self) {
  for (int i = self->m_cards->size - 1; i > 0; i--) {
    int j = xorshift() % (i + 1);
    Card* temp = self->m_cards->items[i];
    self->m_cards->items[i] = self->m_cards->items[j];
    self->m_cards->items[j] = temp;
  }
}