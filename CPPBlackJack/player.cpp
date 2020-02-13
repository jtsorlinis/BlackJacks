#include "player.h"
#include "table.h"

int Player::player_num_count_ = 0;

Player::Player(Table* table, Player* split) {
  m_value_ = 0;
  m_earnings_ = 0;
  m_aces_ = 0;
  m_is_soft_ = false;
  m_split_count_ = 0;
  m_is_done_ = false;
  m_split_from_ = nullptr;
  m_bet_mult_ = 1;
  m_has_natural_ = false;
  m_table_ = table;
  m_initial_bet_ = 0;
  if (m_table_ != nullptr) {
    m_initial_bet_ = m_table_->m_bet_size_;
    if (split != nullptr) {
      m_hand_.push_back(split->m_hand_[1]);
      m_split_count_++;
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
  m_split_count_ = 0;
  m_is_done_ = false;
  m_bet_mult_ = 1;
  m_has_natural_ = false;
  m_initial_bet_ = m_table_->m_bet_size_;
}

int Player::can_split() {
  if (m_hand_.size() == 2 && m_hand_[0]->m_rank_ == m_hand_[1]->m_rank_ &&
      m_split_count_ < max_splits_) {
    return m_hand_[0]->m_value_;
  }
  return 0;
}

void Player::win(const float mult) {
  if (m_split_from_ != nullptr) {
    m_split_from_->win(mult);
  } else {
    m_earnings_ += static_cast<float>(m_initial_bet_) * m_bet_mult_ * mult;
    m_table_->m_casino_earnings_ -=
        static_cast<float>(m_initial_bet_) * m_bet_mult_ * mult;
  }
}

void Player::lose() {
  if (m_split_from_ != nullptr) {
    m_split_from_->lose();
  } else {
    m_earnings_ -= static_cast<float>(m_initial_bet_) * m_bet_mult_;
    m_table_->m_casino_earnings_ += static_cast<float>(m_initial_bet_) *
        m_bet_mult_;
  }
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
    output +=
        "\tBet: " + std::to_string(
            static_cast<float>(m_initial_bet_) * m_bet_mult_);
  }
  return output;
}

int Player::evaluate() {
  m_aces_ = 0;
  m_value_ = 0;
  for (auto& card : m_hand_) {
    m_value_ += card->m_value_;
    // Check for ace
    if (card->m_is_ace_) {
      m_aces_++;
      m_is_soft_ = true;
    }
  }

  while (m_value_ > 21 && m_aces_ > 0) {
    m_value_ -= 10;
    m_aces_--;
  }

  if (m_aces_ == 0) {
    m_is_soft_ = false;
  }

  return m_value_;
}
