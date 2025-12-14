#include "LineDetectTurnAction.h"

LineDetectTurnAction::LineDetectTurnAction(){
}
LineDetectTurnAction::~LineDetectTurnAction(){
}

void LineDetectTurnAction::start(){
	switch (state) {
		case 0:
			if(direction == TURN_RIGHT){
				if(forward){
					mRearMotor -> setSpeed(power, -(power / 5.0 * 4.0));
				}
				else{
					mRearMotor -> setSpeed(power, -power);
				}
			}
			else{
				if(forward){
					mRearMotor -> setSpeed(-(power / 5.0 * 4.0), power);
				}
				else{
					mRearMotor -> setSpeed(-power, power);
				}
			}

			if(mode == DETECT_BLACK){
				if(mEV3ColorSensor -> getBrightness() < black){
					ev3_speaker_play_tone(NOTE_C6, 200);
					state = 1;
				}
			}
			else{
				if(mEV3ColorSensor -> getBrightness() > white){
					ev3_speaker_play_tone(NOTE_C6, 200);
					state = 1;
				}
			}
			break;
		case 1:
			mRearMotor -> setSpeed(0, 0);
			flag = true;
			break;
	}
}

void LineDetectTurnAction::stop(){
	mRearMotor -> setSpeed(0, 0);
	state = 0;
	flag = false;
	mode = DETECT_BLACK;
	direction = TURN_RIGHT;
	mRunParameter -> setForwardTurnFlag(false);
}

void LineDetectTurnAction::updateParameter(){
	state = 0;
	flag = false;
	direction = mRunParameter -> getDetectTurnDirection();
	mode = mRunParameter -> getDetectTurnMode();
	forward = mRunParameter -> getForwardTurnFlag();
	white = mRunParameter -> getWhite() * 0.9;
	black = mRunParameter -> getBlack() + 3;
}

bool LineDetectTurnAction::isFinished(){
	return flag;
}
