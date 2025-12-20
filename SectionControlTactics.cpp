#include "SectionControlTactics.h"
#include "BluetoothLogger.h"

#define CALIB_FONT_HEIGHT (16 /*TODO: magic number*/)

SectionControlTactics::SectionControlTactics(ev3api::ColorSensor &colorSensor,EV3SonarSensor* ev3sonarSensor)
	: mColorSensor(colorSensor),mEV3SonarSensor(ev3sonarSensor)
{
	state = 0;
}

SectionControlTactics::~SectionControlTactics()
{
}

/**
* 区間制御戦術を実行する
*/
void SectionControlTactics::execute()
{

	//BluetoothLogger::dataLogger(0, 0, (int16_t)mCalcCurrentLocation->getPointX(), (int16_t)mCalcCurrentLocation->getPointY(), (int16_t)mCalcCurrentLocation->getAngle(), (int16_t)state);

	//	BluetoothLogger::dataLogger((int8_t)state, 0, (int16_t)mLineTraceAction -> getSpeed(), (int16_t)mLineTraceAction -> getKP(), (int16_t)mLineTraceAction -> getKI(),(int16_t)mLineTraceAction -> getKD());
	/**
	* 区間制御
	*/
	switch (state)
	{
	case 0: //スタート
		/* 走行パラメータを設定する */
		mRunParameter->setRunRightEdgeFlag(true);
		mRunParameter->setChangeSpeedFlag(false);
		//ev3_speaker_set_volume(50);
		// mRunParameter->setLineTraceSpeed(section0[SPEED]);
		// mRunParameter->setKP(section0[KP]);
		// mRunParameter->setKI(section0[KI]);
		// mRunParameter->setKD(section0[KD]);
		//フリーエリアデバッグ
		 mRunParameter->setLineTraceSpeed(sectionFC2[SPEED]);
		 mRunParameter->setKP(sectionFC2[KP]);
		 mRunParameter->setKI(sectionFC2[KI]);
		 mRunParameter->setKD(sectionFC2[KD]);
		
		//mCalcCurrentLocation->setPointY(0);
		//mCalcCurrentLocation->setPointX(0);
		/* ライントレースの走行パラメータを更新 */
		mLineTraceAction->updateParameter();

		/* 座標補正までの距離を指定 */
		//mDistanceJudgement->setDistance(295);//正式バージョン
		//mDistanceJudgement->setDistance(300);//フリーエリアデバッグ用
		mDistanceJudgement->setDistance(40);//フリーエリアから用
		mDistanceJudgement->start();
		//state=1;//正式バージョン
		//state = 20;//格子エリアから
		state=21;//デバッグ用
		//state = 650; //赤９～赤１０
		//state = 780;
		mEV3ColorSensor->getColorBrightness();
		break;

	case 1: //スタートカーブ手前
		/* ライントレースを実行 */
		mLineTraceAction->start();
		if (mDistanceJudgement->isDistanceOut())
		{
			/* 座標を補正 */
			//ev3_speaker_play_tone(NOTE_C5, 320);
			mLineTraceAction->stop();
			mDistanceJudgement->stop();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section1[KP]);
			mRunParameter->setKI(section1[KI]);
			mRunParameter->setKD(section1[KD]);
			mLineTraceAction->updateParameter();
			state =2;
		}
		break;

	case 2://第一カーブ走行
		mLineTraceAction->start();
		if(mDistanceJudgement->isDistanceOut())
		{
			mDistanceJudgement->stop();
			mRunParameter->setLineTraceSpeed(section0[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(section0[KD]);
			mDistanceJudgement->setDistance(170);
			mDistanceJudgement->start();
			mLineTraceAction->updateParameter();

			state=3;
		}
		break;

	case 3://カーブ２
		mLineTraceAction->start();
		if(mDistanceJudgement->isDistanceOut())
		{
			mLineTraceAction->stop();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section1[KP]);
			mRunParameter->setKI(section1[KI]);
			mRunParameter->setKD(section1[KD]);
			mLineTraceAction->updateParameter();
			state=4;
		}

		break;

	case 4://ゴールⅠ
		mLineTraceAction->start();
		if(mDistanceJudgement->isDistanceOut())
		{
			mLineTraceAction->stop();
			//mDistanceJudgement->setDistance(30);
			//mDistanceJudgement->start();
			mRunParameter->setLineTraceSpeed(section0[SPEED]);
			mRunParameter->setKP(section0[KP]);
			mRunParameter->setKI(section0[KI]);
			mRunParameter->setKD(section0[KD]);
			mLineTraceAction->updateParameter();
			state=10;
		}
	break;

	// case 5:
	// mLineTraceAction->start();
	// if(mEV3ColorSensor->isColor_BLUE())
	// {
	// 	mRunParameter->setLineTraceSpeed(section0[SPEED]);
	// 	mRunParameter->setKP(section3[KP]);
	// 	mRunParameter->setKI(section0[KI]);
	// 	mRunParameter->setKD(section3[KD]);
	// 	mLineTraceAction->updateParameter();
	// 	//state=10;
	// 	state = 220;

	// }
	// break;




	case 10: //フリーエリア前の赤、後にsonarsensorに差し替えmEV3ColorSensor->isColor_RED()
		mLineTraceAction->start();
		if(mEV3SonarSensor->isDetected(13))
		{
			mDistanceJudgement->stop();
			mLineTraceAction->stop();
			cycle_set(500);

			
			state=11;
		}
		
		break;


	case 11:
		/*ライントレースを実行する*/
		if (cycle_start()) //50
		{
			//ev3_speaker_set_volume(50);
			//ev3_speaker_play_tone(NOTE_C5, 100);
			mTimerJudgement->stop();
			mRunStraightAction->stop();
			mRunParameter->setRotateAngle(-84);
			mRunParameter->setRotateSpeed(10);
			mRotateMachineAction->updateParameter();

			
			

			state = 20;
		}
		break;

	case 20:
		//
		mRotateMachineAction->start();
		if(mEV3ColorSensor->getColorBrightness()<=20||mRotateMachineAction->isFinished())
		{
			mDistanceJudgement->stop();
			mRunParameter->setLineTraceSpeed(section1[SPEED]);
			mRunParameter->setKP(section2[KP]);
			mRunParameter->setKI(section2[KI]);
			mRunParameter->setKD(section2[KD]);
			mLineTraceAction->updateParameter();
			mDistanceJudgement->setDistance(40);
			mDistanceJudgement->start();
			state=21;

		}
		break;

	//ここからフリーエリア
		
	case 21://強めのライントレース
	mLineTraceAction->start();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=22;
		//state = 500;
	}
	break;

	case 22://青1検知までライントレース
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_BLUE())
	{
		mFaceDisplay->setFace(FaceDisplay::FACE_COLOR2);
		mFaceDisplay->show();
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5, 320);
		// cycle_set(200);
		state=23;
		//state=220;
	}
	break;
	

	case 23://青1円を直進する
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		//mDistanceJudgement->setDistance(20);
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state=24;
		//state = 220;
	}
	break;

	case 24://強めのライントレース
	mLineTraceAction->start();
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
		//state=22;
		//state = 500;
		state = 500;
	}
	break;

	case 500:
