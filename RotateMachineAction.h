#ifndef ROTATE_MACHINE_ACTION_H
#define ROTATE_MACHINE_ACTION_H

#include "Action.h"
#include "math.h"

class RotateMachineAction : public Action
{
public:
	explicit RotateMachineAction();
	virtual ~RotateMachineAction();

	void start();
	void stop();
	void updateParameter();
	bool isFinished();

private:
	int state = 0;
	bool flag = false;
	int targetAngle;
	double startAngle;
	double angle[2] = {0, 0};
	double currentAngle;
	int power;
	bool forward = false;
};

#endif
