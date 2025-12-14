#include "EV3SonarSensor.h"
#include "BluetoothLogger.h"

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)

EV3SonarSensor::EV3SonarSensor(ev3api::SonarSensor &SonarSensor)
	: mSonarSensor(SonarSensor)
{
}

EV3SonarSensor::~EV3SonarSensor()
{
}

bool EV3SonarSensor::isDetected(int target)
{
	return (getDistance() <= target && getDistance() != 0) ? true : false;
}
/*
* 距離を取得する
* @return 距離
*/

int EV3SonarSensor::getDistance()
{
	//100msごとに呼び出し
	if (count % 25 == 0)
	{
		distance = mSonarSensor.getDistance();
		count = 0;
	}
	count++;
	//BluetoothLogger::dataLogger(0, 0, (int16_t)distance, 0, 0, 0,0);
	return distance;
}

void EV3SonarSensor::resetSensor()
{
	count = 0;
	//BluetoothLogger::dataLogger(0, 0, (int16_t)255, 0, 0, 0,0);
	ev3_sensor_config(EV3_PORT_3, HT_NXT_ACCEL_SENSOR);
	ev3_sensor_config(EV3_PORT_3, ULTRASONIC_SENSOR);
}