#include "BlockLineUpBehavior.h"
#include "BluetoothLogger.h"

BlockLineUpBehavior::BlockLineUpBehavior(ev3api::ColorSensor &colorSensor, ev3api::Motor &leftWheel, ev3api::Motor &rightWheel)
    : mColorSensor(colorSensor), mLeftWheel(leftWheel), mRightWheel(rightWheel)
{
}

BlockLineUpBehavior::~BlockLineUpBehavior()
{
}

void BlockLineUpBehavior::execute()
{ //複数回検証用メソッド
    switch (state)
    {
    case 0:
        break;
    default:
        break;
    }
}

//Uturn 2074
bool BlockLineUpBehavior::Straight_X_axis(bool Avoidance)
{ // X軸での直進動作
    switch (Straight_X_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            Straight_X_State = 10; // 回避あり
        }
        else
        {
            Straight_X_State = 100; // 回避なし
        }
        break;

    //回避あり
    case 10: // 回転動作(1回目)
        // 回転動作パラメータを設定する
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(-Straight_Angle_X[0]);
            mRotateMachineAction->updateParameter();
            Straight_X_State++;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(Straight_Angle_X[0]);
            mRotateMachineAction->updateParameter();
            Straight_X_State++;
        }
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            mRotateMachineAction->stop();
            Straight_X_State = 20;
        }
        break;
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        Straight_X_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_X_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            mDistanceJudgement->setDistance(6.3);
            mDistanceJudgement->start();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_X_State++;
        }
        break;
    case 23: // 回転調節用直進動作
        // 直進走行パラメータを設定する
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 黒線を超えた
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            Straight_X_State = 30;
        }
        break;
    case 30: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(Straight_Angle_X[1]);
            mRotateMachineAction->updateParameter();
            Straight_X_State = 51;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(-Straight_Angle_X[1]);
            mRotateMachineAction->updateParameter();
            Straight_X_State = 51;
        }
        break;
    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_G5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            Straight_X_State = 60;
        }
        break;
    case 60: // 直進動作
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        Straight_X_State++;
        break;
    case 61: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_X_State++;
        }
        break;
    case 62: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            // mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_X_State = 70;
        }
        break;
    case 70: // 回転調節用直進動作
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(Straight_Before_Rotation_Distance_X[1]);
            mDistanceJudgement->start();
            Straight_X_State++;
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(Straight_Before_Rotation_Distance_X[0]);
            mDistanceJudgement->start();
            Straight_X_State++;
        }
        break;
    case 71:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_B5, 100);
            Straight_X_State = 80;
        }
        break;
    case 80: // 線復帰動作
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateSpeed(rotateSpeed);
            mRunParameter->setRotateAngle(-350);
            mRotateMachineAction->updateParameter();
            Straight_X_State = 90;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateSpeed(rotateSpeed);
            mRunParameter->setRotateAngle(350);
            mRotateMachineAction->updateParameter();
            Straight_X_State = 95;
        }
        break;
    case 90:
        mRotateMachineAction->start();
        // retentionAngle_Straight_X[1] = mCalcCurrentLocation->getFinalAngle();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness) // || retentionAngle_Straight_X[1] - retentionAngle_Straight_X[0] <= -90)
        {                                                                   // 黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_X_State = 1000;
        }
        break;
    case 95:
        mRotateMachineAction->start();
        // retentionAngle_Straight_X[1] = mCalcCurrentLocation->getFinalAngle();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness) // || retentionAngle_Straight_X[1] - retentionAngle_Straight_X[0] >= 90)
        {                                                                   // 黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_X_State = 1000;
        }
        break;
    case 100:
        if (Block_Set == true)
        {
            Straight_X_State = 150;
        }
        else
        {
            Straight_X_State = 101;
        }
        break;
    case 101:
        //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            Straight_X_State = 104;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            Straight_X_State = 104;
        }
        break;
    case 104:
        //mRunParameter->setRunRightEdgeFlag(false);
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        Straight_X_State = 105;
        break;
    case 105:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 110;
            //Straight_X_State++;
        }
        break;
    case 110: // 直進走行
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(true); // setStraightAngle()に従い直進を行う
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(4.9);
        mDistanceJudgement->start();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作 + 1
            mRunParameter->setStraightAngle(straightAngle - 1);
        }
        else
        { // 左エッジ時の動作 - 1
            mRunParameter->setStraightAngle(straightAngle + 1);
        }
        Straight_X_State++;
        break;
    case 111:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_X_State = 120;
            //Straight_X_State = 1000;
        }
        break;
    case 120: // 直進走行(サークル内の白)
        mRunStraightAction->straight2();
        if (CircleDetection())
        { // サークル内の白を抜けた
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 130;
        }
        break;
    case 130: // 直進走行
        mDistanceJudgement->setDistance(4.3);
        mDistanceJudgement->start();
        Straight_X_State++;
        break;
    case 131:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        { // サークルを抜けた
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            Straight_X_State = 170;
        }
        break;
    case 150:
        //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            Straight_X_State++;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            Straight_X_State++;
        }
        break;
    case 151:
        mRunParameter->setForwardSpeed(20);
        mRunParameter->setTargetAngleFlag(true); // setStraightAngle()に従い直進を行う
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        Straight_X_State++;
        break;
    case 152:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_X_State++;
        }
        break;
    case 153:
        mRunParameter->setRunRightEdgeFlag(true);
        mRunParameter->setDetectTurnDirection(LEFT_TURN);
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State = 156;
        break;
        /*
    case 154:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State++;
        }
        break;
    case 155:
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
        */
    case 156:
        mLineDetectTurnAction->start();
        Straight_X_State++;
        break;
    case 157:
        mTimerJudgement->setTime(600);
        mTimerJudgement->start();
        Straight_X_State++;
        break;
    case 158:
        mLineDetectTurnAction->start();
        if (mTimerJudgement->isTimedOut())
        {
            mLineDetectTurnAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 160;
        }
        else if (mLineDetectTurnAction->isFinished())
        {
            mLineDetectTurnAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 170;
        }
        break;
    case 160:
        mRunParameter->setRunRightEdgeFlag(false);
        mRunParameter->setDetectTurnDirection(RIGHT_TURN);
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State = 163;
        break;
        /*
    case 161:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State++;
        }
        break;
    case 162:
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
        */
    case 163:
        mLineDetectTurnAction->start();
        Straight_X_State++;
        break;
    case 164:
        mTimerJudgement->setTime(1200);
        mTimerJudgement->start();
        Straight_X_State++;
        break;
    case 165:
        mLineDetectTurnAction->start();
        if (mTimerJudgement->isTimedOut())
        {
            mLineDetectTurnAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 170;
        }
        else if (mLineDetectTurnAction->isFinished())
        {
            mLineDetectTurnAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_X_State = 170;
        }
        break;

        /*
    case 155:
        mRunParameter->setDetectTurnMode(DETECT_WHITE);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
    case 156:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            Straight_X_State++;
        }
        break;
    case 157:
        mRunParameter->setDetectTurnDirection(RIGHT_TURN);
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
    case 158:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_E5, 100);
            mLineDetectTurnAction->stop();
            Straight_X_State++;
        }
        break;
    case 159:
        mRunParameter->setDetectTurnMode(DETECT_WHITE);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
    case 160:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            Straight_X_State++;
        }
        break;
    case 161:
        mRunParameter->setDetectTurnDirection(LEFT_TURN);
        mRunParameter->setDetectTurnMode(DETECT_BLACK);
        mLineDetectTurnAction->updateParameter();
        Straight_X_State++;
    case 162:
        mLineDetectTurnAction->start();
        if (mLineDetectTurnAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_E5, 100);
            mLineDetectTurnAction->stop();
            Straight_X_State = 170;
        }
        break;
        */
    case 170: // ライントレース
        //mRunParameter->setRunRightEdgeFlag(false);
        SwitchingMove(false);
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(20);
        mDistanceJudgement->start();
        Straight_X_State++;
        break;
    case 171:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_G5, 100);
            mDistanceJudgement->stop();
            mLineTraceAction->stop();
            Straight_X_State = 1000;
        }
        break;
    case 1000:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::Straight_Y_axis(bool Avoidance)
{ // Y軸での直進動作
    switch (Straight_Y_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            Straight_Y_State = 100; // 回避あり
        }
        else
        {
            Straight_Y_State = 100; // 回避なし
        }
        break;

    //回避あり
    case 10: // 回転動作(1回目)
        // 回転動作パラメータを設定する
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(-Straight_Angle_Y[0]);
            mRotateMachineAction->updateParameter();
            Straight_Y_State++;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(Straight_Angle_Y[0]);
            mRotateMachineAction->updateParameter();
            Straight_Y_State++;
        }
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            mRotateMachineAction->stop();
            Straight_Y_State = 20;
        }
        break;
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        Straight_Y_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_Y_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            mDistanceJudgement->setDistance(6.3);
            mDistanceJudgement->start();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_Y_State++;
        }
        break;
    case 23: // 回転調節用直進動作
        // 直進走行パラメータを設定する
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 黒線を超えた
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            Straight_Y_State = 30;
        }
        break;
    case 30: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(Straight_Angle_Y[1]);
            mRotateMachineAction->updateParameter();
            Straight_Y_State = 51;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(-Straight_Angle_Y[1]);
            mRotateMachineAction->updateParameter();
            Straight_Y_State = 51;
        }
        break;
    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_G5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            Straight_Y_State = 60;
        }
        break;
    case 60: // 直進動作
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        Straight_Y_State++;
        break;
    case 61: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_Y_State++;
        }
        break;
    case 62: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            // mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_Y_State = 70;
        }
        break;
    case 70: // 回転調節用直進動作
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(Straight_Before_Rotation_Distance_Y[1]);
            mDistanceJudgement->start();
            Straight_Y_State++;
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(Straight_Before_Rotation_Distance_Y[0]);
            mDistanceJudgement->start();
            Straight_Y_State++;
        }
        break;
    case 71:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_B5, 100);
            Straight_Y_State = 80;
        }
        break;
    case 80: // 線復帰動作
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateSpeed(rotateSpeed);
            mRunParameter->setRotateAngle(-350);
            mRotateMachineAction->updateParameter();
            Straight_Y_State = 90;
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateSpeed(rotateSpeed);
            mRunParameter->setRotateAngle(350);
            mRotateMachineAction->updateParameter();
            Straight_Y_State = 95;
        }
        break;
    case 90:
        mRotateMachineAction->start();
        // retentionAngle_Straight_Y[1] = mCalcCurrentLocation->getFinalAngle();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness) // || retentionAngle_Straight_Y[1] - retentionAngle_Straight_Y[0] <= -90)
        {                                                                   // 相対角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_Y_State = 1000;
        }
        break;
    case 95:
        mRotateMachineAction->start();
        // retentionAngle_Straight_Y[1] = mCalcCurrentLocation->getFinalAngle();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness) // || retentionAngle_Straight_Y[1] - retentionAngle_Straight_Y[0] >= 90)
        {                                                                   // 相対角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            Straight_Y_State = 1000;
        }
        break;

    case 100: // サークルまでライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        Straight_Y_State = 102;
        break;
    case 102:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 角度の保持のための直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            Straight_Y_State = 103;
        }
        break;
    case 103:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        Straight_Y_State++;
        break;
    case 104:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            // mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_Y_State = 110;
        }
        break;
    case 110: // 直進走行
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(true); // setStraightAngle()に従い直進を行う
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
        }
        else
        {
            mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
        }
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(4);
        mDistanceJudgement->start();
        Straight_Y_State++;
        break;
    case 111:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // サークル内の白
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            Straight_Y_State = 120;
        }
        break;
    case 120: // 直進走行(サークル内の白)
        mRunStraightAction->start();
        if (CircleDetection())
        { // サークル内の白を抜けた
            ev3_speaker_play_tone(NOTE_E5, 100);
            Straight_Y_State = 130;
        }
        break;
    case 130: // 直進走行
        mDistanceJudgement->setDistance(4);
        mDistanceJudgement->start();
        Straight_Y_State++;
        break;
    case 131:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // サークルを抜けた
            // mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            Straight_Y_State = 140;
        }
        break;
    case 140: // ライントレース
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Straight_middle_Distans[1]); // X軸の中間地点までの距離
        mDistanceJudgement->start();
        Straight_Y_State++;
        break;
    case 141:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_G5, 100);
            mDistanceJudgement->stop();
            Straight_Y_State = 1000;
        }
        break;
    case 1000:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::TurnLeft_X_axis(bool Avoidance)
{ // X軸での左折動作
    switch (TurnLeft_X_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            TurnLeft_X_State = 100; // 回避あり
        }
        else
        {
            TurnLeft_X_State = 100; // 回避なし
        }
        break;
    // 回避あり
    case 10: // 回転動作
        mLineTraceAction->stop();
        // 回転動作パラメータを設定する
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRotateAngle(-Turn_Left_Angle_X[0]);
        }
        else
        {
            mRunParameter->setRotateAngle(-Turn_Left_Angle_X[1]);
        }
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnLeft_X_State++;
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnLeft_X_State = 20;
        }
        break;
    // サークル回避 [case 20 ~ 41]
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        TurnLeft_X_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_X_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnLeft_X_State = 40;
        }
        break;
    case 40: // 回転調節用直進動作
        if (mRunParameter->getRunRighEdgeFlag())
        {
            // 右エッジ時の動作
            mDistanceJudgement->setDistance(11.0);
            mDistanceJudgement->start();
            TurnLeft_X_State++;
        }
        else
        {
            // 左エッジ時の動作
            mDistanceJudgement->setDistance(9.0);
            mDistanceJudgement->start();
            TurnLeft_X_State++;
        }
        break;
    case 41:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            TurnLeft_X_State = 50;
        }
        break;
    case 50: // 線復帰動作
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRunParameter->setRotateAngle(-90);
        mRotateMachineAction->updateParameter();
        TurnLeft_X_State++;
        break;
    case 51:
        mRotateMachineAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness || mRotateMachineAction->isFinished())
        { // 指定角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_X_State = 1000;
        }
        break;

    case 100:
        if (Block_Set == true)
        {
            TurnLeft_X_State = 107;
        }
        else
        {
            TurnLeft_X_State = 101;
        }
        break;

    case 101:
        //単体の時は使わない
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            TurnLeft_X_State = 104;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            TurnLeft_X_State = 104;
        }
        break;
    case 104:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        //mRunParameter->setRunRightEdgeFlag(false);
        mLineTraceAction->updateParameter();
        TurnLeft_X_State = 105;
        break;

    case 105:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor();
        //if (color == RED || color == GREEN || color == YELLOW || color == BLUE)
        //mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->isColor() == RED || mEV3ColorSensor->isColor() == GREEN || mEV3ColorSensor->isColor() == YELLOW || mEV3ColorSensor->isColor() == BLUE)
        //if (CircleDetection()) //黄・赤・緑は問題無し
        {
            mLineTraceAction->stop();
            mRunStraightAction->updateParameter();
            mDistanceJudgement->setDistance(6.8);
            mDistanceJudgement->start();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnLeft_X_State++;
        }
        break;

    case 106:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_X_State++;
        }
        break;
    case 107:
        SwitchingMove(false);
        mRunStraightAction->updateParameter();
        black = mRunParameter->getBlack() + 7; //黄・赤・緑(+2.5)
        TurnLeft_X_State++;
        break;
    case 108:
        mRunStraightAction->turnleft();
        TurnLeft_X_State++;
        break;
    case 109:
        mTimerJudgement->setTime(400);
        mTimerJudgement->start();
        TurnLeft_X_State++;
        break;
    case 110:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            TurnLeft_X_State++;
        }
        break;
    case 111:
        if (mEV3ColorSensor->getBrightness() < black)
        {
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnLeft_X_State++;
        }
        break;
    case 112:

        mRunParameter->setRunRightEdgeFlag(true);
        mRunParameter->setLineTraceSpeed(20); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mDistanceJudgement->setDistance(18);  //Turn_middle_Distans[0]
        mDistanceJudgement->start();
        mLineTraceAction->updateParameter();
        TurnLeft_X_State++;
        break;
    case 113:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        //color = mEV3ColorSensor->getBlockFloorColor();
        //if (color == RED || color == GREEN || color == YELLOW || color == BLUE)
        //if (CircleDetection()) //置き場検知Right
        {
            mLineTraceAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_X_State = 1000;
        }
        break;
        /*
    case 109:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor();
        if (color == RED || color == GREEN || color == YELLOW || color == BLUE)
        //if (CircleDetection()) //置き場検知
        //if (mDistanceJudgement->isDistanceOut())
        {
            mLineTraceAction->stop();
            //mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            //Straight_X_State = 110;
            TurnLeft_X_State = 10000;
        }
        break;
        */
        /*
    case 106: // 色判定
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 青を検知した
            TurnLeft_X_State = 110;
        }
        else
        { // 緑または赤または黄を検知した
            TurnLeft_X_State = 111;
        }
        break;
        
    // サークル内通り抜け [case 110 ~ 151]
    case 110: // 直進走行(青)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance_left[0]);
        mDistanceJudgement->start();
        TurnLeft_X_State = 113;
        break;
    case 111: // 直進走行(赤または黄または緑)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance_left[1]);
        mDistanceJudgement->start();
        TurnLeft_X_State = 112;
        break;
    case 112:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnLeft_X_State = 120;
        }
        break;
    case 113:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnLeft_X_State = 122;
        }
        break;
    case 120: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateAngle(-rotation_angle);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        //black = mRunParameter->getBlack();
        TurnLeft_X_State++;
        break;
    case 121:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        //if (mEV3ColorSensor->getBrightness() < black)
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            //straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnLeft_X_State = 130;
        }
        break;
    case 122:
        // 回転動作パラメータを設定する
        mRunParameter->setRotateAngle(-rotation_angle_BLUE);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        //black = mRunParameter->getBlack();
        TurnLeft_X_State++;
        break;
    case 123:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        //if (mEV3ColorSensor->getBrightness() < black)
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            //straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnLeft_X_State = 130;
        }
        break;
    case 130:
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false); // setStraightAngle()に従い直進を行う
        //mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        mDistanceJudgement->start();
        TurnLeft_X_State++;
        break;
    case 131:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_D5, 100);
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            TurnLeft_X_State = 140;
        }
        break;
    case 140: // ライントレース(サークル間の中間地点まで移動する)
        getSwitchingParameter_LOW();
        mRunParameter->setRunRightEdgeFlag(false);
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Turn_middle_Distans[0]); // X軸の中間地点までの距離
        mDistanceJudgement->start();
        TurnLeft_X_State++;
        break;
    case 141:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_A5, 100);
            mDistanceJudgement->stop();
            TurnLeft_X_State = 1000;
        }
        break;
        */
    case 1000:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}
