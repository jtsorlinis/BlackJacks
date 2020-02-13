#pragma once
#include "player.h"

class Dealer : public Player {
 public:
  bool mHideSecond;

  Dealer();
  void resetHand();
  int& upCard();
};
