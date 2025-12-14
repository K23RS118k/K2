#include "StraightDetection.h"

StraightDetection::StraightDetection(RearMotor *rearMotor, TimerJudgement *timerJudgement) : mRearMotor(rearMotor), mTimerJudgement(timerJudgement)
{
}

StraightDetection::~StraightDetection()
{
}

bool StraightDetection::isDetect()
{
	switch (state)
	{
	case 0:
		mTimerJudgement->setTime(300);
		mTimerJudgement->start();
		encoder_diff[0] = mRearMotor->getLeftEncoder() - mRearMotor->getRightEncoder();
		ev3_speaker_set_volume(20);
		state++;
	case 1:
		if (mTimerJudgement->isTimedOut())
		{
			ev3_speaker_play_tone(NOTE_C5, 100);
			mTimerJudgement->stop();
			mTimerJudgement->start();

			encoder_diff[1] = mRearMotor->getLeftEncoder() - mRearMotor->getRightEncoder();
			if (encoder_diff[1] - encoder_diff[0] > -ANGLE_THRESHOLD && encoder_diff[1] - encoder_diff[0] < ANGLE_THRESHOLD)
			{
				ev3_speaker_play_tone(NOTE_E5, 300);
				mTimerJudgement->stop();
				reset();
				return true;
			}
			encoder_diff[0] = encoder_diff[1];
		}
		break;
	}
	return false;
}

void StraightDetection::reset()
{
	state = 0;
	encoder_diff[0] = 0;
	encoder_diff[1] = 0;
	ev3_speaker_set_volume(50);
}