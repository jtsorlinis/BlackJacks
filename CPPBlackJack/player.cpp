#include "player.h"

#include <iostream>

#include "table.h"

int Player::playerNumCount = 0;

Player::Player(Table* table, Player* split) {
  mValue = 0;
  mEarnings = 0;
  mAces = 0;
  mIsSoft = false;
  mSplitCount = 0;
  mIsDone = false;
  mSplitFrom = nullptr;
  mBetMult = 1;
  mHasNatural = false;
  mTable = table;
  mInitialBet = 0;
  if (mTable != nullptr) {
    mInitialBet = mTable->mBetSize;
    if (split != nullptr) {
      mHand.push_back(split->mHand[1]);
      mSplitCount++;
      mPlayerNum = split->mPlayerNum + "S";
      mInitialBet = split->mInitialBet;
      mSplitFrom = split;
    } else {
      playerNumCount++;
      mPlayerNum = std::to_string(playerNumCount);
    }
  }
}

void Player::doubleBet() { mBetMult = 2; }

void Player::resetHand() {
  mHand.clear();
  mValue = 0;
  mAces = 0;
  mIsSoft = false;
  mSplitCount = 0;
  mIsDone = false;
  mBetMult = 1;
  mHasNatural = false;
  mInitialBet = mTable->mBetSize;
}

int Player::canSplit() {
  if (mHand.size() == 2 && mHand[0]->mRank == mHand[1]->mRank &&
      mSplitCount < maxSplits) {
    return mHand[0]->mValue;
  }
  return 0;
}

void Player::win(float mult) {
  if (mSplitFrom != nullptr) {
    mSplitFrom->win(mult);
  } else {
    mEarnings += (static_cast<float>(mInitialBet) * mBetMult * mult);
    mTable->mCasinoEarnings -=
        (static_cast<float>(mInitialBet) * mBetMult * mult);
  }
}

void Player::lose() {
  if (mSplitFrom != nullptr) {
    mSplitFrom->lose();
  } else {
    mEarnings -= (static_cast<float>(mInitialBet) * mBetMult);
    mTable->mCasinoEarnings += (static_cast<float>(mInitialBet) * mBetMult);
  }
}

std::string Player::print() {
  std::string output = "Player " + mPlayerNum + ": ";
  for (auto& i : mHand) {
    output += i->print() + " ";
  }
  for (int i = static_cast<int>(mHand.size()); i < 5; i++) {
    output += "  ";
  }
  output += "\tScore: " + std::to_string(mValue);
  if (mValue > 21) {
    output += " (Bust) ";
  } else {
    output += "        ";
  }
  if (mPlayerNum != "D") {
    output +=
        "\tBet: " + std::to_string(static_cast<float>(mInitialBet) * mBetMult);
  }
  return output;
}

int Player::evaluate() {
  mAces = 0;
  mValue = 0;
  for (auto& card : mHand) {
    mValue += card->mValue;
    // Check for ace
    if (card->mIsAce) {
      mAces++;
      mIsSoft = true;
    }
  }

  while (mValue > 21 && mAces > 0) {
    mValue -= 10;
    mAces--;
  }

  if (mAces == 0) {
    mIsSoft = false;
  }

  return mValue;
}
