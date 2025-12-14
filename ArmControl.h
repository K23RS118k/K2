#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

/* 定数の定義 */
#define INTERVAL 300		  //キャリブレーション時の判定時間の間隔
#define CALIBRATE_POSITION 30 //キャリブレーション時の角度

#include "Motor.h"
#include "TimerJudgement.h"

class ArmControl
{
  public:
	explicit ArmControl(ev3api::Motor &armMotor, TimerJudgement *timerJudgement);
	virtual ~ArmControl();
	void setPower(int power);  //モータの出力値を設定する
	int getEncoder();		   //エンコーダ値を取得する
	void resetEncoder();	   //エンコーダ値を初期化する
	void setBrake(bool brake); //ブレーキを設定する(true:オン, false:オフ)
	bool positionCalibrate();  //アーム位置のキャリブレーション
  private:
	ev3api::Motor &mArmMotor;
	TimerJudgement *mTimerJudgement;

	int state = 0;
	int armEncoder[2] = {0, 0}; //エンコーダ値
};

#endif