/*
bool BlockLineUpBehavior::TurnLeft_Y_axis(bool Avoidance)
{ // Y軸での左折動作
    switch (TurnLeft_Y_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            TurnLeft_Y_State = 100; // 回避あり
        }
        else
        {
            TurnLeft_Y_State = 100; // 回避なし
        }
        break;

    // 回避あり
    case 10: // 回転動作
        mLineTraceAction->stop();
        // 回転動作パラメータを設定する
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRotateAngle(-Turn_Left_Angle_Y[0]);
        }
        else
        {
            mRunParameter->setRotateAngle(-Turn_Left_Angle_Y[1]);
        }
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnLeft_Y_State++;
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnLeft_Y_State = 20;
        }
        break;
    // サークル回避 [case 20 ~ 41]
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        TurnLeft_Y_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_Y_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnLeft_Y_State = 40;
        }
        break;
    case 40: // 回転調節用直進動作
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(10.4);
            mDistanceJudgement->start();
            TurnLeft_Y_State++;
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(8.5);
            mDistanceJudgement->start();
            TurnLeft_Y_State++;
        }
        break;
    case 41:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            TurnLeft_Y_State = 50;
        }
        break;
    case 50: // 線復帰動作
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRunParameter->setRotateAngle(-90);
        mRotateMachineAction->updateParameter();
        TurnLeft_Y_State++;
        break;
    case 51:
        mRotateMachineAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness || mRotateMachineAction->isFinished())
        { // 指定角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnLeft_Y_State = 1000;
        }
        break;

    case 100:
        // サークルまでライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        TurnLeft_Y_State = 102;
        break;
    case 102:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 角度の保持のための直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            TurnLeft_Y_State = 104;
        }
        break;
    case 104:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        TurnLeft_Y_State++;
        break;
    case 105:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            // mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnLeft_Y_State++;
        }
        break;
    case 106: // 置き場内の白を検知
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            TurnLeft_Y_State = 110;
        }
        else
        { // 赤または黄を検知した
            TurnLeft_Y_State = 111;
        }
        break;
    // サークル内通り抜け [case 110 ~ 151]
    case 110: // 直進走行(緑または青)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setRunRightEdgeFlag(true);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[0]);
        mDistanceJudgement->start();
        TurnLeft_Y_State = 112;
        break;
    case 111: // 直進走行(赤または黄)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        //mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[1]);
        mDistanceJudgement->start();
        TurnLeft_Y_State = 112;
        break;
    case 112:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mDistanceJudgement->stop();
            TurnLeft_Y_State = 120;
        }
        break;
    case 120: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateAngle(-rotation_angle);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnLeft_Y_State++;
        break;
    case 121:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnLeft_Y_State = 130;
        }
        break;
    case 130:
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(1);
        }
        mDistanceJudgement->start();
        TurnLeft_Y_State++;
        break;
    case 131:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mDistanceJudgement->stop();
            TurnLeft_Y_State = 140;
        }
        break;
    case 140: // ライントレース(サークル間の中間地点まで移動する)
        // 右エッジ時固定
        getSwitchingParameter_LOW();
        mRunParameter->setTargetAngleFlag(false);
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Turn_middle_Distans[1]); // X軸の中間地点までの距離
        mDistanceJudgement->start();
        TurnLeft_Y_State++;
        break;
    case 141:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_A5, 100);
            mDistanceJudgement->stop();
            TurnLeft_Y_State = 1000;
        }
        break;

    case 1000:
        mRunParameter->setTargetAngleFlag(false);
        mLineTraceAction->updateParameter();
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}
*/

