#ifndef R_GARAGE_TACTICS_H
#define R_GARAGE_TACTICS_H

/* 定数の定義 */
#define SPEED 0
#define KP 1
#define KI 2
#define KD 3

#include "Tactics.h"

class RGarageTactics : public Tactics
{
  public:
	explicit RGarageTactics();
	virtual ~RGarageTactics();

	void execute();
	void execute1();
	void execute2();
	void execute3();

	//private:/
	//int count = 0;
	double black = 0;
};

#endif
