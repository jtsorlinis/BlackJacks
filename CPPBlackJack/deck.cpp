#include "deck.h"
#include <vector>
#include "card.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
	mRanks = { "A", "2", "3", "4", "5", "6", "7", "8","9","10","J","Q","K" };
	mSuits = { "Clubs", "Hearts", "Spades", "Diamonds" };
	for (int i = 0; i < mRanks.size(); i++) {
		for (int j = 0; j < mSuits.size(); j++) {
			mDeck.push_back(Card(mRanks[i], mSuits[j]));
		}
	}
}

std::string Deck::print() {
	std::string output;
	for (int x = 0; x < mDeck.size(); x++) {
		output += mDeck[x].print() + "\n";
	}
	return output;
}

void Deck::shuffle() {
	std::random_device rd;
	std::shuffle(mDeck.begin(), mDeck.end(), rd);
}
