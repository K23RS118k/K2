#ifndef STRAIGHT_DETECTION_H
#define STRAIGHT_DETECTION_H

#include "RearMotor.h"
#include "TimerJudgement.h"

class StraightDetection{
public:
	explicit StraightDetection(RearMotor* rearMotor, TimerJudgement* timerJudgement);
	virtual	~StraightDetection();

	bool isDetect();
	void reset();
private:
	RearMotor* mRearMotor;
	TimerJudgement* mTimerJudgement;

	const int ANGLE_THRESHOLD = 9;	//角度の閾値

	int state = 0;
	int encoder_diff[2] = {0, 0};
};

#endif
