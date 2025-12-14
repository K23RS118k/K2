#include "DistanceJudgement.h"

DistanceJudgement::DistanceJudgement(CalcCurrentLocation* calcCurrentLocation)
: mCalcCurrentLocation(calcCurrentLocation){
}

DistanceJudgement::~DistanceJudgement(){
}

void DistanceJudgement::setDistance(double distance){
	mDistance = distance;
}
void DistanceJudgement::start(){
	mStart = mCalcCurrentLocation -> getDistance();
	mTarget = mStart + mDistance;
}
void DistanceJudgement::stop(){
	mStart = 0;
	mTarget = 0;
}

bool DistanceJudgement::isDistanceOut() const {
	return (mCalcCurrentLocation -> getDistance() >= mTarget)? true : false;
}
bool DistanceJudgement::isDistanceOutBack() const {
	return (mCalcCurrentLocation -> getDistance() <= mTarget)? true : false;
}

bool DistanceJudgement::isStarted() const {
	return (mStart == 0)? false : true;
}
