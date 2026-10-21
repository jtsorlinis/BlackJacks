#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "cardpile.h"
#include "player.h"
#include "table.h"
#include "vector.h"

const int num_players = 5;
const int num_decks = 8;
const int bet_size = 10;
const int min_cards = 52;

int rounds = 1000000;

int main(int argc, char const *argv[]) {
  if (argc == 2) rounds = atoi(argv[1]);

  Table *t = Table__new(num_players, num_decks, bet_size, min_cards);
  CardPile__shuffle(&t->m_card_pile);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int x = 1; x <= rounds; x++) {
    if (verbose) printf("Round %d\n", x);
    if (!verbose && rounds > 1000 && x % (rounds / 100) == 0) {
      fprintf(stderr, "\tProgress: %d%%\r", x * 100 / rounds);
    }

    Table__start_round(t);
    Table__check_earnings(t);
  }

  Table__clear(t);

  for (int i = 0; i < t->m_players.size; i++) {
    printf("Player %s earnings: %d\t\tWin Percentage: %.2f%%\n",
           t->m_players.items[i].m_player_num,
           (int)t->m_players.items[i].m_earnings,
           50 + t->m_players.items[i].m_earnings /
                    ((double)rounds * bet_size) * 50);
  }

  printf("Casino earnings: %d\n", (int)t->m_casino_earnings);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("Played %d rounds in %.3f seconds\n", rounds,
         (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9);

  return 0;
}
