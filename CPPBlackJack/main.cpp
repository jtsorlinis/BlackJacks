#include <chrono>
#include <iomanip>
#include <iostream>
#include "player.h"
#include "table.h"

using namespace std::chrono;

int num_of_players = 5;
int num_of_decks = 8;
int base_bet = 10;
int min_cards = 40;

int rounds = 1000000;
int verbose = 0;

int main(const int argc, char* argv[]) {
  if (argc == 2) {
    rounds = strtol(argv[1], nullptr, 10);
  }

  Table table1(num_of_players, num_of_decks, base_bet, min_cards, verbose);
  table1.m_card_pile_.shuffle();
  std::cout << std::fixed << std::setprecision(2);
  const auto start = high_resolution_clock::now();

  auto x = 0;
  while (x++ < rounds) {
    if (verbose > 0) {
      std::cout << "Round " << x << "\n";
    }
    if (verbose == 0 && rounds > 1000 && x % (rounds / 100) == 0) {
      std::cerr << "\tProgress: " << 100 * x / rounds << "%\r";
    }

    table1.start_round();
    table1.check_earnings();
  }
  table1.clear();

  for (auto& player : table1.m_players_) {
    std::cout << "Player " << player.m_player_num_
        << " earnings: " << player.m_earnings_ << "\t\tWin Percentage: "
        << 50 + player.m_earnings_ /
        static_cast<float>(rounds * base_bet) * 50
        << "%\n";
  }
  std::cout << "Casino earnings: " << table1.m_casino_earnings_ << "\n";

  const auto duration = duration_cast<milliseconds>(high_resolution_clock::now()-start);
  std::cout << "\nPlayed " << x - 1 << " rounds in " << std::setprecision(3)
      << duration.count()/1000.0 << " seconds.\n\n";
 
}
