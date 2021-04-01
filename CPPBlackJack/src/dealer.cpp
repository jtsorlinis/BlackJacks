#include "dealer.h"

#include <iostream>

#include "card.h"

Dealer::Dealer() {
  m_player_num_ = "D";
  m_value_ = 0;
  m_hide_second_ = true;
}

void Dealer::reset_hand() {
  m_hand_.clear();
  m_value_ = 0;
  m_hide_second_ = true;
}

int& Dealer::up_card() { return m_hand_[0]->m_value_; }
