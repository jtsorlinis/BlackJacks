#include "cardpile.h"
#include <random>
#include <time.h>
#include "QuickRand.h"


CardPile::CardPile(int numOfdecks) {
	for (int x = 0; x < numOfdecks; x++) {
		Deck tempDeck;
		mCards.insert(mCards.end(), tempDeck.mCards.begin(), tempDeck.mCards.end());
	}
	mOriginalCards = mCards;
	fast_srand(time(NULL));
}

void CardPile::refresh() {
	mCards = mOriginalCards;
}

std::string CardPile::print() {
	std::string output;
	for (auto& i : mCards) {
		output += i->print() + "\n";
	}
	return output;
}

void CardPile::shuffle() {
	//std::random_device rd;
	//std::shuffle(mCards.begin(), mCards.end(), rd);

	// Fisher yates
	for (int i = mCards.size() - 1; i > 0; i--) {
		int j = fast_rand() % (i + 1);
		std::swap(mCards[i], mCards[j]);
	}
}