void *__dso_handle = 0;

/**
*	Lコースなら 0 (AIアンサー)
*	Rコースなら 1 (ブロック並べ)
*/
#define COURSE 1

// 使うクラスのヘッダファイルを全てインクルードする
#include "ev3api.h"
#include "assertion.h"
#include "ColorID.h"
#include "app.h"
#include "ColorSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"
#include "TouchSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "stdio.h"
#include "BluetoothLogger.h"
#include "EV3ColorSensor.h"
#include "RearMotor.h"
#include "ArmControl.h"
#include "CalcCurrentLocation.h"
#include "DistanceJudgement.h"
#include "TimerJudgement.h"
#include "StraightDetection.h"
#include "RunParameter.h"
#include "SpeedAdjustment.h"
#include "Tactics.h"
#include "SectionControlTactics.h"
#include "Action.h"
#include "LineTraceAction.h"
#include "EdgeChangeAction.h"
#include "ArmPositionSetAction.h"
#include "RunStraightAction.h"
#include "RotateMachineAction.h"
#include "CurvatureRunAction.h"
//#include "LineDetectTurnAction.h"
#include "DecelerationRotaryAction.h"
#include "Calibration.h"
#include "BlockLineUpBehavior.h"
// #include "RGarageTactics.h"
#include "IPCommunication.h"
#include "BlockLineUpSearch.h"
//#include "BlockBingo.h"
#include "etroboc_ext.h"
#include "FaceDisplay.h"
#include "Music.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6 /*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8 /*TODO: magic number*/)

// using宣言
using ev3api::Clock;
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::SonarSensor;
using ev3api::TouchSensor;

// Device objects
// オブジェクトを静的に確保する
Motor gArmMotor(PORT_A);
Motor gRightWheel(PORT_B);
Motor gLeftWheel(PORT_C);
Motor gTailMotor(PORT_D);
Clock gClock;
ColorSensor gColorSensor(PORT_2);
TouchSensor gTouchSensor(PORT_1);
GyroSensor gGyroSensor(PORT_4);
SonarSensor gSonarSensor(PORT_3);

// BluetoothLoggerのための参照を設定する
ev3api::TouchSensor &BluetoothLogger::mTouchSensor = gTouchSensor;
ev3api::SonarSensor &BluetoothLogger::mSonarSensor = gSonarSensor;
ev3api::ColorSensor &BluetoothLogger::mColorSensor = gColorSensor;
ev3api::GyroSensor &BluetoothLogger::mGyroSensor = gGyroSensor;
ev3api::Motor &BluetoothLogger::mRightWheel = gRightWheel;
ev3api::Motor &BluetoothLogger::mLeftWheel = gLeftWheel;
ev3api::Clock &BluetoothLogger::mClock = gClock;

// オブジェクトの定義
//static BluetoothLogger *gBluetoothLogger;
static EV3ColorSensor *gEV3ColorSensor;
static RearMotor *gRearMotor;
static ArmControl *gArmControl;
static CalcCurrentLocation *gCalcCurrentLocation;
static DistanceJudgement *gDistanceJudgement;
static TimerJudgement *gTimerJudgement;
static StraightDetection *gStraightDetection;
static RunParameter *gRunParameter;
static SpeedAdjustment *gSpeedAdjustment;
static EV3SonarSensor *gEV3SonarSensor;
static FaceDisplay *gFaceDisplay;
static Music *gMusic;
static SectionControlTactics *gSectionControlTactics;
static LineTraceAction *gLineTraceAction;
static EdgeChangeAction *gEdgeChangeAction;
static ArmPositionSetAction *gArmPositionSetAction;
static RunStraightAction *gRunStraightAction;
static RotateMachineAction *gRotateMachineAction;
static CurvatureRunAction *gCurvatureRunAction;
//static LineDetectTurnAction *gLineDetectTurnAction;
static DecelerationRotaryAction *gDecelerationRotaryAction;
static Calibration *gCalibration;
//static BlockLineUpBehavior *gBlockLineUpBehavior;
// static RGarageTactics *gRGarageTactics;
static IPCommunication *gIPCommunication;
static BlockLineUpSearch *gBlockLineUpSearch;
//static BlockBingo *gBlockBingo;
//static LightSensor *gLightSensor;

