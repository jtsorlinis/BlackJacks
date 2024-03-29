#include "table.h"

#include <algorithm>
#include <iostream>

#include "strategies.h"

Table::Table(const int num_players, const int num_of_decks, const int bet_size,
             const int min_cards, const bool verbose)
    : m_card_pile_(CardPile(num_of_decks)) {
  m_verbose_ = verbose;
  m_bet_size_ = bet_size;
  m_num_of_decks_ = num_of_decks;
  m_min_cards_ = min_cards;
  m_dealer_ = Dealer();
  m_current_player_ = 0;
  m_casino_earnings_ = 0;
  m_running_count_ = 0;
  m_true_count_ = 0;

  m_players_.reserve(num_players * 3);
  for (auto i = 0; i < num_players; i++) {
    m_players_.emplace_back(this);
  }
  m_strat_hard_ = vec_to_map(strat_hard);
  m_strat_soft_ = vec_to_map(strat_soft);
  m_strat_split_ = vec_to_map(strat_split);
}

void Table::deal_round() {
  for (int i = 0; i < m_players_.size(); i++) {
    deal();
    m_current_player_++;
  }
  m_current_player_ = 0;
}

void Table::evaluate_all() {
  for (auto& player : m_players_) {
    player.evaluate();
  }
}

void Table::deal() {
  m_running_count_ += m_card_pile_.m_cards_.back()->m_count_;
  m_players_[m_current_player_].m_hand_.push_back(m_card_pile_.m_cards_.back());
  m_card_pile_.m_cards_.pop_back();
}

void Table::pre_deal() {
  for (auto& player : m_players_) {
    select_bet(&player);
  }
}

void Table::select_bet(Player* player) const {
  if (m_true_count_ >= 2) {
    player->m_initial_bet_ = m_bet_size_ * (m_true_count_ - 1);
  }
}

void Table::deal_dealer(const bool face_down) {
  m_card_pile_.m_cards_.back()->m_face_down_ = face_down;
  m_dealer_.m_hand_.push_back(m_card_pile_.m_cards_.back());
  if (!face_down) {
    m_running_count_ += m_card_pile_.m_cards_.back()->m_count_;
  }
  m_card_pile_.m_cards_.pop_back();
}

void Table::start_round() {
  clear();
  update_count();
  if (m_verbose_) {
    std::cout << m_card_pile_.m_cards_.size() << " cards left\n";
    std::cout << "Running count is: " << m_running_count_
              << "\tTrue count is: " << m_true_count_ << "\n";
  }
  get_new_cards();
  pre_deal();
  deal_round();
  deal_dealer();
  deal_round();
  deal_dealer(true);
  evaluate_all();
  m_current_player_ = 0;
  if (check_dealer_natural()) {
    finish_round();
  } else {
    check_player_natural();
    if (m_verbose_) {
      print();
    }
    auto_play();
  }
}

void Table::get_new_cards() {
  if (static_cast<int>(m_card_pile_.m_cards_.size()) < m_min_cards_) {
    m_card_pile_.refresh();
    m_card_pile_.shuffle();
    m_true_count_ = 0;
    m_running_count_ = 0;
    if (m_verbose_) {
      std::cout << "Got " << m_num_of_decks_
                << " new decks as number of cards is below " << m_min_cards_
                << "\n";
    }
  }
}

void Table::clear() {
  for (int i = m_players_.size() - 1; i >= 0; i--) {
    if (m_players_[i].m_split_from_ != nullptr) {
      m_players_[i - 1].m_earnings_ += m_players_[i].m_earnings_;
      m_players_.erase(m_players_.begin() + i);
    } else {
      m_players_[i].reset_hand();
    }
  }
  m_dealer_.reset_hand();
  m_current_player_ = 0;
}

void Table::update_count() {
  if (m_card_pile_.m_cards_.size() > 51) {
    m_true_count_ = m_running_count_ /
                    (static_cast<int>(m_card_pile_.m_cards_.size()) / 52);
  }
}

