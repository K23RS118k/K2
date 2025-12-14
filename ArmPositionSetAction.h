#ifndef SET_ARM_POSITION_ACTION_H
#define SET_ARM_POSITION_ACTION_H

/* 定数の定義 */
#define ARM_LIMIT_MAX 90
#define ARM_LIMIT_MIN -20

#include "Action.h"

class ArmPositionSetAction : public Action
{
public:
	explicit ArmPositionSetAction();
	virtual ~ArmPositionSetAction();

	void start();						//動作開始
	void stop();						//動作終了
	void updateParameter(); //パラメータ更新
	bool isFinished();			//動作終了判定
	void reset();						//エンコーダ値リセット

private:
	int state = 0;
	bool flag = false;						//動作完了フラグ
	int targetAngle;							//目標角度
	int angleDiff[2] = {0, 0};		//目標角度と現在の角度の偏差
	int count = 0;								//タイムアウト処理用カウンタ
	const int default_power = 20; //初期アーム動作速度
	int power = 20;								//アーム動作速度
};

#endif
