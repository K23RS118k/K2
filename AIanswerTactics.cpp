#include "AIanswerTactics.h"

// ディスプレイ表示用
#define CALIB_FONT_HEIGHT (16 /*TODO: magic number*/)

AIanswerTactics::AIanswerTactics()
{
	state = 0;
}

AIanswerTactics::~AIanswerTactics()
{
}

void AIanswerTactics::execute()
{
	switch (state)
	{
	case 0:
		if (digitalfont())
		{
			state = 1;
		}
		break;
	case 1:
		if (textbookFont())
		{
			state = 99;
		}
		break;
		///////////////////////////////////////////////////////////////////////////////////////////////
		/*ディスプレイ表示*/
		///////////////////////////////////////////////////////////////////////////////////////////////
	case 99:
		// ディスプレイに表示
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		if (digital_answer == 0)
		{
			sprintf(buf, "digital is 0");
		}
		else if (digital_answer == 1)
		{
			sprintf(buf, "digital is 1");
		}
		else if (digital_answer == 2)
		{
			sprintf(buf, "digital is 2");
		}
		else if (digital_answer == 3)
		{
			sprintf(buf, "digital is 3");
		}
		else if (digital_answer == 4)
		{
			sprintf(buf, "digital is 4");
		}
		else if (digital_answer == 5)
		{
			sprintf(buf, "digital is 5");
		}
		else if (digital_answer == 6)
		{
			sprintf(buf, "digital is 6");
		}
		else if (digital_answer == 7)
		{
			sprintf(buf, "digital is 7");
		}
		// 3行目に表示
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);
		if (textbook_answer == 0)
		{
			sprintf(buf, "textbook is 0");
		}
		else if (textbook_answer == 1)
		{
			sprintf(buf, "textbook is 1");
		}
		else if (textbook_answer == 2)
		{
			sprintf(buf, "textbook is 2");
		}
		else if (textbook_answer == 3)
		{
			sprintf(buf, "textbook is 3");
		}
		else if (textbook_answer == 4)
		{
			sprintf(buf, "textbook is 4");
		}
		else if (textbook_answer == 5)
		{
			sprintf(buf, "textbook is 5");
		}
		else if (textbook_answer == 6)
		{
			sprintf(buf, "textbook is 6");
		}
		else if (textbook_answer == 7)
		{
			sprintf(buf, "textbook is 7");
		}
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);
		ev3_lcd_set_font(EV3_FONT_MEDIUM);
		state = 10;
		break;

		/*読み取り後ビット解答エリアへ移動*/

	case 10:
		//距離指定（20㎝）
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		//20cm前進パラメーター設定
		mRunParameter->setForwardSpeed(20);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		state++;
		break;
	case 11:
		//20㎝前進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//直進パラメータ設定
			mRunParameter->setForwardSpeed(20);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//黒検知のための設定
			black = mRunParameter->getBlack() + 7;
			state++;
		}
		break;
	case 12:
		//直進中黒線検知
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() < black)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//検知後の直進パラメータ
			mRunParameter->setForwardSpeed(20);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(10);
			mDistanceJudgement->start();
			state++;
		}
		break;
	case 13:
		//前進開始
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//前進停止
			mRunStraightAction->stop();

			//右旋回パラメーター
			mRunParameter->setRotateAngle(90);
			mRunParameter->setRotateSpeed(20);
			mRotateMachineAction->updateParameter();
			state++;
		}
		break;
	case 14:
		//右旋回開始
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			//前進パラメーター
			mRunParameter->setForwardSpeed(-13);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			state++;
		}
		break;
	case 15:
		//後退中黒線検知
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() < black)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//後退停止
			mRunStraightAction->stop();
			//前進パラメーター
			mRunParameter->setForwardSpeed(15);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(6);
			mDistanceJudgement->start();
			state++;
		}
		break;
	case 16:
		//6cm前進開始
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//前進停止
			mRunStraightAction->stop();
			//左旋回パラメーター
			mRunParameter->setRotateAngle(-87);
			mRunParameter->setRotateSpeed(20);
			mRotateMachineAction->updateParameter();
			state++;
		}
		break;
	case 17:
		//左旋回開始
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//左旋回停止
			mRotateMachineAction->stop();
			state++;
		}
		break;

		/* ビット解答開始 */

	case 18:
		//ビット解答のカウント判定
		if (bitAnswerCount < 0)
		{
			//ビット解答終了・縦列駐車との接続部分
			state = 20;
		}
		else
		{
			//ビット解答継続
			state++;
		}
		break;
	case 19:
		if (bitAnswer())
		{
			//ビット解答カウント
			bitAnswerCount--;
			state = 18;
		}
		break;
	case 20:
		//ビット解答後ライントレース
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setKP(0.3);
		mRunParameter->setKI(0.0);
		mRunParameter->setKD(2.5);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(20);
		mDistanceJudgement->start();
		state++;
		break;
	case 21:
		//20㎝ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//ライントレース停止
			ev3_speaker_play_tone(NOTE_D4, 100);
			mLineTraceAction->stop();
			state = 30;
		}
		break;
	case 30:
		//終了フラグを立てる
		flag = true;
		break;
	}
}

