#include "card.h"
#include <stdlib.h>
#include <string.h>

Card* Card__new(char *rank, char *suit) {
    Card *c = malloc(sizeof(Card));
    c->mRank = rank;
    c->mSuit = suit;
    c->mValue = Card__evaluate(c);
    c->mCount = Card__count(c);
    return c;
}

char* Card__print(Card* const self) {
    return self->mRank;
}

int Card__evaluate(Card* self) {
    if(strcmp(self->mRank,"J") == 0 || strcmp(self->mRank,"Q") == 0 || strcmp(self->mRank,"K") == 0) {
        return 10;
    } else if (strcmp(self->mRank,"A") == 0) {
        return 11;
    } else {
        return atoi(self->mRank);
    }
}

int Card__count(Card* self) {
    if(strcmp(self->mRank, "10") == 0 || strcmp(self->mRank, "J") == 0 || strcmp(self->mRank, "Q") == 0 || strcmp(self->mRank, "K") == 0 || strcmp(self->mRank, "A") == 0) {
        return -1;
    } else if(strcmp(self->mRank, "7") == 0 || strcmp(self->mRank, "8") == 0 || strcmp(self->mRank, "9") == 0) {
        return 0;
    } else {
        return 1;
    }
}