//青２検知
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_BLUE())
	{
		mFaceDisplay->setFace(FaceDisplay::FACE_COLOR1);
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5, 320);
		cycle_set(200);
		state=505;
		//state=220;
	}
	break;
	
//青２円直進
	case 505:
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section6[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section6[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		//mDistanceJudgement->setDistance(20);
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state = 510;
		//state=24;
		//state = 220;
	}
	break;
	
	//緑3エリア
	case  510://強めのライントレース
	mLineTraceAction->start();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section6[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section6[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		state=512;
	}
	break;

	case 512://緑3検知までライントレース
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_GREEN())
	{
		mFaceDisplay->setFace(FaceDisplay::FACE_COLOR1);//顔を変える
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5, 320);
		cycle_set(200);
		state=515;
		//state=220;
	}
	break;
	

	case 515://緑3円を直進する
	mRunStraightAction->straight(14,12);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		//mDistanceJudgement->setDistance(20);
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state=520;
	}
	break;

	//緑4ライントレース
	case 520://強めのライントレース
	mLineTraceAction->start();
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
		state=525;
	}
	break;

	case 525://緑4検知までライントレース
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_GREEN())
	{
		mFaceDisplay->setFace(FaceDisplay::FACE_COLOR1);//顔を変える
		
		mLineTraceAction->stop();
		mDistanceJudgement->setDistance(5);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5, 320);
		cycle_set(200);
		state=530;		//state=220;
	}
	break;

	case 530://緑4円を直進する
	mRunStraightAction->straight(15,14);
	mFaceDisplay->show();
	if(mDistanceJudgement->isDistanceOut())
	{
		mDistanceJudgement->stop();
		mRunParameter->setLineTraceSpeed(section1[SPEED]);
		mRunParameter->setKP(section2[KP]);
		mRunParameter->setKI(section2[KI]);
		mRunParameter->setKD(section2[KD]);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(25);
		mDistanceJudgement->start();
		//state=520;
		state = 540;
	}
	break;

	//第一コーナー右折する
	case 540:
	mRunStraightAction->straight(9,-4);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		ev3_speaker_play_tone(NOTE_C4 , 100);
		state=550;
	}
	break;

	//第2コーナーまで進む
	case 550:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_GREEN()){
		state = 560;
	}
	break;

	//第二コーナー右折する
	case 560:
	mRunStraightAction->straight(9,-4);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		state=570;
	}
	break;

	//緑７までライントレース
	case 570:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_GREEN()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5 , 100);
		state = 580;
	}
	break;

	//緑７進入
	case 580:
	mRunStraightAction->straight(15,11);
	if(mDistanceJudgement->isDistanceOut()){
		state = 590;
	}
	break;

	//青６検知までライントレース
	case 590:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_BLUE()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state = 600;
	}
	break;

	//青６個目まで進入
	case 600:
	mRunStraightAction->straight(14,12);
	if(mDistanceJudgement->isDistanceOut()){
		state = 610;
	}
	break;

	//青５検知
	case 610:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_BLUE()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		state = 620;
		//state = 220;
	}
	break;

	//第3コーナー左折
	case 620:
	mRunStraightAction->straight(0,15);
	if(mEV3ColorSensor->getColorBrightness()<=25){
		state=630;
	}
	break;

	//赤9検知までらいんとれーす
	case 630:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_RED()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		state = 640;
	}
	break;

	//第4コーナー左折
	case 640:
	mRunStraightAction->straight(-5,15);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		state=650;
	}
	break;

	//赤10検知までライントレース
	case 650:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_RED()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		state = 660;
	}
	break;

	//赤10進入
	case 660:
	mRunStraightAction->straight(14,12);
	if(mDistanceJudgement->isDistanceOut()){
		state = 670;
		//state = 220;
	}
	break;
	
	//黄11検知するまでライントレース
	case 670:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();

		state = 680;
	}
	break;

	case 680://黄11進入
	mRunStraightAction->straight(15,15);
	if(mDistanceJudgement->isDistanceOut()){
		state = 690;
	}
	break;

	case 690://黄12検知するまでライントレース
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state = 700;
	}
	break;

	case 700://黄12進入
	mRunStraightAction->straight(25,25);
	if(mDistanceJudgement->isDistanceOut()){
		state = 710;
	}
	break;
	
	//第５コーナー右折する
	case 710:
	mRunStraightAction->straight(20,0);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		ev3_speaker_play_tone(NOTE_C4 , 100);
		state=720;
	}
	break;

	//第６コーナーまで進む
	case 720:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){
		state = 730;
	}
	break;

	//第６コーナー右折する
	case 730:
	mRunStraightAction->straight(20,0);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		state=740;
	}
	break;

	//黄１５までライントレース
	case 740:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5 , 100);
		state = 750;
	}
	break;

	//黄１５進入
	case 750:
	mRunStraightAction->straight(15,11);
	if(mDistanceJudgement->isDistanceOut()){
		state = 760;
	}
	break;

	//赤14検知までライントレース
	case 760:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_RED()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state = 770;
	}
	break;

	//赤14進入
	case 770:
	mRunStraightAction->straight(14,12);
	if(mDistanceJudgement->isDistanceOut()){
		state = 780;
	}
	break;

	//赤13検知
	case 780:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_RED()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		state = 790;
		//state = 220;
	}
	break;

	//180度回転して直進
	case 790:
	mDistanceJudgement->setDistance(10);
	mRunStraightAction->straight(-10,15);
	if(mEV3ColorSensor->getColorBrightness()<=20){
		state=800;
	}
	break;

	//赤１４までライントレース
	case 800:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_RED()){
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		ev3_speaker_play_tone(NOTE_C5 , 100);
		state = 810;
	}
	break;

	//黄１５進入
	case 810:
	mRunStraightAction->straight(15,11);
	if(mDistanceJudgement->isDistanceOut()){
		state = 820;
	}
	break;

	//黄１６検知までライントレース
	case 820:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){
		ev3_speaker_play_tone(NOTE_C5, 100);
		mDistanceJudgement->stop();
		mDistanceJudgement->setDistance(10);
		mDistanceJudgement->start();
		state = 830;
	}
	break;

	//黄１６進入
	case 830:
	mRunStraightAction->straight(14,12);
	if(mDistanceJudgement->isDistanceOut()){
		state = 840;
	}
	break;

	//最後のストップ
	case 840:
	mLineTraceAction->start();
	if(mEV3ColorSensor->isColor_YELLOW()){

		ev3_speaker_play_tone(NOTE_C5, 100);
		//state = 790;
		state = 220;
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_C5, 100);
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
			ev3_speaker_set_volume(50);
			ev3_speaker_play_tone(NOTE_DS6, 400);
			mTimerJudgement->stop();
			state = 220;
		}
		break;

	case 220:
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