bool BlockLineUpBehavior::TurnRight_X_axis(bool Avoidance)
{ // X軸での右折動作
    switch (TurnRight_X_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            TurnRight_X_State = 100; // 回避あり
        }
        else
        {
            TurnRight_X_State = 100; // 回避なし
        }
        break;

    // 回避あり
    case 10: // 回転動作
        mLineTraceAction->stop();
        // 回転動作パラメータを設定する
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRotateAngle(Turn_Right_Angle_X[0]);
        }
        else
        {
            mRunParameter->setRotateAngle(Turn_Right_Angle_X[1]);
        }
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnRight_X_State++;
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnRight_X_State = 20;
        }
        break;
    // サークル回避 [case 20 ~ 41]
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        TurnRight_X_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_X_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnRight_X_State = 40;
        }
        break;
    case 40: // 回転調節用直進動作
        if (mRunParameter->getRunRighEdgeFlag())
        {
            // 右エッジ時の動作
            mDistanceJudgement->setDistance(7.0);
            mDistanceJudgement->start();
            TurnRight_X_State++;
        }
        else
        {
            // 左エッジ時の動作
            mDistanceJudgement->setDistance(8.5);
            mDistanceJudgement->start();
            TurnRight_X_State++;
        }
        break;
    case 41:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            TurnRight_X_State = 50;
        }
        break;
    case 50: // 線復帰動作
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRunParameter->setRotateAngle(90);
        mRotateMachineAction->updateParameter();
        TurnRight_X_State++;
        break;
    case 51:
        mRotateMachineAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness || mRotateMachineAction->isFinished())
        { // 指定角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_X_State = 1000;
        }
        break;

    case 100:
        if (Block_Set == true)
        {
            TurnRight_X_State = 107;
        }
        else
        {
            TurnRight_X_State = 101;
        }
        break;
    case 101:
        //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            TurnRight_X_State = 104;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            TurnRight_X_State = 104;
        }
        break;
    case 104:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        TurnRight_X_State = 105;
        break;

    case 105:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor();
        //if (color == RED || color == GREEN || color == YELLOW || color == BLUE)
        //mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->isColor() == RED || mEV3ColorSensor->isColor() == GREEN || mEV3ColorSensor->isColor() == YELLOW || mEV3ColorSensor->isColor() == BLUE)
        //if (CircleDetection()) //黄・赤・緑は問題無し
        {
            mLineTraceAction->stop();
            mRunStraightAction->updateParameter();
            mDistanceJudgement->setDistance(6.8);
            mDistanceJudgement->start();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnRight_X_State++;
        }
        break;

    case 106:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_X_State++;
        }
        break;
    case 107:
        SwitchingMove(false);
        mRunStraightAction->updateParameter();
        black = mRunParameter->getBlack() + 7; //黄・赤・緑(+2.5)
        TurnRight_X_State++;
        break;
    case 108:
        mRunStraightAction->turnRight();
        TurnRight_X_State++;
        break;
    case 109:
        mTimerJudgement->setTime(500);
        mTimerJudgement->start();
        TurnRight_X_State++;
        break;
    case 110:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            TurnRight_X_State++;
        }
        break;
    case 111:
        if (mEV3ColorSensor->getBrightness() < black)
        {
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnRight_X_State++;
        }
        break;
    case 112:

        mRunParameter->setRunRightEdgeFlag(false);
        mRunParameter->setLineTraceSpeed(20); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mDistanceJudgement->setDistance(18);  //Turn_middle_Distans[0]
        mDistanceJudgement->start();
        mLineTraceAction->updateParameter();
        TurnRight_X_State++;
        break;
    case 113:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mLineTraceAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_X_State = 1000;
        }
        break;

        /*
    case 106: // 置き場内の白を検知
        //mLineTraceAction->start();
        mRunStraightAction->turnRight();
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == BLUE) //mEV3ColorSensor->getBlockFloorColor() == GREEN ||
        {                                                  // 緑または青を検知した
            //mLineTraceAction->stop();
            TurnRight_X_State = 110;
        }
        else
        { // 赤または黄を検知した
            //mLineTraceAction->stop();
            TurnRight_X_State = 111;
        }
        break;
    // サークル内通り抜け [case 110 ~ 151]
    case 110: // 直進走行(青)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[0]);
        mDistanceJudgement->start();
        TurnRight_X_State = 112;
        break;
    case 111: // 直進走行(赤または黄または緑)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[1]);
        mDistanceJudgement->start();
        TurnRight_X_State = 113;
        break;
    case 112:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            TurnRight_X_State = 120;
        }
        break;
    case 113:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            TurnRight_X_State = 122;
        }
        break;
    case 120: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateAngle(rotation_angle_BLUE);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        //black = mRunParameter->getBlack();
        TurnRight_X_State++;
        break;
    case 121:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        //if (mEV3ColorSensor->getBrightness() < black)
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            mRunStraightAction->stop();
            //straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnRight_X_State = 130;
        }
        break;
    case 122:
        mRunParameter->setRotateAngle(rotation_angle);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        //black = mRunParameter->getBlack();
        TurnRight_X_State++;
        break;
    case 123:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        //if (mEV3ColorSensor->getBrightness() < black)
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            mRunStraightAction->stop();
            //straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnRight_X_State = 130;
        }
        break;
    case 130:
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        //mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        //mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        mDistanceJudgement->start();
        TurnRight_X_State++;
        break;
    case 131:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_D5, 100);
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            TurnRight_X_State = 140;
        }
        break;
    case 140: // ライントレース(サークル間の中間地点まで移動する)
        // 左エッジ時固定
        mRunParameter->setRunRightEdgeFlag(false);
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Turn_middle_Distans[0]); // X軸の中間地点までの距離
        mDistanceJudgement->start();
        TurnRight_X_State++;
        break;
    case 141:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_A5, 100);
            mDistanceJudgement->stop();
            TurnRight_X_State = 1000;
        }
        break;
        */

    case 1000:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::TurnRight_Y_axis(bool Avoidance)
{ // Y軸での右折動作
    switch (TurnRight_Y_State)
    { // 回避動作を行うかどうか
    case 0:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        if (Avoidance == true)
        {
            TurnRight_Y_State = 10; // 回避あり
        }
        else
        {
            TurnRight_Y_State = 100; // 回避なし
        }
        break;

    // 回避あり
    case 10: // 回転動作
        mLineTraceAction->stop();
        // 回転動作パラメータを設定する
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRotateAngle(Turn_Right_Angle_Y[0]);
        }
        else
        {
            mRunParameter->setRotateAngle(Turn_Right_Angle_Y[1]);
        }
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnRight_Y_State++;
        break;
    case 11:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnRight_Y_State = 20;
        }
        break;
    // サークル回避 [case 20 ~ 41]
    case 20: // 直進走行(直径回避)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_HIGH);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Avoidance_Distance);
        mDistanceJudgement->start();
        TurnRight_Y_State++;
        break;
    case 21: // 誤検知回避
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunParameter->setForwardSpeed(straightSpeed_LOW);
            mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
            mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
            mRunStraightAction->updateParameter();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_Y_State++;
        }
        break;
    case 22: // 黒線までライントレース
        mRunStraightAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
        { // 黒検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            TurnRight_Y_State = 40;
        }
        break;
    case 40: // 回転調節用直進動作
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(9.5);
            mDistanceJudgement->start();
            TurnRight_Y_State++;
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(11.5);
            mDistanceJudgement->start();
            TurnRight_Y_State++;
        }
        break;
    case 41:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_F5, 100);
            TurnRight_Y_State = 50;
        }
        break;
    case 50: // 線復帰動作
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRunParameter->setRotateAngle(90);
        mRotateMachineAction->updateParameter();
        TurnRight_Y_State++;
        break;
    case 51:
        mRotateMachineAction->start();
        if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness || mRotateMachineAction->isFinished())
        { // 指定角度または黒検知した
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_A5, 100);
            TurnRight_Y_State = 1000;
        }
        break;

    case 100: // サークルまでライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        TurnRight_Y_State = 102;
        break;
    case 102:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 角度の保持のための直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            TurnRight_Y_State = 104;
        }
        break;
    case 104:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        TurnRight_Y_State++;
        break;
    case 105:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            // mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            TurnRight_Y_State++;
        }
        break;
    case 106: // 置き場内の白を検知
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            TurnRight_Y_State = 110;
        }
        else
        { // 赤または黄を検知した
            TurnRight_Y_State = 111;
        }
        break;
    // サークル内通り抜け [case 110 ~ 151]
    case 110: // 直進走行(緑または青)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[0]);
        mDistanceJudgement->start();
        TurnRight_Y_State = 112;
        break;
    case 111: // 直進走行(赤または黄)
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Distance[1]);
        mDistanceJudgement->start();
        TurnRight_Y_State = 112;
        break;
    case 112:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mDistanceJudgement->stop();
            TurnRight_Y_State = 120;
        }
        break;
    case 120: // 回転動作
        // 回転動作パラメータを設定する
        mRunParameter->setRotateAngle(rotation_angle);
        mRunParameter->setRotateSpeed(rotateSpeed);
        mRotateMachineAction->updateParameter();
        TurnRight_Y_State++;
        break;
    case 121:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            TurnRight_Y_State = 130;
        }
        break;
    case 130:
        // 直進走行パラメータを設定する
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false);       // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mDistanceJudgement->setDistance(3);
        }
        else
        { // 左エッジ時の動作
            mDistanceJudgement->setDistance(1);
        }
        mDistanceJudgement->start();
        TurnRight_Y_State++;
        break;
    case 131:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 白検知した
            ev3_speaker_play_tone(NOTE_D5, 100);
            mDistanceJudgement->stop();
            TurnRight_Y_State = 140;
        }
        break;
    case 140: // ライントレース(サークル間の中間地点まで移動する)
              // 左エッジ時固定
        mRunParameter->setRunRightEdgeFlag(true);
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Turn_middle_Distans[1]); // X軸の中間地点までの距離
        mDistanceJudgement->start();
        TurnRight_Y_State++;
        break;
    case 141:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_A5, 100);
            mDistanceJudgement->stop();
            TurnRight_Y_State = 1000;
        }
        break;

    case 1000:
        mRunParameter->setRunRightEdgeFlag(true); // 左エッジに固定
        mLineTraceAction->updateParameter();
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockLineUpBehavior::Uturn()
{
    switch (UturnState)
    {
    case 0: // サークル直前までライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        //mRunParameter->setRunRightEdgeFlag(true);
        mLineTraceAction->updateParameter();
        UturnState++;
        break;
    case 1:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnState++;
        }
        break;
    case 2:
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(4);
        mDistanceJudgement->start();
        UturnState++;
        break;
    case 3:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            UturnState = 5;
        }
        break;

    case 5: // 回転動作(角度指定)
        // 回転動作パラメータを設定する
        mLineTraceAction->stop();
        retentionAngle_Uturn[0] = mCalcCurrentLocation->getFinalAngle();
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setUturnApporoachFlag(true);
            mDecelerationRotaryAction->updateParameter();
            black = mRunParameter->getBlack() + 3;
            UturnState = 7;
            //UturnState++;
        }
        break;
    case 6:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] >= 120)
        {
            if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnState = 8;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnState = 8;
        }
        break;
    case 7:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] <= -120)
        {
            // if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            if (mEV3ColorSensor->getBrightness() < black)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnState = 8;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnState = 8;
        }
        break;
    case 8:
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        UturnState++;
        break;
    case 9:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            UturnState++;
        }
        break;

    case 10:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::UturnApporoach_X_axis()
{ // X軸でのカラーブロックブロックの色判定のUターン動作
    switch (UturnApporoach_X_State)
    {

    // サークル内通り抜け [case 1~21]
    case 0: // 置き場内の白を検知
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            UturnApporoach_X_State = 2;
        }
        else
        { // 赤または黄を検知した
            UturnApporoach_X_State = 3;
        }
        break;
    case 2: // 直進走行(緑または青)
        mRunParameter->setForwardSpeed(10);
        mRunParameter->setAngleResetFlag(true); // 角度指定を行わないで直進する
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        UturnApporoach_X_State = 4;
        break;
    case 3: // 直進走行(赤または黄)
        mRunParameter->setForwardSpeed(10);
        mRunParameter->setAngleResetFlag(true); // 角度指定を行わないで直進する
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        UturnApporoach_X_State = 4;
        break;
    case 4:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 目標距離に到達した
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_C5, 100);
            UturnApporoach_X_State = 10;
        }
        break;
    // Uターン動作 [case 10 ~ 20]
    case 10: // 回転動作(角度指定)
        // 回転動作パラメータを設定する
        mLineTraceAction->stop();
        retentionAngle_Uturn[0] = mCalcCurrentLocation->getFinalAngle();
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setUturnApporoachFlag(true);
            mDecelerationRotaryAction->updateParameter();
            UturnApporoach_X_State++;
        }
        else
        {
            mRunParameter->setUturnApporoachFlag(true);
            mDecelerationRotaryAction->updateParameter();
            UturnApporoach_X_State = 20;
        }
        break;
    case 11:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] >= 120)
        {
            if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnApporoach_X_State = 30;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnApporoach_X_State = 30;
        }
        break;
    case 20:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] <= -120)
        {
            if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnApporoach_X_State = 30;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnApporoach_X_State = 30;
        }
        break;
    case 30: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Apporoach_middle_Distans[0]);
        mDistanceJudgement->start();
        UturnApporoach_X_State++;
        break;
    case 31:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            UturnApporoach_X_State = 100;
        }
        break;
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::UturnApporoach_Y_axis()
{ // Y軸でのカラーブロックブロックの色判定のUターン動作
    switch (UturnApporoach_Y_State)
    {

    // サークル内通り抜け [case 1~21]
    case 0: // 置き場内の白を検知
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            UturnApporoach_Y_State = 2;
        }
        else
        { // 赤または黄を検知した
            UturnApporoach_Y_State = 3;
        }
        break;
    case 2: // 直進走行(緑または青)
        mRunParameter->setForwardSpeed(10);
        mRunParameter->setAngleResetFlag(true); // 角度指定を行わないで直進する
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        UturnApporoach_Y_State = 4;
        break;
    case 3: // 直進走行(赤または黄)
        mRunParameter->setForwardSpeed(10);
        mRunParameter->setAngleResetFlag(true); // 角度指定を行わないで直進する
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        UturnApporoach_Y_State = 4;
        break;
    case 4:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // 目標距離に到達した
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_C5, 100);
            UturnApporoach_Y_State = 10;
        }
        break;
    // Uターン動作 [case 10 ~ 20]
    case 10: // 回転動作(角度指定)
        // 回転動作パラメータを設定する
        mLineTraceAction->stop();
        retentionAngle_Uturn[0] = mCalcCurrentLocation->getFinalAngle();
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setUturnApporoachFlag(true);
            mDecelerationRotaryAction->updateParameter();
            UturnApporoach_Y_State++;
        }
        else
        {
            mRunParameter->setUturnApporoachFlag(true);
            mDecelerationRotaryAction->updateParameter();
            UturnApporoach_Y_State = 20;
        }
        break;
    case 11:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] >= 120)
        {
            if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnApporoach_Y_State = 30;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnApporoach_Y_State = 30;
        }
        break;
    case 20:
        mDecelerationRotaryAction->start();
        retentionAngle_Uturn[1] = mCalcCurrentLocation->getFinalAngle();
        if (retentionAngle_Uturn[1] - retentionAngle_Uturn[0] <= -120)
        {
            if (mEV3ColorSensor->getCorrectionBrightness() < target_Brightness)
            {
                ev3_speaker_play_tone(NOTE_A5, 100);
                UturnApporoach_Y_State = 30;
            }
        }
        if (mDecelerationRotaryAction->isFinished())
        { // 指定した角度まで回転した
            ev3_speaker_play_tone(NOTE_E5, 100);
            UturnApporoach_Y_State = 30;
        }
        break;
    case 30: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Apporoach_middle_Distans[1]);
        mDistanceJudgement->start();
        UturnApporoach_Y_State++;
        break;
    case 31:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            UturnApporoach_Y_State = 100;
        }
        break;
    case 100:
        // 状態変数のリセット
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_X_left(bool start)
{ // X軸でのブロックをサークル内に置く動作
    switch (BlockPut_X_State)
    {

    case 0: //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            BlockPut_X_State = 20;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            BlockPut_X_State = 20;
        }
        break;
    case 20:
        // 前進
        //mRunParameter->setRunRightEdgeFlag(true);
        mRunParameter->setLineTraceSpeed(18); //PID_Circle[0]
        mRunParameter->setKP(0.2);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(4.0);            //PID_Circle[3]
        mLineTraceAction->updateParameter();
        BlockPut_X_State = 21;
        break;
    case 21:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 30;
        }
        break;
    case 30:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 31:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 50;
        }
        break;

    case 50:
        mRunParameter->setRotateAngle(-90); //120度回転
        mRunParameter->setRotateSpeed(12);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;
    case 52:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(-30);
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 53:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 54:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 55:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 60;
        }
        break;

    case 60: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 61: // アームを上げてブロックを外す
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            mTimerJudgement->stop();
            mRunParameter->setArmAngle(0);
            mRunParameter->setArmPower(10);
            mArmPositionSetAction->updateParameter();
            BlockPut_X_State++;
        }
        break;
    case 62: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_X_State++;
        }
        break;

    case 63:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 64:
        mRunStraightAction->back2();
        BlockPut_X_State++;
        break;
    case 65:
        mTimerJudgement->setTime(1000);
        mTimerJudgement->start();
        BlockPut_X_State++;
        break;
    case 66:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_X_State++;
        }
        break;
    case 67:
        if (CircleDetection())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State = 70;
        }
        break;
    case 70:
        mRunStraightAction->back2();
        mRunParameter->setForwardSpeed(15);
        //mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 71:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State++;
        }
        break;

    case 72:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(30); //120度回転
        mRunParameter->setRotateSpeed(10); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 73:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 74:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(90); //120度回転
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 75:
        SwitchingMove(true);
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State = 100;
        }
        break;
        /*
    case 74:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 75:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State++;
        }
        break;
    case 76:
        // 前進
        mRunParameter->setRunRightEdgeFlag(false);
        mRunParameter->setLineTraceSpeed(18); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 77:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 100;
        }
        break;
        */
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_X_Right(bool start)
{ // X軸でのブロックをサークル内に置く動作
    switch (BlockPut_X_State)
    {
    case 0: //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(false);
            BlockPut_X_State = 20;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(true);
            BlockPut_X_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 30;
        }
        break;
    case 30:
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 31:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 50;
        }
        break;

    case 50:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(90); //90度回転
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State = 54;
        }
        break;

    case 54:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(30); //50度回転
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 55:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 56:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 57:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 60;
        }
        break;

    case 60: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 61: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_X_State++;
            }
        }
        break;
    case 62: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_X_State++;
        }
        break;

    case 63:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State = 70;
        break;

    case 70:
        mRunStraightAction->back2();
        BlockPut_X_State++;
        break;
    case 71:
        mTimerJudgement->setTime(1000);
        mTimerJudgement->start();
        BlockPut_X_State++;
        break;
    case 72:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_X_State++;
        }
        break;
    case 73:
        if (CircleDetection())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State++;
        }
        break;
    case 74:
        mRunStraightAction->start();
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 75:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_C6, 100);
            BlockPut_X_State = 80;
        }
        break;
    case 80:
        mCalcCurrentLocation->setAngle(0);  // 角度をリセット
        mRunParameter->setRotateAngle(-30); //120度回転
        mRunParameter->setRotateSpeed(12);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 81:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;
    case 82:
        mCalcCurrentLocation->setAngle(0);  // 角度をリセット
        mRunParameter->setRotateAngle(-90); //120度回転
        mRunParameter->setRotateSpeed(12);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 83:
        SwitchingMove(true);
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State = 100;
        }
        break;
        /*
    case 77:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 78:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State++;
        }
        break;
    case 79:
        // 前進
        mRunParameter->setRunRightEdgeFlag(true);
        mRunParameter->setLineTraceSpeed(18); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 80:
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 100;
        }
        break;
        */
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_X_Diagonal(bool start)
{ // X軸でのブロックをサークル内に置く動作
    switch (BlockPut_X_State)
    {
    case 0: //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            BlockPut_X_State = 10;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            BlockPut_X_State = 10;
        }
        break;
    case 10: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
        BlockPut_X_State++;
        break;
    case 11:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockPut_X_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 30;
        }
        break;
    case 30:
        mRunParameter->setForwardSpeed(20);
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 31:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 50;
        }
        break;

    case 50:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(45);
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 52:
        mRunParameter->setForwardSpeed(20);
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 53:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State++;
        }
        break;

    case 54: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 55: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_X_State++;
            }
        }
        break;
    case 56: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_X_State = 60;
        }
        break;

    case 60:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 61:
        mRunStraightAction->back2();
        BlockPut_X_State++;
        break;
    case 62:
        mTimerJudgement->setTime(1000);
        mTimerJudgement->start();
        BlockPut_X_State++;
        break;
    case 63:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_X_State++;
        }
        break;
    case 64:
        if (CircleDetection())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State = 70;
        }
        break;
    case 70:
        mRunStraightAction->start();
        ev3_speaker_play_tone(NOTE_C6, 100);
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(false);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 71:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State++;
        }
        break;
    case 72:
        mCalcCurrentLocation->setAngle(0);  // 角度をリセット
        mRunParameter->setRotateAngle(-40); //回転角度
        mRunParameter->setRotateSpeed(12);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 73:
        SwitchingMove(true);
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State = 100;
        }
        break;
        /*
    case 72:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-20);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 73:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State++;
        }
        break;

    case 74:
        // 前進
        mRunParameter->setRunRightEdgeFlag(true);
        mRunParameter->setLineTraceSpeed(18); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(8);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;
    case 75:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 100;
        }
        break;
*/
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_Y_Diagonal(bool start)
{ // X軸でのブロックをサークル内に置く動作
    switch (BlockPut_Y_State)
    {
    case 0: //エッジの維持
        if (mRunParameter->getRunRighEdgeFlag())
        {
            mRunParameter->setRunRightEdgeFlag(true);
            BlockPut_Y_State++;
        }
        else
        {
            mRunParameter->setRunRightEdgeFlag(false);
            BlockPut_Y_State++;
        }

        break;
    case 1: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
        BlockPut_Y_State = 11;
        break;
    case 11:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockPut_Y_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_Y_State = 30;
        }
        break;
    case 30:
        mRunParameter->setForwardSpeed(20);
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 31:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 50;
        }
        break;
    case 50:
        //45度旋回
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(-45);
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;

    case 52:
        mRunParameter->setForwardSpeed(20);
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 53:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State++;
        }
        break;

    case 54: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 55: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_Y_State++;
            }
        }
        break;
    case 56: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_Y_State++;
        }
        break;

    case 57:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-20);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 58:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            if (CircleDetection()) //置き場検知
            {
                mRunStraightAction->stop();
                ev3_speaker_play_tone(NOTE_E5, 100);
            }
            BlockPut_Y_State++;
        }
        break;

    case 59:
        mRunStraightAction->start();
        ev3_speaker_play_tone(NOTE_C6, 100);
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(false);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(5);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;
    case 60:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 70;
        }
        break;

    case 70:
        mCalcCurrentLocation->setAngle(0); // 角度をリセット
        mRunParameter->setRotateAngle(45);
        mRunParameter->setRotateSpeed(12); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 71:
        SwitchingMove(true);
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State = 100;
        }
        break;
        /*
    case 72:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-12);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 73:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_Y_State++;
        }
        break;
    case 74:
        // 前進
        mRunParameter->setRunRightEdgeFlag(false);
        mRunParameter->setLineTraceSpeed(18); //PID_Circle[0]
        mRunParameter->setKP(0.3);            //PID_Circle[1]
        mRunParameter->setKI(0);              //PID_Circle[2]
        mRunParameter->setKD(6.0);            //PID_Circle[3]
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(8);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;
    case 75:
        //ライントレース中置き場検知
        mLineTraceAction->start();
        if (mDistanceJudgement->isDistanceOut()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_Y_State = 100;
        }
        break;
*/
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::GoGarage(bool directionRobot)
{
    switch (GoGarage_State)
    {
    case 0: // 回転動作
        if (directionRobot == true)
        { // 緑サークルから入っていた
            mRunParameter->setRotateSpeed(rotateSpeed);
            mRunParameter->setRotateAngle(95);
            mRotateMachineAction->updateParameter();
            GoGarage_State++;
        }
        else
        { // 黄サークルから入っていた
            mRunParameter->setRotateAngle(-85);
            mRotateMachineAction->updateParameter();
            GoGarage_State++;
        }
        break;
    case 1:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            ev3_speaker_play_tone(NOTE_E5, 100);
            GoGarage_State++;
        }
        break;
    case 2: // 直進動作
        mRunParameter->setForwardSpeed(35);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(20);
        mDistanceJudgement->start();
        GoGarage_State++;
        break;
    case 3:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            GoGarage_State++;
        }
        break;
    case 4:
        mRunParameter->setForwardSpeed(25);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(15);
        mDistanceJudgement->start();
        GoGarage_State++;
        break;
    case 5:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            // mRunStraightAction->stop();
            mDistanceJudgement->stop();
            GoGarage_State++;
        }
        break;
    case 6:
        GoGarage_State = 0;
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockSortArea_Intrusion(bool ArmUP)
{
    switch (BlockSortArea_Intrusion_State)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        BlockSortArea_Intrusion_State++;
        break;
    case 1:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 角度の保持のための直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockSortArea_Intrusion_State = 10;
        }
        break;
    case 10:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockSortArea_Intrusion_State++;
        break;
    case 11:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();
        if (CircleDetection())
        { //置き場検知
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            if (ArmUP)
            { // ブロックの色を調べる為にアームを上げるかどうか
                BlockSortArea_Intrusion_State = 20;
            }
            else
            {
                BlockSortArea_Intrusion_State = 100;
            }
        }
        break;
    case 20: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockSortArea_Intrusion_State++;
        break;
    case 21:
        if (mTimerJudgement->isTimedOut())
        {
            BlockSortArea_Intrusion_State = 30;
        }
        break;
    case 30: // 後進
        mRunParameter->setForwardSpeed(-10);
        mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(-3);
        mDistanceJudgement->start();
        BlockSortArea_Intrusion_State++;
        break;
    case 31:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockSortArea_Intrusion_State = 40;
        }
        break;
    case 40: // アーム角度設定
        mRunParameter->setArmAngle(62);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockSortArea_Intrusion_State++;
        break;
    case 41: // アーム上げ
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockSortArea_Intrusion_State = 50;
        }
        break;
    case 50:
        mTimerJudgement->setTime(100); //100ms色を読み続ける
        mTimerJudgement->start();
        BlockSortArea_Intrusion_State++;
        break;
    case 51: //100ms色を読み続ける
        mEV3ColorSensor->getHsvBlockColor();
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockSortArea_Intrusion_State++;
        }
        break;
    case 52:
        mTimerJudgement->setTime(500);
        mTimerJudgement->start();
        BlockSortArea_Intrusion_State = 60;
        break;
    case 60: //ブロックの色検知orタイマー判定
        mEV3ColorSensor->getHsvBlockColor();
        if (mTimerJudgement->isTimedOut())
        { //タイムアウト
            ev3_speaker_play_tone(NOTE_DS6, 400);
            Block_Color_Result = 4;
            mTimerJudgement->stop();
            if (TimeOut_flag == true)
            {
                BlockSortArea_Intrusion_State = 80;
            }
            else
            {
                mTimerJudgement->stop();
                TimeOut_flag = true;
                BlockSortArea_Intrusion_State = 70;
            }
        }
        if (mEV3ColorSensor->getHsvBlockColor() == RED || mEV3ColorSensor->getHsvBlockColor() == GREEN || mEV3ColorSensor->getHsvBlockColor() == YELLOW || mEV3ColorSensor->getHsvBlockColor() == BLUE)
        {
            if (mEV3ColorSensor->getHsvBlockColor() == RED)
            {
                REDCount++; // 赤ブロックを読んだ
                GREENCount = 0;
                YELLOWCount = 0;
                BLUECount = 0;
                if (REDCount > ColorBlockDetectedCount)
                { // 赤ブロックを検知した
                    Block_Color_Result = 0;
                    ev3_speaker_play_tone(NOTE_A6, 200);
                    BlockSortArea_Intrusion_State = 80;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == GREEN)
            {
                REDCount = 0;
                GREENCount++; // 緑ブロックを読んだ
                YELLOWCount = 0;
                BLUECount = 0;
                if (GREENCount > ColorBlockDetectedCount)
                { // 緑ブロックを検知した
                    Block_Color_Result = 1;
                    ev3_speaker_play_tone(NOTE_B6, 200);
                    BlockSortArea_Intrusion_State = 80;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == YELLOW)
            {
                REDCount = 0;
                GREENCount = 0;
                YELLOWCount++; // 黄ブロックを読んだ
                BLUECount = 0;
                if (YELLOWCount > ColorBlockDetectedCount)
                { // 黄ブロックを検知した
                    Block_Color_Result = 3;
                    ev3_speaker_play_tone(NOTE_D6, 200);
                    BlockSortArea_Intrusion_State = 80;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == BLUE)
            {
                REDCount = 0;
                GREENCount = 0;
                YELLOWCount = 0;
                BLUECount++; // 青ブロックを読んだ
                if (BLUECount > ColorBlockDetectedCount)
                { // 青ブロックを検知した
                    Block_Color_Result = 2;
                    ev3_speaker_play_tone(NOTE_F6, 200);
                    BlockSortArea_Intrusion_State = 80;
                }
            }
        }
        break;
    case 70: //後退
        mRunParameter->setForwardSpeed(-10);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(-1.5);
        mDistanceJudgement->start();
        BlockSortArea_Intrusion_State++;
        break;
    case 71:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mRunStraightAction->stop();
            BlockSortArea_Intrusion_State = 50;
        }
        break;
    case 80: //アーム角度設定
        mTimerJudgement->stop();
        mRunParameter->setArmAngle(0);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockSortArea_Intrusion_State++;
        break;
    case 81: //アーム下げ
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C6, 200);
            mArmPositionSetAction->stop();
            mArmPositionSetAction->reset();
            BlockSortArea_Intrusion_State = 90;
        }
        break;
    case 90: //ライントレース中置き場検知
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockSortArea_Intrusion_State++;
        break;
    case 91:
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockSortArea_Intrusion_State = 100;
        }
        break;
    case 100:
        BlockSortArea_Intrusion_State = 0; // 状態変数の初期化
        // caseの開始位置を変更
        Straight_X_State = 110;
        Straight_Y_State = 110;
        TurnLeft_X_State = 110;
        TurnLeft_Y_State = 110;
        TurnRight_X_State = 110;
        TurnRight_Y_State = 110;
        BlockPut_X_State = 30;
        BlockPut_Y_State = 30;
        //カラーブロックの連続検知回数を初期化
        REDCount = 0;
        GREENCount = 0;
        YELLOWCount = 0;
        BLUECount = 0;
        TimeOut_flag = false;                                  // フラグの初期化
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::CircleDetection()
{
    mEV3ColorSensor->getColorBrightness(); // RGBラントレースに切り替えて走行する為のメソッド呼び出し
    if (mEV3ColorSensor->isColor())
    {
        return true;
    }
    return false;
}

