#ifndef L_GARAGE_TACTICS_H
#define L_GARAGE_TACTICS_H

/* 定数の定義 */
#define SPEED 0
#define KP 1
#define KI 2
#define KD 3

#include "Tactics.h"

class LGarageTactics : public Tactics{
public:
	explicit LGarageTactics();
	virtual ~LGarageTactics();

	void execute();
//private:/
	//int count = 0;
	double black = 0;
};

#endif
