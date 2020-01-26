#include <vector>
#include "deck.h"

class CardPile {
public:
	int mNumOfdecks;
	std::vector<Card*> mCards;
	std::vector<Card*> mOriginalCards;
	CardPile(int numOfdecks);
	void refresh();
	std::string print();
	void shuffle();
};

