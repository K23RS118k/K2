#pragma once

#include "Action.h"
#include "math.h"

class DecelerationRotaryAction : public Action
{
public:
  explicit DecelerationRotaryAction();
  virtual ~DecelerationRotaryAction();

  void start();
  void stop();
  void updateParameter();
  bool isFinished();

private:
  int state = 0;
  bool flag = false;

  /*Runparameterでセットされる値*/
  double targetAngle;
  double power;

  double currentAngle;
  double angle[2] = {0, 0};

  double split_angle[5] = {0, 0, 0, 0, 0};
  double split_speed[5] = {0, 0, 0, 0, 0};
};
