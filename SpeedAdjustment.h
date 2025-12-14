#ifndef SPEED_ADJUSTMENT_H
#define SPEED_ADJUSTMENT_H

#include "Clock.h"
#include "RunParameter.h"

class SpeedAdjustment
{
  public:
	explicit SpeedAdjustment(const ev3api::Clock &clock, RunParameter *runParameter);
	virtual ~SpeedAdjustment();

	double calcSpeed(double speed, int brightness, double kp, double ki, double kd); //調整後の速度を取得する

	/**	
*各値ゲッター
*/
	void start();
	void stop();
	double getEm();
	double getEa();
	double getEm_short();
	double getEa_short();
	double getCurrentSpeed();
	int getCount();

	double getCurrentKP();
	double getCurrentKI();
	double getCurrentKD();

	/* ポリシーの取得と更新 */
	void updatePolicy();

  private:
	const ev3api::Clock &mClock;
	uint32_t time;

	RunParameter *mRunParameter;
	void machine();  //emを計算する
	void machine2(); //eaを計算する

	int times = 0;
	double currentSpeed; //現在速度
	double currentKp;
	double currentKi;
	double currentKd;
	double targetSpeed; //目標速度
	double targetKp;
	double targetKi;
	double targetKd;
	double target;	//目標値
	int currentValue; //輝度値

	int et;  //em学習内で使用する目標値と現在地の偏差
	int et2; //ea学習内で使用する目標値と現在地の偏差
	int et3;
	int et4;
	int index = 0;
	int index2 = 0;
	double em;		 //現在地－目標値を150回繰り返した時の平均値
	double ea;		 //現在地－目標値を150回繰り返した時の絶対平均値
	double em_short; //現在地-目標値を25回繰り返した時の平均値
	double ea_short; //現在地-目標値を25回繰り返した時の絶対平均値
	int count = 0;
	int count2 = 0;
	int a[150];
	int b[150];

	enum condition
	{
		Stable,
		Zigzag,
		Out,
		Unkown
	};

	enum condition currentState = Stable;

	//初期の走行状態判定定数を設定する
	double cStableBase = 15.0;  //安定定数
	double cUnstableBase = 2.0; //不安定定数（ジグザグ）
	double cOutBase = 0.0001;   //コースアウト定数

	//updatePolicy()をするたびに変化する走行状態判定定数
	double cStable;   //安定定数
	double cUnstable; //不安定定数（ジグザグ）
	double cOut;	  //コースアウト定数

	//初期のポリシーを設定する
	double speedPolicyBase = 0.001;  //速度変化量
	double pgainPolicyBase = 0;		 //pゲインの変化量
	double igainPolicyBase = 0;		 //iゲインの変化量
	double dgainPolicyBase = 0.0001; //dゲインの変化量

	//updatePolicy()をするたびに変化するポリシー
	double speedPolicy; //速度変化量
	double pgainPolicy; //pゲインの変化量
	double igainPolicy; //iゲインの変化量
	double dgainPolicy; //dゲインの変化量

	double reward[11] = {};
};

#endif