/**
* EV3システム生成
*/
static void user_system_create()
{
	// オブジェクトの生成
	gTimerJudgement = new TimerJudgement(gClock);
	gEV3ColorSensor = new EV3ColorSensor(gColorSensor);
	gEV3SonarSensor = new EV3SonarSensor(gSonarSensor);
	gFaceDisplay = new FaceDisplay();
	gMusic = new Music();
	gRearMotor = new RearMotor(gLeftWheel, gRightWheel);
	gArmControl = new ArmControl(gArmMotor, gTimerJudgement);
	gCalcCurrentLocation = new CalcCurrentLocation(gRearMotor);
	gDistanceJudgement = new DistanceJudgement(gCalcCurrentLocation);
	gStraightDetection = new StraightDetection(gRearMotor, gTimerJudgement);
	gRunParameter = new RunParameter();
	gSpeedAdjustment = new SpeedAdjustment(gClock, gRunParameter);
	gLineTraceAction = new LineTraceAction(gSpeedAdjustment);
	gEdgeChangeAction = new EdgeChangeAction();
	gArmPositionSetAction = new ArmPositionSetAction();
	gRunStraightAction = new RunStraightAction();
	gRotateMachineAction = new RotateMachineAction();
	gCurvatureRunAction = new CurvatureRunAction();
	//gLineDetectTurnAction = new LineDetectTurnAction();
	gDecelerationRotaryAction = new DecelerationRotaryAction();
	gSectionControlTactics = new SectionControlTactics(gColorSensor,gEV3SonarSensor);
	gCalibration = new Calibration(gTouchSensor, gRunParameter, gTimerJudgement, gEV3ColorSensor, gRearMotor, gCalcCurrentLocation);
	//gBlockLineUpBehavior = new BlockLineUpBehavior(gColorSensor, gLeftWheel, gRightWheel);
	// gRGarageTactics = new RGarageTactics();
	gIPCommunication = new IPCommunication();
	//gBlockLineUpSearch = new BlockLineUpSearch(gBlockLineUpBehavior, gRearMotor, gIPCommunication);
	//gBlockBingo = new BlockBingo();

	//Actionクラスに参照を設定する
	Action::setObject(gRunParameter, gRearMotor, gArmControl, gEV3ColorSensor, gCalcCurrentLocation, gLineTraceAction, gRunStraightAction, gCurvatureRunAction);
	//Tacticsクラスに参照を設定する
	Tactics::setObject(gEV3ColorSensor, gRunParameter, gCalcCurrentLocation, gDistanceJudgement, gTimerJudgement, gStraightDetection, gArmPositionSetAction, gLineTraceAction, gEdgeChangeAction, gRunStraightAction, gRotateMachineAction, gCurvatureRunAction, gDecelerationRotaryAction,gFaceDisplay,gMusic);

	// LEDをオレンジに光らせる
	ev3_led_set_color(LED_ORANGE);
}

/**
* EV3システム破棄
*/
static void user_system_destroy()
{
	//モータのエンコーダ値をリセット
	gRightWheel.reset();
	gLeftWheel.reset();
	gArmMotor.reset();
	gTailMotor.reset();

	//オブジェクトの削除
}

/*void ev3_cyc_run(intptr_t exinf)
{
	act_tsk(RUN_TASK);
}

void ev3_cyc_calibration(intptr_t exinf)
{
	act_tsk(CALIBRATION_TASK);
}
void ev3_cyc_camera(intptr_t exinf)
{
	act_tsk(CAMERA_TASK);
}
*/
/**
* メインタスク
*/

static int calib_state;
static int camera_state = 0;
static int spot[11] = {};
static int cardnumber = 0;
static int blocknumber = 0;

