#include "deck.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

Deck::Deck() {
  mRanks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  mSuits = {"Clubs", "Hearts", "Spades", "Diamonds"};
  for (auto& suit : mSuits) {
    for (auto& rank : mRanks) {
      Card* temp = new Card(rank, suit);
      mCards.push_back(temp);
    }
  }
}

std::string Deck::print() {
  std::string output;
  for (auto& card : mCards) {
    output += card->print() + "\n";
  }
  return output;
}

void Deck::shuffle() {
  std::random_device rd;
  std::shuffle(mCards.begin(), mCards.end(), rd);
}
