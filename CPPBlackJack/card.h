#pragma once
#include <string>

class Card {
public:
  Card(std::string rank, std::string suit);
  std::string print() const;
  int evaluate() const;
  int count() const;
  std::string m_rank_;
  std::string m_suit_;
  bool m_face_down_;
  int m_value_;
  int m_count_;
  bool m_is_ace_ = false;
};
