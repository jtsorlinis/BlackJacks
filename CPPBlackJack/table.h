#pragma once
#include "dealer.h"
#include "cardpile.h"
#include <list>
#include <unordered_map>

class Table {
public:
  int m_verbose_;
  int m_bet_size_;
  std::list<Player> m_players_;
  int m_num_of_decks_;
  CardPile m_card_pile_;
  int m_min_cards_;
  Dealer m_dealer_;
  std::list<Player>::iterator m_current_player_;
  float m_casino_earnings_;
  int m_running_count_;
  int m_true_count_;
  std::unordered_map<int, std::string> m_strat_hard_;
  std::unordered_map<int, std::string> m_strat_soft_;
  std::unordered_map<int, std::string> m_strat_split_;

  Table(int num_players, int num_of_decks, int bet_size, int min_cards,
        int verbose = 0);
  void deal_round();
  void evaluate_all();
  void deal();
  void pre_deal();
  void select_bet(Player* player) const;
  void deal_dealer(bool face_down = false);
  void start_round();
  void get_new_cards();
  void clear();
  void update_count();
  void hit();
  void stand();
  void split();
  void split_aces();
  void double_bet();
  void auto_play();
  void action(std::string const& action);
  void dealer_play();
  void next_player();
  void check_player_natural();
  bool check_dealer_natural();
  void check_earnings();
  void finish_round();
  void print();
};
