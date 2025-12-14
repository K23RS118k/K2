#include "RGarageTactics.h"

RGarageTactics::RGarageTactics()
{
	state = 0;
}

RGarageTactics::~RGarageTactics()
{
}

void RGarageTactics::execute() //直進入り
{
	switch (state)
	{
		    case 0:
        mRunParameter->setForwardSpeed(65);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(100);
        mDistanceJudgement->start();
        state++;
        break;
    case 1:
        mRunStraightAction->straight3();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            
        }
        
        break;
    



    /*case 2:
        //走行パラメータを設定する
        mRunParameter->setForwardSpeed(10);
        mRunParameter->setLineTraceSpeed(20);
        mRunParameter->setKP(0.5);
        mRunParameter->setKI(0);
        mRunParameter->setKD(5);
        mRunParameter->setRunRightEdgeFlag(true);
	case 0:
		//走行パラメータを設定する
		mRunParameter->setForwardSpeed(10);
		//直進走行パラメータを更新
		mRunStraightAction->updateParameter();
		state++;

	case 1:
		/*アームパラメーター設定*/
		mRunParameter->setArmAngle(40);
		mRunParameter->setArmPower(30);
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(-42); //45°回転(左回転)
		mRunParameter->setRotateSpeed(15);  //回転時のスピード
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

	case 10: //ガレージ前の黒線にたどりつくまで
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(30);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		state++;
		break;

	case 11:
		//35cm前進開始(黒線前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state++;
		}
		break;

	case 12:
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(43); //45°回転(右回転)
		mRunParameter->setRotateSpeed(20); //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		mArmPositionSetAction->updateParameter();
		state++;
		break;

	case 13:
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
			state++;
		}
		break;

	case 14: //ガレージ前の黒線
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(30);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(43);
		mDistanceJudgement->start();
		state++;
		break;

	case 15:
		//45cm前進開始(ガレージ前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 20;
		}
		break;

		/* 駐車開始 */

	case 20:
		mRunParameter->setRotateAngle(-95); //90°回転(左回転)
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
		mRunParameter->setForwardSpeed(40);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(42);
		mDistanceJudgement->start();
		state++;

	case 31:
		//52cm前進(ガレージ内へ侵入)
		mRunStraightAction->straight();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 40;
		}
	default:
		break;
	}
}

void RGarageTactics::execute1() //下から上へ
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
		mRunParameter->setArmAngle(40);
		mRunParameter->setArmPower(30);
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(-98); //90°回転(左回転)
		mRunParameter->setRotateSpeed(15);  //回転時のスピード
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

	case 10: //ガレージ前にたどりつくまで
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(25);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(45);
		mDistanceJudgement->start();
		state++;
		break;

	case 11:
		//35cm前進開始(黒線前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 20;
		}
		break;

		/* 駐車開始 */

	case 20:
		mRunParameter->setRotateAngle(-92); //90°回転(左回転)
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
		mRunParameter->setForwardSpeed(30);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(45);
		mDistanceJudgement->start();
		state++;

	case 31:
		//52cm前進(ガレージ内へ侵入)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 40;
		}
	default:
		break;
	}
}

void RGarageTactics::execute2() //上から下へ
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
		mRunParameter->setArmAngle(40);
		mRunParameter->setArmPower(30);
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(45); //50°回転(右回転)
		mRunParameter->setRotateSpeed(15); //回転時のスピード
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

	case 10: //ガレージ前の黒線にたどりつくまで
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(25);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(17);
		mDistanceJudgement->start();
		state++;
		break;

	case 11:
		//35cm前進開始(黒線前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state++;
		}
		break;

	case 12:
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(48); //35°回転(右回転)
		mRunParameter->setRotateSpeed(20); //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		mArmPositionSetAction->updateParameter();
		state++;
		break;

	case 13:
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
			state++;
		}
		break;

	case 14: //ガレージ前の黒線
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(25);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(38);
		mDistanceJudgement->start();
		state++;
		break;

	case 15:
		//45cm前進開始(ガレージ前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 20;
		}
		break;

		/* 駐車開始 */

	case 20:
		mRunParameter->setRotateAngle(-95); //90°回転(左回転)
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
		mRunParameter->setForwardSpeed(40);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(51);
		mDistanceJudgement->start();
		state++;

	case 31:
		//52cm前進(ガレージ内へ侵入)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 40;
		}
	default:
		break;
	}
}
void RGarageTactics::execute3() //直進入り緑
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
		mRunParameter->setArmAngle(40);
		mRunParameter->setArmPower(30);
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(42); //45°回転(左回転)
		mRunParameter->setRotateSpeed(15); //回転時のスピード
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

	case 10: //ガレージ前の黒線にたどりつくまで
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(30);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(32);
		mDistanceJudgement->start();
		state++;
		break;

	case 11:
		//35cm前進開始(黒線前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state++;
		}
		break;

	case 12:
		//旋回パラメーター設定
		mRunParameter->setRotateAngle(-43); //45°回転(右回転)
		mRunParameter->setRotateSpeed(20);  //回転時のスピード
		//回転走行パラメータを更新
		mRotateMachineAction->updateParameter();
		mArmPositionSetAction->updateParameter();
		state++;
		break;

	case 13:
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
			state++;
		}
		break;

	case 14: //ガレージ前の黒線
		//直進パラメータ設定
		mRunParameter->setForwardSpeed(30);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(38);
		mDistanceJudgement->start();
		state++;
		break;

	case 15:
		//45cm前進開始(ガレージ前まで移動)
		mRunStraightAction->straight3();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 20;
		}
		break;

		/* 駐車開始 */

	case 20:
		mRunParameter->setRotateAngle(-100); //90°回転(左回転)
		mRunParameter->setRotateSpeed(20);   //回転時のスピード
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
		mRunParameter->setForwardSpeed(40);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state++;

	case 31:
		//52cm前進(ガレージ内へ侵入)
		mRunStraightAction->straight();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			state = 40;
		}
	default:
		break;
	}
}
