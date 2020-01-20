// CPPBlackJack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "deck.h"

int main()
{
    Deck myDeck;
    myDeck.shuffle();
    std::cout << myDeck.print();
}

