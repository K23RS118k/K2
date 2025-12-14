#ifndef TIMER_JUDGEMENT_H
#define TIMER_JUDGEMENT_H

#include "Clock.h"

class TimerJudgement {
public:
	explicit TimerJudgement(const ev3api::Clock& clock);
	virtual ~TimerJudgement();

	void setTime(uint32_t delay);
	void start();
	void stop();
	bool isTimedOut() const;
	bool isStarted() const;

private:
	const ev3api::Clock& mClock;
	uint32_t mDelayMs;
	uint32_t mStartTime;
	uint32_t mTargetTime;
};

#endif
