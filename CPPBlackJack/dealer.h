#pragma once
#include "player.h"

class Dealer : public Player {
 public:
  bool m_hide_second_;

  Dealer();
  void reset_hand() override;
  int& up_card();
};
