#include "table.h"
#include <iostream>
#include <algorithm>
#include "strategies.h"

Table::Table(int numPlayers, int numOfDecks, int betSize, int minCards, int verbose) :mCardPile(CardPile(numOfDecks)) {
	mVerbose = verbose;
	mBetSize = betSize;
	mNumOfdecks = numOfDecks;
	mMinCards = minCards;
	mDealer = Dealer();
	mCurrentPlayer = mPlayers.end();
	mCasinoEarnings = 0;
	mRunningcount = 0;
	mTrueCount = 0;

	for (int i = 0; i < numPlayers; i++) {
		mPlayers.push_back(Player(this));
	}
	mStratHard = vecToMap(stratHard);
	mStratSoft = vecToMap(stratSoft);
	mStratSplit = vecToMap(stratSplit);
}

void Table::dealRound() {
	for (std::list<Player>::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it) {
		mCurrentPlayer = it;
		deal();
		mCurrentPlayer->evaluate();
	}
	mCurrentPlayer = mPlayers.begin();
}

void Table::deal() {
	Card& card = mCardPile.mCards.back();
	mCurrentPlayer->mHand.push_back(card);
	updatecount(&card);
	mCardPile.mCards.pop_back();
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
	Card card = mCardPile.mCards.back();
	mCardPile.mCards.pop_back();
	card.mFaceDown = faceDown;
	mDealer.mHand.push_back(card);
	if (!faceDown) {
		updatecount(&card);
	}
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
	mCurrentPlayer = mPlayers.begin();
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
		mCardPile.refresh();
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
	mRunningcount += card->mCount;
	mTrueCount = mRunningcount / (mCardPile.mCards.size() / (float)52);
}

void Table::hit() {
	deal();
	mCurrentPlayer->evaluate();
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " hits\n";
	}
}

void Table::stand() {
	if (mVerbose > 0 && mCurrentPlayer->mValue <= 21) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " stands\n";
		print();
	}
	mCurrentPlayer->mIsDone = true;
}

void Table::split() {
	Player splitPlayer(this, &*mCurrentPlayer);
	mCurrentPlayer->mHand.pop_back();
	mPlayers.insert(next(mCurrentPlayer), splitPlayer);
	mCurrentPlayer->evaluate();
	next(mCurrentPlayer)->evaluate();
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits\n";
	}
}

void Table::splitAces() {
	if (mVerbose > 0) {
		std::cout << "Player " << mCurrentPlayer->mPlayerNum << " splits aces\n";
	}
	Player splitPlayer(this, &*mCurrentPlayer);
	mCurrentPlayer->mHand.pop_back();
	mPlayers.insert(next(mCurrentPlayer), splitPlayer);
	deal();
	mCurrentPlayer->evaluate();
	stand();
	mCurrentPlayer++;
	deal();
	mCurrentPlayer->evaluate();
	stand();
	if (mVerbose > 0) {
		print();
	}
}

void Table::doubleBet() {
	if (mCurrentPlayer->mBetMult == 1 && mCurrentPlayer->mHand.size() == 2) {
		mCurrentPlayer->doubleBet();
		if (mVerbose > 0) {
			std::cout << "Player " << mCurrentPlayer->mPlayerNum << " doubles\n";
		}
		hit();
		stand();
	}
	else {
		hit();
	}
}

void Table::autoPlay() {
	while (!mCurrentPlayer->mIsDone) {
		// check if player just split
		if (mCurrentPlayer->mHand.size() == 1) {
			if (mVerbose > 0) {
				std::cout << "Player " << mCurrentPlayer->mPlayerNum << " gets 2nd card after splitting";
			}
			deal();
			mCurrentPlayer->evaluate();
		}

		if (mCurrentPlayer->mHand.size() < 5 && mCurrentPlayer->mValue < 21) {
			std::string canSplit = mCurrentPlayer->canSplit();
			if (canSplit == "A") {
				splitAces();
			}
			else if (!canSplit.empty() && (canSplit != "5" && canSplit != "10" && canSplit != "J" && canSplit != "Q" && canSplit != "K")) {
				action(getAction(std::stoi(canSplit), mDealer.upCard(), mStratSplit));
			}
			else if (mCurrentPlayer->mIsSoft) {
				action(getAction(mCurrentPlayer->mValue, mDealer.upCard(), mStratSoft));
			}
			else {
				action(getAction(mCurrentPlayer->mValue, mDealer.upCard(), mStratHard));
			}
		}
		else {
			stand();
		}
	}
	nextPlayer();
}

void Table::action(std::string const &action) {
	if (action == "H") {
		hit();
	}
	else if (action == "S") {
		stand();
	}
	else if (action == "D") {
		doubleBet();
	}
	else if (action == "P") {
		split();
	}
	else {
		std::cout << "No action found";
		exit(1);
	}
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
			dealDealer();
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
	if (next(mCurrentPlayer) != mPlayers.end()) {
		mCurrentPlayer++;
		autoPlay();
	}
	else {
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

