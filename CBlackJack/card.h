#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int mCount;
  bool mIsAce;
  char* mRank;
  int mValue;
  char* mSuit;
  bool mFaceDown;
} Card;

Card* Card__new(char* rank, char* suit);
char* Card__print(Card* self);
int Card__evaluate(Card* self);
int Card__count(Card* self);
