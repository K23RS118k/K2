#ifndef LINE_TRACE_ACTION_H
#define LINE_TRACE_ACTION_H

#include "Action.h"
#include "SpeedAdjustment.h"

class LineTraceAction : public Action{
public:
	explicit LineTraceAction(SpeedAdjustment* speedAdjustment);
	virtual ~LineTraceAction();

	void start();				//動作開始
	void stop();				//動作停止
	void updateParameter();		//パラメータ更新

	double getSpeed();
	double getKP();
	double getKI();
	double getKD();

private:
	SpeedAdjustment* mSpeedAdjustment;

	double speed;				//速度
	double deceleration;		//目標速度と現在速度の差
	double decelerationKp;
	double decelerationKi;
	double decelerationKd;
	double prev_Speed;			//前回の速度
	double prev_Kp;
	double prev_Ki;
	double prev_Kd;
	double changeSpeed;			//調整後の速度
	double changeKp;
	double changeKi;
	double changeKd;
	double kp;					//KPゲイン
	double ki;					//KIゲイン
	double kd;					//KDゲイン
	double target;				//目標値
	int brightness;				//輝度値
	bool run_right_edge_flag;	//走行エッジフラグ
	bool change_speed_flag;		//速度調整有効フラグ
	double turn;				//旋回量

	double integral = 0;		//累積
	double deviation = 0;		//目標値と現在値の偏差
	double prev_deviation = 0;	//前回の偏差
	double prev = 0;	//前回の偏差

};

# endif
