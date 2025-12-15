#include "Tactics.h"

/**
* オブジェクトのポインタ変数を初期化
*/
EV3ColorSensor *Tactics::mEV3ColorSensor;

RunParameter *Tactics::mRunParameter;
CalcCurrentLocation *Tactics::mCalcCurrentLocation;

DistanceJudgement *Tactics::mDistanceJudgement;
TimerJudgement *Tactics::mTimerJudgement;
StraightDetection *Tactics::mStraightDetection;

ArmPositionSetAction *Tactics::mArmPositionSetAction;
LineTraceAction *Tactics::mLineTraceAction;
EdgeChangeAction *Tactics::mEdgeChangeAction;
RunStraightAction *Tactics::mRunStraightAction;
RotateMachineAction *Tactics::mRotateMachineAction;
CurvatureRunAction *Tactics::mCurvatureRunAction;
LineDetectTurnAction *Tactics::mLineDetectTurnAction;
DecelerationRotaryAction *Tactics::mDecelerationRotaryAction;
FaceDisplay *Tactics::mFaceDisplay;
Music *Tactics::mMusic;

Tactics::~Tactics()
{
}

/**
* 使用するクラスの参照を設定する
*/
void Tactics::setObject(
	EV3ColorSensor *ev3ColorSensor,
	RunParameter *runParameter,
	CalcCurrentLocation *calcCurrentLocation,
	DistanceJudgement *distanceJudgement,
	TimerJudgement *timerJudgement,
	StraightDetection *straightDetection,
	ArmPositionSetAction *armPositionSetAction,
	LineTraceAction *lineTraceAction,
	EdgeChangeAction *edgeChangeAction,
	RunStraightAction *runStraightAction,
	RotateMachineAction *rotateMachineAction,
	CurvatureRunAction *curvatureRunAction,
	//LineDetectTurnAction *lineDetectTurnAction,
	DecelerationRotaryAction *decelerationRotaryAction,
	FaceDisplay *facedisplay,
	Music *music)
{
	mEV3ColorSensor = ev3ColorSensor;
	mRunParameter = runParameter;
	mCalcCurrentLocation = calcCurrentLocation;
	mDistanceJudgement = distanceJudgement;
	mTimerJudgement = timerJudgement;
	mStraightDetection = straightDetection;
	mArmPositionSetAction = armPositionSetAction;
	mLineTraceAction = lineTraceAction;
	mEdgeChangeAction = edgeChangeAction;
	mRunStraightAction = runStraightAction;
	mRotateMachineAction = rotateMachineAction;
	mCurvatureRunAction = curvatureRunAction;
	//mLineDetectTurnAction = lineDetectTurnAction;
	mDecelerationRotaryAction = decelerationRotaryAction;
	mFaceDisplay = facedisplay;
	mMusic = music;
}
