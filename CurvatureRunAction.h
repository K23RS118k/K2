#ifndef CURVATURE_RUN_ACTION_H
#define CURVATURE_RUN_ACTION_H

#include "Action.h"

class CurvatureRunAction : public Action{
public:
	explicit CurvatureRunAction();
	virtual ~CurvatureRunAction();

	void start();
	void stop();
	void updateParameter();

private:
	int state = 0;
	double radias;						//曲率半径
	double speedRatio;
	int count = 0;
	int speed = 30;						//速度
	double left_speed = 0;				//左モータの操作量
	double right_speed = 0;				//右モータの操作量
	double left_speed_limit = 0;		//左モータの操作量上限
	double right_speed_limit = 0;		//右モータの操作量上限
	const double MACHINE_WIDTH = 13.0;	//走行体の幅

	int curevature_speed1[2] = {80, 40};

	double left_encoder[2] = {0.0, 0.0};
	double right_encoder[2] = {0.0, 0.0};
	double left_encoder_diff = 0.0;
	double right_encoder_diff = 0.0;
	double encoder_ratio = 0.0;
};

#endif
