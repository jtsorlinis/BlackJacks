#include "cardpile.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "card.h"
#include "vector.h"

char* ranks[] = {"A", "2", "3",  "4", "5", "6", "7",
                 "8", "9", "10", "J", "Q", "K"};
char* suits[] = {"Clubs", "Hearts", "Spades", "Diamonds"};

uint64_t state;

// From https://www.pcg-random.org/download.html#minimal-c-implementation
uint32_t pcg32() {
  uint64_t oldstate = state;
  // Advance internal state
  state = oldstate * 6364136223846793005ULL + 1;
  // Calculate output function (XSH RR), uses old state for max ILP
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

// use nearly divisionless technique found here
// https://github.com/lemire/FastShuffleExperiments
uint32_t pcg32_range(uint32_t s) {
  uint32_t x = pcg32();
  uint64_t m = (uint64_t)x * (uint64_t)s;
  uint32_t l = (uint32_t)m;
  if (l < s) {
    uint32_t t = -s % s;
    while (l < t) {
      x = pcg32();
      m = (uint64_t)x * (uint64_t)s;
      l = (uint32_t)m;
    }
  }
  return m >> 32;
}

CardPile* CardPile__new(int numdecks) {
  state = time(NULL);
  CardPile* cp = malloc(sizeof(CardPile));
  cp->m_cards = Vector__new(52 * numdecks);
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
  Vector__free(self->m_cards);
  self->m_cards = Vector__copy(self->m_original_cards);
}

void CardPile__print(CardPile* self) {
  for (int i = 0; i < self->m_cards->size; i++) {
    printf("%s", ((Card*)self->m_cards->items[i])->m_rank);
  }
  printf("\n");
}

void CardPile__shuffle(CardPile* self) {
  for (int i = self->m_cards->size - 1; i > 0; i--) {
    int j = pcg32_range(i + 1);
    Card* temp = self->m_cards->items[i];
    self->m_cards->items[i] = self->m_cards->items[j];
    self->m_cards->items[j] = temp;
  }
}