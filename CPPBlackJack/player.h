#pragma once
#include <vector>
#include <string>


class Table;
class Card;
class Player {
public:
	static int playerNumCount;
	int maxSplits = 10;

	std::string mPlayerNum;
	std::vector<Card> mHand;
	int mValue;
	float mEarnings;
	int mAces;
	bool mIsSoft;
	int mSplitCount;
	bool mIsDone;
	Player* mSplitFrom;
	float mBetMult;
	bool mHasNatural;
	Table* mTable;
	int mInitialBet;
	bool operator==(const Player& obj2) const;

	Player(int real);
	Player(Table* table = NULL, Player* split = NULL);
	void doubleBet();
	virtual void resetHand();
	std::string canSplit();
	void win(float mult = 1);
	void lose();
	std::string print();
	int evaluate();
};
