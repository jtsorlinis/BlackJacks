#pragma once
#include <string>
#include <vector>

class Table;
class Card;

class Player {
 public:
  static int player_num_count_;
  const static int max_splits_ = 10;

  std::string m_player_num_;
  std::vector<Card*> m_hand_;
  int m_value_;
  float m_earnings_;
  int m_aces_;
  bool m_is_soft_;
  int m_split_count_;
  bool m_is_done_;
  Player* m_split_from_;
  float m_bet_mult_;
  bool m_has_natural_;
  Table* m_table_;
  int m_initial_bet_;

  explicit Player(Table* table = nullptr, Player* split = nullptr);
  void double_bet();
  void reset_hand();
  int can_split();
  void win(float mult = 1);
  void lose();
  std::string print();
  int evaluate();
};