void Table::hit() {
  deal();
  m_players_[m_current_player_].evaluate();
  if (m_verbose_) {
    std::cout << "Player " << m_players_[m_current_player_].m_player_num_
              << " hits\n";
  }
}

void Table::stand() {
  if (m_verbose_ && m_players_[m_current_player_].m_value_ <= 21) {
    std::cout << "Player " << m_players_[m_current_player_].m_player_num_
              << " stands\n";
    print();
  }
  m_players_[m_current_player_].m_is_done_ = true;
}

void Table::split() {
  Player split_player(this, &m_players_[m_current_player_]);
  m_players_[m_current_player_].m_hand_.pop_back();
  m_players_.insert(m_players_.begin() + m_current_player_ + 1,
                    std::move(split_player));
  m_players_[m_current_player_].evaluate();
  m_players_[m_current_player_ + 1].evaluate();
  if (m_verbose_) {
    std::cout << "Player " << m_players_[m_current_player_].m_player_num_
              << " splits\n";
  }
}

void Table::split_aces() {
  if (m_verbose_) {
    std::cout << "Player " << m_players_[m_current_player_].m_player_num_
              << " splits aces\n";
  }
  Player split_player(this, &m_players_[m_current_player_]);
  m_players_[m_current_player_].m_hand_.pop_back();
  m_players_.insert(m_players_.begin() + m_current_player_ + 1,
                    std::move(split_player));
  deal();
  m_players_[m_current_player_].evaluate();
  stand();
  ++m_current_player_;
  deal();
  m_players_[m_current_player_].evaluate();
  stand();
  if (m_verbose_) {
    print();
  }
}

void Table::double_bet() {
  if (m_players_[m_current_player_].m_bet_mult_ < 1.1 &&
      m_players_[m_current_player_].m_hand_.size() == 2) {
    m_players_[m_current_player_].double_bet();
    if (m_verbose_) {
      std::cout << "Player " << m_players_[m_current_player_].m_player_num_
                << " doubles\n";
    }
    hit();
    stand();
  } else {
    hit();
  }
}

void Table::auto_play() {
  while (!m_players_[m_current_player_].m_is_done_) {
    // check if player just Split
    if (m_players_[m_current_player_].m_hand_.size() == 1) {
      if (m_verbose_) {
        std::cout << "Player " << m_players_[m_current_player_].m_player_num_
                  << " gets 2nd card after splitting\n";
      }
      deal();
      m_players_[m_current_player_].evaluate();
    }

    if (m_players_[m_current_player_].m_hand_.size() < 5 &&
        m_players_[m_current_player_].m_value_ < 21) {
      auto split_card_val = m_players_[m_current_player_].can_split();
      if (split_card_val == 11) {
        split_aces();
      } else if (split_card_val != 0 &&
                 (split_card_val != 5 && split_card_val != 10)) {
        action(
            get_action(split_card_val, m_dealer_.up_card(), &m_strat_split_));
      } else if (m_players_[m_current_player_].m_is_soft_) {
        action(get_action(m_players_[m_current_player_].m_value_,
                          m_dealer_.up_card(), &m_strat_soft_));
      } else {
        action(get_action(m_players_[m_current_player_].m_value_,
                          m_dealer_.up_card(), &m_strat_hard_));
      }
    } else {
      stand();
    }
  }
  next_player();
}

void Table::action(char action) {
  switch (action) {
    case 'H':
      hit();
      break;
    case 'S':
      stand();
      break;
    case 'D':
      double_bet();
      break;
    case 'P':
      split();
      break;
    default:
      std::cout << "No action found";
      exit(1);
  }
}

