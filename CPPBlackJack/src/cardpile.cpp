#include "cardpile.h"

#include <chrono>

uint64_t state =
    std::chrono::high_resolution_clock::now().time_since_epoch().count();

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

CardPile::CardPile(const int num_of_decks) {
  for (auto x = 0; x < num_of_decks; x++) {
    Deck temp_deck;
    m_cards_.insert(m_cards_.end(), temp_deck.m_cards_.begin(),
                    temp_deck.m_cards_.end());
  }
  m_original_cards_ = m_cards_;
  m_num_of_decks_ = num_of_decks;
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
    const auto j = pcg32_range(i + 1);
    std::swap(m_cards_[i], m_cards_[j]);
  }
}
