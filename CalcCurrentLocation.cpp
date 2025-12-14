#include "CalcCurrentLocation.h"
//#include "BluetoothLogger.h"

CalcCurrentLocation::CalcCurrentLocation(RearMotor *rearMotor)
	: mRearMotor(rearMotor)
{
}

CalcCurrentLocation::~CalcCurrentLocation()
{
}

/* ブロック並べ攻略時、機体の幅設定する */
void CalcCurrentLocation::setChangeWidthFlag(bool flag)
{
	change_width_flag = flag;
}

/* 座標を計算する */
void CalcCurrentLocation::calcCurrentLocation()
{
	if (change_width_flag)
	{
		MACHINE_WIDTH = 14.9;
	}
	else
	{
		MACHINE_WIDTH = 14.5;
	}
	//モータのエンコーダ値を取得
	right_wheel_angle[1] = mRearMotor->getRightEncoder();
	left_wheel_angle[1] = mRearMotor->getLeftEncoder();

	//右モータの回転角度変化量を計算
	delta_right_wheel_angle = right_wheel_angle[1] - right_wheel_angle[0];
	//右モータの過去のエンコーダ値を更新
	right_wheel_angle[0] = right_wheel_angle[1];
	//左モータの角度変化量変化量を計算
	delta_left_wheel_angle = left_wheel_angle[1] - left_wheel_angle[0];
	//左モータの過去のエンコーダ値を更新
	left_wheel_angle[0] = left_wheel_angle[1];

	//右タイヤの距離変化量を計算
	delta_right_wheel_distance = delta_right_wheel_angle * M_PI * WHEEL_DIAMETER / 360.0;
	//左タイヤの距離変化量を計算
	delta_left_wheel_distance = delta_left_wheel_angle * M_PI * WHEEL_DIAMETER / 360.0;
	//走行体の距離変化量を計算
	delta_distance = (delta_right_wheel_distance + delta_left_wheel_distance) / 2.0;
	//走行体の走行距離を計算
	distance += delta_distance;

	//角度変化量を計算
	delta_angle = (delta_left_wheel_distance - delta_right_wheel_distance) / MACHINE_WIDTH;

	//過去の角度
	angle[0] = angle[1];
	//角度を更新
	angle[1] = angle[0] + delta_angle;
	//角度を変換
	final_angle = angle[1] / M_PI * 180;

	//360°～-360°以内に制限
	if (final_angle <= -360 || final_angle >= 360)
	{
		final_angle = 0;
		angle[0] = 0;
		angle[1] = 0;
	}

	//x座標を計算して格納
	point[X] += delta_distance * cos(angle[0] + (delta_angle / 2.0));
	//y座標を計算して格納
	point[Y] += delta_distance * sin(angle[0] + (delta_angle / 2.0));

	//角度を格納
	//180°(-180°)を超えた時に反転させる
	if (final_angle > 180 && final_angle <= 360)
	{
		point[ANGLE] = -360 + final_angle;
	}
	else if (final_angle < -180 && final_angle >= -360)
	{
		point[ANGLE] = 360 + final_angle;
	}
	else
	{
		point[ANGLE] = final_angle;
	}

	/* ログ取り用 */
	//	BluetoothLogger::dataLogger(0, 0, (int16_t)point[X], (int16_t)point[Y], (int16_t)point[ANGLE], (int16_t)distance);
}

/* X座標を設定する */
void CalcCurrentLocation::setPointX(double pointx)
{
	point[X] = pointx;
}
/* Y座標を設定する */
void CalcCurrentLocation::setPointY(double pointy)
{
	point[Y] = pointy;
}
/* 角度を設定する */
void CalcCurrentLocation::setAngle(double angle)
{
	this->angle[0] = angle * M_PI / 180;
	this->angle[1] = angle * M_PI / 180;
}
/* X座標を取得する */
double CalcCurrentLocation::getPointX()
{
	return point[X];
}
/* Y座標を取得する */
double CalcCurrentLocation::getPointY()
{
	return point[Y];
}
/* 角度を取得する */
double CalcCurrentLocation::getAngle()
{
	return point[ANGLE];
}
/* 距離を取得する */
double CalcCurrentLocation::getDistance()
{
	return distance;
}
/* 計算用の角度を取得する */
double CalcCurrentLocation::getFinalAngle()
{
	return final_angle;
}
double CalcCurrentLocation::getDeltaAngle()
{
	return delta_angle;
}
