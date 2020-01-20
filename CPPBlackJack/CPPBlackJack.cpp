// CPPBlackJack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cardpile.h"

int main()
{
    CardPile cardpile(5);
    cardpile.shuffle();
    std::cout << cardpile.print();
}

