#pragma once
#include <memory>
#include <string>
#include <vector>

class Table;
class Card;

class Player {
 public:
  static int player_num_count_;
  const static int max_hands_ = 4;

  std::string m_player_num_;
  std::vector<Card*> m_hand_;
  int m_value_;
  double m_earnings_;
  int m_aces_;
  bool m_is_soft_;
  std::shared_ptr<int> m_split_count_;
  bool m_is_done_;
  Player* m_split_from_;
  double m_bet_mult_;
  bool m_has_natural_;
  int m_initial_bet_;
  int m_original_bet_;

  explicit Player(Table* table = nullptr, Player* split = nullptr);
  void double_bet();
  void reset_hand();
  int can_split();
  double settle(double mult);
  std::string print();
  int evaluate();
};
