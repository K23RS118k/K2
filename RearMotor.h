#ifndef REAR_MOTOR_H
#define REAR_MOTOR_H

#include "Motor.h"

class RearMotor{
public:
	explicit RearMotor(ev3api::Motor& leftWheel, ev3api::Motor& rightWheel);
	virtual ~RearMotor();

	void setSpeed(double left_power, double right_power);	//モータの出力値を設定する
	int getRightEncoder();									//右モータのエンコーダ値を取得する
	int getLeftEncoder();									//左モータのエンコーダ値を取得する
	void resetEncoder();									//エンコーダ値をリセットする


private:
	ev3api::Motor& mLeftWheel;
	ev3api::Motor& mRightWheel;
};

#endif
