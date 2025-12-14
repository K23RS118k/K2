#ifndef EV3_SONAR_SENSOR_H
#define EV3_SONAR_SENSOR_H

#include "SonarSensor.h"
#include "ev3api.h"

class EV3SonarSensor{
public:
	explicit EV3SonarSensor(ev3api::SonarSensor &SonarSensor);
	virtual ~EV3SonarSensor();
	bool isDetected(int target);	//物体検知
	int getDistance();				//距離を取得する
	void resetSensor();				//センサを初期化する
private:
	ev3api::SonarSensor& mSonarSensor;
	double distance = 0;	//距離
	int count = 0;
};

#endif
