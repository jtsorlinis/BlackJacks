#pragma once
#include <vector>
#include "dealer.h"
#include "cardpile.h"
#include <list>

class Table {
public:
	int mVerbose;
	int mBetSize;
	std::list<Player> mPlayers;
	int mNumOfdecks;
	CardPile mCardPile;
	int mMinCards;
	Dealer mDealer;
	std::list<Player>::iterator mCurrentPlayer;
	float mCasinoEarnings;
	int mRunningcount;
	float mTrueCount;

	Table(int numPlayers, int numOfDecks, int betSize, int minCards, int verbose = 0);
	void dealRound();
	void deal();
	void preDeal();
	void selectBet(Player* player);
	void dealDealer(bool faceDown = false);
	void startRound();
	void getNewcards();
	void clear();
	void updatecount(Card* card);
	void hit();
	void stand();
	void split();
	void splitAces();
	void doubleBet();
	void playHard();
	void playSoft();
	void playSplit();
	void autoPlay();
	void action(std::string action);
	void dealerPlay();
	void nextPlayer();
	void checkPlayerNatural();
	bool checkDealerNatural();
	void checkEarnings();
	void finishRound();
	void print();
};

