#include "dealer.h"

#include <iostream>

#include "card.h"

Dealer::Dealer() {
  m_hand_.reserve(5);
  m_player_num_ = "D";
  m_value_ = 0;
  m_hide_second_ = true;
}

void Dealer::reset_hand() {
  m_hand_.clear();
  m_value_ = 0;
  m_hide_second_ = true;
}

int Dealer::up_card() { return m_hand_[0]->m_value_; }

std::string Dealer::print() {
  auto output = "Player " + m_player_num_ + ": ";
  for (auto i = 0; i < static_cast<int>(m_hand_.size()); i++) {
    if (i == 1 && m_hide_second_) {
      output += "X ";
    } else {
      output += m_hand_[i]->print() + " ";
    }
  }
  for (auto i = static_cast<int>(m_hand_.size()); i < 5; i++) {
    output += "  ";
  }
  output += "\tScore: " + std::to_string(m_value_);
  if (m_value_ > 21) {
    output += " (Bust) ";
  } else {
    output += "        ";
  }
  return output;
}
