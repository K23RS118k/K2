#include "LGarageTactics.h"

LGarageTactics::LGarageTactics()
{
	state = 0;
}

LGarageTactics::~LGarageTactics()
{
}

void LGarageTactics::execute()
{
	switch (state)
	{
	case 0:
		//走行パラメータを設定する
		mRunParameter->setForwardSpeed(10);
		//直進走行パラメータを更新
		mRunStraightAction->updateParameter();
		state++;

	case 1:
		/*アームパラメーター設定*/
		mRunParameter->setArmAngle(70);
		mRunParameter->setArmPower(30);
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(-20); //30°回転(左回転)
		mRunParameter->setRotateSpeed(20);  //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		mArmPositionSetAction->updateParameter();
		state++;
		break;

	case 2:
		//アーム上げ＋回転開始
		mRotateMachineAction->start();
		mArmPositionSetAction->start();
		if (mRotateMachineAction->isFinished() && mArmPositionSetAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			//アーム停止
			mArmPositionSetAction->stop();
			ev3_speaker_play_tone(NOTE_C6, 200);
			state = 10;
		}
		break;

	case 10:
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(20);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(45);
		mDistanceJudgement->start();
		state++;

	case 11:
		//45cm前進開始(ガレージ前まで移動)
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 20;
		}
		break;

		/* 駐車開始 */

	case 20:
		mRunParameter->setRotateAngle(-25); //20°回転(左回転)
		mRunParameter->setRotateSpeed(20);  //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		state++;

	case 21:
		//旋回開始
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			ev3_speaker_play_tone(NOTE_C6, 200);
			state = 30;
		}
		break;

	case 30:
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(20);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(27);
		mDistanceJudgement->start();
		state++;

	case 31:
		//24cm前進(ガレージ内へ侵入)
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 40;
		}
		break;

	case 40:
		mRunParameter->setRotateAngle(90); //90°回転(右回転)
		mRunParameter->setRotateSpeed(20); //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		state++;

	case 41:
		//ガレージ内で旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			ev3_speaker_play_tone(NOTE_C6, 200);
			state++;
		}
	default:
		break;
	}
}
