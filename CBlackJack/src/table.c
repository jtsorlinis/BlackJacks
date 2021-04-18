#include "table.h"

#include <stdio.h>
#include <stdlib.h>

#include "card.h"
#include "cardpile.h"
#include "dealer.h"
#include "player.h"
#include "strategies.h"
#include "vector.h"

Table* Table__new(int num_players, int num_decks, int bet_size, int min_cards,
                  int verbose) {
  Table* t = malloc(sizeof(Table));
  t->m_card_pile = CardPile__new(num_decks);
  t->m_verbose = verbose;
  t->m_bet_size = bet_size;
  t->m_num_decks = num_decks;
  t->m_min_cards = min_cards;
  t->m_casino_earnings = 0;
  t->m_current_player = 0;
  t->m_running_count = 0;
  t->m_true_count = 0;

  t->m_strat_hard = array_to_map(strat_hard, 21);
  t->m_strat_soft = array_to_map(strat_soft, 10);
  t->m_strat_split = array_to_map(strat_split, 9);

  t->m_players = Vector__new(num_players);
  for (int i = 0; i < num_players; i++) {
    Vector__push(t->m_players, Player__new(t, NULL));
  }
  t->m_dealer = Dealer__new();

  return t;
}

void Table__deal_round(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Table__deal(self);
    self->m_current_player++;
  }
  self->m_current_player = 0;
}

void Table__evaluate_all(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Player__evaluate(self->m_players->items[i]);
  }
}

void Table__deal(Table* self) {
  Card* card = Vector__last(self->m_card_pile->m_cards);
  Vector__push(
      ((Player*)self->m_players->items[self->m_current_player])->m_hand, card);
  self->m_running_count += card->m_count;
  Vector__pop(self->m_card_pile->m_cards);
}

void Table__pre_deal(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Table__select_bet(self, self->m_players->items[i]);
  }
}

void Table__select_bet(Table* self, Player* player) {
  if (self->m_true_count >= 2) {
    player->m_initial_bet = self->m_bet_size * (self->m_true_count - 1);
  }
}

void Table__deal_dealer(Table* self, bool face_down) {
  Card* card = Vector__last(self->m_card_pile->m_cards);
  card->m_face_down = face_down;
  Vector__push(self->m_dealer->m_hand, card);
  if (!face_down) {
    self->m_running_count += card->m_count;
  }
  Vector__pop(self->m_card_pile->m_cards);
}

void Table__start_round(Table* self) {
  Table__clear(self);
  Table__update_count(self);
  if (self->m_verbose) {
    printf("%d cards left\n", self->m_card_pile->m_cards->size);
    printf("Running count is: %d\tTrue count is: %d\n", self->m_running_count,
           self->m_true_count);
  }

  Table__get_new_cards(self);
  Table__pre_deal(self);
  Table__deal_round(self);
  Table__deal_dealer(self, false);
  Table__deal_round(self);
  Table__deal_dealer(self, true);
  Table__evaluate_all(self);
  self->m_current_player = 0;
  if (Table__check_dealer_natural(self)) {
    Table__finish_round(self);
  } else {
    Table__check_player_natural(self);
    if (self->m_verbose) Table__print(self);
    Table__auto_play(self);
  }
}

void Table__get_new_cards(Table* self) {
  if (self->m_card_pile->m_cards->size >= self->m_min_cards) return;
  CardPile__refresh(self->m_card_pile);
  CardPile__shuffle(self->m_card_pile);
  self->m_true_count = 0;
  self->m_running_count = 0;
  if (self->m_verbose) {
    printf("Got %d new decks as number of cards left is below %d\n",
           self->m_num_decks, self->m_min_cards);
  }
}

void Table__clear(Table* self) {
  for (int i = self->m_players->size - 1; i >= 0; i--) {
    if (((Player*)self->m_players->items[i])->m_split_from != NULL) {
      ((Player*)self->m_players->items[i - 1])->m_earnings +=
          ((Player*)self->m_players->items[i])->m_earnings;
      Vector__delete(self->m_players, i);
    } else {
      Player__reset_hand(self->m_players->items[i]);
    }
  }
  Dealer__reset_hand(self->m_dealer);
  self->m_current_player = 0;
}

