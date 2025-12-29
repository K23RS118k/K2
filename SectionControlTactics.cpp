#include "SectionControlTactics.h"
#include "BluetoothLogger.h"

#define CALIB_FONT_HEIGHT (16 /*TODO: magic number*/)

SectionControlTactics::SectionControlTactics(ev3api::ColorSensor &colorSensor,EV3SonarSensor* ev3sonarSensor)
	: mColorSensor(colorSensor),mEV3SonarSensor(ev3sonarSensor)
{
	state = 0;	//初めから
	//state = 2000; //フリーエリア用
}

SectionControlTactics::~SectionControlTactics()
{
}

/**
* 区間制御戦術を実行する
*/
void SectionControlTactics::execute()
{
	/**
	* 区間制御
	*/
	mEV3ColorSensor->getColorBrightness();
	//mEV3ColorSensor->getEncodeHSV();
	switch (state)
	{
	case 0: //スタート
		/* 走行パラメータを設定する */
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setChangeSpeedFlag(false);
		mRunParameter->setLineTraceSpeed(section0[SPEED]);
		mRunParameter->setKP(section0[KP]);
		mRunParameter->setKI(section0[KI]);
		mRunParameter->setKD(section0[KD]);
		mLineTraceAction->updateParameter();

		/* 現在位置を(0,0)に設定 */
		mCalcCurrentLocation->setPointY(0);
		mCalcCurrentLocation->setPointX(0);
		
		/* 座標補正までの距離を指定 */
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(285);
		mDistanceJudgement->start();

		//face load
		mFaceDisplay->load();

		// 音楽関連
		memfile_t memfile; // メモリファイルの構造体を作成
		ev3_memfile_load("/ev3rt/res/kakekko3.wav", &memfile); //SDカード内の"test.wav"をメモリファイルとしてロード
		ev3_speaker_set_volume(15); //音量の設定
		ev3_speaker_play_file(&memfile, SOUND_MANUAL_STOP); // 音声ファイルを再生
		// mMusic->load("/ev3rt/res/kakekko3.wav");
		// mMusic->play(15, SOUND_MANUAL_STOP);
		//mMusic->load("/ev3rt/res/jinguru.wav");
		//mMusic->play(15, SOUND_MANUAL_STOP);
		state=1;

		cycle_set(1500);// サイクルセット

		mFaceDisplay->setFace(0);
	break;


	case 1: //スタートカーブ手前
		/* ライントレースを実行 */
		mLineTraceAction->start();
		mFaceDisplay->show();
		if (mDistanceJudgement->isDistanceOut())
		{
			/* 座標を補正 */
			mLineTraceAction->stop();
			mDistanceJudgement->stop();
			mDistanceJudgement->setDistance(45);
			mDistanceJudgement->start();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section1[KP]);
			mRunParameter->setKI(section1[KI]);
			mRunParameter->setKD(section1[KD]);
			mLineTraceAction->updateParameter();
			state = 2;
		}
	break;

	case 2://第一カーブ走行
		mLineTraceAction->start();
		mFaceDisplay->show();
		if(mDistanceJudgement->isDistanceOut())
		{
			mRunParameter->setLineTraceSpeed(section0[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(section0[KD]);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->stop();
			mDistanceJudgement->setDistance(165);
			mDistanceJudgement->start();	
			state=3;
		}
	break;

	case 3://カーブ２
		mLineTraceAction->start();
		mFaceDisplay->show();
		if(mDistanceJudgement->isDistanceOut())
		{
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section1[KP]);
			mRunParameter->setKI(section1[KI]);
			mRunParameter->setKD(section1[KD]);
			mLineTraceAction->updateParameter();
			mLineTraceAction->stop();
			mDistanceJudgement->setDistance(60);
			mDistanceJudgement->start();
			state=4;
		}
	break;

	case 4://ゴールⅠ
		mLineTraceAction->start();
		mFaceDisplay->show();
		if(mDistanceJudgement->isDistanceOut())
		{
			mLineTraceAction->stop();
			mRunParameter->setLineTraceSpeed(section3[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(2.0);
			mLineTraceAction->updateParameter();
			memfile_t memfile; // メモリファイルの構造体を作成
			ev3_memfile_load("/ev3rt/res/bell5.wav", &memfile); //SDカード内の"test.wav"をメモリファイルとしてロード
			ev3_speaker_set_volume(15); //音量の設定
			ev3_speaker_play_file(&memfile, SOUND_MANUAL_STOP); // 音声ファイルを再生
			state=10;
		}
	break;

	case 10://フリーエリア前の壁検知
		mLineTraceAction->start();
		mFaceDisplay->show();
		if(mEV3SonarSensor->isDetected(13))
		{
			mLineTraceAction->stop();
			cycle_set(500);	
			state=11;
		}
	break;


	case 11:
		/*ライントレースを実行する*/
		if (cycle_start())
		{
			////ev3_speaker_set_volume(50);
			//ev3_speaker_play_tone(NOTE_C5, 100);
			mRunParameter->setRotateAngle(-84);
			mRunParameter->setRotateSpeed(10);
			mRotateMachineAction->updateParameter();
			state = 20;
		}
	break;

	case 20:
		mRotateMachineAction->start();
		if(mEV3ColorSensor->getColorBrightness()<=20 || mRotateMachineAction->isFinished())
		{
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section2[KP]);
			mRunParameter->setKI(section2[KI]);
			mRunParameter->setKD(section2[KD]);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->stop();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();
			state=21;
		}
	break;

	/*
	ここからフリーエリア
	*/

	case 2000:
		//フリーエリアデバッグ
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setChangeSpeedFlag(false);
		mRunParameter->setLineTraceSpeed(sectionFC2[SPEED]);
		mRunParameter->setKP(sectionFC2[KP]);
		mRunParameter->setKI(sectionFC2[KI]);
		mRunParameter->setKD(sectionFC2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=21;//デバッグ用
	break;	

	//青1エリア
	case 21://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->setFace(5);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		//mFaceDisplay->setFace(5);
		//mFaceDisplay->show();
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		mLineTraceAction->stop();
		cycle_set(500);
		state=5000;
	}
	break;

	case 5000:
		mLineTraceAction->stop();
		//memfile_t memfile; // メモリファイルの構造体を作成
		ev3_memfile_load("/ev3rt/res/jinglebell5.wav", &memfile); //SDカード内の"test.wav"をメモリファイルとしてロード
		ev3_speaker_set_volume(15); //音量の設定
		ev3_speaker_play_file(&memfile, SOUND_MANUAL_STOP); // 音声ファイルを再生
		state=22;
	break;


	case 5005:
	mLineTraceAction->stop();
	if(cycle_start()){
		state=22;
	}
	break;

	case 22://青1検知までライントレース
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_BLUE())
	{
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		
		////ev3_speaker_set_volume(50);
		//	//	ev3_speaker_play_tone(NOTE_C5, 320);;

		mFaceDisplay->setFace(2);

		state=23;
	}
	break;
	
	case 23://青1円を直進する
	mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();

		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();

		mFaceDisplay->setFace(5);

		state=24;
	}
	break;

	//青２エリア
	case 24://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();

		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();

		state = 500;
	}
	break;

	case 500:
	//青２検知までライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_BLUE())
	{
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();

		//	//	ev3_speaker_play_tone(NOTE_C5, 320);;

		mFaceDisplay->setFace(2);
		
		state=505;
	}
	break;

	//青２円を直進する
	case 505:
	mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();

		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();

		mFaceDisplay->setFace(5);

		state = 510;
	}
	break;
	
	//緑3エリア
	case  510://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=512;
	}
	break;

	case 512://緑3検知までライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_GREEN())
	{
	
		mFaceDisplay->setFace(4);
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();

		//	//	ev3_speaker_play_tone(NOTE_C5, 320);;

		state=515;
	}
	break;
	

	case 515://緑3円を直進する
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();

		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();

		mFaceDisplay->setFace(5);

		state=520;
	}
	break;

	//緑4ライントレース
	case 520://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=525;
	}
	break;

	case 525://緑4検知までライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_GREEN())
	{
		mLineTraceAction->stop();
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(4);
		mDistanceJudgement->start();
		//	//	ev3_speaker_play_tone(NOTE_C5, 320);;
		mFaceDisplay->setFace(4);
		state=530;		
	}
	break;

	case 530://緑4円を直進する
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		state = 540;
	}
	break;

	//第1コーナー右折する
	case 540:
	mRunStraightAction->straight(7,-4);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		////	ev3_speaker_play_tone(NOTE_C4 , 100);
		mRunParameter->setLineTraceSpeed(section30[SPEED]);
		mRunParameter->setKP(section30[KP]);
		mRunParameter->setKI(section30[KI]);
		mRunParameter->setKD(section30[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state=550;
	}
	break;

	//第2コーナーまで進む
	case 550:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
		state = 555;
	}
	break;

	case 555:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_GREEN()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(3);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(4);
		state = 560;
	}
	break;

	//緑8エリア
	//第2コーナーまで直進
	case 560:
    mRunStraightAction->straight(10,10);
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut()){
        mDistanceJudgement->stop();
        state = 565;
    }
    break;

	//第2コーナー右折する
	case 565:
	mRunStraightAction->straight(9,-4);
	mFaceDisplay->show();
	if(mEV3ColorSensor->getColorBrightness()<=20){
		mRunParameter->setLineTraceSpeed(section30[SPEED]);
		mRunParameter->setKP(section30[KP]);
		mRunParameter->setKI(section30[KI]);
		mRunParameter->setKD(section30[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();	
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();

		mFaceDisplay->setFace(5);
		state=570;
	}
	break;

	//緑7エリア
	case 570://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		state=572;
	}
	break;

	//緑７検知までライントレース
	case 572:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_GREEN()){
		mFaceDisplay->setFace(4);
		mFaceDisplay->show();
		////	ev3_speaker_play_tone(NOTE_C5 , 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(4);
		state = 580;
	}
	break;

	//緑７円を直進する
	case 580:
	mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();	
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 590;
	}
	break;

	//青6エリア
	//青６検知までライントレース
	case 590://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=592;
	}
	break;

	//青6検知までライントレース
	case 592:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_BLUE()){
		mFaceDisplay->setFace(2);
		//////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();

		state = 600;
	}
	break;

	//青６円を直進する
	case 600:
    mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
        state = 610;
    }
    break;

	//青5エリア
	case 610://強めのライントレース
    mLineTraceAction->start();
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut())
    {
		
        mDistanceJudgement->stop();
        mRunParameter->setLineTraceSpeed(section6[SPEED]);
        mRunParameter->setKP(section6[KP]);
        mRunParameter->setKI(section6[KI]);
        mRunParameter->setKD(section6[KD]);
        mLineTraceAction->updateParameter();
        state=612;
    }
    break;

	//青５検知までライントレース
	case 612:
    mLineTraceAction->start();
	mFaceDisplay->show();
    if(mEV3ColorSensor->isColor_BLUE()){
        //////	ev3_speaker_play_tone(NOTE_C5, 100);
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(1);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(2);
        state = 615;
    }
    break;

	case 615://青5円を直進する
	mRunStraightAction->straight(14,14);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();

		state = 620;
	}
	break;

	//第3コーナー左折
	case 620:
    mRunStraightAction->straight(0,10);
	mFaceDisplay->show();
    if(mEV3ColorSensor->getColorBrightness()<=15){
        ////	ev3_speaker_play_tone(NOTE_C5, 100);
        mRunParameter->setLineTraceSpeed(section30[SPEED]);
        mRunParameter->setKP(section30[KP]);
        mRunParameter->setKI(section30[KI]);
        mRunParameter->setKD(section30[KD]);
        mLineTraceAction->updateParameter();
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
        state=625;
    }
    break;

	case 625://強めのライントレース
		mLineTraceAction->start();
		mFaceDisplay->show();
		if(mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			mRunParameter->setLineTraceSpeed(section6[SPEED]);
			mRunParameter->setKP(section6[KP]);
			mRunParameter->setKI(section6[KI]);
			mRunParameter->setKD(section6[KD]);
			mLineTraceAction->updateParameter();
			state=630;
		}
	break;

	//第4コーナーまで進む
	case 630:
    mLineTraceAction->start();
	mFaceDisplay->show();
    if(mEV3ColorSensor->isColor_RED()){
		
        ////	ev3_speaker_play_tone(NOTE_C5, 100);
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(1);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(1);
        state = 640;
    }
    break;

	//赤9エリア
	//第4コーナーまで直進
	case 640:
    mRunStraightAction->straight(8,10);
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut()){
        mDistanceJudgement->stop();
        state = 645;
    }
    break;

	//第4コーナー左折
	case 645:
	mRunStraightAction->straight(0,15);
	mFaceDisplay->show();
	if(mEV3ColorSensor->getColorBrightness()<=20){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();	
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state=647;
	}
	break;


	//赤10エリア
	case 647://	強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		state=650;
	}
	break;	

	//赤10検知までライントレース
	case 650:
    mLineTraceAction->start();
	mFaceDisplay->show();
    if(mEV3ColorSensor->isColor_RED()){
		mFaceDisplay->setFace(1);
        ////	ev3_speaker_play_tone(NOTE_C5, 100);
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        state = 660;
    }
    break;

	//赤10円を直進する
	case 660:
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 670;
	}
	break;
	
	//黄11エリア
	case 670://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		
		state=672;
	}
	break;
	
	//黄11検知するまでライントレース
	case 672:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		state = 680;
	}
	break;

	case 680://黄11円を直進する
	mRunStraightAction->straight(15,15);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 682;
	}
	break;

	//黄12エリア
	case 682://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);				
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=690;
	}
	break;

	case 690://黄12検知するまでライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		state = 700;
	}
	break;

	case 700://黄12円を直進する
	mRunStraightAction->straight(15,14);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		state = 710;
	}
	break;
	
	//第５コーナー右折する
	case 710:
	mRunStraightAction->straight(6,-4);
	mFaceDisplay->show();
	if(mEV3ColorSensor->getColorBrightness()<=20){
		////	ev3_speaker_play_tone(NOTE_C4 , 100);
		mFaceDisplay->setFace(5);
		state=720;
	}
	break;

	//第６コーナーまで進む
	case 720:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(3);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		state = 722;
	}
	break;

	//赤13エリア
	//第6コーナーまで直進
	case 722:
    mRunStraightAction->straight(10,10);
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut()){
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
        state = 730;
    }
    break;

	//第６コーナー右折する
	case 730:
	mRunStraightAction->straight(9,-4);
	mFaceDisplay->show();
	if(mEV3ColorSensor->getColorBrightness()<=20){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();	
		mDistanceJudgement->setDistance(15);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state=733;
	}
	break;

	//黄15エリア
	case 733://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		
		mDistanceJudgement->stop();				
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);		
		mRunParameter->setKD(section6[KD]);			
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=740;
	}
	break;	

	//黄１５までライントレース
	case 740:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		////	ev3_speaker_play_tone(NOTE_C5 , 100);
		state = 750;
	}
	break;

	//黄15円を直進する
	case 750:
	mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 752;
	}
	break;

	//赤14エリア
	case 752://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())					
	{
		
		mDistanceJudgement->stop();				
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);		
		mRunParameter->setKD(section6[KD]);			
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=760;
	}
	break;

	//赤14検知までライントレース
	case 760:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_RED()){
		mFaceDisplay->setFace(1);
		mFaceDisplay->show();
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(1);
		state = 770;
	}
	break;

	//赤14円を直進する
	case 770:
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 780;
	}
	break;

	//赤13エリア
	case 780://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())					
	{
		
		mDistanceJudgement->stop();		
		mRunParameter->setLineTraceSpeed(section6[SPEED]);				
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);				
		mRunParameter->setKD(section6[KD]);			
		mLineTraceAction->updateParameter();			
		state=782;
	}
	break;

	//赤13検知
	case 782:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_RED()){
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mFaceDisplay->setFace(1);
		state = 790;
	}
	break;

	//180度回転して直進
	case 790:
	mRunStraightAction->straight(-5,8);
	mFaceDisplay->show();
    if(mEV3ColorSensor->getColorBrightness()<=20){
       mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();	
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
        state=800;
    }
    break;

	//赤14エリア
	case 800://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())				
	{
		mDistanceJudgement->stop();			
		mRunParameter->setLineTraceSpeed(section6[SPEED]);			
		mRunParameter->setKP(section6[KP]);			
		mRunParameter->setKI(section6[KI]);		
		mRunParameter->setKD(section6[KD]);			
		mLineTraceAction->updateParameter();		
		mDistanceJudgement->setDistance(40);		
		mDistanceJudgement->start();		
		state=802;	
	}
	break;	

	//赤１４までライントレース
	case 802:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_RED()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(11);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(1);
		////	ev3_speaker_play_tone(NOTE_C5 , 100);
		state = 810;
	}
	break;

	//赤14円を直進する
	case 810:
    mRunStraightAction->straight(13,11);
	mFaceDisplay->show();
    if(mDistanceJudgement->isDistanceOut()){
        mRunParameter->setLineTraceSpeed(section2[SPEED]);
        mRunParameter->setKP(section2[KP]);
        mRunParameter->setKI(section2[KI]);
        mRunParameter->setKD(section2[KD]);
        mLineTraceAction->updateParameter();
        mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
        state = 812;
    }
    break;


	//黄15エリア
	case 812://強めのライントレース
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();			
		mRunParameter->setLineTraceSpeed(section6[SPEED]);				
		mRunParameter->setKP(section6[KP]);			
		mRunParameter->setKI(section6[KI]);		
		mRunParameter->setKD(section6[KD]);	
		mLineTraceAction->updateParameter();		
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=820;
	}
	break;	

	//黄15検知までライントレース
	case 820:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		state = 830;
	}
	break;

	//黄15進入
	case 830:
	mRunStraightAction->straight(13,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut()){
		mRunParameter->setLineTraceSpeed(section2[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->stop();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
		mFaceDisplay->setFace(5);
		state = 840;
	}
	break;

	//黄16検知	
	//最後のストップ
	case 840:
	mLineTraceAction->start();
	mFaceDisplay->show();
	if(mEV3ColorSensor->isColor_YELLOW()){
		////	ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(12);
		mDistanceJudgement->start();
		mFaceDisplay->setFace(3);
		state = 845;
	}
	break;

	case 845:
	mRunStraightAction->straight(12,12);
	mFaceDisplay->show();
	if(mEV3SonarSensor->isDetected(5))//mEV3SonarSensor->isDetected(13)
		{
			mLineTraceAction->stop();
			cycle_set(500);	
			state=999;
		}
		break;




	case 28:
	mLineTraceAction->start();
	if(mDistanceJudgement->isDistanceOut()|| mEV3ColorSensor->isColor_GREEN())
	{
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state=220;
	}
	break;








	case 31://青検知
	
		mRunStraightAction->straight(25,25);
		if(mDistanceJudgement->isDistanceOut()|| mEV3ColorSensor->isColor_BLUE())
		{	
			mDistanceJudgement->setDistance(13);
			mDistanceJudgement->start();
			state=32;
		}
		break;

	case 32://青と青の間
		mRunStraightAction->straight(30,32);
		if(mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(section0[KD]);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();
			state=33;
		}
		break;

	case 33://間のライントレースと青検知
		mLineTraceAction->start();
		if(mDistanceJudgement->isDistanceOut()|| mEV3ColorSensor->isColor_GREEN())
		{
			mDistanceJudgement->stop();
			mDistanceJudgement->setDistance(13);
			mDistanceJudgement->start();

			state=34;

		}
	break;

	case 34://青から緑へ
		mRunStraightAction->straight(20,27);
		if(mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(section0[KD]);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();

			state=35;
		}
	break;

	case 35:
		mLineTraceAction->start();
		if(mDistanceJudgement->isDistanceOut()|| mEV3ColorSensor->isColor_BLUE())
		{
			state=220;
		}
	break;





	case 300:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section3[SPEED]);
		mRunParameter->setKP(section3[KP]);
		mRunParameter->setKI(section3[KI]);
		mRunParameter->setKD(section3[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 310:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() < -125) //175->110
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 40;
		}
		break;
	case 40:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section4[SPEED]);
		mRunParameter->setKP(section4[KP]);
		mRunParameter->setKI(section4[KI]);
		mRunParameter->setKD(section4[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 41:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() > 62) //110,92
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 50;
		}
		break;

	case 50:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section5[SPEED]);
		mRunParameter->setKP(section5[KP]);
		mRunParameter->setKI(section5[KI]);
		mRunParameter->setKD(section5[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 51:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() > -115) //143
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 60;
		}
		break;

	case 60:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 61:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() > -40) //y80
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 70;
		}
		break;

	case 70:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section7[SPEED]);
		mRunParameter->setKP(section7[KP]);
		mRunParameter->setKI(section7[KI]);
		mRunParameter->setKD(section7[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 71:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() >145 )
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 80;
		}
		break;

	case 80:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section8[SPEED]);
		mRunParameter->setKP(section8[KP]);
		mRunParameter->setKI(section8[KI]);
		mRunParameter->setKD(section8[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 81:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() > 165)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 90;
		}
		break;

	case 90:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section9[SPEED]);
		mRunParameter->setKP(section9[KP]);
		mRunParameter->setKI(section9[KI]);
		mRunParameter->setKD(section9[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 91:
		/*ライントレースを実行する*/
		mLineTraceAction->start();

		if (mCalcCurrentLocation->getPointY() < -55)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 95;
		}
		break;

	case 95:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section10[SPEED]);
		mRunParameter->setKP(section10[KP]);
		mRunParameter->setKI(section10[KI]);
		mRunParameter->setKD(section10[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 96:
		/*ライントレースを実行する*/
		mLineTraceAction->start();

		if (mCalcCurrentLocation->getPointY() < -100)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 110;
		}
		break;

	case 110:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section11[SPEED]);
		mRunParameter->setKP(section11[KP]);
		mRunParameter->setKI(section11[KI]);
		mRunParameter->setKD(section11[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 111:
		//ライントレースを実行する
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() < 155)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 120;
		}
		break;

	case 120:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section12[SPEED]);
		mRunParameter->setKP(section12[KP]);
		mRunParameter->setKI(section12[KI]);
		mRunParameter->setKD(section12[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 121:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() < 5) //85
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 130;
		}
		break;

	case 130:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section13[SPEED]);
		mRunParameter->setKP(section13[KP]);
		mRunParameter->setKI(section13[KI]);
		mRunParameter->setKD(section13[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 131:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() < -145) //Y68
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 140;
		}
		break;

	case 140:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section14[SPEED]);
		mRunParameter->setKP(section14[KP]);
		mRunParameter->setKI(section14[KI]);
		mRunParameter->setKD(section14[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 141:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() < -223)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 150;
		}
		break;

	case 150:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section15[SPEED]);
		mRunParameter->setKP(section15[KP]);
		mRunParameter->setKI(section15[KI]);
		mRunParameter->setKD(section15[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 151:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() < -263)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 160;
		}
		break;

	case 160:
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section16[SPEED]);
		mRunParameter->setKP(section16[KP]);
		mRunParameter->setKI(section16[KI]);
		mRunParameter->setKD(section16[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 161:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() > 40)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 170;
		}
		break;

	case 170: //青色通過
		//target = mRunParameter->getTargetBrightness();
		//mRunParameter->setTargetBrightness(target + 5);
		//走行パラメーターを設定する
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section17[SPEED]);
		mRunParameter->setKP(section17[KP]);
		mRunParameter->setKI(section17[KI]);
		mRunParameter->setKD(section17[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 171:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() > 45)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 211;
		}
		break;

	case 180:
		//走行パラメーターを設定
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section18[SPEED]);
		mRunParameter->setKP(section18[KP]);
		mRunParameter->setKI(section18[KI]);
		mRunParameter->setKD(section18[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state++;
		break;

	case 181:
		mCalcCurrentLocation->setPointX(187);
		mCalcCurrentLocation->setAngle(90);
		if (mStraightDetection->isDetect())
		{
			retentionAngle[0] = mCalcCurrentLocation->getFinalAngle();
		}
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() > 220)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state = 190;
		}
		break;

	case 190:
		//走行パラメーターを設定
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section19[SPEED]);
		mRunParameter->setKP(section19[KP]);
		mRunParameter->setKI(section19[KI]);
		mRunParameter->setKD(section19[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state = 200;
		break;

	case 200:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointY() > 248)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state++;
		}
		break;

	case 201:
		//走行パラメーターを設定
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setLineTraceSpeed(section20[SPEED]);
		mRunParameter->setKP(section20[KP]);
		mRunParameter->setKI(section20[KI]);
		mRunParameter->setKD(section20[KD]);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();
		state = 210;
		break;

	case 210:
		/*ライントレースを実行する*/
		mLineTraceAction->start();
		if (mCalcCurrentLocation->getPointX() < 155)
		{
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_C5, 100);
			state++;
		}
		break;

	case 211:
		/*カメラとの通信待ち*/
		mTimerJudgement->setTime(1500000);
		mTimerJudgement->start();
		mLineTraceAction->stop();
		state++;
		break;

	case 212:
		if (mTimerJudgement->isTimedOut())
		{ //タイムアウト
			//ev3_speaker_set_volume(50);
			////	ev3_speaker_play_tone(NOTE_DS6, 400);
			mTimerJudgement->stop();
			state = 220;
		}
		break;

	case 220:
		mLineTraceAction->stop();
		flag = false;
	break;

	case 7000:
			if(cycle_start())
			{
				mMusic->load("/ev3rt/res/kakekko1.wav");
        		mMusic->play(15, SOUND_MANUAL_STOP);
				state = 21;
			}
			//state = 7005;
	break;

	case 999:
		mLineTraceAction->stop();
		flag = false;
	break;

	default:
		break;
	}

	
}

bool SectionControlTactics::isFinished()
{
	return flag;
}

void SectionControlTactics::cycle_set(double cycle)
{
    cycle_time = cycle;
    cycle_count = 0;
}

bool SectionControlTactics::cycle_start()
{
    if (cycle_count < cycle_time)
    {
        cycle_count += 1;
        return false;
    }
    else
    {
        cycle_count = 0;
        return true;
    }
}