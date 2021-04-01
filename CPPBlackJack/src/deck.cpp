#include "deck.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

Deck::Deck() {
  m_ranks_ = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  m_suits_ = {"Clubs", "Hearts", "Spades", "Diamonds"};
  for (auto& suit : m_suits_) {
    for (auto& rank : m_ranks_) {
      m_cards_.push_back(new Card(rank, suit));
    }
  }
}

std::string Deck::print() {
  std::string output;
  for (auto& card : m_cards_) {
    output += card->print() + "\n";
  }
  return output;
}

void Deck::shuffle() {
  std::random_device rd;
  std::shuffle(m_cards_.begin(), m_cards_.end(), rd);
}
