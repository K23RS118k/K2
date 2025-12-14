#ifndef CALC_CURRENT_LOCATION_H
#define CALC_CURRENT_LOCATION_H

/* 定数の定義 */
#define X 0
#define Y 1
#define ANGLE 2

#include "RearMotor.h"
#include "math.h"

class CalcCurrentLocation
{
  public:
	explicit CalcCurrentLocation(RearMotor *rearMotor);
	virtual ~CalcCurrentLocation();

	void calcCurrentLocation();			//座標を計算する
	void setChangeWidthFlag(bool flag); //ブロック並べ攻略時の機体の幅を切り替える true:ブロック並べ攻略時, false:スピード競技時

	void setPointX(double pointx); //x座標を設定する
	void setPointY(double pointy); //y座標を設定する
	void setAngle(double angle);   //角度を設定する
	double getPointX();			   //x座標を取得する
	double getPointY();			   //y座標を取得する
	double getAngle();			   //角度を取得する
	double getDistance();		   //距離を取得する
	double getFinalAngle();		   //計算用の角度を取得する
	double getDeltaAngle();

  private:
	RearMotor *mRearMotor;

	/* ブロック並べ攻略、設定される */
	bool change_width_flag = false;

	/* モータのエンコーダ値 */
	double left_wheel_angle[2] = {0, 0};
	double right_wheel_angle[2] = {0, 0};
	/* モータの回転角度の変化量 */
	double delta_left_wheel_angle = 0;
	double delta_right_wheel_angle = 0;

	/* 距離変化量 */
	double delta_left_wheel_distance = 0;
	double delta_right_wheel_distance = 0;
	double delta_distance = 0;
	/* 総走行距離 */
	double distance = 0;

	/* 角度 */
	double delta_angle = 0;
	double angle[2] = {0, 0};
	double final_angle;

	/* 座標を格納する配列 */
	/* point[3] = {X, Y, ANGLE} */
	double point[3];

	//車輪の直径(cm)
	const double WHEEL_DIAMETER = 10.0;
	//機体の幅(cm) ← タイヤの接地面間の距離
	// double MACHINE_WIDTH = 14.9;
	double MACHINE_WIDTH = 14.5;
};

#endif
