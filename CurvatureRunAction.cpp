#include "CurvatureRunAction.h"
//#include "BluetoothLogger.h"

CurvatureRunAction::CurvatureRunAction(){
}
CurvatureRunAction::~CurvatureRunAction(){
}

void CurvatureRunAction::start(){

	switch (state) {
		case 0:
			// left / right = speedRatio
			speedRatio = (1.0 - MACHINE_WIDTH / (2.0 * radias)) / (1.0 + MACHINE_WIDTH / (2.0 * radias));
/*
			left_speed = curevature_speed1[0];
			right_speed = curevature_speed1[1];
*/
			left_encoder[0] = mRearMotor -> getLeftEncoder();
			right_encoder[0] = mRearMotor -> getRightEncoder();

			count = 1;


			if(radias > 0){//右旋回
				left_speed = speed;
				right_speed = left_speed * speedRatio;
			}
			else{//左旋回
				right_speed = speed;
				left_speed = right_speed / speedRatio;
			}
			left_speed_limit = left_speed;
			right_speed_limit = right_speed;

			state++;
		case 1:
			if(count >= 25){
				left_encoder[1] = mRearMotor -> getLeftEncoder();
				right_encoder[1] = mRearMotor -> getRightEncoder();
				left_encoder_diff = left_encoder[1] - left_encoder[0];
				right_encoder_diff = right_encoder[1] - right_encoder[0];
				encoder_ratio = right_encoder_diff / left_encoder_diff;


				if(radias > 0){	//右旋回
					if(encoder_ratio > speedRatio * 1.05){	//旋回量不足
						right_speed *= 0.95;
					}
					else if(encoder_ratio < speedRatio * 0.95){	//旋回量過剰
						if(right_speed < right_speed_limit){
							right_speed *= 1.05;
						}
					}
				}else{	//左旋回
					if(encoder_ratio < speedRatio * 0.95){	//旋回量不足
						left_speed *= 0.95;
					}
					else if(encoder_ratio > speedRatio * 1.05){	//旋回量過剰
						if(left_speed < left_speed_limit){
							left_speed *= 1.05;
						}
					}
				}
				left_encoder[0] = left_encoder[1];
				right_encoder[0] = right_encoder[1];
				count = 1;

//				BluetoothLogger::dataLogger((int8_t)left_encoder_diff, (int8_t)right_encoder_diff, (int16_t)(speedRatio * 100.0), (int16_t)(encoder_ratio * 100.0), (int16_t)(left_speed * 100.0), (int16_t)(right_speed * 100.0));

				break;
			}
			count++;
			break;
	}
	mRearMotor -> setSpeed(left_speed, right_speed);
}

void CurvatureRunAction::stop(){
	mRearMotor -> setSpeed(0, 0);
	count = 0;
	state = 0;
}

void CurvatureRunAction::updateParameter(){
	radias = mRunParameter -> getCurvatureRadias();
	speed = mRunParameter -> getCurvatureRunSpeed();
	state = 0;
	count = 0;
}
