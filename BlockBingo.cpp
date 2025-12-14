#include "BlockBingo.h"

BlockBingo::BlockBingo()
{
    state = 0;
}

BlockBingo::~BlockBingo()
{
}

void BlockBingo::execute()
{
    switch (state)
    {
    case 0:
        //初期パラメーター設定
        mRunParameter->setRunRightEdgeFlag(true); //左エッジ走行
        mRunParameter->setChangeSpeedFlag(false); //速度調整有効
        mRunParameter->setLineTraceSpeed(15);     //ライントレース速度
        mRunParameter->setForwardSpeed(15);       //直進速度
        //PID設定
        mRunParameter->setKP(0.5);
        mRunParameter->setKI(0);
        mRunParameter->setKD(1);
        mLineTraceAction->updateParameter();
        state++;
        break;

        //ブロックビンゴ直進
    case 1:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 2:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 3:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 4:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;

    case 5:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 6:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 7:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 8:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;
    case 9:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 10:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 11:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 12:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;
    case 13:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 14:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 15:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 16:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state = 19;
        }
        break;

    case 19:
        flag = true;
        break;
    }
}

void BlockBingo::execute_turn()
{
    switch (state)
    {
    case 0:
        //初期パラメーター設定
        mRunParameter->setRunRightEdgeFlag(true); //左エッジ走行
        mRunParameter->setChangeSpeedFlag(false); //速度調整有効
        mRunParameter->setLineTraceSpeed(10);     //ライントレース速度
        mRunParameter->setForwardSpeed(10);       //直進速度
        //PID設定
        mRunParameter->setKP(0.5);
        mRunParameter->setKI(0);
        mRunParameter->setKD(1);
        mLineTraceAction->updateParameter();
        state++;
        break;

        //ブロックビンゴ直進
    case 1:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 2:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 3:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 4:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;

    case 5:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 6:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 7:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 8:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;
    case 9:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 10:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 11:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 12:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state++;
        }
        break;
    case 13:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 14:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 15:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 16:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークルから出たことを検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_F5, 100);
            state = 17;
        }
        break;

    case 17:
        //ブロックビンゴ攻略を行わない場合の回転
        mRunParameter->setRotateAngle(-380); //360°回転(右回転)
        mRunParameter->setRotateSpeed(40);   //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        state++;
        break;

    case 18:
        //回転開始
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            state++;
        }
        break;

    case 19:
        flag = true;
        break;
    }
}

void BlockBingo::left_turn() //左折
{
    switch (state)
    {
    case 1:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 2:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 3:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 4:
        //左折
        mRunParameter->setRotateAngle(-90); //90°回転(右回転)
        mRunParameter->setRotateSpeed(20);  //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        state++;
        break;

    case 5:
        //回転開始
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            state++;
        }
        break;

    case 6:
        flag = true;
        break;
    }
}

void BlockBingo::right_turn() //右折
{
    switch (state)
    {
    case 1:
        //ライントレース
        mLineTraceAction->start();
        color = mEV3ColorSensor->getBlockFloorColor(); //サークルの色を検知
        if (color == GREEN || color == YELLOW)         //緑または黄を検知した
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_C5, 100);
            //直進パラメーター設定
            mRunParameter->setTargetAngleFlag(true);               //setStraightAngle()に従い直進を行う
            straightAngle = mCalcCurrentLocation->getFinalAngle(); //現在の角度を取得
            if (mRunParameter->getRunRighEdgeFlag())
            {
                mRunParameter->setStraightAngle(straightAngle + 1); //直進時の目標角度指定
            }
            else
            {
                mRunParameter->setStraightAngle(straightAngle - 1); //直進時の目標角度指定
            }
            mRunStraightAction->updateParameter();
            state++;
        }
        break;

    case 2:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() != color) //サークル内の白を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_D5, 100);
            state++;
        }
        break;

    case 3:
        //直進
        mRunStraightAction->start();
        if (mEV3ColorSensor->getBlockFloorColor() == color) //サークルの色を検知
        {
            //検知確認用
            ev3_speaker_set_volume(50);
            ev3_speaker_play_tone(NOTE_E5, 100);
            state++;
        }
        break;

    case 4:
        //右折
        mRunParameter->setRotateAngle(90); //90°回転(右回転)
        mRunParameter->setRotateSpeed(20); //回転時のスピード
        //回転走行パラメータを更新
        mRotateMachineAction->updateParameter();
        state++;
        break;

    case 5:
        //回転開始
        mRotateMachineAction->start();
        if (mRotateMachineAction->isFinished())
        {
            mRotateMachineAction->stop();
            ev3_speaker_play_tone(NOTE_C6, 200);
            state++;
        }
        break;

    case 6:
        flag = true;
        break;
    }
}
int BlockBingo::isFinished()
{
    return flag;
}