void Table__update_count(Table* self) {
  if (self->m_card_pile->m_cards->size > 51) {
    self->m_true_count =
        self->m_running_count / (self->m_card_pile->m_cards->size / 52);
  }
}

void Table__hit(Table* self) {
  Table__deal(self);
  Player__evaluate(self->m_players->items[self->m_current_player]);
  if (self->m_verbose)
    printf("Player %s hits\n",
           ((Player*)self->m_players->items[self->m_current_player])
               ->m_player_num);
}

void Table__stand(Table* self) {
  if (self->m_verbose &&
      ((Player*)self->m_players->items[self->m_current_player])->m_value <=
          21) {
    printf("Player %s stands\n",
           ((Player*)self->m_players->items[self->m_current_player])
               ->m_player_num);
  }
  ((Player*)self->m_players->items[self->m_current_player])->m_is_done = true;
}

void Table__split(Table* self) {
  Player* split_player =
      Player__new(self, self->m_players->items[self->m_current_player]);
  Vector__insert(self->m_players, split_player, self->m_current_player + 1);
  Player__evaluate(self->m_players->items[self->m_current_player]);
  Player__evaluate(self->m_players->items[self->m_current_player + 1]);
  if (self->m_verbose)
    printf("Player %s splits\n",
           ((Player*)self->m_players->items[self->m_current_player])
               ->m_player_num);
}

void Table__split_aces(Table* self) {
  if (self->m_verbose)
    printf("Player %s splits aces\n",
           ((Player*)self->m_players->items[self->m_current_player])
               ->m_player_num);
  Player* split_player =
      Player__new(self, self->m_players->items[self->m_current_player]);
  Vector__insert(self->m_players, split_player, self->m_current_player + 1);
  Table__deal(self);
  Player__evaluate(self->m_players->items[self->m_current_player]);
  Table__stand(self);
  self->m_current_player++;
  Table__deal(self);
  Player__evaluate(self->m_players->items[self->m_current_player]);
  Table__stand(self);
  if (self->m_verbose) Table__print(self);
}

void Table__double_bet(Table* self) {
  if (((Player*)self->m_players->items[self->m_current_player])->m_bet_mult ==
          1 &&
      ((Player*)self->m_players->items[self->m_current_player])->m_hand->size ==
          2) {
    Player__double_bet(self->m_players->items[self->m_current_player]);
    if (self->m_verbose)
      printf("Player %s doubles\n",
             ((Player*)self->m_players->items[self->m_current_player])
                 ->m_player_num);
    Table__hit(self);
    Table__stand(self);
  } else {
    Table__hit(self);
  }
}

void Table__auto_play(Table* self) {
  while (
      !((Player*)self->m_players->items[self->m_current_player])->m_is_done) {
    // check if player just split
    if (((Player*)self->m_players->items[self->m_current_player])
            ->m_hand->size == 1) {
      Table__deal(self);
      Player__evaluate(self->m_players->items[self->m_current_player]);
    }

    if (((Player*)self->m_players->items[self->m_current_player])
                ->m_hand->size < 5 &&
        ((Player*)self->m_players->items[self->m_current_player])->m_value <
            21) {
      int split_card_val =
          Player__can_split(self->m_players->items[self->m_current_player]);
      if (split_card_val == 11) {
        Table__split_aces(self);
      } else if (split_card_val != 0 && split_card_val != 5 &&
                 split_card_val != 10) {
        Table__action(
            self, get_action(split_card_val, Dealer__up_card(self->m_dealer),
                             self->m_strat_split));
      } else if (((Player*)self->m_players->items[self->m_current_player])
                     ->m_is_soft) {
        Table__action(
            self,
            get_action(((Player*)self->m_players->items[self->m_current_player])
                           ->m_value,
                       Dealer__up_card(self->m_dealer), self->m_strat_soft));
      } else {
        Table__action(
            self,
            get_action(((Player*)self->m_players->items[self->m_current_player])
                           ->m_value,
                       Dealer__up_card(self->m_dealer), self->m_strat_hard));
      }
    } else {
      Table__stand(self);
    }
  }
  Table__next_player(self);
}

