#include <string>

class Card {
public:
	std::string pictures = "JQK";
	Card(std::string rank, std::string suit);
	std::string print();
	int evaluate();
	int count();
	std::string mRank;
	std::string mSuit;
	bool mFaceDown;
	int mValue;
	int mCount;
	int mIsAce = false;
};

