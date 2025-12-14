/**
* 車輪制御クラス
*/

#include "RearMotor.h"

/**
* コンストラクタ
* @param leftWheel 左モータ
* @param rightWheel 右モータ
*/
RearMotor::RearMotor(ev3api::Motor& leftWheel, ev3api::Motor& rightWheel)
: mLeftWheel(leftWheel), mRightWheel(rightWheel){
}

RearMotor::~RearMotor(){
}

/**
* モータの出力値を指定する
* @param left_power 左モータの出力値
* @param right_power 右モータの出力値
*/
void RearMotor::setSpeed(double left_power, double right_power){
	// 出力値を100から-100の間に修正
	if(left_power > 100){
		left_power = 100;
	}else if(left_power < -100){
		left_power = -100;
	}
	if(right_power > 100){
		right_power = 100;
	}else if(right_power < -100){
		right_power = -100;
	}

	// 左モータに出力値を設定
	mLeftWheel.setPWM(left_power);
	// 右モータに出力値を設定
	mRightWheel.setPWM(right_power);
}

/**
* エンコーダ値を取得する
* @return エンコーダ値
*/
int RearMotor::getRightEncoder(){
	return mRightWheel.getCount();
}

int RearMotor::getLeftEncoder(){
	return mLeftWheel.getCount();
}

/*　エンコーダ値をリセットする　*/
void RearMotor::resetEncoder(){
	mLeftWheel.reset();
	mRightWheel.reset();
}
