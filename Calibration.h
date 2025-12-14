#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "TouchSensor.h"
#include "RunParameter.h"
#include "TimerJudgement.h"
#include "EV3ColorSensor.h"
#include "RearMotor.h"
#include "CalcCurrentLocation.h"

class Calibration
{

  public:
	explicit Calibration(ev3api::TouchSensor &touchSensor, RunParameter *runParameter, TimerJudgement *timerJudgement, EV3ColorSensor *ev3ColorSensor, RearMotor *rearMotor, CalcCurrentLocation *calcCurrentLocation);
	virtual ~Calibration();

	bool inputCode();
	bool calibrate();

  private:
	void decode();
	void startTimer();

	ev3api::TouchSensor &mTouchSensor;
	RunParameter *mRunParameter;
	TimerJudgement *mTimerJudgement;
	EV3ColorSensor *mEV3ColorSensor;
	RearMotor *mRearMotor;
	CalcCurrentLocation *mCalcCurrentLocation;

	char buf[20];

	//初期位置コード入力用変数
	int state = 0;
	int input;
	int one;
	int two;
	int three;
	int four;
	int power;
	/*	int black;
	int red;
	int yellow;
	int blue;*/
	int code[6] = {0, 0, 0, 0, 0, 0};

	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	//ブロック位置コードテーブル
	//	int redPosition[12] = {0, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 15};
	//	int yellowPosition[12] = {0, 1, 2, 4, 6, 7, 8, 9, 10, 11, 12, 14};
	//	int bluePosition[12] = {0, 1, 3, 5, 6, 7, 8, 10, 11, 13, 14, 15};

	//キャリブレーション用変数
	int state_b = 0;
	int white_brightness;
	int black_brightness;
	int target_brightness;
};

#endif
