#include <iostream>
#include "player.h"
#include "table.h"
#include <algorithm>
#include <time.h>
#include <iomanip>

int numOfPlayers = 5;
int numOfDecks = 8;
int baseBet = 10;
int minCards = 40;

int rounds = 100000;
int verbose = 0;

int main(int argc, char *argv[]) {
	
	if(argc == 2) {
		rounds = atoi(argv[1]);
	}

	Table table1(numOfPlayers,numOfDecks,baseBet,minCards,verbose);
	table1.mCardPile.shuffle();
	std::cout << std::fixed << std::setprecision(2);
	clock_t start;
	double duration;
	start = clock();

	int x = 0;
	while (x++ < rounds) {
		if (verbose > 0) {
			std::cout << "Round " << x << "\n";
		}
		if (verbose == 0 && rounds > 1000 && x % (rounds / 100) == 0) {
			std::cerr << "\tProgress: " << (int)(((float)x/rounds)*100) << "%\r";
		}
		
		table1.startRound();
		table1.checkEarnings();
	}
	table1.clear();

	for (auto& player : table1.mPlayers) {
		std::cout << "Player " << player.mPlayerNum << " earnings: " << player.mEarnings << "\t\tWin Percentage: " << (50 + (player.mEarnings / (rounds * baseBet) * 50)) << "%\n";
	}
	std::cout << "Casino earnings: " << table1.mCasinoEarnings << "\n";

	duration = ((double)clock() - start)/(double)CLOCKS_PER_SEC;
	std::cout << "\nPlayed " << x-1 << " rounds in " << std::setprecision(3) << duration << " seconds.\n\n";
	/*system("pause");*/
}

