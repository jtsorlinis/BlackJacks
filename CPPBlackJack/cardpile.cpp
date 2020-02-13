#include "cardpile.h"

#include <ctime>
#include <random>

#include "QuickRand.h"

CardPile::CardPile(const int num_of_decks) {
  for (auto x = 0; x < num_of_decks; x++) {
    Deck temp_deck;
    m_cards_.insert(m_cards_.end(), temp_deck.m_cards_.begin(),
                    temp_deck.m_cards_.end());
  }
  m_original_cards_ = m_cards_;
  m_num_of_decks_ = num_of_decks;
  fast_srand(static_cast<int>(time(nullptr)));
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
  // std::random_device rd;
  // std::shuffle(mCards.begin(), mCards.end(), rd);

  // Fisher yates
  for (auto i = static_cast<int>(m_cards_.size()) - 1; i > 0; i--) {
    const auto j = fast_rand() % (i + 1);
    std::swap(m_cards_[i], m_cards_[j]);
  }
}