bool AIanswerTactics::digitalfont()
{
	switch (digital_state)
	{
	case 0:
		//パラメータ設定

		//緑設定
		green = mRunParameter->getBlack() + 6;
		//白設定
		white = mRunParameter->getWhite() * 0.7;
		//黒線検知用 WHY+6?
		black = mRunParameter->getBlack() + 6;
		digital_state = 1;
		break;
	case 1:

		//AI アンサーに入るためのライントレース設定
		mRunParameter->setRunRightEdgeFlag(false);
		mRunParameter->setKP(0.3); //0.3
		mRunParameter->setKI(0.0); //0.0
		mRunParameter->setKD(2.0); //1.6
		mRunParameter->setLineTraceSpeed(15);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		digital_state = 10;
		break;

	case 10:
		//AI アンサーに入るためのライントレース
		mLineTraceAction->start();
		//ライントレース中に直進検知をする
		if (mStraightDetection->isDetect())
		{
			//検知時の角度を保持
			angleRetention = mCalcCurrentLocation->getFinalAngle();
			mStraightDetection->reset();
		}
		//もしトレース中に緑を検知したら（切り替わる位置がAIアンサにより過ぎたら）
		if (mEV3ColorSensor->getBrightness() < green)
		{

			mLineTraceAction->stop();
			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-30);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			digital_state = 15;
		}
		//緑を検知せずに15センチ進んだら
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mLineTraceAction->stop();
			digital_state = 20;
		}
		break;

		//ライントレースに必要な距離が足りずに緑を読むか、トレース中に黒に入りすぎて緑として誤検知した場合の復帰state
	case 15:
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//旋回停止
			mRotateMachineAction->stop();
			//停止位置が白の上だったら回転して
			if (mEV3ColorSensor->getBrightness() > white)
			{
				//左旋回パラメーター設定
				mRunParameter->setRotateAngle(25);
				mRunParameter->setRotateSpeed(15);
				mRotateMachineAction->updateParameter();
				digital_state = 16;
			}
			else
			{
				//停止位置が緑の上だったら回転
				//左旋回パラメーター設定
				mRunParameter->setRotateAngle(32);
				mRunParameter->setRotateSpeed(15);
				mRotateMachineAction->updateParameter();
				digital_state = 18;
			}
		}
		break;

	case 16:
		//元の位置より5度左よりからトレース(case 10)再開
		//旋回開始
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			//AI アンサーに入るためのライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.3); //0.3
			mRunParameter->setKI(0.0); //0.0
			mRunParameter->setKD(2.0); //1.6
			mRunParameter->setLineTraceSpeed(15);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->setDistance(15);
			mDistanceJudgement->start();
			digital_state = 10;
		}
		break;
	case 18:
		//元の位置より2度右寄りから白検知のランストレート(case 50)へ移行
		//旋回開始
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			//ランストレート設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			mRunStraightAction->updateParameter();
			digital_state = 50;
		}
		break;

		//ライントレースに必要な距離が足りずに緑を読むか、トレース中に黒に入りすぎて緑として誤検知した場合の復帰state

	case 20:
		//ランストレート設定
		mRunParameter->setForwardSpeed(15);
		//保持した角度を設定
		mRunParameter->setTargetAngleFlag(true);
		mRunParameter->setStraightAngle(angleRetention);
		mRunStraightAction->updateParameter();
		digital_state = 30;
		break;

	case 30:
		//走行中緑検知
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() < green)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mRunStraightAction->stop();
			digital_state = 40;
		}
		break;

	case 40:
		//ランストレート設定
		mRunParameter->setForwardSpeed(15);
		//保持した角度を設定
		mRunParameter->setTargetAngleFlag(true);
		mRunParameter->setStraightAngle(angleRetention);
		mRunStraightAction->updateParameter();
		digital_state = 50;
		break;

	case 50:
		//
		mRunStraightAction->start();
		//走行中白検知
		if (mEV3ColorSensor->getBrightness() > white)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//距離設定
			mDistanceJudgement->setDistance(5);
			mDistanceJudgement->start();
			digital_state = 60;
		}
		break;

	case 60:
		//旋回のための位置補正
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			//右旋回パラメーター設定
			mRunParameter->setRotateAngle(90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			digital_state = 70;
		}
		break;

	case 70:
		//90度右旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//旋回停止
			mRotateMachineAction->stop();
			//ストレート設定
			mRunParameter->setForwardSpeed(-10);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(-10);
			mDistanceJudgement->start();
			digital_state = 80;
		}
		break;
	case 80:
		//後進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOutBack())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//後進
			mRunStraightAction->stop();
			//ライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.3);
			mRunParameter->setKI(0.0);
			mRunParameter->setKD(1.6);
			mRunParameter->setLineTraceSpeed(10);
			mLineTraceAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(10);
			mDistanceJudgement->start();
			digital_state = 90;
		}
		break;

	case 90:
		//ライントレース中角検知
		mLineTraceAction->start();
		//ライントレース中に直進検知をする
		if (mStraightDetection->isDetect())
		{
			//検知時の角度を保持
			angleRetention = mCalcCurrentLocation->getFinalAngle();
			mStraightDetection->reset();
		}
		if (mDistanceJudgement->isDistanceOut() && mEV3ColorSensor->getBrightness() < green + 6)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//8
			mRunParameter->setForwardSpeed(10);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離設定
			mDistanceJudgement->setDistance(5);
			mDistanceJudgement->start();
			digital_state = 100;
		}
		break;

	case 100:
		//旋回のための位置補正
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//Runstraight停止
			mRunStraightAction->stop();

			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			digital_state = 110;
		}
		break;
	case 110:
		////////////////////////////状態判別用case/////////////////////////
		//左旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			//////////////////////////////////////回転後の地点が白なら復帰動作digital_state = 115へ////////////////////////
			if (mEV3ColorSensor->getBrightness() > white)
			{
				//右旋回パラメーター設定
				mRunParameter->setRotateAngle(90);
				mRunParameter->setRotateSpeed(15);
				mRotateMachineAction->updateParameter();
				digital_state = 115;
			}
			else /////////////////////////////////回転後の地点が白ではないなら通常動作digital_state = 120へ///////////////////////
			{
				//ストレート設定
				mRunParameter->setForwardSpeed(-10);
				mRunParameter->setAngleResetFlag(true);
				mRunStraightAction->updateParameter();
				//距離指定
				mDistanceJudgement->setDistance(-2); /////////////本来の動作はー3センチだが一度距離が足らず失敗したので-2㎝にした
				mDistanceJudgement->start();
				digital_state = 120;
			}
		}

		break;

	case 115:
		////////復帰動作（回転後白なら誤動作とみなし回転前の位置に戻り、２㎝直進して回転を緑を検知するまで（case110）に戻るを繰り返す）///////
		//右旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			//ストレート設定
			mRunParameter->setForwardSpeed(10);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(2);
			mDistanceJudgement->start();
			digital_state = 116;
		}

		break;
	case 116:
		//////////状態判別用case（case110）へ////////////
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut() && mEV3ColorSensor->getBrightness() < green + 12)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//直進停止
			mRunStraightAction->stop();
			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			digital_state = 110;
		}
		break;

	case 118:

		break;

	case 120:
		//後進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOutBack())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//後進
			mRunStraightAction->stop();
			//ライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.3);
			mRunParameter->setKI(0.0);
			mRunParameter->setKD(1.6);
			mRunParameter->setLineTraceSpeed(10);
			mLineTraceAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(18.0);
			mDistanceJudgement->start();
			digital_state = 130;
		}
		break;
	case 130:
		//指定距離ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//ライントレース停止
			mLineTraceAction->stop();
			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			digital_state = 140;
		}
		break;

	case 140:
		//左旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			digital_state = 150;
		}
		break;

	case 150:
		//前進設定
		mRunParameter->setForwardSpeed(10);
		mDistanceJudgement->setDistance(13);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->start();
		digital_state = 160;
		break;

	case 160:
		//13cm前進　辺1を読む
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() < black + 15)
		{
			ev3_speaker_play_tone(NOTE_A4, 1000);
			digital_blackCount = 1;
		}
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mDistanceJudgement->stop();
			mRunStraightAction->stop();
			digital_state = 170;
		}
		break;

	case 170:
		//回転設定
		mRunParameter->setRotateAngle(90);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state = 180;
		break;

	case 180:
		//右90度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			digital_state = 190;
		}
		break;

	case 190:
		//前進設定
		mRunParameter->setForwardSpeed(10);
		mDistanceJudgement->setDistance(8.5);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->start();
		digital_state = 200;
		break;

	case 200:
		//8cm前進　辺２を読む
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() < black + 15)
		{
			ev3_speaker_play_tone(NOTE_A4, 1000);
			if (digital_answer == 0 && digital_blackCount == 0)
			{
				digital_answer = 4;
			}
			digital_blackCount = 2;
		}

		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mDistanceJudgement->stop();
			mRunStraightAction->stop();
			//digital_blackCountが0で辺２読まなかったとき
			if (digital_answer == 0 && digital_blackCount == 0)
			{
				digital_answer = 1;
			}
			digital_state = 210;
		}
		break;

	case 210:
		//回転設定
		mRunParameter->setRotateAngle(-90);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state = 220;
		break;

	case 220:
		//左90度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			digital_state = 230;
		}
		break;

	case 230:
		//前進設定
		mRunParameter->setForwardSpeed(10);
		mDistanceJudgement->setDistance(9);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		mDistanceJudgement->start();
		digital_state++;
		break;

	case 231:
		//9cm前進　
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_G6, 200);
			mDistanceJudgement->stop();
			mRunStraightAction->stop();
			digital_state = 240;
		}
		break;

	case 240:
		//角度を指定　
		mRunParameter->setRotateAngle(-30);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state++;
		break;

	case 241:
		//辺4を読むために旋回
		mRotateMachineAction->start();
		if (mEV3ColorSensor->getBrightness() < black + 20)
		{
			ev3_speaker_play_tone(NOTE_A4, 500);
			if (digital_answer == 0 && digital_blackCount == 1)
			{
				digital_answer = 2;
			}
			digital_blackCount = 3;
		}

		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			digital_state = 245;
		}
		break;

	case 245:
		//回転設定
		mRunParameter->setRotateAngle(-40);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state = 246;
		break;

	case 246:
		//左30度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			digital_state = 250;
		}
		break;

	case 250:
		//角度を指定　
		mRunParameter->setRotateAngle(-30);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state++;
		break;

	case 251:
		//辺3を読むために旋回（0を６と誤検知）
		mRotateMachineAction->start();
		if (mEV3ColorSensor->getBrightness() < black + 15)
		{
			ev3_speaker_play_tone(NOTE_A4, 500);
			if (digital_answer == 0 && digital_blackCount == 1)
			{
				digital_answer = 3;
			}
			else if (digital_answer == 0 && digital_blackCount == 2)
			{
				digital_answer = 5;
			}
			else if (digital_answer == 0 && digital_blackCount == 3)
			{
				digital_answer = 6;
			}
		}

		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			if (digital_answer == 0 && digital_blackCount == 1)
			{
				digital_answer = 7;
			}
			digital_state = 260;
		}

		break;

	case 260:
		//角度を指定　
		mRunParameter->setRotateAngle(-172);
		mRunParameter->setRotateSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRotateMachineAction->updateParameter();
		digital_state++;
		break;

	case 261:
		//デジタル読み取り後　教科書フォントを向くための旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 200);
			mRotateMachineAction->stop();
			digital_state++;
		}
		break;
	case 262:
		//ランストレート設定
		mRunParameter->setForwardSpeed(15);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(6);
		mDistanceJudgement->start();
		digital_state++;
		break;

	case 263:
		//走行中緑検知
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mRunStraightAction->stop();
			digital_state++;
		}
		break;

	case 264:
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		digital_state++;
		return true;
		break;

	default:
		break;
	}
	return false;
}

