#include <iostream>
#include "cardpile.h"
#include "player.h"
#include "table.h"
#include <algorithm>

int numOfPlayers = 5;
int numOfDecks = 8;
int baseBet = 10;
int minCards = 40;

int rounds = 100;
int verbose = 1;

int main() {
	Table table1(numOfPlayers,numOfDecks,baseBet,minCards,verbose);
	table1.mCardPile.shuffle();
	table1.startRound();
	table1.split();
	std::cout << "\n";
	table1.print();
}

