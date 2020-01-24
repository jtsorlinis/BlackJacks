#include <vector>
#include "dealer.h"
#include "cardpile.h"
#include <list>
#include <unordered_map>

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
	std::unordered_map<int, std::string> mStratHard;
	std::unordered_map<int, std::string> mStratSoft;
	std::unordered_map<int, std::string> mStratSplit;

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
	void autoPlay();
	void action(std::string const &action);
	void dealerPlay();
	void nextPlayer();
	void checkPlayerNatural();
	bool checkDealerNatural();
	void checkEarnings();
	void finishRound();
	void print();
};

