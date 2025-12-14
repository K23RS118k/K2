#ifndef LINE_DETECT_TURN_ACTION_H
#define LINE_DETECT_TURN_ACTION_H

#define DETECT_BLACK true
#define DETECT_WHITE false
#define TURN_RIGHT true
#define TURN_LEFT false

#include "Action.h"

class LineDetectTurnAction : public Action{
public:
	explicit LineDetectTurnAction();
	virtual ~LineDetectTurnAction();

	void start();
	void stop();
	void updateParameter();
	bool isFinished();

private:
	int state = 0;
	bool flag = false;
	//bool mode = BLACK;
	bool direction = TURN_RIGHT;
	bool forward = false;
	int power = 15;
	int white;
	int black;

};

#endif
