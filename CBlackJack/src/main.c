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
const bool verbose = false;

int main(int argc, char const *argv[]) {
  if (argc == 2) rounds = atoi(argv[1]);

  Table *t = Table__new(num_players, num_decks, bet_size, min_cards, verbose);
  CardPile__shuffle(t->m_card_pile);

  clock_t start = clock();

  for (int x = 1; x <= rounds; x++) {
    if (verbose) printf("Round %d\n", x);
    if (!verbose && rounds > 1000 && x % (rounds / 100) == 0) {
      fprintf(stderr, "\tProgress: %d%%\r", x * 100 / rounds);
    }

    Table__start_round(t);
    Table__check_earnings(t);
  }

  Table__clear(t);

  for (int i = 0; i < t->m_players->size; i++) {
    printf("Player %s earnings: %d\t\tWin Percentage: %.2f%%\n",
           ((Player *)t->m_players->items[i])->m_player_num,
           (int)((Player *)t->m_players->items[i])->m_earnings,
           50 + ((Player *)t->m_players->items[i])->m_earnings /
                    ((double)rounds * bet_size) * 50);
  }

  printf("Casino earnings: %d\n", (int)t->m_casino_earnings);
  printf("Played %d rounds in %.3f seconds\n", rounds,
         (double)(clock() - start) / CLOCKS_PER_SEC);

  return 0;
}
