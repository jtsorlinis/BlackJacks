#include "table.h"
#include <iostream>
#include <algorithm>

Table::Table(int numPlayers, int numOfDecks, int betSize, int minCards, int verbose) :mCardPile(CardPile(numOfDecks)) {
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
	for (auto& player : mPlayers) {
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
	for (auto& player : mPlayers) {
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
	}
	else {
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
	mPlayers.erase(std::remove_if(mPlayers.begin(), mPlayers.end(), [](auto& player) { return player.mSplitFrom; }), mPlayers.end());
	for (auto& player : mPlayers) {
		player.resetHand();
	}
	mDealer.resetHand();
}

void Table::updatecount(Card* card) {
	mRunningcount += card->count();
	mTrueCount = mRunningcount / (mCardPile.mCards.size() / (float)52);
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
	Player splitPlayer(this, mCurrentPlayer);
	std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
	if (itr != mPlayers.cend()) {
		mPlayers.insert(++itr, std::move(splitPlayer));
		std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
		mCurrentPlayer->mHand.pop_back();
		mCurrentPlayer->evaluate();
		itr++;
		itr->evaluate();
		if (mVerbose > 0) {
			std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits\n";
		}

	}
	else {
		std::cout << "Couldn't split player.";
		exit(1);
	}
}

void Table::splitAces() {
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits aces\n";
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

	}
	else {
		std::cout << "Couldn't split player.";
		exit(1);
	}

}

void Table::doubleBet() {
	if (mCurrentPlayer->mBetMult == 1 && mCurrentPlayer->mHand.size() == 2) {
		mCurrentPlayer->doubleBet();
		if (mVerbose > 0) {
			std::cout << "Player " << mCurrentPlayer->mPlayerNum << " doubles";
		}
		hit();
		stand();
	}
}

// TODO: Implement this
void Table::playHard() {

}

// TODO: Implement this
void Table::playSoft() {
}

// TODO: Implement this
void Table::playSplit() {
}

// TODO: Add Strategy
void Table::autoPlay() {
	while (mCurrentPlayer->mHand.size() < 5 && mCurrentPlayer->mValue < 17) {
		hit();
		if (mVerbose > 0) {
			print();
		}
		
	}
	stand();
	nextPlayer();
}

// TODO: Implement this
void Table::action(std::string action) {
}

void Table::dealerPlay() {
	bool allBusted = true;
	for (auto& player : mPlayers) {
		if (player.mValue < 22) {
			allBusted = false;
		}
	}
	mDealer.mHand[1].mFaceDown = false;
	updatecount(&mDealer.mHand[1]);
	mDealer.evaluate();
	if (mVerbose > 0) {
		std::cout << "Dealer's turn\n";
		print();
	}
	if (allBusted) {
		if (mVerbose > 0) {
			std::cout << "Dealer automatically wins cause all players busted\n";
		}
		finishRound();
	}
	else {
		while (mDealer.mValue < 17 && mDealer.mHand.size() < 5) {
			deal();
			mDealer.evaluate();
			if (mVerbose > 0) {
				std::cout << "Dealer hits\n";
				print();
			}
		}
		finishRound();
	}
}

void Table::nextPlayer() {
	std::vector<Player>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), *mCurrentPlayer);
	if (itr + 1 != mPlayers.end()) {
		itr++;
		mCurrentPlayer = &*itr;
		autoPlay();
	}
	else {
		mCurrentPlayer = &mDealer;
		dealerPlay();
	}
}

void Table::checkPlayerNatural() {
	for (auto& player : mPlayers) {
		if (player.mValue == 21 && player.mHand.size() == 2 && !player.mSplitFrom) {
			player.mHasNatural = 1;
		}
	}
}

bool Table::checkDealerNatural() {
	if (mDealer.evaluate() == 21) {
		mDealer.mHand[1].mFaceDown = false;
		updatecount(&mDealer.mHand[1]);
		if (mVerbose > 0) {
			print();
			std::cout << "Dealer has a natural 21\n\n";
		}
		return true;
	}
	else {
		return false;
	}
	
}

void Table::checkEarnings() {
	float check = 0;
	for (auto& player : mPlayers) {
		check += player.mEarnings;
	}
	if (check * -1 != mCasinoEarnings) {
		std::cout << "NO MATCH";
		exit(1);
	}
}

void Table::finishRound() {
	if (mVerbose > 0) {
		std::cout << "Scoring round\n";
	}
	for (auto& player : mPlayers) {
		if (player.mHasNatural) {
			player.win(1.5);
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Wins " << (1.5 * player.mBetMult * player.mInitialBet) << " with a natural 21\n";
			}
		}
		else if (player.mValue > 21) {
			player.lose();
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Busts and Loses " << (player.mBetMult * player.mInitialBet) << "\n";
			}

		}
		else if (mDealer.mValue > 21) {
			player.win();
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Wins " << (player.mBetMult * player.mInitialBet) << "\n";
			}
		}
		else if (player.mValue > mDealer.mValue) {
			player.win();
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Wins " << (player.mBetMult * player.mInitialBet) << "\n";
			}
		}
		else if (player.mValue == mDealer.mValue) {
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Draws\n";
			}
		}
		else {
			player.lose();
			if (mVerbose > 0) {
				std::cout << "Player " << player.mPlayerNum << " Loses " << (player.mBetMult * player.mInitialBet) << "\n";
			}
		}
	}
	if (mVerbose > 0) {
		for (auto& player : mPlayers) {
			if (!player.mSplitFrom) {
				std::cout << "Player " << player.mPlayerNum << " Earnings: " << player.mEarnings << "\n";
			}
		}
		std::cout << "\n";
	}
}

void Table::print() {
	for (auto& player : mPlayers) {
		std::cout << player.print() + "\n";
	}
	std::cout << mDealer.print() + "\n\n";
}

