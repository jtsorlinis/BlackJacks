#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "vector.h"

typedef struct Player {
  Vector m_hand;
  bool m_is_split_hand;
  int m_aces;
  int* m_split_count;
  double m_bet_mult;
  double m_earnings;
  bool m_has_natural;
  int m_initial_bet;
  int m_original_bet;
  bool m_is_done;
  bool m_is_soft;

  char m_player_num[10];
  int m_value;
} Player;

typedef struct PlayerVector {
  Player* items;
  int size;
  int capacity;
} PlayerVector;

static inline void PlayerVector__init(PlayerVector* self, int capacity) {
  self->items = malloc(capacity * sizeof(Player));
  self->size = 0;
  self->capacity = capacity;
}

static inline void PlayerVector__insert(PlayerVector* self, Player* player,
                                        int index) {
  if (self->size == self->capacity) {
    Player* temp = realloc(self->items, (self->capacity + 2) * sizeof(Player));
    if (!temp) return;
    self->items = temp;
    self->capacity += 2;
  }
  memmove(&self->items[index + 1], &self->items[index],
          (self->size - index) * sizeof(Player));
  self->items[index] = *player;
  self->size++;
}

static inline void PlayerVector__push(PlayerVector* self, Player* player) {
  PlayerVector__insert(self, player, self->size);
}

static inline void PlayerVector__delete(PlayerVector* self, int index) {
  memmove(&self->items[index], &self->items[index + 1],
          (self->size - index - 1) * sizeof(Player));
  self->size--;
}

void Player__init(Player* self, int bet_size, Player* split);
void Player__double_bet(Player* self);
void Player__reset_hand(Player* self);
int Player__can_split(Player* self);
double Player__settle(Player* self, double mult);
void Player__print(Player* self);
void Player__evaluate(Player* self);
void Player__destroy(Player* self);

#endif
