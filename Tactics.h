#ifndef TACTICS_H
#define TACTICS_H

#include "assertion.h"

#include "EV3ColorSensor.h"

#include "RunParameter.h"
#include "CalcCurrentLocation.h"

#include "DistanceJudgement.h"
#include "TimerJudgement.h"
#include "StraightDetection.h"

#include "ArmPositionSetAction.h"
#include "LineTraceAction.h"
#include "EdgeChangeAction.h"
#include "RunStraightAction.h"
#include "RotateMachineAction.h"
#include "CurvatureRunAction.h"
#include "LineDetectTurnAction.h"
#include "DecelerationRotaryAction.h"
#include "FaceDisplay.h"
#include "Music.h"


class Tactics
{
  public:
	virtual ~Tactics();

	virtual void execute() = 0; //戦術を実行する

	/* 使用するクラスのオブジェクトへの参照を設定 */
	static void setObject(
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
		FaceDisplay *faceDisplay,
		Music *music);


  protected:
	/* 使用するクラスのオブジェクト */
	static EV3ColorSensor *mEV3ColorSensor;

	static RunParameter *mRunParameter;
	static CalcCurrentLocation *mCalcCurrentLocation;

	static DistanceJudgement *mDistanceJudgement;
	static TimerJudgement *mTimerJudgement;
	static StraightDetection *mStraightDetection;

	static ArmPositionSetAction *mArmPositionSetAction;
	static LineTraceAction *mLineTraceAction;
	static EdgeChangeAction *mEdgeChangeAction;
	static RunStraightAction *mRunStraightAction;
	static RotateMachineAction *mRotateMachineAction;
	static CurvatureRunAction *mCurvatureRunAction;
	static LineDetectTurnAction *mLineDetectTurnAction;
	static DecelerationRotaryAction *mDecelerationRotaryAction;
	static FaceDisplay *mFaceDisplay;
	static Music *mMusic;

	//状態
	int state;
};

#endif
