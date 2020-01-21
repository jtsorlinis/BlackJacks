#pragma once
#include <vector>
#include "card.h"
#include "deck.h"

class CardPile {
public:
	int mNumOfdecks;
	std::vector<Card> mCards;
	CardPile(int numOfdecks);
	std::string print();
	void shuffle();
};