int BlockLineUpBehavior::ColorBlockReturn()
{ // 何色のカラーブロックを取得したか
    return Block_Color_Result;
}

void BlockLineUpBehavior::getSwitchingParameter_LOW()
{
    if (Block_Hold == true)
    {
        mRunParameter->setLineTraceSpeed(PID_Bolck_LOW[0]);
        mRunParameter->setKP(PID_Bolck_LOW[1]);
        mRunParameter->setKI(PID_Bolck_LOW[2]);
        mRunParameter->setKD(PID_Bolck_LOW[3]);
    }
    else
    {
        mRunParameter->setLineTraceSpeed(PID_LOW[0]);
        mRunParameter->setKP(PID_LOW[1]);
        mRunParameter->setKI(PID_LOW[2]);
        mRunParameter->setKD(PID_LOW[3]);
    }
}

void BlockLineUpBehavior::SetSwitchingParameter(bool PID)
{
    if (PID == true)
    {
        Block_Hold = true;
    }
    else
    {
        Block_Hold = false;
    }
}
void BlockLineUpBehavior::SwitchingMove(bool move)
{
    if (move == true)
    {
        Block_Set = true;
    }
    else
    {
        Block_Set = false;
    }
}

bool BlockLineUpBehavior::isFinished()
{ // ブロック並べ動作確認用終了メソッド
    return flag;
}

