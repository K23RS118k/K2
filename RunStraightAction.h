#ifndef RUN_STRAIGHT_ACTION_H
#define RUN_STRAIGHT_ACTION_H

#include "Action.h"

class RunStraightAction : public Action
{
  public:
	explicit RunStraightAction();
	virtual ~RunStraightAction();

	void start(); //動作開始
	void stop();  //動作停止
	void straight();
	void straight2();
	void straight3();
	void back();
	void back2();
	void turnleft();
	void turnRight();
	void updateParameter(); //パラメータ更新
	void straight(int r,int l);

  private:
	int state = 0;
	double currentAngle;	   //現在の機体の角度
	double targetAngle;		   //目標角度
	bool resetFlag = true;	 //角度再設定フラグ
	bool setAngleFlag = false; //角度指定フラグ
	int speed;				   //速度

	const int offset = 3; //補正値
};

#endif
