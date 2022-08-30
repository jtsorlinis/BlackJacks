#include "cardpile.h"

#include <chrono>

uint64_t state =
    std::chrono::high_resolution_clock::now().time_since_epoch().count();

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

CardPile::CardPile(const int num_of_decks) {
  for (auto x = 0; x < num_of_decks; x++) {
    Deck temp_deck;
    m_cards_.insert(m_cards_.end(), temp_deck.m_cards_.begin(),
                    temp_deck.m_cards_.end());
  }
  m_original_cards_ = m_cards_;
}

void CardPile::refresh() { m_cards_ = m_original_cards_; }

std::string CardPile::print() {
  std::string output;
  for (auto& i : m_cards_) {
    output += i->print() + "\n";
  }
  return output;
}

void CardPile::shuffle() {
  // Fisher yates
  for (auto i = static_cast<int>(m_cards_.size()) - 1; i > 0; i--) {
    const auto j = rand_range(i + 1);
    std::swap(m_cards_[i], m_cards_[j]);
  }
}
