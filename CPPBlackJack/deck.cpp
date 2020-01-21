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
	for (auto &i : mRanks) {
		for (auto &j :mSuits) {
			mCards.push_back(Card(i, j));
		}
	}
}

std::string Deck::print() {
	std::string output;
	for (int x = 0; x < mCards.size(); x++) {
		output += mCards[x].print() + "\n";
	}
	return output;
}

void Deck::shuffle() {
	std::random_device rd;
	std::shuffle(mCards.begin(), mCards.end(), rd);
}
