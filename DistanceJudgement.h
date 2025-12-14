#ifndef DISTANCE_JUDGEMENT_H
#define DISTANCE_JUDGEMENT_H

#include "CalcCurrentLocation.h"

class DistanceJudgement{
public:
	explicit DistanceJudgement(CalcCurrentLocation* calcCurrentLocation);
	virtual ~DistanceJudgement();

	void setDistance(double distance);
	void start();
	void stop();
	bool isDistanceOut() const;
	bool isDistanceOutBack() const;
	bool isStarted() const;

private:
	CalcCurrentLocation* mCalcCurrentLocation;
	double mDistance = 0.0;
	double mStart = 0.0;
	double mTarget = 0.0;
};

#endif
