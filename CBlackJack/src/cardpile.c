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

// From https://github.com/lemire/testingRNG
uint64_t wyrand() {
  state += UINT64_C(0xa0761d6478bd642f);
  __uint128_t t = (__uint128_t)state * (state ^ UINT64_C(0xe7037ed1a0b428db));
  return (t >> 64) ^ t;
}

// use nearly divisionless technique found here
// https://github.com/lemire/FastShuffleExperiments
uint64_t rand_range(uint64_t s) {
  uint64_t x = wyrand();
  __uint128_t m = (__uint128_t)x * (__uint128_t)s;
  uint64_t l = (uint64_t)m;
  if (l < s) {
    uint64_t t = -s % s;
    while (l < t) {
      x = wyrand();
      m = (__uint128_t)x * (__uint128_t)s;
      l = (uint64_t)m;
    }
  }
  return m >> 64;
}

CardPile* CardPile__new(int numdecks) {
  state = time(NULL);
  CardPile* cp = malloc(sizeof(CardPile));
  cp->m_original_cards = Vector__new(52 * numdecks);
  cp->m_cards = Vector__new(52 * numdecks);
  for (int i = 0; i < numdecks; i++) {
    for (int suit = 0; suit < 4; suit++) {
      for (int rank = 0; rank < 13; rank++) {
        Vector__push(cp->m_original_cards, Card__new(ranks[rank], suits[suit]));
      }
    }
  }
  CardPile__refresh(cp);
  return cp;
}

void CardPile__refresh(CardPile* self) {
  Vector__copy(self->m_original_cards, self->m_cards);
}

void CardPile__print(CardPile* self) {
  for (int i = 0; i < self->m_cards->size; i++) {
    printf("%s", ((Card*)self->m_cards->items[i])->m_rank);
  }
  printf("\n");
}

void CardPile__shuffle(CardPile* self) {
  for (int i = self->m_cards->size - 1; i > 0; i--) {
    int j = rand_range(i + 1);
    Card* temp = self->m_cards->items[i];
    self->m_cards->items[i] = self->m_cards->items[j];
    self->m_cards->items[j] = temp;
  }
}