#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"
#include "player.h"
#include "table.h"
#include "vector.h"

const int max_splits = 10;
int player_num_count = 0;

Player* Player__new(Table* table, Player* split) {
  Player* p = malloc(sizeof(Player));
  p->m_hand = Vector__new(5);
  p->m_table = table;
  p->m_bet_mult = 1;
  p->m_earnings = 0;
  if (table == NULL) return p;
  p->m_initial_bet = table->m_bet_size;
  if (split != NULL) {
    Vector__push(p->m_hand, split->m_hand->items[1]);
    Vector__delete(split->m_hand,1);
    p->m_split_count++;
    sprintf(p->m_player_num, "%sS", split->m_player_num);
    p->m_split_from = split;

  } else {
    p->m_split_from = NULL;
    player_num_count++;
    sprintf(p->m_player_num, "%d", player_num_count);
  }
  return p;
}

void Player__double_bet(Player* self) { self->m_bet_mult = 2; }

void Player__reset_hand(Player* self) {
  Vector__clear(self->m_hand);
  self->m_value = 0;
  self->m_aces = 0;
  self->m_is_soft = false;
  self->m_split_count = 0;
  self->m_is_done = false;
  self->m_bet_mult = 1;
  self->m_has_natural = false;
  self->m_initial_bet = self->m_table->m_bet_size;
}

int Player__can_split(Player* self) {
  if (self->m_hand->size == 2 &&
      strcmp(((Card*)self->m_hand->items[0])->m_rank,
          ((Card*)self->m_hand->items[1])->m_rank) == 0 &&
      self->m_split_count < max_splits) {
    return ((Card*)self->m_hand->items[0])->m_value;
  }
  return 0;
}

void Player__win(Player* self, float mult) {
  if (self->m_split_from != NULL) {
    Player__win(self->m_split_from, mult);
  } else {
    self->m_earnings += self->m_initial_bet * self->m_bet_mult * mult;
    self->m_table->m_casino_earnings -=
        self->m_initial_bet * self->m_bet_mult * mult;
  }
}

void Player__lose(Player* self) {
  if (self->m_split_from != NULL) {
    Player__lose(self->m_split_from);
  } else {
    self->m_earnings -= self->m_initial_bet * self->m_bet_mult;
    self->m_table->m_casino_earnings += self->m_initial_bet * self->m_bet_mult;
  }
}

void Player__print(Player* self) {
  printf("Player %s: ", self->m_player_num);
  for (int i = 0; i < self->m_hand->size; i++) {
    printf("%s ", Card__print(self->m_hand->items[i]));
  }
  for (int i = self->m_hand->size; i < 5; i++) {
    printf("  ");
  }
  printf("\tScore: %d", self->m_value);
  if (self->m_value > 21) {
    printf(" (Bust) ");
  } else {
    printf("        ");
  }
  printf("\tBet: %f\n", self->m_initial_bet * self->m_bet_mult);
}

void Player__evaluate(Player* self) {
  self->m_aces = 0;
  self->m_value = 0;
  for (int i = 0; i < self->m_hand->size; i++) {
    self->m_value += ((Card*)self->m_hand->items[i])->m_value;
    // Check for ace
    if (((Card*)self->m_hand->items[i])->m_is_ace) {
      self->m_aces++;
      self->m_is_soft = true;
    }
  }

  while (self->m_value > 21 && self->m_aces > 0) {
    self->m_value -= 10;
    self->m_aces--;
  }

  if (self->m_aces == 0) {
    self->m_is_soft = false;
  };
}

void Player__free(Player* self) {
  Vector__free(self->m_hand);
  free(self);
}