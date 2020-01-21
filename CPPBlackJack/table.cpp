#include "table.h"
#include <iostream>
#include <algorithm>

Table::Table(int numPlayers, int numOfDecks, int betSize, int minCards, int verbose):mCardPile(CardPile(numOfDecks)) {
	mVerbose = verbose;
	mBetSize = betSize;
	mNumOfdecks = numOfDecks;
	mMinCards = minCards;
	mDealer = Dealer();
	mCurrentPlayer = NULL;
	mCasinoEarnings = 0;
	mRunningcount = 0;
	mTrueCount = 0;

	for (int i = 0; i < numPlayers; i++) {
		mPlayers.push_back(Player(this));
	}
}

void Table::dealRound() {
	for (auto &player : mPlayers) {
		mCurrentPlayer = &player;
		deal();
		player.evaluate();
	}
}

void Table::deal(bool faceDown) {
	Card card = mCardPile.mCards.back();
	mCardPile.mCards.pop_back();
	card.mFaceDown = faceDown;
	mCurrentPlayer->mHand.push_back(card);
	updatecount(&card);
}

void Table::preDeal() {
	for (auto &player : mPlayers) {
		selectBet(&player);
	}
}

void Table::selectBet(Player* player) {
	if (mTrueCount >= 2) {
		player->mInitialBet = mBetSize * (int)(mTrueCount - 1) * 1.25;
	}
}

void Table::dealDealer(bool faceDown) {
	mCurrentPlayer = &mDealer;
	deal(faceDown);
}

void Table::startRound() {
	clear();
	if (mVerbose > 0) {
		std::cout << mCardPile.mCards.size() << " cards left\n";
		std::cout << "Running count is: " << mRunningcount << "\tTrue count is: " << (int)mTrueCount << "\n";
	}
	getNewcards();
	preDeal();
	dealRound();
	dealDealer();
	dealRound();
	dealDealer(true);
	mCurrentPlayer = &mPlayers.front();
	if (checkDealerNatural()) {
		finishRound();
	} else {
		checkPlayerNatural();
		if (mVerbose > 0) {
			print();
		}
		autoPlay();
	}

}

void Table::getNewcards() {
	if (mCardPile.mCards.size() < mMinCards) {
		mCardPile = CardPile(mNumOfdecks);
		mCardPile.shuffle();
		mTrueCount = 0;
		mRunningcount = 0;
		if (mVerbose > 0) {
			std::cout << "Got " << mNumOfdecks << " new decks as number of cards is below " << mMinCards << "\n";
		}
	}
}

void Table::clear() {
	mPlayers.erase(std::remove_if(mPlayers.begin(), mPlayers.end(), [](auto& player) { return player.mSplitFrom;}), mPlayers.end());
	for (auto& player : mPlayers) {
		player.resetHand();
	}
	mDealer.resetHand();
}

void Table::updatecount(Card* card) {
	mRunningcount += card->count();
	mTrueCount = mRunningcount / (mCardPile.mCards.size() / 52);
}

void Table::hit() {
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " hits\n";
	}
	deal();
	mCurrentPlayer->evaluate();
}

void Table::stand() {
	if (mVerbose > 0 && mCurrentPlayer->mValue <= 21) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " stands\n";
	}
	mCurrentPlayer->mIsDone = true;
}

void Table::split() {
	Player splitPlayer(this,mCurrentPlayer);
	std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
	if (itr != mPlayers.cend()) {
		mPlayers.insert(++itr, std::move(splitPlayer));
		std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
		mCurrentPlayer->mHand.pop_back();
		mCurrentPlayer->evaluate();
		itr++;
		itr->evaluate();
		if (mVerbose > 0) {
			std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits";
		}
		
	} else {
		std::cout << "Couldn't split player.";
		exit(1);
	}
}

void Table::splitAces() {
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits aces";
	}
	Player splitPlayer(this, mCurrentPlayer);
	std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
	if (itr != mPlayers.cend()) {
		mPlayers.insert(++itr, std::move(splitPlayer));
		std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
		mCurrentPlayer->mHand.pop_back();
		deal();
		mCurrentPlayer->evaluate();
		stand();
		itr++;
		mCurrentPlayer = &*itr;
		deal();
		mCurrentPlayer->evaluate();
		stand();
		if (mVerbose > 0) {
			print();
		}

	} else {
		std::cout << "Couldn't split player.";
		exit(1);
	}

}

void Table::doubleBet() {
}

void Table::playHard() {
}

void Table::playSoft() {
}

void Table::playSplit() {
}

void Table::autoPlay() {
}

void Table::action(std::string action) {
}

void Table::dealerPlay() {
}

void Table::nextPlayer() {
}

void Table::checkPlayerNatural() {
}

bool Table::checkDealerNatural() {
	return false;
}

void Table::checkEarnings() {
}

void Table::finishRound() {
}

void Table::print() {
	for (auto &player : mPlayers) {
		std::cout << player.print() + "\n";
	}
	std::cout << mDealer.print() + "\n\n";
}

