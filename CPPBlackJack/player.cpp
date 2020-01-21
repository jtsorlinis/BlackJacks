#include "player.h"
#include <iostream>
#include "table.h"
#include "card.h"

int Player::playerNumCount = 0;

bool Player::operator==(const Player& obj2) const {
	if (&*this == &obj2) {
		return true;
	} else {
		return false;
	}
}

Player::Player(int real) {

}

Player::Player(Table* table, Player* split) {
	mValue = 0;
	mEarnings = 0;
	mAces = 0;
	mIsSoft = false;
	mSplitCount = 0;
	mIsDone = false;
	mSplitFrom = NULL;
	mBetMult = 1;
	mHasNatural = false;
	mTable = table;
	if (mTable) {
		mInitialBet = mTable->mBetSize;
	}

	if (split) {
		mHand.push_back(split->mHand[1]);
		mSplitCount++;
		mPlayerNum = split->mPlayerNum + "S";
		mInitialBet = split->mInitialBet;
	} else {
		playerNumCount++;
		mPlayerNum = std::to_string(playerNumCount);
	}
}

void Player::doubleBet() {
	mBetMult = 2;
}

void Player::resetHand() {
	mHand.clear();
	mValue = 0;
	mAces = 0;
	mIsSoft = false;
	mSplitCount = 0;
	mIsDone = false;
	mBetMult = 1;
	mHasNatural = 0;
	mInitialBet = mTable->mBetSize;
}

std::string Player::canSplit() {
	if (mHand.size() == 2 && mHand[0].mRank == mHand[1].mRank && mSplitCount < maxSplits) {
		return mHand[0].mRank;
	} else {
		return "";
	}
}

void Player::win(float mult) {
	if (mSplitFrom) {
		mSplitFrom->win(mult);
	} else {
		mEarnings += (mInitialBet * mBetMult * mult);
		mTable->mCasinoEarnings -= (mInitialBet * mBetMult * mult);
	}
}

void Player::lose() {
	if (mSplitFrom) {
		mSplitFrom->lose();
	} else {
		mEarnings -= (mInitialBet * mBetMult);
		mTable->mCasinoEarnings += (mInitialBet * mBetMult);
	}
}

std::string Player::print() {
	std::string output = "Player " + mPlayerNum + ": ";
	for (auto &i : mHand) {
		output += i.print() + " ";
	}
	for (int i = mHand.size(); i < 5; i++) {
		output += "  ";
	}
	output += "\tScore: " + std::to_string(mValue);
	if (mValue > 21) {
		output += " (Bust) ";
	} else {
		output += "        ";
	}
	if (mPlayerNum != "D") {
		output += "\tBet: " + std::to_string(mInitialBet * mBetMult);
	}
	return output;
}

int Player::evaluate() {
	mAces = 0;
	mValue = 0;
	for (auto &i : mHand) {
		mValue += i.evaluate();
		// Check for ace
		if (i.mRank == "A") {
			mAces++;
			mIsSoft = true;
		}
	}

	while (mValue > 21 and mAces > 0) {
		mValue -= 10;
		mAces--;
	}

	if (mAces == 0) {
		mIsSoft = false;
	}

	return mValue;
}
