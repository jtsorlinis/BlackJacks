#include "card.h"

#include <iostream>

Card::Card(std::string rank, std::string suit) {
  mRank = std::move(rank);
  mSuit = std::move(suit);
  mFaceDown = false;
  mValue = evaluate();
  mCount = count();
  if (mRank == "A") {
    mIsAce = true;
  }
}

std::string Card::print() {
  if (mFaceDown) {
    return "X";
  }
  return mRank;
}

int Card::evaluate() {
  if (mRank == "J" || mRank == "Q" || mRank == "K") {
    return 10;
  }
  if (mRank == "A") {
    return 11;
  }
  return std::stoi(mRank);
}

int Card::count() {
  if (mRank == "10" || mRank == "J" || mRank == "Q" || mRank == "K" ||
      mRank == "A") {
    return -1;
  }
  if (mRank == "7" || mRank == "8" || mRank == "9") {
    return 0;
  }
  return 1;
}