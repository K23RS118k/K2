#include "SpeedAdjustment.h"
#include <math.h>
#include "ev3api.h"
#include <stdlib.h>
#include "BluetoothLogger.h"

SpeedAdjustment::SpeedAdjustment(const ev3api::Clock &clock, RunParameter *runParameter)
	: mClock(clock), mRunParameter(runParameter)
{
}
SpeedAdjustment::~SpeedAdjustment()
{
}

double SpeedAdjustment::calcSpeed(double speed, int brightness, double kp, double ki, double kd)
{
	times++;
	currentSpeed = speed;
	currentKp = kp;
	currentKi = ki;
	currentKd = kd;
	currentValue = brightness;
	targetSpeed = (double)mRunParameter->getLineTraceSpeed();
	targetKp = (double)mRunParameter->getKP();
	targetKi = (double)mRunParameter->getKI();
	targetKd = (double)mRunParameter->getKD();
	target = mRunParameter->getTargetBrightness();
	machine();
	machine2();

	// スタートからゴールまでの時間が短いほうが評価が高い

	// 現在の状態のチェック
	if (fabs(em) + cUnstable < ea)
	{ // ジグザグ走行しているとき
		currentState = Zigzag;
		ev3_speaker_play_tone(NOTE_C5, 100);
	}
	else if (((ea_short - cOut) < fabs(em_short)) && (fabs(em_short) < (ea_short + cOut)))
	{ // コースアウトしているとき
		currentState = Out;
		ev3_speaker_play_tone(NOTE_E6, 100);
	}
	else if (ea <= cStable)
	{ // 安定しているとき
		currentState = Stable;
	}
	else
	{ // どれにも当てはまらない状態
		currentState = Unkown;
	}

	// パラメータの更新
	if (currentState == Out)
	{ // コースアウトしているとき、減速する
		currentSpeed -= speedPolicy;
	}
	else if (currentState == Zigzag)
	{ // ジグザグ走行しているとき、Dゲインを減らす
		currentKd -= dgainPolicy;
	}
	else if (currentState == Stable)
	{ // 安定しているとき、区間パラメータへ戻す
		// SPEED
		if (currentSpeed < targetSpeed)
		{
			currentSpeed += speedPolicy;
		}
		else if (currentSpeed > targetSpeed)
		{
			currentSpeed -= speedPolicy;
		}
		// KP
		if (currentKp < targetKp)
		{
			currentKp += pgainPolicy;
		}
		else if (currentKp > targetKp)
		{
			currentKp -= pgainPolicy;
		}
		// KI
		if (currentKi < targetKi)
		{
			currentKi += igainPolicy;
		}
		else if (currentKi > targetKi)
		{
			currentKi -= igainPolicy;
		}
		// KD
		if (currentKd < targetKd)
		{
			currentKd += dgainPolicy;
		}
		else if (currentKd > targetKd)
		{
			currentKd -= dgainPolicy;
		}
	}

	// ログの出力（状態, 速度変化量, 時間, ea_short, ea, em_short, em）
	//BluetoothLogger::dataLogger(currentState, ea_short, time, speedPolicy * 100, ea, em_short, em);

	return currentSpeed;
}

void SpeedAdjustment::start()
{
	time = mClock.now();
}

void SpeedAdjustment::stop()
{
	time = mClock.now() - time;
	//BluetoothLogger::dataLogger(currentState, 1, cStable * 10, cUnstable * 100, cOut * 10000, 0, time);
	//BluetoothLogger::dataLogger(currentState, 0, speedPolicy * 10000, pgainPolicy * 100000, igainPolicy * 100000, dgainPolicy * 100000, time);
}

/**
 * 新しいポリシーの設定
 */
/* ポリシーの更新 繰り返すことにより最大3^4=81パターンできる */
void SpeedAdjustment::updatePolicy()
{
	srand((unsigned int)mClock.now()); //現在時刻を元に種を生成
	cStable = cStableBase;
	//+((rand() % 3) - 1) * 3.0;							  //-3, 0, 3
	cUnstable = cUnstableBase;
	//+((rand() % 3) - 1) * 0.5;							  //-0.5, 0, 0.5
	cOut = cOutBase;
	//+((rand() % 3) - 1) * 0.002; //-0.002, 0, 0.002

	speedPolicy = speedPolicyBase;
	//+((rand() % 3) - 1) * 0.001;								 //-0.001, 0, 0.001
	pgainPolicy = pgainPolicyBase;
	//+((rand() % 3) - 1) * 0.0001;								 //-0.0001, 0, 0.0001
	igainPolicy = igainPolicyBase;
	//+((rand() % 3) - 1) * 0.0001;								 //-0.0001, 0, 0.0001
	dgainPolicy = dgainPolicyBase;
	//+((rand() % 3) - 1) * 0.0001; //-0.0001, 0, 0.0001
}

/**
*emの計算
*/
void SpeedAdjustment::machine()
{
	if (count <= 149)
	{ // 変更
		et = currentValue - target;
		a[index] = et;
		count++;
		index++;
	}
	else
	{
		if (index == 150)
		{ // 変更
			index = 0;
		}
		et = currentValue - target;
		a[index] = et;
		et3 = 0; // 追加
		for (int i = 0; i < 150; i++)
		{
			et3 += a[i];
		}
		em = et3 / 150.0; // 変更

		em_short = 0;
		if (index + 25 >= 150)
		{
			for (int i = index; i < 150; i++)
			{
				em_short += a[i];
			}
			for (int i = 0; i < (index + 25) % 150; i++)
			{
				em_short += a[i];
			}
		}
		else
		{
			for (int i = index; i < (index + 25); i++)
			{
				em_short += a[i];
			}
		}
		em_short = em_short / 25.0;

		index++;
	}
}

/**
*eaの計算
*/
void SpeedAdjustment::machine2()
{
	if (count2 <= 149)
	{ // 変更
		et2 = fabs(currentValue - target);
		b[index2] = et2;
		count2++;
		index2++;
	}
	else
	{
		if (index2 == 150)
		{				// 変更
			index2 = 0; // 変更
		}
		et2 = fabs(currentValue - target); // 変更
		b[index2] = et2;
		et4 = 0; // 追加
		for (int i = 0; i < 150; i++)
		{
			et4 += b[i];
		}
		ea = et4 / 150.0; // 変更

		ea_short = 0;
		if (index2 + 25 >= 150)
		{
			for (int i = index2; i < 150; i++)
			{
				ea_short += b[i];
			}
			for (int i = 0; i < (index2 + 25) % 150; i++)
			{
				ea_short += b[i];
			}
		}
		else
		{
			for (int i = index2; i < (index + 25); i++)
			{
				ea_short += b[i];
			}
		}
		ea_short = ea_short / 25.0;

		index2++;
	}
}

int SpeedAdjustment::getCount()
{
	return times;
}

double SpeedAdjustment::getEm()
{
	return em;
}

double SpeedAdjustment::getEa()
{
	return ea;
}

double SpeedAdjustment::getEm_short()
{
	return em_short;
}

double SpeedAdjustment::getEa_short()
{
	return ea_short;
}

double SpeedAdjustment::getCurrentSpeed()
{
	return currentSpeed;
}

double SpeedAdjustment::getCurrentKP()
{
	return currentKp;
}

double SpeedAdjustment::getCurrentKI()
{
	return currentKi;
}

double SpeedAdjustment::getCurrentKD()
{
	return currentKd;
}
