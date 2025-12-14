#include "BluetoothLogger.h"
#include <syssvc/serial.h>

static char data_log_buffer[34] = {0};

void BluetoothLogger::dataLogger(int8_t data1, int8_t data2) {
	
	data_log_buffer[0] = (unsigned char) (32 & 0xFF);
	data_log_buffer[1] = (unsigned char) ((32 >> 8) & 0xFF);
	
	*((uint32_t *)(&data_log_buffer[2]))  = (uint32_t)mClock.now();
	*(( int8_t *)(&data_log_buffer[6]))  =  (int8_t)data1;
	*(( int8_t *)(&data_log_buffer[7]))  =  (int8_t)data2;
	*((uint16_t *)(&data_log_buffer[8]))  = (uint16_t)ev3_battery_voltage_mV(); // 注意：int型をunit16_tへキャストしている
//	*((int *)(&data_log_buffer[10]))  = (int)mSteer.getCount();
	*((int *)(&data_log_buffer[14])) = (int)mRightWheel.getCount();
	*((int *)(&data_log_buffer[18])) = (int)mLeftWheel.getCount();
	*((int16_t *)(&data_log_buffer[22])) = (int16_t)mTouchSensor.isPressed();
	*((int16_t *)(&data_log_buffer[24])) = 0;
	*((int16_t *)(&data_log_buffer[26])) = (int16_t)mColorSensor.getBrightness();
	*((int16_t *)(&data_log_buffer[28])) = (int16_t)mGyroSensor.getAnglerVelocity();
	*((int *)(&data_log_buffer[30])) = (int)0;
	
	// カーネルのシリアルインターフェスドライバを使う場合
	// bufが指すバッファからBluetooth仮想シリアルポートへ最大lenバイトを書き込む
	serial_wri_dat(SIO_PORT_BT, data_log_buffer, 34);
}

void BluetoothLogger::dataLogger(int8_t data1, int8_t data2, int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4) {
	
	data_log_buffer[0] = (unsigned char) (32 & 0xFF);
	data_log_buffer[1] = (unsigned char) ((32 >> 8) & 0xFF);
	
	*((uint32_t *)(&data_log_buffer[2]))  = (uint32_t)mClock.now();
	*(( int8_t *)(&data_log_buffer[6]))  =  (int8_t)data1;
	*(( int8_t *)(&data_log_buffer[7]))  =  (int8_t)data2;
	*((uint16_t *)(&data_log_buffer[8]))  = (uint16_t)ev3_battery_voltage_mV(); // 注意：int型をunit16_tへキャストしている
//	*((int *)(&data_log_buffer[10]))  = (int)mSteer.getCount();
	*((int *)(&data_log_buffer[14])) = (int)mRightWheel.getCount();
	*((int *)(&data_log_buffer[18])) = (int)mLeftWheel.getCount();
	*((int16_t *)(&data_log_buffer[22])) = (int16_t)adc1;
	*((int16_t *)(&data_log_buffer[24])) = (int16_t)adc2;
	*((int16_t *)(&data_log_buffer[26])) = (int16_t)adc3;
	*((int16_t *)(&data_log_buffer[28])) = (int16_t)adc4;
	*((int *)(&data_log_buffer[30])) = (int)0;
	
	// カーネルのシリアルインターフェスドライバを使う場合
	// bufが指すバッファからBluetooth仮想シリアルポートへ最大lenバイトを書き込む
	serial_wri_dat(SIO_PORT_BT, data_log_buffer, 34);
}

void BluetoothLogger::dataLogger(int8_t data1, int8_t data2, int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4, int data32) {
	
	data_log_buffer[0] = (unsigned char) (32 & 0xFF);
	data_log_buffer[1] = (unsigned char) ((32 >> 8) & 0xFF);
	
	*((uint32_t *)(&data_log_buffer[2]))  = (uint32_t)mClock.now();
	*(( int8_t *)(&data_log_buffer[6]))  =  (int8_t)data1;
	*(( int8_t *)(&data_log_buffer[7]))  =  (int8_t)data2;
	*((uint16_t *)(&data_log_buffer[8]))  = (uint16_t)ev3_battery_voltage_mV(); // 注意：int型をunit16_tへキャストしている
//	*((int *)(&data_log_buffer[10]))  = (int)mSteer.getCount();
	*((int *)(&data_log_buffer[14])) = (int)mRightWheel.getCount();
	*((int *)(&data_log_buffer[18])) = (int)mLeftWheel.getCount();
	*((int16_t *)(&data_log_buffer[22])) = (int16_t)adc1;
	*((int16_t *)(&data_log_buffer[24])) = (int16_t)adc2;
	*((int16_t *)(&data_log_buffer[26])) = (int16_t)adc3;
	*((int16_t *)(&data_log_buffer[28])) = (int16_t)adc4;
	*((int *)(&data_log_buffer[30])) = (int)data32;
	
	// カーネルのシリアルインターフェスドライバを使う場合
	// bufが指すバッファからBluetooth仮想シリアルポートへ最大lenバイトを書き込む
	serial_wri_dat(SIO_PORT_BT, data_log_buffer, 34);
}