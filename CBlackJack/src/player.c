#define _CRT_SECURE_NO_WARNINGS
#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"
#include "vector.h"

const int max_hands = 4;
int player_num_count = 0;

void Player__init(Player* p, int bet_size, Player* split) {
  Vector__init(&p->m_hand, 5);
  p->m_value = 0;
  p->m_aces = 0;
  p->m_is_soft = false;
  p->m_bet_mult = 1;
  p->m_earnings = 0;
  p->m_has_natural = false;
  p->m_is_done = false;
  p->m_initial_bet = bet_size;
  p->m_original_bet = bet_size;
  if (split != NULL) {
    p->m_split_count = split->m_split_count;
    (*p->m_split_count)++;
    Vector__push(&p->m_hand, split->m_hand.items[1]);
    Vector__delete(&split->m_hand, 1);
    strcpy(p->m_player_num, split->m_player_num);
    strcat(p->m_player_num, "S");
    p->m_initial_bet = split->m_initial_bet;
    p->m_is_split_hand = true;

  } else {
    p->m_is_split_hand = false;
    p->m_split_count = malloc(sizeof(int));
    *p->m_split_count = 0;
    player_num_count++;
    sprintf(p->m_player_num, "%d", player_num_count);
  }
}

void Player__double_bet(Player* self) { self->m_bet_mult = 2; }

void Player__reset_hand(Player* self) {
  Vector__clear(&self->m_hand);
  self->m_value = 0;
  self->m_aces = 0;
  self->m_is_soft = false;
  *self->m_split_count = 0;
  self->m_is_done = false;
  self->m_bet_mult = 1;
  self->m_has_natural = false;
  self->m_initial_bet = self->m_original_bet;
}

int Player__can_split(Player* self) {
  if (self->m_hand.size == 2 &&
      ((Card*)self->m_hand.items[0])->m_rank_id ==
          ((Card*)self->m_hand.items[1])->m_rank_id &&
      *self->m_split_count < max_hands - 1) {
    return ((Card*)self->m_hand.items[0])->m_value;
  }
  return 0;
}

double Player__settle(Player* self, double mult) {
  double x = self->m_initial_bet * self->m_bet_mult * mult;
  self->m_earnings += x;
  return -x;
}

void Player__print(Player* self) {
  printf("Player %s: ", self->m_player_num);
  for (int i = 0; i < self->m_hand.size; i++) {
    printf("%s ", Card__print(self->m_hand.items[i]));
  }
  for (int i = self->m_hand.size; i < 5; i++) {
    printf("  ");
  }
  printf("\tScore: %d", self->m_value);
  if (self->m_value > 21) {
    printf(" (Bust) ");
  } else {
    printf("        ");
  }
  printf("\tBet: %d\n", (int)(self->m_initial_bet * self->m_bet_mult));
}

void Player__evaluate(Player* self) {
  int value = 0;
  int aces = 0;
  for (int i = 0; i < self->m_hand.size; i++) {
    value += ((Card*)self->m_hand.items[i])->m_value;
    aces += ((Card*)self->m_hand.items[i])->m_is_ace;
  }

  while (value > 21 && aces > 0) {
    value -= 10;
    aces--;
  }

  self->m_value = value;
  self->m_aces = aces;
  self->m_is_soft = aces > 0;
}

void Player__destroy(Player* self) {
  if (!self->m_is_split_hand) free(self->m_split_count);
  Vector__destroy(&self->m_hand);
}