bool AIanswerTactics::textbookFont()
{
	//BluetoothLogger::dataLogger(0,0,(int16_t)textbook_state,(int16_t)mEV3ColorSensor->getBrightness(),0,0);
	switch (textbook_state)
	{
	case 0:
		//緑設定
		green = mRunParameter->getBlack() + 6;
		//白設定
		white = mRunParameter->getWhite() * 0.7;
		//ランストレート設定
		mRunParameter->setForwardSpeed(10);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		textbook_state++;
		break;

	case 1:
		//走行中白検知
		mRunStraightAction->start();
		if (mEV3ColorSensor->getBrightness() > white)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//距離設定
			mDistanceJudgement->setDistance(4);
			mDistanceJudgement->start();
			textbook_state = 20;
		}
		break;

	case 20:
		//指定距離前進後、停止する
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			//右旋回パラメーター設定
			mRunParameter->setRotateAngle(90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			textbook_state = 30;
		}
		break;

	case 30:
		//90度右旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//旋回停止
			mRotateMachineAction->stop();
			//ライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.3);
			mRunParameter->setKI(0.0);
			mRunParameter->setKD(1.6);
			mRunParameter->setLineTraceSpeed(10);
			mLineTraceAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(13);
			mDistanceJudgement->start();
			textbook_state = 40;
		}
		break;
	case 40:
		//指定距離ライントレース
		mLineTraceAction->start();
		//ライントレース中に直進検知をする
		if (mStraightDetection->isDetect())
		{
			//検知時の角度を保持
			angleRetention = mCalcCurrentLocation->getFinalAngle();
			mStraightDetection->reset();
		}
		if (mDistanceJudgement->isDistanceOut())
		{
			//ライントレース停止
			mLineTraceAction->stop();
			//ライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.0);
			mRunParameter->setKI(0.0);
			mRunParameter->setKD(0.0);
			mRunParameter->setLineTraceSpeed(10);
			mLineTraceAction->updateParameter();
			textbook_state = 50;
		}
		break;

	case 50:
		//ライントレース中角検知
		mLineTraceAction->start();
		if (mEV3ColorSensor->getBrightness() < green + 4)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//Runstraightパラメーター設定
			mRunParameter->setForwardSpeed(10);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			mRunStraightAction->updateParameter();
			//距離設定
			mDistanceJudgement->setDistance(4.5);
			mDistanceJudgement->start();
			textbook_state = 60;
		}
		break;

	case 60:
		//旋回のための位置補正
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//Runstraight停止
			mRunStraightAction->stop();
			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			textbook_state = 70;
		}
		break;

	case 70:
		//////////////////////////////////////////////////////状態判別用case//////////////////////////////////////////////////////////
		//90度左旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//旋回停止
			mRotateMachineAction->stop();
			//////////////////////////////////////回転後の地点が白なら復帰動作textbook_state = 75;へ////////////////////////
			if (mEV3ColorSensor->getBrightness() > white)
			{
				//右旋回パラメーター設定
				mRunParameter->setRotateAngle(90);
				mRunParameter->setRotateSpeed(15);
				mRotateMachineAction->updateParameter();
				textbook_state = 75;
			}
			else /////////////////////////////////回転後の地点が白ではないなら通常動作textbook_state = 80;へ///////////////////////
			{
				//ストレート設定
				mRunParameter->setForwardSpeed(-10);
				mRunParameter->setAngleResetFlag(true);
				mRunStraightAction->updateParameter();
				//距離指定
				mDistanceJudgement->setDistance(-3);
				mDistanceJudgement->start();
				textbook_state = 80;
			}
		}

		break;

	case 75:
		////////復帰動作（回転後白なら誤動作とみなし回転前の位置に戻り、２㎝直進して回転を緑を検知するまで（case70）に戻るを繰り返す）///////
		//右旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			//ストレート設定
			mRunParameter->setForwardSpeed(10);
			mRunParameter->setAngleResetFlag(true);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(2);
			mDistanceJudgement->start();
			textbook_state = 76;
		}

		break;
	case 76:
		//////////状態判別用case（case78）へ////////////
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut() && mEV3ColorSensor->getBrightness() < green + 12)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//直進停止
			mRunStraightAction->stop();
			//左旋回パラメーター設定
			mRunParameter->setRotateAngle(-90);
			mRunParameter->setRotateSpeed(15);
			mRotateMachineAction->updateParameter();
			textbook_state = 78;
		}
		break;

	case 78:
		//////////////////////////////////////////////////////状態判別用case//////////////////////////////////////////////////////////
		//90度左旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//旋回停止
			mRotateMachineAction->stop();
			//////////////////////////////////////回転後の地点が白なら復帰動作textbook_state = 75;へ////////////////////////
			if (mEV3ColorSensor->getBrightness() > white)
			{
				//右旋回パラメーター設定
				mRunParameter->setRotateAngle(90);
				mRunParameter->setRotateSpeed(15);
				mRotateMachineAction->updateParameter();
				textbook_state = 75;
			}
			else /////////////////////////////////回転後の地点が白ではないなら通常動作textbook_state = 80;へ///////////////////////
			{
				//ストレート設定
				mRunParameter->setForwardSpeed(-10);
				mRunParameter->setAngleResetFlag(true);
				mRunStraightAction->updateParameter();
				//距離指定
				mDistanceJudgement->setDistance(-2); /////////////本来の動作はー3センチだが一度距離が足らず失敗したので-2㎝にした
				mDistanceJudgement->start();
				textbook_state = 80;
			}
		}

		break;

	case 80:
		/////////////////////////////////////////////////////////////////////通常動作//////////////////////////////////////////////////////////////
		//後進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOutBack())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			//後進
			mRunStraightAction->stop();
			//ライントレース設定
			mRunParameter->setRunRightEdgeFlag(false);
			mRunParameter->setKP(0.3);
			mRunParameter->setKI(0.0);
			mRunParameter->setKD(1.6);
			mRunParameter->setLineTraceSpeed(10);
			mLineTraceAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(10.5); //ライントレースを2区間に分ける場合は(6.8)分けない場合は（10.8）でやっていた。
			mDistanceJudgement->start();
			textbook_state = 100; //ライントレースを2区間に分ける場合はtextbook_state = 90;
		}
		break;

		/*case 90:
                //指定距離ライントレース
                mLineTraceAction->start();
                if (mDistanceJudgement->isDistanceOut())
                {
                        //ライントレース停止
                        mLineTraceAction->stop();
                         //ライントレース設定
                        mRunParameter->setRunRightEdgeFlag(false);
                        mRunParameter->setKP(0.1);//このパラメータは未調整
                        mRunParameter->setKI(0.0);//このパラメータは未調整
                        mRunParameter->setKD(1.0);//このパラメータは未調整
                        mRunParameter->setLineTraceSpeed(10);
                        mLineTraceAction->updateParameter();
                        //距離指定
                        mDistanceJudgement->setDistance(4.0);
                        mDistanceJudgement->start();
                        textbook_state = 100;
                }
                break;
        */

		/*
        case 100: //ライントレースでズレが出ていた時の対処を実装する前のcase
                //指定距離ライントレース
                mLineTraceAction->start();
                if (mDistanceJudgement->isDistanceOut())
                {
                        //ライントレース停止
                        mLineTraceAction->stop();
                        //ライントレース後の位置の輝度値が目標値(緑と白の中間値)より大幅(±5)にずれているなら
                        //₊の場合は右旋回で目標値になるまで、―の場合は左旋回で目標値になるまで回転する

                        //左旋回パラメーター設定
                        mRunParameter->setRotateAngle(-90);
                        mRunParameter->setRotateSpeed(15);
                        mRotateMachineAction->updateParameter();
                        textbook_state = 110;
                }
                break;
        */

	case 100: //目標輝度値まで回転させるプログラム（目標値の取り方や決定法）が不安→ログをとって目標値を決定したが大会本番で決め打ちした値を使用すると失敗する可能性も
		//指定距離ライントレース
		//target = mRunParameter->getTargetBrightness();
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			//ライントレース停止
			mLineTraceAction->stop();
			//ライントレース後の位置の輝度値が目標値(緑と白の中間値にしたいのだが現状は最初に設定した白と黒の中間地になってる)より大幅(±5)にずれているなら
			//₊の場合は右旋回で目標値になるまで、
			if (mEV3ColorSensor->getBrightness() > 22)
			{

				ev3_speaker_play_tone(NOTE_C6, 100);
				//ライントレース中の輝度値が分かれば使う
				//textbook_state = 103;
				mRunParameter->setRotateAngle(-88); //白に入りすぎて回転した場合は回転角度を2度下げる
				mRunParameter->setRotateSpeed(10);
				mRotateMachineAction->updateParameter();
				textbook_state = 110;

			} //―の場合は左旋回で目標値になるまで回転する
			else if (mEV3ColorSensor->getBrightness() < 13)
			{

				ev3_speaker_play_tone(NOTE_C6, 1000);
				//ライントレース中の輝度値が分かれば使う
				//textbook_state = 106;
				mRunParameter->setRotateAngle(-93); //緑に入りすぎて回転した場合は回転角度を3度上げる
				mRunParameter->setRotateSpeed(10);
				mRotateMachineAction->updateParameter();
				textbook_state = 110;
			}
			else
			{
				//左旋回パラメーター設定
				mRunParameter->setRotateAngle(-90);
				mRunParameter->setRotateSpeed(10);
				mRotateMachineAction->updateParameter();
				textbook_state = 110;
			}
		}
		break;

		/*        case 103:
				//右旋回パラメーター設定
                mRunParameter->setRotateSpeed(-10);
                mRotateMachineAction->updateParameter();
                //右旋回
                mRotateMachineAction->start();
                if (mEV3ColorSensor->getBrightness() < 16)
                {
                        //目標値になれば旋回停止
                        mRotateMachineAction->stop();
                        //左旋回パラメーター設定
                        mRunParameter->setRotateAngle(-90);
                        mRunParameter->setRotateSpeed(10);
                        mRotateMachineAction->updateParameter();
                        textbook_state = 110;
                }
                break;

        case 106:
				//左旋回パラメーター設定
                mRunParameter->setRotateSpeed(10);
                mRotateMachineAction->updateParameter();
                //左旋回
                mRotateMachineAction->start();
                if (mEV3ColorSensor->getBrightness() > 13)
                {
                        //目標値になれば旋回停止
                        mRotateMachineAction->stop();
                        //左旋回パラメーター設定
                        mRunParameter->setRotateAngle(-90);
                        mRunParameter->setRotateSpeed(10);
                        mRotateMachineAction->updateParameter();
                        textbook_state = 110;
                }
                break;
*/

	case 110:
		//左旋回
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回後の角度を保持
			angleRetention = mCalcCurrentLocation->getFinalAngle();
			//旋回停止
			mRotateMachineAction->stop();
			//速度設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			mRunStraightAction->updateParameter();
			//距離指定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			textbook_state = 150;
		}
		break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*読み取り開始*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case 150: //区間1
		//フィルム上の黒の輝度値設定(黒を読み易い輝度値)
		black = mRunParameter->getBlack() + 18;

		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount1++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount1 >= 5)
			{
				black_flag1 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount1++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount1 >= 5)
			{
				white_flag1 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag1)
			{
				ev3_speaker_play_tone(NOTE_D6, 200);
				blackCount++;
				firstTime_black = 1;
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 160;
		}
		break;

	case 160: //区間2
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 5)
		{
			blackCount2++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount2 >= 5)
			{
				black_flag2 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount2++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount2 >= 5)
			{
				white_flag2 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag2)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 2;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 170;
		}
		break;

	case 170: //区間3
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 5)
		{
			blackCount3++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount3 >= 5)
			{
				black_flag3 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount3++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount3 >= 5)
			{
				white_flag3 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag3)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 3;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(1.5);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 180;
		}
		break;

	case 180: //区間4
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 4)
		{
			blackCount4++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount4 >= 5)
			{
				black_flag4 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount4++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount4 >= 5)
			{
				white_flag4 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag4)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 4;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(10);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(1.5);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 185;
		}
		break;

	case 185: //区間4.5
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 4)
		{
			blackCountM4++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCountM4 >= 5)
			{
				black_flagM4 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCountM4++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCountM4 >= 5)
			{
				white_flagM4 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flagM4)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 4;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(10);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 190;
		}
		break;

	case 190: //区間5
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 4)
		{
			blackCount5++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount5 >= 5)
			{
				black_flag5 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount5++;
			//ノイズ対策として3回目から白がある区間として判定
			if (whiteCount5 >= 5)
			{
				white_flag5 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag5)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 5;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(10);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 200;
		}
		break;

	case 200: //区間6
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 4)
		{
			blackCount6++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount6 >= 5)
			{
				black_flag6 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から17以上なら区間内に白があった→3が20回連続成功しかし0の時に3と誤検知なので）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount6++;
			//ノイズ対策として3回目から白がある区間として判定
			if (whiteCount6 >= 5)
			{
				white_flag6 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag6)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 6;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 210;
		}
		break;

	case 210: //区間7
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black + 4)
		{
			blackCount7++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount7 >= 5)
			{
				black_flag7 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount7++;
			//ノイズ対策として3回目から白がある区間として判定
			if (whiteCount7 >= 5)
			{
				white_flag7 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag7)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 7;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(10);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 220;
		}
		break;

	case 220: //区間8
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount8++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount8 >= 5)
			{
				black_flag8 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount8++;
			//ノイズ対策として3回目から白がある区間として判定
			if (whiteCount8 >= 3)
			{
				white_flag8 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag8)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 8;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 230;
		}
		break;

	case 230: //区間9
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount9++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount9 >= 5)
			{
				black_flag9 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount9++;
			//ノイズ対策として3回目から白がある区間として判定
			if (whiteCount9 >= 3)
			{
				white_flag9 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag9)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 9;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			mRunParameter->setAngleResetFlag(true);
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();

			//9区間の終わり10区間への変わり目で黒を読んだか（4か6の判定用）
			if (mEV3ColorSensor->getBrightness() < black)
			{
				black_flag_endline_of9 = true;
			}
			textbook_state = 240;
		}
		break;

	case 240: //区間10
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount10++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount10 >= 5)
			{
				black_flag10 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount10++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount10 >= 3)
			{
				white_flag10 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag10)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 10;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 250;
		}
		break;

	case 250: //区間11
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount11++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount11 >= 5)
			{
				black_flag11 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount11++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount11 >= 5)
			{
				white_flag11 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag11)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 11;
				}
			}
			//パラメーター設定
			mRunParameter->setForwardSpeed(15);
			//保持した角度を設定
			mRunParameter->setTargetAngleFlag(true);
			mRunParameter->setStraightAngle(angleRetention);
			//距離設定
			mDistanceJudgement->setDistance(3);
			mDistanceJudgement->start();
			mRunStraightAction->updateParameter();
			textbook_state = 260;
		}
		break;

	case 260: //区間12
		//Run開始
		mRunStraightAction->start();
		//区間内黒判定
		if (mEV3ColorSensor->getBrightness() < black)
		{
			blackCount12++;
			//ノイズ対策として5回目から黒がある区間として判定
			if (blackCount12 >= 5)
			{
				black_flag12 = true;
			}
		}
		//区間内白判定（フィルム上黒の値から-3以上なら区間内に白があった）
		if (mEV3ColorSensor->getBrightness() >= black + 4)
		{
			whiteCount12++;
			//ノイズ対策として5回目から白がある区間として判定
			if (whiteCount12 >= 5)
			{
				white_flag12 = true;
			}
		}
		//区間距離判定
		if (mDistanceJudgement->isDistanceOut())
		{
			//区間内黒カウント
			if (black_flag12)
			{
				ev3_speaker_play_tone(NOTE_C6, 100);
				blackCount++;
				//黒を初めて読んだ区間か判定
				if (blackCount == 1)
				{
					firstTime_black = 12;
				}
			}
			textbook_state = 270;
		}
		break;

		///////////////////////////////////////////////////////////////////////////////////////////////////
		/*3cm 12区間終了*/
		///////////////////////////////////////////////////////////////////////////////////////////////////

	case 270:
		//全区間走行完了
		mRunStraightAction->stop();

		//教科書フォントアンサー判定（初期設定は１）

		//4～8全てで黒を読み、かつ567全てで白を読まなかったら（前回までの判定条件4～8全てで黒を読み、かつ5又は6又は7で白を読まなかったらこれはまずい）

		if (black_flag4 && black_flagM4 && black_flag5 && black_flag6 && black_flag7 && black_flag8 && (white_flag5 == false && white_flag6 == false && white_flag7 == false))
		{
			textbook_answer = 0;

			//4と10で黒を読み、かつ8で黒を読まなかったら
		}
		else if (black_flag4 && black_flag10 && black_flag8 == false)
		{
			textbook_answer = 2;

			//4の後半と8と9で黒を読み、かつ5又は6又は7で白を読んだら（前回までの判定条件は4と8と9で黒を読み、かつ5又は6又は7で黒を読まなかったら）
		}
		else if (black_flagM4 && black_flag8 && black_flag9 && (white_flag5 == true || white_flag6 == true || white_flag7 == true))
		{
			textbook_answer = 3;

			/*8で初めて黒を読む又は、9で初めて黒を読む又は、7で初めて黒を読みかつ9の区間の終わりで黒を読まなかったら（6と誤検知したことがあったので下の条件分岐と比較してどちらを使用するか検討）
                                }else if(firstTime_black==8||firstTime_black==9||(firstTime_black==7&&black_flag_endline_of9==false)){
                                        textbook_answer=4;
                                */
			//8で初めて黒を読む又は、9で初めて黒を読む又は、7で初めて黒を読みかつ9で白を読んだら（6と誤検知したことがあったので下の条件分岐と比較してどちらを使用するか検討）
		}
		else if (firstTime_black == 8 || firstTime_black == 9 || (firstTime_black == 7 && white_flag9 == true))
		{
			textbook_answer = 4;
			//2又は3と8で黒を読み、かつ4又は5で黒を読まなかったら
		}
		else if ((black_flag2 || black_flag3) && black_flag8 && (black_flagM4 == false || black_flag5 == false))
		{
			textbook_answer = 5;

			/*6で初めて黒を読む又は、7で初めて黒を読みかつ9の区間の終わりでも黒だったら（4と誤検知したことがあったので下の条件分岐と比較してどちらを使用するか検討）
                                }else if(firstTime_black==6||(firstTime_black==7&&black_flag_endline_of9==true)){
                                        textbook_answer=6;        
                                */
			//6で初めて黒を読む又は、7で初めて黒を読みかつ9で白を読まなかったら（4と誤検知したことがあったので上の条件分岐と比較してどちらを使用するか検討）
		}
		else if (firstTime_black == 5 || firstTime_black == 6 || (firstTime_black == 7 && white_flag9 == false))
		{
			textbook_answer = 6;

			//2を読み、かつ8で黒を読まなかったら
		}
		else if (black_flag2 && black_flag8 == false)
		{
			textbook_answer = 7;
		}
		else if (white_flag5 == true && white_flag6 == true && white_flag7 == true && blackCount > 0)
		{
			//ここからは読み取り失敗時に出来る限り正解の数字に近づける条件
			textbook_answer = 4;
		}
		return true;
		break;
	}
	return false;
}

