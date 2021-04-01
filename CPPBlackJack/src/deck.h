#pragma once
#include <string>
#include <vector>

#include "card.h"

class Deck {
 public:
  Deck();
  std::vector<Card*> m_cards_;
  std::vector<std::string> m_ranks_;
  std::vector<std::string> m_suits_;
  std::string print();
  void shuffle();
};