void Table::dealer_play() {
  auto all_busted = true;
  for (auto& player : m_players_) {
    if (player.m_value_ < 22) {
      all_busted = false;
      break;
    }
  }
  m_dealer_.m_hand_[1]->m_face_down_ = false;
  m_running_count_ += m_dealer_.m_hand_[1]->m_count_;
  m_dealer_.evaluate();
  if (m_verbose_) {
    std::cout << "Dealer's turn\n";
    print();
  }
  if (all_busted) {
    if (m_verbose_) {
      std::cout << "Dealer automatically wins cause all players busted\n";
    }
    finish_round();
  } else {
    while (m_dealer_.m_value_ < 17 && m_dealer_.m_hand_.size() < 5) {
      deal_dealer();
      m_dealer_.evaluate();
      if (m_verbose_) {
        std::cout << "Dealer hits\n";
        print();
      }
    }
    finish_round();
  }
}

void Table::next_player() {
  if (m_current_player_ < m_players_.size() - 1) {
    ++m_current_player_;
    auto_play();
  } else {
    dealer_play();
  }
}

void Table::check_player_natural() {
  for (auto& player : m_players_) {
    if (player.m_value_ == 21 && player.m_hand_.size() == 2 &&
        player.m_split_from_ == nullptr) {
      player.m_has_natural_ = true;
    }
  }
}

bool Table::check_dealer_natural() {
  if (m_dealer_.evaluate() == 21) {
    m_dealer_.m_hand_[1]->m_face_down_ = false;
    m_running_count_ += m_dealer_.m_hand_[1]->m_count_;
    if (m_verbose_) {
      print();
      std::cout << "Dealer has a natural 21\n\n";
    }
    return true;
  }
  return false;
}

void Table::check_earnings() {
  float check = 0;
  for (auto& player : m_players_) {
    check += player.m_earnings_;
  }
  if (check + m_casino_earnings_ != 0) {
    std::cout << "NO MATCH\t Casino earnings: " << m_casino_earnings_
              << "\t Player earnings: " << check << "\n";
    exit(1);
  }
}

void Table::finish_round() {
  if (m_verbose_) {
    std::cout << "Scoring round\n";
  }
  for (auto& player : m_players_) {
    if (player.m_has_natural_) {
      player.win(1.5);
      if (m_verbose_) {
        std::cout << "Player " << player.m_player_num_ << " Wins "
                  << 1.5 * player.m_bet_mult_ * player.m_initial_bet_
                  << " with a natural 21\n";
      }
    } else if (player.m_value_ > 21) {
      player.lose();
      if (m_verbose_) {
        std::cout << "Player " << player.m_player_num_ << " Busts and Loses "
                  << player.m_bet_mult_ *
                         static_cast<float>(player.m_initial_bet_)
                  << "\n";
      }

    } else if (m_dealer_.m_value_ > 21 ||
               player.m_value_ > m_dealer_.m_value_) {
      player.win();
      if (m_verbose_) {
        std::cout << "Player " << player.m_player_num_ << " Wins "
                  << player.m_bet_mult_ *
                         static_cast<float>(player.m_initial_bet_)
                  << "\n";
      }
    } else if (player.m_value_ == m_dealer_.m_value_) {
      if (m_verbose_) {
        std::cout << "Player " << player.m_player_num_ << " Draws\n";
      }
    } else {
      player.lose();
      if (m_verbose_) {
        std::cout << "Player " << player.m_player_num_ << " Loses "
                  << player.m_bet_mult_ *
                         static_cast<float>(player.m_initial_bet_)
                  << "\n";
      }
    }
  }
  if (m_verbose_) {
    for (auto& player : m_players_) {
      if (player.m_split_from_ == nullptr) {
        std::cout << "Player " << player.m_player_num_
                  << " Earnings: " << player.m_earnings_ << "\n";
      }
    }
    std::cout << "\n";
  }
}

void Table::print() {
  for (auto& player : m_players_) {
    std::cout << player.print() + "\n";
  }
  std::cout << m_dealer_.print() + "\n\n";
}