bool BlockLineUpBehavior::Apporoach()
{ // カラーブロック判定
    //BluetoothLogger::dataLogger((int8_t)BlockDetectionState, 0);
    switch (BlockDetectionState)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        //mDistanceJudgement->setDistance(1);
        //mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        BlockDetectionState = 2;
        break;
    case 2:
        mLineTraceAction->start();
        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockDetectionState++;
        }
        break;
    case 3: //後退
        mRunParameter->setForwardSpeed(-12);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(-1);
        mDistanceJudgement->start();
        BlockDetectionState++;
        break;

    case 4:
        mTimerJudgement->setTime(350);
        mTimerJudgement->start();
        mLineTraceAction->stop();
        BlockDetectionState++;
        break;

    case 5:
        if (mTimerJudgement->isTimedOut())
        { //タイムアウト
            ev3_speaker_play_tone(NOTE_DS6, 400);
            mTimerJudgement->stop();
            BlockDetectionState++;
        }
        break;

    case 6:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mRunStraightAction->stop();
            BlockDetectionState = 60;
        }
        break;

        // case 10:
        //     mRunParameter->setRotateSpeed(rotateSpeed);
        //     if (mRunParameter->getRunRighEdgeFlag())
        //     { // 右エッジ時の動作
        //         mRunParameter->setRotateAngle(-1);
        //     }
        //     else
        //     { // 左エッジ時の動作
        //         mRunParameter->setRotateAngle(1);
        //     }
        //     mRotateMachineAction->updateParameter();
        //     BlockDetectionState++;
        //     break;
        // case 11:
        //     mRotateMachineAction->start();
        //     if (mRotateMachineAction->isFinished())
        //     {
        //         ev3_speaker_play_tone(NOTE_C5, 100);
        //         mRotateMachineAction->stop();
        //         BlockDetectionState = 20;
        //     }
        //     break;
        // case 20:
        //     mRunParameter->setForwardSpeed(straightSpeed_LOW);
        //     mRunParameter->setAngleResetFlag(true);
        //     mRunStraightAction->updateParameter();
        //     BlockDetectionState++;
        //     break;
        // case 21:
        //     mRunStraightAction->start();
        //     if (CircleDetection())
        //     { // サークル内の白を抜けた
        //         mRunStraightAction->stop();
        //         ev3_speaker_play_tone(NOTE_E5, 100);
        //         BlockDetectionState = 30;
        //     }
        //     break;
        // case 30:
        //     mRunParameter->setRotateSpeed(rotateSpeed);
        //     if (mRunParameter->getRunRighEdgeFlag())
        //     { // 右エッジ時の動作
        //         mRunParameter->setRotateAngle(1);
        //     }
        //     else
        //     { // 左エッジ時の動作
        //         mRunParameter->setRotateAngle(-1);
        //     }
        //     mRotateMachineAction->updateParameter();
        //     BlockDetectionState++;
        //     break;
        // case 31:
        //     mRotateMachineAction->start();
        //     if (mRotateMachineAction->isFinished())
        //     {
        //         ev3_speaker_play_tone(NOTE_C5, 100);
        //         mRotateMachineAction->stop();
        //         BlockDetectionState = 40;
        //     }
        //     break;
        // case 40: // 300ms停止
        //     mTimerJudgement->setTime(300);
        //     mTimerJudgement->start();
        //     BlockDetectionState++;
        //     break;
        // case 41:
        //     if (mTimerJudgement->isTimedOut())
        //     {
        //         BlockDetectionState++;
        //     }
        //     break;

        // case 42:
        //     mRunParameter->setForwardSpeed(15);
        //     mRunStraightAction->updateParameter();
        //     mDistanceJudgement->setDistance(25);
        //     mDistanceJudgement->start();
        //     BlockDetectionState++;
        //     break;
        // case 43:
        //     mRunStraightAction->start();
        //     if (mDistanceJudgement->isDistanceOut())
        //     {
        //         mRunStraightAction->stop();
        //         mDistanceJudgement->stop();
        //         BlockDetectionState++;
        //     }
        //     break;

        // case 44:
        //     //90度旋回
        //     mRunParameter->setRotateAngle(-90); //60°回転(左回転)
        //     mRunParameter->setRotateSpeed(15);  //回転時のスピード
        //     //回転走行パラメータを更新
        //     mRotateMachineAction->updateParameter();
        //     mArmPositionSetAction->updateParameter();
        //     BlockDetectionState++;
        //     break;
        // case 45:
        //     mRotateMachineAction->start();
        //     if (mRotateMachineAction->isFinished())
        //     {
        //         //旋回停止
        //         mRotateMachineAction->stop();
        //         ev3_speaker_play_tone(NOTE_C6, 200);
        //         BlockDetectionState++;
        //     }
        //     break;
        // case 46:
        //     mRunParameter->setForwardSpeed(15);
        //     mRunStraightAction->updateParameter();
        //     mDistanceJudgement->setDistance(3);
        //     mDistanceJudgement->start();
        //     BlockDetectionState++;
        //     break;
        // case 47:
        //     mRunStraightAction->start();
        //     if (mDistanceJudgement->isDistanceOut())
        //     {
        //         mRunStraightAction->stop();
        //         mDistanceJudgement->stop();
        //         BlockDetectionState = 60;
        //     }
        //     break;

        // case 50:
        //     // 後進
        //     mRunParameter->setForwardSpeed(-10);
        //     mRunParameter->setTargetAngleFlag(true);        // setStraightAngle()に従い直進を行う
        //     mRunParameter->setStraightAngle(straightAngle); //直進時の目標角度指定
        //     mRunStraightAction->updateParameter();
        //     mDistanceJudgement->setDistance(-3);
        //     mDistanceJudgement->start();
        //     BlockDetectionState++;
        //     break;
        // case 51:
        //     mRunStraightAction->start();
        //     if (mDistanceJudgement->isDistanceOutBack())
        //     {
        //         mDistanceJudgement->stop();
        //         mRunStraightAction->stop();
        //         BlockDetectionState = 60;
        //     }
        //     break;

    case 60:                            // アーム角度設定
        mRunParameter->setArmAngle(50); //62
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockDetectionState++;
        break;
    case 61: // アーム上げ
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            mTimerJudgement->setTime(100); //100ms色を読み続ける
            mTimerJudgement->start();
            BlockDetectionState = 70;
        }
        break;

    case 70: //100ms色を読み続ける
        mEV3ColorSensor->getHsvBlockColor();
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockDetectionState++;
        }
        break;
    case 71:
        mTimerJudgement->setTime(500);
        mTimerJudgement->start();
        BlockDetectionState = 80;
        break;
    case 80: //ブロックの色検知orタイマー判定
        mEV3ColorSensor->getHsvBlockColor();
        if (mTimerJudgement->isTimedOut())
        { //タイムアウト
            ev3_speaker_play_tone(NOTE_DS6, 400);
            Block_Color_Result = 4;
            mTimerJudgement->stop();
            if (TimeOut_flag == true)
            {
                BlockDetectionState = 100;
            }
            else
            {
                mTimerJudgement->stop();
                TimeOut_flag = true;
                BlockDetectionState = 90;
            }
        }
        if (mEV3ColorSensor->getHsvBlockColor() == RED || mEV3ColorSensor->getHsvBlockColor() == GREEN || mEV3ColorSensor->getHsvBlockColor() == YELLOW || mEV3ColorSensor->getHsvBlockColor() == BLUE)
        {
            if (mEV3ColorSensor->getHsvBlockColor() == RED)
            {
                REDCount++; // 赤ブロックを読んだ
                GREENCount = 0;
                YELLOWCount = 0;
                BLUECount = 0;
                if (REDCount > ColorBlockDetectedCount)
                { // 赤ブロックを検知した
                    Block_Color_Result = 1;
                    ev3_speaker_play_tone(NOTE_A6, 200);
                    BlockDetectionState = 100;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == GREEN)
            {
                REDCount = 0;
                GREENCount++; // 緑ブロックを読んだ
                YELLOWCount = 0;
                BLUECount = 0;
                if (GREENCount > ColorBlockDetectedCount)
                { // 緑ブロックを検知した
                    Block_Color_Result = 2;
                    ev3_speaker_play_tone(NOTE_B6, 200);
                    BlockDetectionState = 100;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == YELLOW)
            {
                REDCount = 0;
                GREENCount = 0;
                YELLOWCount++; // 黄ブロックを読んだ
                BLUECount = 0;
                if (YELLOWCount > ColorBlockDetectedCount)
                { // 黄ブロックを検知した
                    Block_Color_Result = 3;
                    ev3_speaker_play_tone(NOTE_D6, 200);
                    BlockDetectionState = 100;
                }
            }
            else if (mEV3ColorSensor->getHsvBlockColor() == BLUE)
            {
                REDCount = 0;
                GREENCount = 0;
                YELLOWCount = 0;
                BLUECount++; // 青ブロックを読んだ
                if (BLUECount > ColorBlockDetectedCount)
                { // 青ブロックを検知した
                    Block_Color_Result = 0;
                    ev3_speaker_play_tone(NOTE_F6, 200);
                    BlockDetectionState = 100;
                }
            }
        }
        break;
    case 90: //後退
        mRunParameter->setForwardSpeed(-10);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(-5);
        mDistanceJudgement->start();
        BlockDetectionState = 100;
        break;
    case 91:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mRunStraightAction->stop();
            BlockDetectionState = 100;
        }
        break;
    case 100: //アーム角度設定
        mTimerJudgement->stop();
        mRunParameter->setArmAngle(0);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockDetectionState++;
        break;
    case 101: //アーム下げ
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C6, 200);
            mArmPositionSetAction->stop();
            //mArmPositionSetAction->reset();
            BlockDetectionState = 150;
        }
        break;
    case 110:
        mRunParameter->setForwardSpeed(-10);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(-3);
        mDistanceJudgement->start();
        BlockDetectionState++;
        break;
    case 111:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockDetectionState = 150;
        }
        break;

    // case 102:
    //     //90度旋回
    //     mRunParameter->setRotateAngle(-90); //60°回転(左回転)
    //     mRunParameter->setRotateSpeed(15);  //回転時のスピード
    //     //回転走行パラメータを更新
    //     mRotateMachineAction->updateParameter();
    //     mArmPositionSetAction->updateParameter();
    //     BlockDetectionState++;
    //     break;

    // case 103:
    //     mRotateMachineAction->start();
    //     if (mRotateMachineAction->isFinished())
    //     {
    //         //旋回停止
    //         mRotateMachineAction->stop();
    //         ev3_speaker_play_tone(NOTE_C6, 200);
    //         BlockDetectionState = 105;
    //     }
    //     break;

    // case 105:
    //     // ライントレース走行パラメータを設定する
    //     getSwitchingParameter_LOW();
    //     mLineTraceAction->updateParameter();
    //     mDistanceJudgement->setDistance(Circle_Front_Distance);
    //     mDistanceJudgement->start();
    //     straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
    //     BlockDetectionState++;
    //     break;
    // case 106:
    //     mLineTraceAction->start();
    //     if (mStraightDetection->isDetect())
    //     {                                                          // 角度の保持のための直進検知
    //         straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
    //         ev3_speaker_set_volume(100);
    //         ev3_speaker_play_tone(NOTE_A5, 100);
    //         ev3_speaker_set_volume(50);
    //     }
    //     if (mDistanceJudgement->isDistanceOut())
    //     {
    //         ev3_speaker_play_tone(NOTE_C5, 100);
    //         mDistanceJudgement->stop();
    //         BlockDetectionState = 1000;
    //     }
    //     break;

    // case 110:
    //     mRunParameter->setRotateSpeed(rotateSpeed);
    //     if (mRunParameter->getRunRighEdgeFlag())
    //     { // 右エッジ時の動作
    //         mRunParameter->setRotateAngle(2.5);
    //     }
    //     else
    //     { // 左エッジ時の動作
    //         mRunParameter->setRotateAngle(-2.5);
    //     }
    //     mRotateMachineAction->updateParameter();
    //     BlockDetectionState++;
    //     break;
    // case 111:
    //     mRotateMachineAction->start();
    //     if (mRotateMachineAction->isFinished())
    //     {
    //         ev3_speaker_play_tone(NOTE_C5, 100);
    //         mRotateMachineAction->stop();
    //         BlockDetectionState = 120;
    //     }
    //     break;
    // case 120: //ライントレース中置き場検知
    //     mRunParameter->setLineTraceSpeed(PID_Circle[0]);
    //     mRunParameter->setKP(PID_Circle[1]);
    //     mRunParameter->setKI(PID_Circle[2]);
    //     mRunParameter->setKD(PID_Circle[3]);
    //     mLineTraceAction->updateParameter();
    //     BlockDetectionState++;
    //     break;
    // case 121:
    //     mLineTraceAction->start();
    //     if (CircleDetection()) //置き場検知
    //     {
    //         // mLineTraceAction->stop();
    //         ev3_speaker_play_tone(NOTE_E5, 100);
    //         BlockDetectionState = 150;
    //     }
    //     break;
    case 150:
        BlockDetectionState = 0; // 状態変数の初期化
        // caseの開始位置を変更
        Straight_X_State = 104;
        Straight_Y_State = 103;
        TurnLeft_X_State = 104;
        TurnLeft_Y_State = 0;
        TurnRight_X_State = 104;
        TurnRight_Y_State = 0;
        BlockPut_X_State = 20;
        BlockPut_Y_State = 0;
        //カラーブロックの連続検知回数を初期化
        REDCount = 0;
        GREENCount = 0;
        YELLOWCount = 0;
        BLUECount = 0;
        TimeOut_flag = false;                                  // フラグの初期化
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::Apporoach_Not_Arm()
{
    switch (BlockDetectionState)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        BlockDetectionState = 2;
        break;
    case 2:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 角度の保持のための直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockDetectionState++;
        }
        break;
    case 3:
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(-1);
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(1);
        }
        mRotateMachineAction->updateParameter();
        BlockDetectionState++;
        break;
    case 4:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            BlockDetectionState++;
        }
        break;
    case 5:
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        BlockDetectionState++;
        break;
    case 6:
        mRunStraightAction->start();
        if (CircleDetection())
        { // サークル内の白を抜けた
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockDetectionState++;
        }
        break;
    case 7:
        mRunParameter->setRotateSpeed(rotateSpeed);
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setRotateAngle(2);
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setRotateAngle(-2);
        }
        mRotateMachineAction->updateParameter();
        BlockDetectionState++;
        break;
    case 8:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mRotateMachineAction->stop();
            BlockDetectionState = 100;
        }
        break;
    case 100:
        BlockDetectionState = 0; // 状態変数の初期化
        // caseの開始位置を変更
        Straight_X_State = 110;
        Straight_Y_State = 110;
        TurnLeft_X_State = 110;
        TurnLeft_Y_State = 110;
        TurnRight_X_State = 110;
        TurnRight_Y_State = 110;
        BlockPut_X_State = 30;
        BlockPut_Y_State = 30;
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_arm_Right(bool start)
{
    switch (BlockPut_arm_state)
    {
    case 0:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_arm_state = 11;
        break;
    case 11:
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_arm_state = 30;
        }
        break;
    case 20:
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_arm_state++;
        break;
    case 21:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_arm_state = 30;
        }
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            ColorFlag_1 = true;
            ColorFlag_2 = false;
        }
        else
        { // 赤または黄を検知した
            ColorFlag_1 = false;
            ColorFlag_2 = true;
        }
        break;
    case 30:
        mRunParameter->setRotateAngle(60);
        mRunParameter->setRotateSpeed(10); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_arm_state++;
        break;
    case 31:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_arm_state++;
        }
        break;
    case 32:
        mRunParameter->setArmAngle(45);
        mRunParameter->setArmPower(100);
        mArmPositionSetAction->updateParameter();
        BlockPut_arm_state++;
        break;

    case 33: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_arm_state++;
        break;

    case 34:
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_arm_state++;
            }
        }
        break;
    case 35:
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_arm_state++;
        }
        break;
    case 36:
        mRunParameter->setRotateAngle(-60);
        mRunParameter->setRotateSpeed(10); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_arm_state++;
        break;
    case 37:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_arm_state = 100;
        }
        break;
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        BlockPut_arm_state = 0;
        return true;
        break;
    }
    return false;
}
bool BlockLineUpBehavior::BlockPut_arm_left(bool start)
{
    switch (BlockPut_arm_state)
    {
    case 0:
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_arm_state = 11;
        break;
    case 11:
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_arm_state = 30;
        }
        break;
    case 20:
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_arm_state++;
        break;
    case 21:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_arm_state = 30;
        }
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            ColorFlag_1 = true;
            ColorFlag_2 = false;
        }
        else
        { // 赤または黄を検知した
            ColorFlag_1 = false;
            ColorFlag_2 = true;
        }
        break;
    case 30:
        mRunParameter->setRotateAngle(-60);
        mRunParameter->setRotateSpeed(10); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_arm_state++;
        break;
    case 31:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_arm_state++;
        }
        break;
    case 32:
        mRunParameter->setArmAngle(45);
        mRunParameter->setArmPower(100);
        mArmPositionSetAction->updateParameter();
        BlockPut_arm_state++;
        break;

    case 33: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_arm_state++;
        break;

    case 34:
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_arm_state++;
            }
        }
        break;
    case 35:
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_arm_state++;
        }
        break;
    case 36:
        mRunParameter->setRotateAngle(60);
        mRunParameter->setRotateSpeed(10); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_arm_state++;
        break;
    case 37:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_arm_state = 100;
        }
        break;
    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        BlockPut_arm_state = 0;
        return true;
        break;
    }
    return false;
}
bool BlockLineUpBehavior::BlockPut_X_Outline(bool start)
{ // X軸でのブロックをサークル内に置く動作
    switch (BlockPut_X_State)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
        BlockPut_X_State = 11;
        break;
    case 11:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockPut_X_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_X_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 30;
        }
        break;
    case 30: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_X_State++;
        break;

    case 31:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_X_State++;
        }
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            ColorFlag_1 = true;
            ColorFlag_2 = false;
        }
        else
        { // 赤または黄を検知した
            ColorFlag_1 = false;
            ColorFlag_2 = true;
        }
        break;

    case 32: // 直進走行
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false); // setStraightAngle()に従い直進を行う
        if (mRunParameter->getRunRighEdgeFlag())
        { // 右エッジ時の動作
            mRunParameter->setStraightAngle(straightAngle + 1);
        }
        else
        { // 左エッジ時の動作
            mRunParameter->setStraightAngle(straightAngle - 1);
        }
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 33:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        { // サークル内の白を検知した
            mDistanceJudgement->stop();
            ev3_speaker_play_tone(NOTE_D5, 100);
            BlockPut_X_State++;
        }
        break;
    case 35:
        mRunParameter->setForwardSpeed(straightSpeed_LOW);
        mRunParameter->setTargetAngleFlag(false); // setStraightAngle()に従い直進を行う
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 34:
        if (CircleDetection()) //置き場検知
        {
            mRunStraightAction->stop();
            mRunStraightAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_X_State = 50;
        }
        break;

    case 50:
        mRunParameter->setRotateAngle(120); //120度回転
        mRunParameter->setRotateSpeed(10);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 52:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(10);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        break;

    case 53:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State++;
        }
        break;

    case 54: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(15);
        mArmPositionSetAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 55: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_X_State++;
            }
        }
        break;
    case 56: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_X_State++;
        }
        break;

    case 57:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-13);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 58:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            //black = mRunParameter->getBlack() + 7;
            if (CircleDetection()) //置き場検知
            {
                // mLineTraceAction->stop();
                mRunStraightAction->stop();
                ev3_speaker_play_tone(NOTE_E5, 100);
            }
            BlockPut_X_State++;
        }
        break;

    case 59:
        //後進中黒線検知
        mRunStraightAction->start();
        //if (mEV3ColorSensor->getBrightness() < black)
        //{
        ev3_speaker_play_tone(NOTE_C6, 100);
        //前進パラメーター
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        //距離指定
        mDistanceJudgement->setDistance(4.5);
        mDistanceJudgement->start();
        BlockPut_X_State++;
        //}
        break;
    case 60:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_X_State = 70;
        }
        break;

    case 70:
        mRunParameter->setRotateAngle(-120); //120度回転
        mRunParameter->setRotateSpeed(10);   //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 71:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_X_State++;
        }
        break;

    case 72:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-5);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_X_State++;
        break;

    case 73:
        mRunStraightAction->back();

        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_X_State = 100;
        }
        break;

    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}