void main_task(intptr_t unused)
{
	user_system_create(); // センサやモータの初期化処理

	// // 電圧を表示する
	ev3_lcd_set_font(EV3_FONT_MEDIUM);
	char buf[20];
	int voltage = ev3_battery_voltage_mV();
	sprintf(buf, "Batt(AFT):%4.1f%%", voltage / 100.0);
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);
	//gSonarSensor->getDistance();


	// ev3_lcd_draw_string("COLOR", 0, CALIB_FONT_HEIGHT * 3);
	// char buf2[20];
	// sprintf(buf2, "Terget:%d", gRunParameter->getTargetBrightness());
	// ev3_lcd_draw_string(buf2, 0, CALIB_FONT_HEIGHT * 5);

	


	
	//1000ミリ秒バッテリー表示画面を保持する
	gTimerJudgement->setTime(1000);
	gTimerJudgement->start();
	while (!gTimerJudgement->isTimedOut())
	{
	}
	// ev3_speaker_set_volume(50);
	// ev3_speaker_play_tone(NOTE_C5, 100);

	/**
	* キャリブレーション
	*/

	//キャリブレーションタスク開始
	
	if (COURSE)
	{
		sta_cyc(EV3_CYC_CALIBRATION);
		//タスク終了まで待機
		slp_tsk();
		//キャリブレーションタスク終了
		stp_cyc(EV3_CYC_CALIBRATION);
	}
	
	//アーム位置固定
	/*
	while (!gArmControl->positionCalibrate(10)
	
	{
		gArmControl->positionCalibrate(10)
	}
	calib_state = 220;
	*/

	//キャリブレーションタスク再開
	// ev3_sta_cyc(EV3_CYC_CALIBRATION);
	// //タスク終了まで待機
	// slp_tsk();
	// //キャリブレーションタスク終了
	// ev3_stp_cyc(EV3_CYC_CALIBRATION);

	//gTimerJudgement->setTime(1000);
	//gTimerJudgement->start();
	// while (!gTimerJudgement->isTimedOut())
	// {
	// }
	// gTimerJudgement->stop();

	// while (!gTouchSensor.isPressed())
	// {
	// 	//タッチセンサが押されるまでループ
	// }
	// while (gTouchSensor.isPressed())
	// {
	// 	//タッチセンサが離されるまでループ
	// }
	// ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	// ev3_speaker_play_tone(NOTE_C6, 200);

	gFaceDisplay->load();
	// スタート前に輝度値の基準を適当に設定
	int black_brightness = 0;
	int white_brightness = 50;
	//int target_brightness = (MAX_BRIGHTNESS + black_brightness) / 2;
	//パラメータの格納
	//gEV3ColorSensor->setWhiteReference(white_brightness);
	gRunParameter->setWhite(white_brightness);
	gRunParameter->setBlack(black_brightness);
	//gRunParameter->setTargetBrightness(target_brightness);

	/* スタート */
	// 周期ハンドラ開始
	sta_cyc(EV3_CYC_RUN);
	//ev3_sta_cyc(EV3_CYC_CAMERA);

	// バックボタンが押されるまで待つ
	slp_tsk();

	// 再度電圧を表示する
	// voltage = ev3_battery_voltage_mV();
	// sprintf(buf, "Batt(AFT):%4.1f%%", voltage / 100.0);
	// ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);

	// int number = gIPCommunication->getNumber();
	// int block10 = gIPCommunication->getColorSircle();
	// int message = gIPCommunication->returnmessage();
	// sprintf(buf, "message: %d", message);
	// ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);
	// sprintf(buf, "camera: %d", camera_state);
	// ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);
	// sprintf(buf, "number,block : %d,%d", number, block10);
	// ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 7);

	// 周期ハンドラ停止
	stp_cyc(EV3_CYC_RUN);

	ETRoboc_notifyCompletedToSimulator();//競技終了通知

	// 終了処理
	user_system_destroy();

	// タスク終了
	ext_tsk();
}