bool AIanswerTactics::bitAnswer()
{
	switch (bitstate)
	{
	case 0:
		//デジタルフォントアンサーを10進数を2進数へ
		for (i = 2; i >= 0; i--)
		{
			binary_digital[i] = digital_answer % 2;
			digital_answer = digital_answer / 2;
		}
		//教科書フォントのアンサーを10進数を2進数へ
		for (i = 2; i >= 0; i--)
		{
			binary_textbook[i] = textbook_answer % 2;
			textbook_answer = textbook_answer / 2;
		}
		bitstate = 9;
		break;
	case 9:
		//ライントレースパラメーター設定
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setRunRightEdgeFlag(true);
		mLineTraceAction->updateParameter();
		//十字検知までの距離指定
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		//十字検知のための変数
		black = mRunParameter->getBlack() + 6;
		bitstate = 10;
		break;

	case 10:
		//ライントレース中十字検知
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut() && mEV3ColorSensor->getBrightness() < black)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			bitstate++;
		}
		break;

	case 11:
		//前進パラメーター設定（位置補正）
		mRunParameter->setForwardSpeed(run_straight_speed);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(7.5); //7.5
		mDistanceJudgement->start();
		bitstate++;
		break;

	case 12:
		//7.5㎝前進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			bitstate = 20;
		}
		break;

	case 20:
		//右方回転のためのパラメーター設定
		mRunParameter->setRotateAngle(90);
		mRunParameter->setRotateSpeed(rotate_speed);
		mRotateMachineAction->updateParameter();
		bitstate++;
		break;

	case 21:
		//右に90度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			bitstate = 30;
		}
		break;

		/*教科書フォント側解答欄へ移動*/

	case 30:
		//距離指定とライントレースパラメーター設定
		mRunParameter->setRunRightEdgeFlag(false);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(9);
		mDistanceJudgement->start();
		bitstate++;
		break;

	case 31:
		//指定距離ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			bitstate++;
		}
		break;

	case 32:
		//ブロック保持後 指定距離前進　パラメーター設定
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		mRunParameter->setAngleResetFlag(true);
		mRunParameter->setForwardSpeed(run_straight_speed);
		mRunStraightAction->updateParameter();
		bitstate++;
		break;

	case 33:
		//指定距離前進(5cm)
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			bitstate = 40;
		}
		break;

		/*教科書フォントブロックを移動（0:左側へ,1:右側へ）*/

	case 40:
		//回転パラメーター設定
		if ((bitAnswerCount == 2 && binary_textbook[0] == 0) || (bitAnswerCount == 1 && binary_textbook[1] == 0) || (bitAnswerCount == 0 && binary_textbook[2] == 0))
		{
			mRunParameter->setRotateAngle(-30);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_textbook[0] == 1) || (bitAnswerCount == 1 && binary_textbook[1] == 1) || (bitAnswerCount == 0 && binary_textbook[2] == 1))
		{
			mRunParameter->setRotateAngle(30);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		break;
	case 41:
		//30度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			bitstate = 50;
		}
		break;

		/*アームでブロックを飛ばす*/

	case 50:
		//アームパラメーター設定
		mRunParameter->setArmAngle(70);
		mRunParameter->setArmPower(30);
		mArmPositionSetAction->updateParameter();
		bitstate++;
		break;

	case 51:
		//アームを既定の角度上げる
		mArmPositionSetAction->start();
		if (mArmPositionSetAction->isFinished())
		{
			mArmPositionSetAction->stop();
			bitstate++;
		}
		break;

	case 52:
		//アームの角度を初期位置に設定
		mRunParameter->setArmAngle(0);
		mRunParameter->setArmPower(15);
		mArmPositionSetAction->updateParameter();
		bitstate++;
		break;

	case 53:
		//アームを下げる
		mArmPositionSetAction->start();
		if (mArmPositionSetAction->isFinished())
		{
			mArmPositionSetAction->stop();
			bitstate = 60;
		}
		break;

	case 60:
		//旋回パラメーター設定（ブロック設置の方向により回転方向を切り替える）
		if ((bitAnswerCount == 2 && binary_textbook[0] == 0) || (bitAnswerCount == 1 && binary_textbook[1] == 0) || (bitAnswerCount == 0 && binary_textbook[2] == 0))
		{
			mRunParameter->setRotateAngle(-150);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_textbook[0] == 1) || (bitAnswerCount == 1 && binary_textbook[1] == 1) || (bitAnswerCount == 0 && binary_textbook[2] == 1))
		{
			mRunParameter->setRotateAngle(150);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		break;

	case 61:
		//150度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mRotateMachineAction->stop();
			//回転パラメーター設定
			mRunParameter->setRotateAngle(80);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate = 70;
		}
		break;

	case 70:
		//十字までライントレース　パラメーター設定
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		/* ブロック設置方向によりエッジを切り替える（0:左エッジ,1:右エッジ） */
		if ((bitAnswerCount == 2 && binary_textbook[0] == 0) || (bitAnswerCount == 1 && binary_textbook[1] == 0) || (bitAnswerCount == 0 && binary_textbook[2] == 0))
		{
			mRunParameter->setRunRightEdgeFlag(false);
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_textbook[0] == 1) || (bitAnswerCount == 1 && binary_textbook[1] == 1) || (bitAnswerCount == 0 && binary_textbook[2] == 1))
		{
			mRunParameter->setRunRightEdgeFlag(true);
			bitstate++;
		}
		break;

	case 71:
		//5㎝ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			bitstate++;
		}
		break;

	case 72:
		//十字検知
		mLineTraceAction->start();
		if (mEV3ColorSensor->getBrightness() < black)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			bitstate++;
		}
		break;

	case 73:
		//前進パラメーター設定（位置補正）
		mRunParameter->setForwardSpeed(run_straight_speed);
		mRunParameter->setAngleResetFlag(true);
		mRunStraightAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(6);
		mDistanceJudgement->start();
		bitstate++;
		break;

	case 74:
		//6㎝前進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			bitstate = 80;
		}
		break;

		/*
	デジタルフォント解答欄側へ移動
	*/

	case 80:
		//距離指定とライントレースパラメーター設定
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(9);
		mDistanceJudgement->start();
		bitstate++;
		break;

	case 81:
		//指定距離ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mDistanceJudgement->stop();
			bitstate++;
		}
		break;

	case 82:
		//ブロック保持後 指定距離前進　パラメーター設定
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		mRunParameter->setAngleResetFlag(true);
		mRunParameter->setForwardSpeed(run_straight_speed);
		mRunStraightAction->updateParameter();
		bitstate++;
		break;

	case 83:
		//指定距離前進(5cm)
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
			bitstate = 90;
		}
		break;

		/*
	デジタルフォント側ブロックをつかむ（0:右側,1:左側）
	*/

	case 90:
		//回転パラメーター設定
		if ((bitAnswerCount == 2 && binary_digital[0] == 0) || (bitAnswerCount == 1 && binary_digital[1] == 0) || (bitAnswerCount == 0 && binary_digital[2] == 0))
		{
			mRunParameter->setRotateAngle(30);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_digital[0] == 1) || (bitAnswerCount == 1 && binary_digital[1] == 1) || (bitAnswerCount == 0 && binary_digital[2] == 1))
		{
			mRunParameter->setRotateAngle(-30);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		break;

	case 91:
		//30度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();
			bitstate = 100;
		}
		break;

		/*アームでブロックを飛ばす*/

	case 100:
		//アームパラメーター設定
		mRunParameter->setArmAngle(70);
		mRunParameter->setArmPower(30);
		mArmPositionSetAction->updateParameter();
		bitstate++;
		break;

	case 101:
		//アームを80度上げる
		mArmPositionSetAction->start();
		if (mArmPositionSetAction->isFinished())
		{
			mArmPositionSetAction->stop();
			bitstate++;
		}
		break;

	case 102:
		//アームの角度を初期位置に設定
		mRunParameter->setArmAngle(0);
		mRunParameter->setArmPower(15);
		mArmPositionSetAction->updateParameter();
		bitstate++;
		break;

	case 103:
		//アームを下げる
		mArmPositionSetAction->start();
		if (mArmPositionSetAction->isFinished())
		{
			mArmPositionSetAction->stop();
			bitstate = 110;
		}
		break;

	case 110:
		//旋回パラメーター設定（ブロック設置の方向により回転方向を切り替える）
		if ((bitAnswerCount == 2 && binary_digital[0] == 0) || (bitAnswerCount == 1 && binary_digital[1] == 0) || (bitAnswerCount == 0 && binary_digital[2] == 0))
		{
			mRunParameter->setRotateAngle(150);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_digital[0] == 1) || (bitAnswerCount == 1 && binary_digital[1] == 1) || (bitAnswerCount == 0 && binary_digital[2] == 1))
		{
			mRunParameter->setRotateAngle(-150);
			mRunParameter->setRotateSpeed(rotate_speed);
			mRotateMachineAction->updateParameter();
			bitstate++;
		}
		break;

	case 111:
		//150度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mRotateMachineAction->stop();
			bitstate = 120;
		}
		break;

	case 120:
		//十字までライントレース　パラメーター設定
		mRunParameter->setRunRightEdgeFlag(false);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		/* ブロック設置方向によりエッジを切り替える（0:右エッジ,1:左エッジ） */
		if ((bitAnswerCount == 2 && binary_digital[0] == 0) || (bitAnswerCount == 1 && binary_digital[1] == 0) || (bitAnswerCount == 0 && binary_digital[2] == 0))
		{
			mRunParameter->setRunRightEdgeFlag(true);
			bitstate++;
		}
		else if ((bitAnswerCount == 2 && binary_digital[0] == 1) || (bitAnswerCount == 1 && binary_digital[1] == 1) || (bitAnswerCount == 0 && binary_digital[2] == 1))
		{
			mRunParameter->setRunRightEdgeFlag(false);
			bitstate++;
		}
		break;

	case 121:
		//5㎝ライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mDistanceJudgement->stop();
			bitstate++;
		}
		break;

	case 122:
		//ライントレース中十字検知
		mLineTraceAction->start();
		if (mEV3ColorSensor->getBrightness() < black)
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			bitstate++;
		}
		break;

	case 123:
		//7㎝前進(パラメーター設定)
		mDistanceJudgement->setDistance(7.5); //7
		mDistanceJudgement->start();
		mRunParameter->setAngleResetFlag(true);
		mRunParameter->setForwardSpeed(run_straight_speed);
		mRunStraightAction->updateParameter();
		bitstate++;
		break;

	case 124:
		//7.5㎝前進
		mRunStraightAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			mRunStraightAction->stop();
			bitstate = 130;
		}
		break;

	case 130:
		//左に回転パラメーター設定
		mRunParameter->setRotateAngle(-90);
		mRunParameter->setRotateSpeed(rotate_speed);
		mRotateMachineAction->updateParameter();
		bitstate++;
		break;

	case 131:
		//90度回転
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mRotateMachineAction->stop();
			bitstate++;
		}
		break;

	case 132:
		//黒線復帰後前進（距離設定）
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setKP(PID[0]);
		mRunParameter->setKI(PID[1]);
		mRunParameter->setKD(PID[2]);
		mRunParameter->setLineTraceSpeed(line_trace_speed_LOW);
		mLineTraceAction->updateParameter();
		//距離指定
		mDistanceJudgement->setDistance(8);
		mDistanceJudgement->start();
		bitstate++;
		break;

	case 133:
		//8cmライントレース
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			ev3_speaker_play_tone(NOTE_C6, 100);
			mDistanceJudgement->stop();
			bitstate = 140;
		}
		break;

	case 140:
		bitstate = 9;
		//終了フラグを立てる
		return true;
		break;
	}
	return false;
}

bool AIanswerTactics::isFinished()
{
	return flag;
}
