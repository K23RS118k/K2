#ifndef BLUETOOTH_LOGGER_H
#define BLUETOOTH_LOGGER_H

#include "TouchSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Clock.h"


class BluetoothLogger {
	static ev3api::TouchSensor& mTouchSensor;
	static ev3api::SonarSensor& mSonarSensor;
	static ev3api::ColorSensor& mColorSensor;
	static ev3api::GyroSensor& mGyroSensor;
	static ev3api::Motor& mSteer;
	static ev3api::Motor& mRightWheel;
	static ev3api::Motor& mLeftWheel;
	static ev3api::Clock& mClock;

public:
	static void dataLogger(int8_t data1, int8_t data2);
	static void dataLogger(int8_t data1, int8_t data2, int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4);
	static void dataLogger(int8_t data1, int8_t data2, int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4, int data32);
	
private:
	
};

#endif
