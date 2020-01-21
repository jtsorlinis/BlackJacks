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
	if (std::string("JQK").find(mRank) != std::string::npos) {
		return 10;
	} else if (mRank == "A") {
		return 11;
	} else {
		return std::stoi(mRank);
	}
}

int Card::count() {
	if (mFaceDown) {
		return 0;
	} else if (std::string("10JQKA").find(mRank) != std::string::npos) {
		return -1;
	} else if (std::string("789").find(mRank) != std::string::npos) {
		return 0;
	} else if (std::string("23456").find(mRank) != std::string::npos) {
		return 1;
	} else {
		std::cout << "Errored with " + mRank;
		exit(1);
	}
}