bool BlockLineUpBehavior::BlockPut_Y_Right(bool start)
{ // Y軸でのブロックをサークル内に置く動作
    switch (BlockPut_Y_State)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
        BlockPut_Y_State = 11;
        break;
    case 11:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockPut_Y_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_Y_State = 30;
        }
        break;

    case 30: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_Y_State++;
        break;

    case 31:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_Y_State++;
        }
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            ColorFlag_1 = true;
            ColorFlag_2 = false;
        }
        else
        { // 赤または黄を検知した
            ColorFlag_1 = false;
            ColorFlag_2 = true;
        }
        break;
    case 32:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;
    case 33:
        mRunStraightAction->straight2();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 50;
        }
        break;
    case 50:
        mRunParameter->setRotateAngle(90); //120度回転
        mRunParameter->setRotateSpeed(15); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;
    case 52:
        mRunParameter->setRotateAngle(30); //120度回転
        mRunParameter->setRotateSpeed(30); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 53:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;
    case 54:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;

    case 55:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State++;
        }
        break;

    case 56: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 57: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_Y_State++;
            }
        }
        break;

    case 58: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_Y_State++;
        }
        break;

    case 59:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-12);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 60:
        mRunStraightAction->back();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            //black = mRunParameter->getBlack() + 7;
            if (CircleDetection()) //置き場検知
            {
                // mLineTraceAction->stop();
                mRunStraightAction->stop();
                ev3_speaker_play_tone(NOTE_E5, 100);
            }
            BlockPut_Y_State++;
        }
        break;

    case 61:
        //後進中黒線検知
        mRunStraightAction->start();
        //if (mEV3ColorSensor->getBrightness() < black)
        //{
        ev3_speaker_play_tone(NOTE_C6, 100);
        //前進パラメーター
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(false);
        mRunStraightAction->updateParameter();
        //距離指定
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        //}
        break;
    case 62:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 70;
        }
        break;
    case 70:
        mRunParameter->setRotateAngle(-120); //120度回転
        mRunParameter->setRotateSpeed(15);   //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 71:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;

    case 72:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-13);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 73:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_Y_State++;
        }
        break;

    case 74:
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_Y_State++;
        break;

    case 75:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_Y_State = 100;
        }
        break;

    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}

