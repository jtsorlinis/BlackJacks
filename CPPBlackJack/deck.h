#pragma once
#include <string>
#include <vector>
#include "card.h"

class Deck
{
	public:
		Deck();
		std::vector<Card> mDeck;
		std::vector<std::string> mRanks;
		std::vector<std::string> mSuits;
		std::string print();
		void shuffle();
};

