#pragma once

#include "Tactics.h"
#include "ColorID.h"
#include "ColorSensor.h"
#include "RearMotor.h"
#include "IPCommunication.h"

#include "BlockLineUpBehavior.h"
#include "BluetoothLogger.h"

class BlockLineUpSearch : public Tactics
{
  public:
	explicit BlockLineUpSearch(BlockLineUpBehavior *blockLineUpBehavior, RearMotor *rearMotor, IPCommunication *iPCommunication);
	virtual ~BlockLineUpSearch();

	void execute();
	void routeAction(bool colorCircle);
	void routeSearch(int goal, bool colorCircle, bool blockState);
	void setStart(int s);
	int getDistance();
	int compareColorCircle(int s, int color);
	int compareIntersectionCircle(int s);
	void routeTactics(int color);
	int getDirection();
	int bingoCheck();
	void fullSearch(int count);
	void fullRouteTactics();
	void setColor(int camera, int n);

	int isFinished();

  private:
	BlockLineUpBehavior *mBlockLineUpBehavior;
	RearMotor *mrearMotor;
	IPCommunication *mIPCommunication;
	int dN = 16;																		   //交点サークル数
	int distance[16][16] = {															   //各交点サークル同士の距離設定
							{0, 1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //ブロックの設置されている交点サークルは10
							{10, 0, 10, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							{-1, 1, 0, 1, -1, -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
							{-1, -1, 10, 0, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1},

							{10, -1, -1, -1, 0, 10, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1},
							{-1, 1, -1, -1, 1, 0, 1, -1, -1, 1, -1, -1, -1, -1, -1, -1},
							{-1, -1, 10, -1, -1, 10, 0, 10, -1, -1, 10, -1, -1, -1, -1, -1},
							{-1, -1, -1, 1, -1, -1, 1, 0, -1, -1, -1, 1, -1, -1, -1, -1},

							{-1, -1, -1, -1, 1, -1, -1, -1, 0, 1, -1, -1, 1, -1, -1, -1},
							{-1, -1, -1, -1, -1, 10, -1, -1, 10, 0, 10, -1, -1, 10, -1, -1},
							{-1, -1, -1, -1, -1, -1, 1, -1, -1, 1, 0, 1, -1, -1, 1, -1},
							{-1, -1, -1, -1, -1, -1, -1, 10, -1, -1, 10, 0, -1, -1, -1, 10},

							{-1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, 0, 10, -1, -1},
							{-1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 1, 0, 1, -1},
							{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, 10, 0, 10},
							{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 1, 0}};
	int iN = 4; //四角1つに対する交点サークル数
	int cN = 9; //四角の数

	int point[9][4]{//[カラーサークル番号][4角の交点サークル番号]
					{5, 6, 10, 9},
					{0, 1, 5, 4},
					{1, 2, 6, 5},
					{2, 3, 7, 6},
					{4, 5, 9, 8}, //ブロックサークル番号0は真ん中の四角
					{6, 7, 11, 10},
					{8, 9, 13, 12},
					{9, 10, 14, 13},
					{10, 11, 15, 14}};
	int fRoute[16];  //経路
	int fDist;		 //距離
	bool pFixed[16]; //探索済判定
	int order;

	int action[16][16] = {																 //各交点サークルの位置の設定
						  {0, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //1：上　2：右　3：下　4：左
						  {4, 0, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
						  {-1, 4, 0, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1},
						  {-1, -1, 4, 0, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1},

						  {1, -1, -1, -1, 0, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1},
						  {-1, 1, -1, -1, 4, 0, 2, -1, -1, 3, -1, -1, -1, -1, -1, -1},
						  {-1, -1, 1, -1, -1, 4, 0, 2, -1, -1, 3, -1, -1, -1, -1, -1},
						  {-1, -1, -1, 1, -1, -1, 4, 0, -1, -1, -1, 3, -1, -1, -1, -1},

						  {-1, -1, -1, -1, 1, -1, -1, -1, 0, 2, -1, -1, 3, -1, -1, -1},
						  {-1, -1, -1, -1, -1, 1, -1, -1, 4, 0, 2, -1, -1, 3, -1, -1},
						  {-1, -1, -1, -1, -1, -1, 1, -1, -1, 4, 0, 2, -1, -1, 3, -1},
						  {-1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 4, 0, -1, -1, -1, 3},

						  {-1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, 0, 2, -1, -1},
						  {-1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 4, 0, 2, -1},
						  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 4, 0, 2},
						  {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, 4, 0}};
	int direction = 4; //現在の進行方向　初期値->L：2　R：4
	int colorAction = -1;
	bool avoidance = false;
	int block[16] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1}; //ブロック設置位置;
	int circle[9][2] = {{5, 6}, {1, 7}, {2, 8}, {3, 4}, {0, 0}};	  //[色][カラーサークル番号]　Rコース
	int cCircle[9][2] = {{5, 6}, {1, 7}, {2, 8}, {3, 4}, {0, 0}};	 //定数
	int blockCount = 5;												  //移動させるブロックの個数設定
	int blockColor = 5;

	int cameraColor[8]; //カメラで取得した情報
	int cameraCircle[2];
	int cameraNum;
	int startCircle[8] = {0, 2, 5, 7, 8, 10, 13, 15};					//初期ブロックが置かれている交点サークル番号
	int bingoLine[4][3] = {{1, 2, 3}, {1, 4, 6}, {6, 7, 8}, {3, 5, 8}}; //	ビンゴになるカラーサークルの組み合わせ
	bool bingoFlag[9] = {false, false, false, false, false, false, false, false, false};

	int stack[8][2] = {{-1, -1}, {-1, -1}, {-1 - 1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}; //動かすブロックの順序を保存
	int stackCircle[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
	int stackDist[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int tacticsList[8][2];
	int fullCost = 1000;
	int tacticsCount = 0;

	bool actionFlag = false;
	bool tacticsFlag = false;
	bool cameraFlag = true;
	bool flag = false;

	int tactics_state = 0; //L:2 R:0
	int state = 12;
	//int state = 5;	//カメラあり

	int start = 11; //探索開始の交点サークル番号　初期値->L：8　R：11
	int goal;		//探索目標の交点サークル番号

	int count=0;
};