#include "card.h"

#include <iostream>

Card::Card(std::string rank, std::string suit) {
  m_rank_ = std::move(rank);
  m_suit_ = std::move(suit);
  m_face_down_ = false;
  m_value_ = evaluate();
  m_count_ = count();
  if (m_rank_ == "A") {
    m_is_ace_ = true;
  }
}

std::string Card::print() const {
  if (m_face_down_) {
    return "X";
  }
  return m_rank_;
}

int Card::evaluate() const {
  if (m_rank_ == "J" || m_rank_ == "Q" || m_rank_ == "K") {
    return 10;
  }
  if (m_rank_ == "A") {
    return 11;
  }
  return std::stoi(m_rank_);
}

int Card::count() const {
  if (m_rank_ == "10" || m_rank_ == "J" || m_rank_ == "Q" || m_rank_ == "K" ||
      m_rank_ == "A") {
    return -1;
  }
  if (m_rank_ == "7" || m_rank_ == "8" || m_rank_ == "9") {
    return 0;
  }
  return 1;
}
