#include "Action.h"

/**
* オブジェクトのポインタ変数を初期化
*/
RearMotor *Action::mRearMotor;
ArmControl *Action::mArmControl;
RunParameter *Action::mRunParameter;
EV3ColorSensor *Action::mEV3ColorSensor;
CalcCurrentLocation *Action::mCalcCurrentLocation;
Action *Action::mLineTraceAction;
Action *Action::mRunStraightAction;
Action *Action::mCurvatureRunAction;

Action::~Action()
{
}

/**
* 使用するクラスの参照を設定する
*/
void Action::setObject(
	RunParameter *runParameter,
	RearMotor *rearMotor,
	ArmControl *armControl,
	EV3ColorSensor *ev3ColorSensor,
	CalcCurrentLocation *calcCurrentLocation,
	Action *lineTraceAction,
	Action *runStraightAction,
	Action *curvatureRunAction)
{
	mRunParameter = runParameter;
	mRearMotor = rearMotor;
	mArmControl = armControl;
	mEV3ColorSensor = ev3ColorSensor;
	mCalcCurrentLocation = calcCurrentLocation;
	mLineTraceAction = lineTraceAction;
	mRunStraightAction = runStraightAction;
	mCurvatureRunAction = curvatureRunAction;
}
