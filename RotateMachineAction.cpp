#include "RotateMachineAction.h"

RotateMachineAction::RotateMachineAction()
{
}
RotateMachineAction::~RotateMachineAction()
{
}

void RotateMachineAction::start()
{
	switch (state)
	{
	case 0:
		updateParameter();
		startAngle = mCalcCurrentLocation->getFinalAngle();
		angle[0] = 0;
		if (targetAngle >= 0)
		{
			state = 1;
		}
		else
		{
			state = 2;
		}
		break;
	case 1:
		angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
		angle[0] = angle[1];
		currentAngle = angle[1] / M_PI * 180;

		//			currentAngle = mCalcCurrentLocation -> getFinalAngle();

		mRearMotor->setSpeed(power, -power);
		if (currentAngle >= targetAngle)
		{
			//ev3_speaker_play_tone(NOTE_C6, 200);
			flag = true;
		}
		break;
	case 2:
		angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
		angle[0] = angle[1];
		currentAngle = angle[1] / M_PI * 180;

		//			currentAngle = mCalcCurrentLocation -> getFinalAngle();

		mRearMotor->setSpeed(-power, power);
		if (currentAngle <= targetAngle)
		{
			//ev3_speaker_play_tone(NOTE_C6, 200);
			flag = true;
		}
		break;
	}
}

void RotateMachineAction::stop()
{
	mRearMotor->setSpeed(0, 0);
	state = 0;
	flag = false;
}

void RotateMachineAction::updateParameter()
{
	targetAngle = mRunParameter->getRotateAngle();
	power = mRunParameter->getRotateSpeed();
	state = 0;
	flag = false;
}

bool RotateMachineAction::isFinished()
{
	return flag;
}
