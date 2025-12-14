#pragma once

#include "Tactics.h"

class BlockBingo : public Tactics
{
public:
  explicit BlockBingo();
  virtual ~BlockBingo();

  void execute();
  void straight();
  void execute_turn();
  void left_turn();
  void right_turn();
  int isFinished();

private:
  int state;
  int straight_state;
  int straightAngle;
  int color;

  bool flag = false;
};