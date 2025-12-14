#include "ArmControl.h"

ArmControl::ArmControl(ev3api::Motor& armMotor, TimerJudgement* timerJudgement)
: mArmMotor(armMotor), mTimerJudgement(timerJudgement){
}

ArmControl::~ArmControl(){
}

/**
* モータの出力値を指定する
* @param power 出力値
*/
void ArmControl::setPower(int power){
	// 出力値を100から-100の間に修正
	if(power > 100){
		power = 100;
	}
	else if(power < -100){
		power = -100;
	}
	// 出力値を設定
	mArmMotor.setPWM(power);
}

/**
* エンコーダ値を取得する
* @return エンコーダ値
*/
int ArmControl::getEncoder(){
	return mArmMotor.getCount();
}
/**
* エンコーダ値を初期化する
*/
void ArmControl::resetEncoder(){
	mArmMotor.reset();
}
/**
* ブレーキを設定する
* @param ブレーキのオンオフ
*
*/
void ArmControl::setBrake(bool brake){
	mArmMotor.setBrake(brake);
}

/**
* アーム位置のキャリブレーション
* @return キャリブレーションが終了したかどうか
*/
bool ArmControl::positionCalibrate(){
	switch (state) {
		case 0:
			//判定する時間を設定
			mTimerJudgement -> setTime(INTERVAL);
			resetEncoder();
			state = 1;
			break;
		case 1:
			//時間計測開始
			mTimerJudgement -> start();
			state = 2;
			break;
		case 2:
			//アームを後方に動かす
			setPower(-10);
			if(mTimerJudgement -> isTimedOut()){//指定時間経過
				//時間計測終了
				mTimerJudgement -> stop();
				armEncoder[1] = getEncoder();
				if(armEncoder[0] - armEncoder[1] < 1){//アームの角度変化量が1°未満
					state = 3;
				}
				else{
					//過去のエンコーダ値を更新
					armEncoder[0] = armEncoder[1];
					state = 1;
				}
			}
			break;
		case 3:
			ev3_speaker_play_tone(NOTE_G6, 100);
			setPower(0);	//アーム停止
			resetEncoder();	//エンコーダ値をリセット
			state = 4;
			break;
		case 4:
			//アームを前方に動かす
			setPower(10);
			if(getEncoder() >= CALIBRATE_POSITION){//指定角度まで動かす
				ev3_speaker_play_tone(NOTE_G6, 100);
				setPower(0);	//アーム停止
				setBrake(true);
				resetEncoder();	//エンコーダ値をリセット
				return true;	//動作終了
			}
		break;
	}
	return false;	//動作中
}
