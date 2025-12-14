#include "ArmPositionSetAction.h"

ArmPositionSetAction::ArmPositionSetAction()
{
}
ArmPositionSetAction::~ArmPositionSetAction()
{
}
/**
* 動作開始
*/
void ArmPositionSetAction::start()
{
	switch (state)
	{
	case 0:
		//パラメータの取得
		updateParameter();
		//角度の制限
		if (targetAngle > ARM_LIMIT_MAX)
		{
			targetAngle = ARM_LIMIT_MAX;
		}
		else if (targetAngle < ARM_LIMIT_MIN)
		{
			targetAngle = ARM_LIMIT_MIN;
		}
		state++;
	case 1:
		//目標角度と現在の角度の偏差を計算
		angleDiff[1] = targetAngle - mArmControl->getEncoder();

		//タイムアウト処理
		if (count++ == 125)
		{ //50ms経過したら
			if (angleDiff[1] - angleDiff[0] == 0)
			{ //アームの角度が50ms前から変化していない
				ev3_speaker_play_tone(NOTE_E6, 300);
				mArmControl->setPower(0);
				state++;
				break;
			}
			angleDiff[0] = angleDiff[1]; //50ms前の角度を更新
			count = 0;
		}

		if (angleDiff[1] == 0)
		{ //目標角度に到達
			ev3_speaker_play_tone(NOTE_C6, 200);
			mArmControl->setPower(0);
			state++;
			break;
		}
		else if (angleDiff[1] > 0)
		{ //目標角度より大きい
			mArmControl->setPower(power);
		}
		else
		{ //目標角度より小さい
			mArmControl->setPower(-power);
		}

		break;
	case 2:
		//アームを固定
		mArmControl->setBrake(true);
		//動作完了フラグを立てる
		flag = true;
		break;
	}
}
/**
* 動作終了
* 終了時に必ず呼ぶこと
*/
void ArmPositionSetAction::stop()
{
	mArmControl->setPower(0);
	mArmControl->setBrake(true);
	mRunParameter->setArmPower(default_power);
	count = 0;
	state = 0;
	flag = false;
}
/**
* パラメータ更新
*/
void ArmPositionSetAction::updateParameter()
{
	targetAngle = mRunParameter->getArmAngle();
	power = mRunParameter->getArmPower();
}
/**
* 動作終了判定
*/
bool ArmPositionSetAction::isFinished()
{
	return flag;
}
/**
* エンコーダ値リセット 
*/
void ArmPositionSetAction::reset()
{
	mArmControl->resetEncoder();
}