bool BlockLineUpBehavior::BlockPut_Y_left(bool start)
{ // Y軸でのブロックをサークル内に置く動作
    switch (BlockPut_Y_State)
    {
    case 0: // ライントレース
        // ライントレース走行パラメータを設定する
        getSwitchingParameter_LOW();
        mLineTraceAction->updateParameter();
        mDistanceJudgement->setDistance(Circle_Front_Distance);
        mDistanceJudgement->start();
        straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
        BlockPut_Y_State = 11;
        break;
    case 11:
        mLineTraceAction->start();
        if (mStraightDetection->isDetect())
        {                                                          // 直進検知
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //角度を保持
            ev3_speaker_set_volume(100);
            ev3_speaker_play_tone(NOTE_A5, 100);
            ev3_speaker_set_volume(50);
        }
        if (mDistanceJudgement->isDistanceOut())
        {
            ev3_speaker_play_tone(NOTE_C5, 100);
            mDistanceJudgement->stop();
            BlockPut_Y_State = 20;
        }
        break;
    case 20: // 前進
        mRunParameter->setLineTraceSpeed(PID_Circle[0]);
        mRunParameter->setKP(PID_Circle[1]);
        mRunParameter->setKI(PID_Circle[2]);
        mRunParameter->setKD(PID_Circle[3]);
        mLineTraceAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 21:
        //ライントレース中置き場検知
        if (mStraightDetection->isDetect()) // 角度の保持のための直進検知
        {
            straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
        }
        mLineTraceAction->start();

        if (CircleDetection()) //置き場検知
        {
            mLineTraceAction->stop();
            ev3_speaker_play_tone(NOTE_E5, 100);
            BlockPut_Y_State = 30;
        }
        break;

    case 30: // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_Y_State++;
        break;

    case 31:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_Y_State++;
        }
        mEV3ColorSensor->getBlockFloorColor();
        if (mEV3ColorSensor->getBlockFloorColor() == GREEN || mEV3ColorSensor->getBlockFloorColor() == BLUE)
        { // 緑または青を検知した
            ColorFlag_1 = true;
            ColorFlag_2 = false;
        }
        else
        { // 赤または黄を検知した
            ColorFlag_1 = false;
            ColorFlag_2 = true;
        }
        break;
    case 32:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;

    case 33:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 50;
        }
        break;
    case 50:
        mRunParameter->setRotateAngle(-90); //120度回転
        mRunParameter->setRotateSpeed(15);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 51:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;
    case 52:
        mRunParameter->setRotateAngle(-30); //120度回転
        mRunParameter->setRotateSpeed(30);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;
    case 53:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;
    case 54:
        //直進パラメータ設定
        mRunParameter->setForwardSpeed(20);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(7);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        break;

    case 55:
        mRunStraightAction->straight();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State++;
        }
        break;

    case 56: // アーム上げパラメーター
        mRunParameter->setArmAngle(30);
        mRunParameter->setArmPower(30);
        mArmPositionSetAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 57: // アームを上げてブロックを外す
        if (mTimerJudgement->isTimedOut())
        {
            mArmPositionSetAction->start();
            if (mArmPositionSetAction->isFinished())
            {
                mArmPositionSetAction->stop();
                mTimerJudgement->stop();
                mRunParameter->setArmAngle(0);
                mRunParameter->setArmPower(10);
                mArmPositionSetAction->updateParameter();
                BlockPut_Y_State++;
            }
        }
        break;
    case 58: // アームを下げる
        mArmPositionSetAction->start();
        if (mArmPositionSetAction->isFinished())
        {
            mArmPositionSetAction->stop();
            BlockPut_Y_State++;
        }
        break;

    case 59:
        mRunParameter->setForwardSpeed(-10);
        mDistanceJudgement->setDistance(-12);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 60:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            //black = mRunParameter->getBlack() + 7;
            if (CircleDetection()) //置き場検知
            {
                // mLineTraceAction->stop();
                mRunStraightAction->stop();
                ev3_speaker_play_tone(NOTE_E5, 100);
            }
            BlockPut_Y_State++;
        }
        break;

    case 61:
        //後進中黒線検知
        mRunStraightAction->start();
        //if (mEV3ColorSensor->getBrightness() < black)
        //{
        ev3_speaker_play_tone(NOTE_C6, 100);
        //前進パラメーター
        mRunParameter->setForwardSpeed(15);
        mRunParameter->setAngleResetFlag(true);
        mRunStraightAction->updateParameter();
        //距離指定
        mDistanceJudgement->setDistance(6);
        mDistanceJudgement->start();
        BlockPut_Y_State++;
        //}
        break;
    case 62:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            BlockPut_Y_State = 70;
        }
        break;

    case 70:
        mRunParameter->setRotateAngle(120); //120度回転
        mRunParameter->setRotateSpeed(20);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 71:
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            BlockPut_Y_State++;
        }
        break;

    case 72:
        mRunParameter->setForwardSpeed(-5);
        mDistanceJudgement->setDistance(-13);
        mDistanceJudgement->start();
        mRunStraightAction->updateParameter();
        BlockPut_Y_State++;
        break;

    case 73:
        mRunStraightAction->back2();
        if (mDistanceJudgement->isDistanceOutBack())
        {
            mDistanceJudgement->stop();
            mRunStraightAction->stop();
            BlockPut_Y_State++;
        }
        break;

    case 74:
        // 300ms停止
        mTimerJudgement->setTime(300);
        mTimerJudgement->start();
        BlockPut_Y_State++;
        break;

    case 75:
        if (mTimerJudgement->isTimedOut())
        {
            mTimerJudgement->stop();
            BlockPut_Y_State = 100;
        }
        break;

    case 100:
        Straight_X_State = 0;       // Straight_X_axisの状態変数
        Straight_Y_State = 0;       // Straight_Y_axisの状態変数
        TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
        TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
        TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
        TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
        UturnState = 0;             // Uturnの状態変数
        UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
        UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
        BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
        BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
        return true;
        break;
    }
    return false;
}