#include "card.h"
#include <iostream>

Card::Card(std::string rank, std::string suit) {
	mRank = rank;
	mSuit = suit;
	mFaceDown = false;
}

std::string Card::print() {
	if (mFaceDown) {
		return "X";
	} else {
		return mRank;
	}
}

int Card::evaluate() {
	if (mRank == "J" || mRank == "Q" || mRank == "K") {
		return 10;
	} else if (mRank == "A") {
		return 11;
	} else {
		return std::stoi(mRank);
	}
}

int Card::count() {
	if (mRank == "10" || mRank == "J" || mRank == "Q" || mRank == "K" || mRank == "A") {
		return -1;
	}
	else if (mRank == "7" || mRank == "8" || mRank == "9") {
		return 0;
	}
	else {
		return 1;
	}
}