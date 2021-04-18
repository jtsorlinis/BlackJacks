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
const int min_cards = 40;

int rounds = 1000000;
const bool verbose = false;

int main(int argc, char const *argv[]) {
  if (argc == 2) rounds = atoi(argv[1]);

  Table *t = Table__new(5, 8, 10, 40, verbose);
  CardPile__shuffle(t->m_card_pile);

  clock_t start = clock();

  for (int x = 0; x < rounds; x++) {
    if (verbose) printf("Round %d\n", x + 1);
    if (!verbose && rounds > 1000 && x % (rounds / 100) == 0) {
      printf("\tProgress: %d%%\r", x * 100 / rounds);
      fflush(stdout);
    }

    Table__start_round(t);
    Table__check_earnings(t);
  }

  Table__clear(t);

  for (int i = 0; i < t->m_players->size; i++) {
    printf("Player %s earnings: %f\t\tWin Percentage: %f%%\n",
           ((Player *)t->m_players->items[i])->m_player_num,
           ((Player *)t->m_players->items[i])->m_earnings,
           (50 + ((Player *)t->m_players->items[i])->m_earnings /
                     (rounds * bet_size) * 50.0));
  }

  printf("Casino earnings: %f\n\n", t->m_casino_earnings);
  printf("Played %d rounds in %f seconds\n", rounds,
         (double)(clock() - start) / CLOCKS_PER_SEC);

  return 0;
}
