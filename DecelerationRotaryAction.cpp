#include "DecelerationRotaryAction.h"

DecelerationRotaryAction::DecelerationRotaryAction()
{
}
DecelerationRotaryAction::~DecelerationRotaryAction()
{
}

void DecelerationRotaryAction::start()
{
    switch (state)
    {
    case 0:
        updateParameter();
        angle[0] = 0;
        /* 目標角度を5等分 */
        split_angle[0] = targetAngle / 5;
        split_angle[1] = targetAngle * 2 / 5;
        split_angle[2] = targetAngle * 3 / 5;
        split_angle[3] = targetAngle * 4 / 5;
        split_angle[4] = targetAngle;
        /* 目標速度を5分割 */
        split_speed[0] = power * 0.6;
        split_speed[1] = power * 0.7;
        split_speed[2] = power * 1.0;
        split_speed[3] = power * 0.7; //0.75
        split_speed[4] = power * 0.6; //0.65
        state++;
        break;
    case 1:
        //設定された角度で回転方向判定する
        if (targetAngle >= 0)
        { //右回転
            state = 10;
        }
        else
        { //左回転
            state = 30;
        }
        break;
    //右回転
    case 10: //PWM値65%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(split_speed[0], -split_speed[0]);
        if (currentAngle >= split_angle[0])
        {
            state++;
        }
        break;
    case 11: //PWM値75%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(split_speed[1], -split_speed[1]);
        if (currentAngle >= split_angle[1])
        {
            state++;
        }
        break;
    case 12: //PWM値100%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(split_speed[2], -split_speed[2]);
        if (currentAngle >= split_angle[2])
        {
            state++;
        }
        break;
    case 13: //PWM値75%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(split_speed[3], -split_speed[3]);
        if (currentAngle >= split_angle[3])
        {
            state++;
        }
        break;
    case 14: //PWM値65%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(split_speed[4], -split_speed[4]);
        if (currentAngle >= split_angle[4])
        {
            ev3_speaker_play_tone(NOTE_C6, 200);
            flag = true;
        }
        break;
    //左回転
    case 30: //PWM値65%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(-split_speed[0], split_speed[0]);
        if (currentAngle <= split_angle[0])
        {
            state++;
        }
        break;
    case 31: //PWM値75%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(-split_speed[1], split_speed[1]);
        if (currentAngle <= split_angle[1])
        {
            state++;
        }
        break;
    case 32: //PWM値100%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(-split_speed[2], split_speed[2]);
        if (currentAngle <= split_angle[2])
        {
            state++;
        }
        break;
    case 33: //PWM値75%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(-split_speed[3], split_speed[3]);
        if (currentAngle <= split_angle[3])
        {
            state++;
        }
        break;
    case 34: //PWM値65%
        angle[1] = angle[0] + mCalcCurrentLocation->getDeltaAngle();
        angle[0] = angle[1];
        currentAngle = angle[1] * 180 / M_PI;

        mRearMotor->setSpeed(-split_speed[4], split_speed[4]);
        if (currentAngle <= split_angle[4])
        {
            ev3_speaker_play_tone(NOTE_C6, 200);
            flag = true;
        }
        break;
    }
}
void DecelerationRotaryAction::stop()
{
    mRearMotor->setSpeed(0, 0);
    state = 0;
    flag = false;
}
void DecelerationRotaryAction::updateParameter()
{
    if (mRunParameter->getUturnApporoachFlag())
    { //Uターン動作時
        if (mRunParameter->getRunRighEdgeFlag())
        { //エッジにより回転方向を切り替える
            //パラメーターを固定
            targetAngle = -181;
        }
        else
        {
            //パラメーターを固定
            targetAngle = -181;
        }
        power = 25;
    }
    else
    {
        //Runparameterで設定された値を格納
        targetAngle = mRunParameter->getDecelerationRotaryAngle();
        power = mRunParameter->getDecelerationRotarySpeed();
    }
    state = 0;
    flag = false;
}

bool DecelerationRotaryAction::isFinished()
{
    return flag;
}