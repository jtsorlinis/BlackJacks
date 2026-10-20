#include "player.h"

#include "table.h"

int Player::player_num_count_ = 0;

Player::Player(Table* table, Player* split)
    : m_split_count_(split != nullptr ? split->m_split_count_
                                      : std::make_shared<int>(0)) {
  m_hand_.reserve(5);
  m_value_ = 0;
  m_earnings_ = 0;
  m_aces_ = 0;
  m_is_soft_ = false;
  m_is_done_ = false;
  m_split_from_ = nullptr;
  m_bet_mult_ = 1;
  m_has_natural_ = false;
  m_initial_bet_ = 0;
  m_original_bet_ = 0;
  if (table != nullptr) {
    m_initial_bet_ = table->m_bet_size_;
    m_original_bet_ = table->m_bet_size_;
    if (split != nullptr) {
      ++(*m_split_count_);
      m_hand_.push_back(split->m_hand_[1]);
      m_player_num_ = split->m_player_num_ + "S";
      m_initial_bet_ = split->m_initial_bet_;
      m_split_from_ = split;
    } else {
      player_num_count_++;
      m_player_num_ = std::to_string(player_num_count_);
    }
  }
}

void Player::double_bet() { m_bet_mult_ = 2; }

void Player::reset_hand() {
  m_hand_.clear();
  m_value_ = 0;
  m_aces_ = 0;
  m_is_soft_ = false;
  *m_split_count_ = 0;
  m_is_done_ = false;
  m_bet_mult_ = 1;
  m_has_natural_ = false;
  m_initial_bet_ = m_original_bet_;
}

int Player::can_split() {
  if (m_hand_.size() == 2 && m_hand_[0]->m_rank_id_ == m_hand_[1]->m_rank_id_ &&
      *m_split_count_ < max_hands_ - 1) {
    return m_hand_[0]->m_value_;
  }
  return 0;
}

double Player::win(const double mult) {
  double x = static_cast<double>(m_initial_bet_) * m_bet_mult_ * mult;
  m_earnings_ += x;
  return -x;
}

double Player::lose() {
  double x = static_cast<double>(m_initial_bet_) * m_bet_mult_;
  m_earnings_ -= x;
  return x;
}

std::string Player::print() {
  auto output = "Player " + m_player_num_ + ": ";
  for (auto& i : m_hand_) {
    output += i->print() + " ";
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
  if (m_player_num_ != "D") {
    output += "\tBet: " + std::to_string(static_cast<int>(
                              static_cast<double>(m_initial_bet_) * m_bet_mult_));
  }
  return output;
}

int Player::evaluate() {
  auto value = 0;
  auto aces = 0;
  for (auto& card : m_hand_) {
    value += card->m_value_;
    aces += card->m_is_ace_;
  }

  while (value > 21 && aces > 0) {
    value -= 10;
    aces--;
  }

  m_value_ = value;
  m_aces_ = aces;
  m_is_soft_ = aces > 0;

  return m_value_;
}
