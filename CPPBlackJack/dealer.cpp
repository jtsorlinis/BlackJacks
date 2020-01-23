#include "dealer.h"
#include "card.h"
#include <iostream>

Dealer::Dealer() {
	mPlayerNum = "D";
	mValue = 0;
	mHideSecond = true;
}

void Dealer::resetHand() {
	mHand.clear();
	mValue = 0;
	mHideSecond = true;
}

int& Dealer::upCard() {
	return mHand[0].mValue;
}
