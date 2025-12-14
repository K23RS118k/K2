#ifndef EDGE_CHANGE_ACTION_H
#define EDGE_CHANGE_ACTION_H

#include "Action.h"

class EdgeChangeAction : public Action{
public:
	explicit EdgeChangeAction();
	virtual ~EdgeChangeAction();

	void start();			//動作開始
	void stop();			//動作停止
	void updateParameter();	//パラメータ更新
	bool isFinished();		//動作終了判定

private:
	int state = 0;
	bool flag = false;	//動作完了フラグ(true:動作完了, false:動作中)
	bool edge;			//現在の走行エッジ
	double distance;	//距離
	int count = 0;		//時間計測用カウンタ
	double target;		//目標輝度値

};

#endif
