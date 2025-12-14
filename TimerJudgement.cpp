#include "TimerJudgement.h"

/**
* コンストラクタ
* @param clock クロック
*/
TimerJudgement::TimerJudgement(const ev3api::Clock &clock)
	: mClock(clock),
	  mDelayMs(0),
	  mStartTime(0),
	  mTargetTime(0)
{
}

/**
* デストラクタ
*/
TimerJudgement::~TimerJudgement()
{
}

/**
* タイマ満期時間設定
* @param delay 満期時間[ms]
*/
void TimerJudgement::setTime(uint32_t delay)
{
	mDelayMs = delay;
}

/**
* タイマ開始
*/
void TimerJudgement::start()
{
	mStartTime = mClock.now();
	mTargetTime = mStartTime + mDelayMs;
}

/**
* タイマ停止
*/
void TimerJudgement::stop()
{
	mStartTime = 0;
	mTargetTime = 0;
}

/**
* タイマ満期したか否か
* @retval true  タイマ満期した
* @retval false タイマ満期していない
*/
bool TimerJudgement::isTimedOut() const
{
    if (!isStarted()) {
        return false;
    }
    return (mClock.now() >= mTargetTime);
}


/**
* タイマ開始したか否か
* @retval true  タイマ開始した
* @retval false タイマ開始していない
*/
bool TimerJudgement::isStarted() const
{
	return (mStartTime == 0) ? false : true;
}
