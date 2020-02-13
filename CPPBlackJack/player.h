#pragma once
#include <string>
#include <vector>

class Table;
class Card;
class Player {
 public:
  static int playerNumCount;
  const static int maxSplits = 10;

  std::string mPlayerNum;
  std::vector<Card*> mHand;
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

  Player(Table* table = NULL, Player* split = NULL);
  void doubleBet();
  virtual void resetHand();
  int canSplit();
  void win(float mult = 1);
  void lose();
  std::string print();
  int evaluate();
};
