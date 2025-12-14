#ifndef ACTION_H
#define ACTION_H

#include "RunParameter.h"
#include "RearMotor.h"
#include "ArmControl.h"
#include "EV3ColorSensor.h"
#include "CalcCurrentLocation.h"

class Action
{
  public:
	virtual ~Action();

	virtual void start() = 0;			//動作開始
	virtual void stop() = 0;			//動作停止
	virtual void updateParameter() = 0; //パラメータ更新(動作前に一度呼ぶこと)

	/* 使用するクラスのオブジェクトへの参照を設定 */
	static void setObject(
		RunParameter *runParameter,
		RearMotor *rearMotor,
		ArmControl *armControl,
		EV3ColorSensor *ev3ColorSensor,
		CalcCurrentLocation *calcCurrentLocation,
		Action *lineTraceAction,
		Action *RunStraightAction,
		Action *curvatureRunAction);

  protected:
	/* 使用するクラスのオブジェクト */
	static RunParameter *mRunParameter;
	static RearMotor *mRearMotor;
	static ArmControl *mArmControl;
	static EV3ColorSensor *mEV3ColorSensor;
	static CalcCurrentLocation *mCalcCurrentLocation;
	static Action *mLineTraceAction;
	static Action *mRunStraightAction;
	static Action *mCurvatureRunAction;
};

#endif