void camera_task(intptr_t exinf)
{
	/*
	switch (camera_state)
	{
	case 10:
		char buf[20];
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		sprintf(buf, "camera_task");
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);
		break;
	case 11:
		camera_state = 0;
		wup_tsk(MAIN_TASK);
		break;
	}
	*/
	switch (camera_state)
	{
	case 0:
		//	IPCommunication::setStartFlag(); //走行開始状態を送信中
		if (gIPCommunication->setFlag('s'))
		{
			camera_state = 1;
			slp_tsk(); //タスク終了まで待機
		}
		break;
	case 1:
		if (gSectionControlTactics->isFinished()) //スピード競技終了まで待機
		{
			camera_state++;
		}
		break;
	case 2:
		//IPCommunication::setRequestFlag(); //ブロックビンゴ前の直線で送受信(１回目)
		if (gIPCommunication->setFlag('r'))
		{
			camera_state++;
		}
		break;
	case 3:
		//IPCommunication::setInfo(); //画像認識結果の情報をセット中(前半)
		if (gIPCommunication->recieveData())
		{
			gIPCommunication->setFlag('l');
			gIPCommunication->decode();
			camera_state++;
		}
		break;
	case 4:
		//IPCommunication::setRequestFlag(); //ブロックビンゴ前の直線で送受信(２回目)
		if (gIPCommunication->setFlag('r'))
		{
			camera_state++;
		}
		break;
	case 5:
		//IPCommunication::setInfo(); //画像認識結果の情報をセット中(後半)
		if (gIPCommunication->recieveData())
		{
			gIPCommunication->setFlag('l');
			gIPCommunication->decode();
			camera_state++;
		}
		break;
	case 6:
		//カメラタスク周期ハンドラ終了
		//ev3_stp_cyc(EV3_CYC_CAMERA);
		break;
	}
	ext_tsk();
}
/**
* キャリブレーションタスク
*/

void calibration_task(intptr_t exinf)
{
	switch (calib_state)
	{
		
	

	/*
	case 0:
		if (gCalibration->inputCode())
		{
			calib_state = 2;
		}
		break;
	*/

	case 0:
		if (gCalibration->calibrate())
		{
			calib_state = 1;
		}
		break;
	case 1:
		calib_state = 0;
		wup_tsk(MAIN_TASK);
		break;
	}
	ext_tsk();
}

/**
* ランタスク
*/
/**
* ランタスク
*/
// 
static int state = 10; // 5 or 12

void run_task(intptr_t exinf)
{
    if (ev3_button_is_pressed(BACK_BUTTON))
    {
        wup_tsk(MAIN_TASK); // バックボタン押下
    }
    else
    {
        gCalcCurrentLocation->calcCurrentLocation(); // 計算メソッド

        switch (state)
        {
			case 0:
                // スピード競技
                gSectionControlTactics->execute();
                if (gSectionControlTactics->isFinished())
                {
                    state = 2;
                }
            break;

            case 1:
                // コース情報を取得
                if (gIPCommunication->getCourceInfo_sim())
                {
                    for (int i = 1; i <= 10; i++)
                    {
                        spot[i] = gIPCommunication->getBlockSpot_sim(i);
                        syslog(LOG_NOTICE, "spot = %d", spot[i]);
                    }
                    cardnumber = gIPCommunication->getCardNumber_sim();
                    blocknumber = gIPCommunication->getBlockNumber_sim();
                }

                if (gTimerJudgement->isTimedOut() || gIPCommunication->isCompleted())
                {
                    gTimerJudgement->stop();
                    state++;
                }
            break;

            case 2:
                gBlockLineUpSearch->execute();
                if (gBlockLineUpSearch->isFinished())
                {
                    if (gBlockLineUpSearch->getDirection() == 4) state = 5;
                    else if (gBlockLineUpSearch->getDirection() == 1) state = 6;
                    else if (gBlockLineUpSearch->getDirection() == 3) state = 7;
                }
            break;

            case 10:
                
				gMusic->load("/ev3rt/res/kakekko1.wav");
				gMusic->play(15, SOUND_MANUAL_STOP);
                state = 0;
            break;

            case 11:
                gFaceDisplay->show();
				state = 0;
            break;

            case 12:
                gMusic->load("/ev3rt/res/kakekko1.wav");
                gMusic->play(15, SOUND_MANUAL_STOP);
                state = 0;
            break;

            case 13:
                gRunParameter->setArmPower(60);
                gRunParameter->setArmAngle(30);
                gArmPositionSetAction->updateParameter();
                state = 14;
            break;

            case 14:
                gArmPositionSetAction->start();
            break;

			case 101:
				gEV3ColorSensor->getEncodeHSV();
				gEV3ColorSensor->getColorBrightness();

				char buf3[30];
				sprintf(buf3, "Target:%d",
						gEV3ColorSensor->getColorBrightness());

				// 状態維持
				state = 101;
			break;
		
            default:
                break;
		}
            
    }
	ext_tsk();
}

 