void Table__action(Table* self, const char action) {
  switch (action) {
    case 'H':
      Table__hit(self);
      break;
    case 'S':
      Table__stand(self);
      break;
    case 'D':
      Table__double_bet(self);
      break;
    case 'P':
      Table__split(self);
      break;
    default:
      printf("No action found\n");
      exit(1);
  }
}

void Table__dealer_play(Table* self) {
  bool all_busted = true;
  for (int i = 0; i < self->m_players->size; i++) {
    if (((Player*)self->m_players->items[i])->m_value < 22) {
      all_busted = false;
      break;
    }
  }
  ((Card*)self->m_dealer->m_hand->items[1])->m_face_down = false;
  self->m_running_count += ((Card*)self->m_dealer->m_hand->items[1])->m_count;
  Dealer__evaluate(self->m_dealer);
  if (self->m_verbose) {
    printf("Dealer's turn\n");
    Table__print(self);
  }

  if (all_busted) {
    if (self->m_verbose)
      printf("Dealer automatically wins cause all players busted\n");
    Table__finish_round(self);
  } else {
    while (self->m_dealer->m_value < 17 && self->m_dealer->m_hand->size < 5) {
      Table__deal_dealer(self, false);
      Dealer__evaluate(self->m_dealer);
      if (self->m_verbose) {
        printf("Dealer hits\n");
        Table__print(self);
      }
    }

    Table__finish_round(self);
  }
}

void Table__next_player(Table* self) {
  if (++self->m_current_player < self->m_players->size) {
    Table__auto_play(self);
  } else {
    Table__dealer_play(self);
  }
}

void Table__check_player_natural(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    if (((Player*)self->m_players->items[i])->m_value == 21 &&
        ((Player*)self->m_players->items[i])->m_hand->size == 2 &&
        ((Player*)self->m_players->items[i])->m_split_from == NULL) {
      ((Player*)self->m_players->items[i])->m_has_natural = true;
    }
  }
}

bool Table__check_dealer_natural(Table* self) {
  Dealer__evaluate(self->m_dealer);
  if (self->m_dealer->m_value != 21) return false;
  ((Card*)self->m_dealer->m_hand->items[1])->m_face_down = false;
  self->m_running_count += ((Card*)self->m_dealer->m_hand->items[1])->m_count;
  if (self->m_verbose) {
    Table__print(self);
    printf("Dealer has a natural 21\n");
  }
  return true;
}

void Table__check_earnings(Table* self) {
  float check = 0;
  for (int i = 0; i < self->m_players->size; i++) {
    check += ((Player*)self->m_players->items[i])->m_earnings;
  }
  if (check * -1 == self->m_casino_earnings) return;
  printf("Earnings don't match\n");
  exit(1);
}

void Table__finish_round(Table* self) {
  if (self->m_verbose) printf("Scoring round\n");
  for (int i = 0; i < self->m_players->size; i++) {
    if (((Player*)self->m_players->items[i])->m_has_natural) {
      Player__win(self->m_players->items[i], 1.5);
    } else if (((Player*)self->m_players->items[i])->m_value > 21) {
      Player__lose(self->m_players->items[i]);
    } else if (self->m_dealer->m_value > 21 ||
               ((Player*)self->m_players->items[i])->m_value >
                   self->m_dealer->m_value) {
      Player__win(self->m_players->items[i], 1);
    } else if (((Player*)self->m_players->items[i])->m_value ==
               self->m_dealer->m_value) {
    } else {
      Player__lose(self->m_players->items[i]);
    }
  }
}

void Table__print(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Player__print(self->m_players->items[i]);
  }
  Dealer__print(self->m_dealer);
  printf("\n");
}