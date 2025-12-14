#include "BlockLineUpSearch.h"
#include "BluetoothLogger.h"

BlockLineUpSearch::BlockLineUpSearch(BlockLineUpBehavior *blockLineUpBehavior, RearMotor *rearMotor, IPCommunication *iPCommunication)
	: mBlockLineUpBehavior(blockLineUpBehavior), mrearMotor(rearMotor), mIPCommunication(iPCommunication)
{
}

BlockLineUpSearch::~BlockLineUpSearch()
{
}
void BlockLineUpSearch::execute()
{
	switch (state)
	{
		/*
	case 0:
		setColor(mIPCommunication->getNo1(), 0);
		setColor(mIPCommunication->getNo2(), 1);
		setColor(mIPCommunication->getNo3(), 2);
		setColor(mIPCommunication->getNo4(), 3);
		setColor(mIPCommunication->getNo5(), 4);
		setColor(mIPCommunication->getNo6(), 5);
		setColor(mIPCommunication->getNo7(), 6);
		setColor(mIPCommunication->getNo8(), 7);
		cameraNum = mIPCommunication->getNumber();
		if (cameraNum > 8 || cameraNum < 0)
		{
			cameraFlag = false;
		}
		cameraCircle[0] = mIPCommunication->getColorSircle();
		cameraCircle[1] = mIPCommunication->getBlackBlock();
		if (cameraCircle[0] == 9 || cameraCircle[1] == 9)
		{
			cameraFlag = false;
		}
		if (cameraFlag)
		{
			state = 0;
		}
		else
		{
			state++;
		}
		state++;
		break;
	case 1:
		routeTactics(blockColor);
		if (tacticsFlag)
		{
			state++;
		}
		break;
	case 2:
		routeTactics(blockColor);
		if (tacticsFlag)
		{
			state++;
		}
		break;
	case 3:
		if (blockCount == 0)
		{
			state = 10;
		}
		else if (mBlockLineUpBehavior->Apporoach())
		{
			blockColor = mBlockLineUpBehavior->ColorBlockReturn();
			state--;
		}
		break;
	case 5:
		fullSearch(0);
		start = 11;
		if (tacticsList[0][1] == 0)
		{
			state = 1;
		}
		else
		{
			state++;
		}
		break;
	case 6:
		fullRouteTactics();
		if (tacticsFlag)
		{
			state = 10;
		}
		break;
		*/
	case 10: // ライントレース
		// ライントレース走行パラメータを設定する
		mBlockLineUpBehavior->getSwitchingParameter_LOW();
		mRunParameter->setLineTraceSpeed(40);
		mRunParameter->setKP(0.5);
		mRunParameter->setKI(0);
		mRunParameter->setKD(5.0);
		mRunParameter->setRunRightEdgeFlag(true);
		mLineTraceAction->updateParameter();
		mDistanceJudgement->setDistance(40);
		mDistanceJudgement->start();
		//straightAngle = mCalcCurrentLocation->getFinalAngle(); // 角度の保持
		state++;
		break;
		
	case 11:
		mLineTraceAction->start();
		//mLineTraceAction->getSpeed();
		if (mDistanceJudgement->isDistanceOut() == true) //置き場検知
		//if (mBlockLineUpBehavior->CircleDetection())
		{
			mLineTraceAction->stop();
			mDistanceJudgement->stop();
			ev3_speaker_play_tone(NOTE_E5, 100);
			state++;

		}
		
		break;
		

    case 12:
	    mRunParameter->setArmAngle(30);
		mRunParameter->setArmPower(30);
		mArmPositionSetAction->updateParameter();
		state++;		
		
		break;

	case 13:
	    mArmPositionSetAction->start();
		if(mArmPositionSetAction->isFinished())
		{
			mArmPositionSetAction->stop();
			state++;
		}
		break;
		

	case 14:
        mRunParameter->setForwardSpeed(43);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(155);
        mDistanceJudgement->start();
        state++;
        break;
    case 15:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            state++;    
        }
    	
        break;

	case 16:
	mRunParameter->setRotateAngle(95); //45°回転(左回転)
		mRunParameter->setRotateSpeed(10);  //回転時のスピード
		mRotateMachineAction->updateParameter();
		state++;
		/*if(mDistanceJudgement->getDistance() < 10){
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
		}*/

	break;
	
	case 17:
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			mRunStraightAction->stop();
			state++;
		}
		break;

	case 18:
		
        mRunParameter->setForwardSpeed(27);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(40);
        mDistanceJudgement->start();
        state++;
        break;
    case 19:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
          state++;  
        }

		break;

	case 20:
		mRunParameter->setRotateAngle(40);//45°回転(左回転)
		mRunParameter->setRotateSpeed(13);  //回転時のスピード
		mRotateMachineAction->updateParameter();
		state++;
		/*if(mDistanceJudgement->getDistance() < 10){
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
		}*/

	break;
	
	case 21:
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			mRunStraightAction->stop();
			state++;
		}
		break;


	case 22:
		
        mRunParameter->setForwardSpeed(40);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(97);
        mDistanceJudgement->start();
        state++;
        break;
    case 23:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            
        }

	break;
	/*case 14:
		
        mRunParameter->setForwardSpeed(65);
        mRunStraightAction->updateParameter();
        mDistanceJudgement->setDistance(50);
        mDistanceJudgement->start();
        state++;
        break;
    case 15:
        mRunStraightAction->start();
        if (mDistanceJudgement->isDistanceOut())
        {
            mRunStraightAction->stop();
            mDistanceJudgement->stop();
            
        }
        
        
    
		mRunParameter->setRotateAngle(13); //45°回転(左回転)
		mRunParameter->setRotateSpeed(15);  //回転時のスピード
		mRotateMachineAction->updateParameter();
		//state++;
		/*if(mDistanceJudgement->getDistance() < 10){
			mRunStraightAction->stop();
			mDistanceJudgement->stop();
		}*/

	//break;
	
	/*case 16:
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			//旋回停止
			mRotateMachineAction->stop();
			state++;
		}
		break;

	//case 14:
		mrearMotor->setSpeed(30,30);
		state++;
		break;


	//case 15:
		
	if (mEV3ColorSensor->getBlockFloorColor() == BLACK)
        {
			mrearMotor->setSpeed(0,0);
			state++;
		}
		break;*/
	/*case 16:
		mRunParameter->setRotateAngle(77); //45°回転(左回転)
		mRunParameter->setRotateSpeed(15);  //回転時のスピード
		mRotateMachineAction->updateParameter();
		
		state++;
		break;

	case 17:
		mRotateMachineAction->start();
		if (mRotateMachineAction->isFinished())
		{
			mRotateMachineAction->stop();//旋回停止
			mrearMotor->setSpeed(0,0);
			//state++;
			
		}
		break;

	case 18:
		mLineTraceAction->start();
		count++;
		if (count>=5000/4)
			{
			mLineTraceAction->stop();
			mRunStraightAction->straight();
			count=0;
			state++;
			}
		break;

	case 19:
		count++;
		if(count>=4200/4){
			mRunStraightAction->stop();
			state++;
		}
		*/
	}
	

	
	
}
void BlockLineUpSearch::setColor(int camera, int n)
{
	if (camera == 1)
	{
		cameraColor[n] = 1;
	}
	else if (camera == 2)
	{
		cameraColor[n] = 0;
	}
	else if (camera == 3)
	{
		cameraColor[n] = 3;
	}
	else if (camera == 4)
	{
		cameraColor[n] = 2;
	}
	else if (camera == 5)
	{
		cameraColor[n] = 4;
	}
	else
	{
		cameraFlag = false;
	}
}
void BlockLineUpSearch::routeAction(bool colorCircle)
{
	int motion;
	actionFlag = false;
	if (order != 0)
	{
		motion = direction - action[fRoute[order]][fRoute[order - 1]];
		if (motion == 0)
		{
			//printf("直進");
			if (mBlockLineUpBehavior->Straight_X_axis(avoidance))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
				direction = action[fRoute[order]][fRoute[order - 1]];
			}
		}
		else if (motion == 1 || motion == -3)
		{
			//printf("左折");
			if (mBlockLineUpBehavior->TurnLeft_X_axis(avoidance))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
				direction = action[fRoute[order]][fRoute[order - 1]];
			}
		}
		else if (motion == 2 || motion == -2)
		{
			//printf("Uターン");
			if (mBlockLineUpBehavior->Uturn())
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
				direction = action[fRoute[order]][fRoute[order - 1]];
			}
		}
		else if (motion == -1 || motion == 3)
		{
			//printf("右折");
			if (mBlockLineUpBehavior->TurnRight_X_axis(avoidance))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
				direction = action[fRoute[order]][fRoute[order - 1]];
			}
		}
		if (colorCircle && block[fRoute[order - 1]] == 1)
		{
			//printf("回避");
			avoidance = true;
		}
		else
		{
			avoidance = false;
		}
	}
	if (colorCircle && order == 0)
	{
		motion = direction - colorAction;
		if (motion == 1)
		{
			//printf("右後ろ設置\n");
			if (mBlockLineUpBehavior->BlockPut_X_Right(false))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
			}
		}
		else if (motion == 0 || motion == 4)
		{
			//printf("左後ろ設置\n");
			if (mBlockLineUpBehavior->BlockPut_X_left(false))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
			}
		}
		else if (motion == -1 || motion == 3)
		{
			//printf("左前設置\n");
			if (mBlockLineUpBehavior->BlockPut_Y_Diagonal(false))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
			}
		}
		else if (motion == -2 || motion == 2)
		{
			//printf("右前設置\n");
			if (mBlockLineUpBehavior->BlockPut_X_Diagonal(false))
			{
				mrearMotor->setSpeed(0, 0);
				actionFlag = true;
			}
		}
	}
	else if (!colorCircle && order == 0)
	{
		actionFlag = true;
	}
}
void BlockLineUpSearch::routeSearch(int goal, bool colorCircle, bool blockState)
{
	int sRoute[16], pRoute[16], sDist[4], pDist[16], sPoint, newDist, i, j;
	//変数の初期化
	for (i = 0; i < dN; i++)
	{
		sRoute[i] = -1;
		fRoute[i] = -1;
		if (i < iN)
		{
			sDist[i] = 1000;
		}
		pDist[i] = 1000;
		pFixed[i] = false;
	}
	fDist = 1000;
	//ダイクストラ法
	pDist[start] = 0;
	while (true)
	{
		i = 0;
		while (i < dN)
		{
			if (pFixed[i] == false)
			{
				break;
			}
			i += 1;
		}
		if (i == dN)
		{
			break;
		}
		for (j = i + 1; j < dN; j++)
		{
			if ((pFixed[j] == false) && (pDist[j] < pDist[i]))
			{
				i = j;
			}
		}
		sPoint = i;
		pFixed[sPoint] = true;
		for (j = 0; j < dN; j++)
		{
			if ((distance[sPoint][j] > 0) && (pFixed[j] == false))
			{
				newDist = pDist[sPoint] + distance[sPoint][j];
				if (newDist < pDist[j])
				{
					pDist[j] = newDist;
					pRoute[j] = sPoint;
				}
			}
		}
	}
	//行先によって最短距離の求め方を分ける
	order = 0;
	if (colorCircle)
	{ //カラーサークルの場合
		for (int k = 0; k < iN; k++)
		{
			i = point[goal][k];
			sDist[k] = pDist[i];
			j = 0;
			while (i != start)
			{
				sRoute[j] = i;
				i = pRoute[i];
				j = j + 1;
			}
			sRoute[j] = start;
			if (fDist > sDist[k] /*&& (block[sRoute[0]] == 0 || j == 0)*/)
			{
				fDist = sDist[k];
				order = j;
				colorAction = k;
				for (int l = 0; l < dN; l++)
				{
					fRoute[l] = sRoute[l];
				}
			}
		}
	}
	else
	{ //交点サークルの場合
		colorAction = -1;
		i = goal;
		fDist = pDist[i];
		j = 0;
		while (i != start)
		{
			fRoute[j] = i;
			i = pRoute[i];
			j = j + 1;
		}
		fRoute[j] = start;
		order = j;
	}
	//設定距離の更新
	start = fRoute[0];
	if (blockState && colorCircle)
	{ //ブロックをカラーサークルへ
		for (int k = 0; k < dN; k++)
		{
			if (distance[k][fRoute[order]] == 10)
			{
				distance[k][fRoute[order]] = 1;
			}
		}
		block[fRoute[order]] = 0;
	}
	else if (blockState && !colorCircle)
	{ //ブロックを交点サークルへ
		for (int k = 0; k < dN; k++)
		{
			if (distance[k][fRoute[order]] == 10)
			{
				distance[k][fRoute[order]] = 1;
			}
			else if (distance[k][fRoute[0]] == 1)
			{
				distance[k][fRoute[0]] = 10;
			}
		}
	}
}
int BlockLineUpSearch::getDirection()
{
	return direction;
}
int BlockLineUpSearch::compareColorCircle(int s, int color)
{ //現在地から近いカラーサークルを探す
	int a, b;
	int result = -1;
	a = 1000;
	b = 1000;
	if (color == 4 || (circle[color][0] == -1 && circle[color][1] == -1))
	{ //黒または同色を3回以上検知した
		for (int i = 0; i < cN; i++)
		{ //現在地から近いカラーサークルを探す
			for (int j = 0; j < iN; j++)
			{
				if (point[i][j] == s)
				{
					result = i;
				}
			}
		}
	}
	else
	{
		if (circle[color][0] > 0)
		{
			routeSearch(circle[color][0], true, false);
			a = fDist;
			start = s;
		}
		if (circle[color][1] > 0)
		{
			routeSearch(circle[color][1], true, false);
			b = fDist;
			start = s;
		}
		if (a <= b)
		{
			if (a >= 10)
			{
				result = cCircle[color][1];
			}
			else
			{
				result = circle[color][0];
				circle[color][0] = -1;
			}
		}
		else
		{
			if (b >= 10)
			{
				result = cCircle[color][0];
			}
			else
			{
				result = circle[color][1];
				circle[color][1] = -1;
			}
		}
	}
	return result;
}
int BlockLineUpSearch::compareIntersectionCircle(int s)
{ //現在地から近いブロックを探す
	int a, b, result;
	a = 1000;
	result = 1000;
	start = s;
	for (int i = 0; i < dN; i++)
	{
		if (block[i] == 1 && i != 7)
		{
			routeSearch(i, false, false);
			b = fDist;
			start = s;
			if (a > b)
			{
				a = b;
				result = i;
			}
		}
	}
	return result;
}
int BlockLineUpSearch::bingoCheck()
{ //ビンゴの数を判定する
	int bingoCount = 0;
	int result = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (bingoFlag[bingoLine[i][j]])
			{
				bingoCount++;
			}
		}
		if (bingoCount == 3)
		{
			result++;
		}
		bingoCount = 0;
	}
	/*printf("%d%d%d\n", bingoFlag[bingoLine[0][0]], bingoFlag[bingoLine[0][1]], bingoFlag[bingoLine[0][2]]);
		printf("%d %d\n", bingoFlag[bingoLine[1][1]], bingoFlag[bingoLine[3][1]]);
		printf("%d%d%d\n", bingoFlag[bingoLine[2][0]], bingoFlag[bingoLine[2][1]], bingoFlag[bingoLine[2][2]]);
		printf("\n");*/
	return result;
}

void BlockLineUpSearch::routeTactics(int color)
{
	tacticsFlag = false;
	switch (tactics_state)
	{
		//Rコースは全て
	case 0:
		routeSearch(10, false, false);
		tactics_state++;
		break;
	case 1:
		routeAction(false);
		if (actionFlag)
		{
			if (order == 0)
			{
				tactics_state = 6;
			}
			order--;
		}
		break;
		//Lコースはここから
	case 2:
		blockCount--;
		goal = compareColorCircle(start, color);
		routeSearch(goal, true, true);
		tactics_state++;
		break;
	case 3:
		routeAction(true);
		if (actionFlag)
		{
			if (order == 0)
			{
				goal = compareIntersectionCircle(start);
				tactics_state++;
			}
			order--;
		}
		break;
	case 4:
		if (blockCount == 0 || goal == -1)
		{
			routeSearch(4, false, false); //L = 11, R = 4
		}
		else
		{
			routeSearch(goal, false, false);
		}
		tactics_state++;
		break;
	case 5:
		routeAction(false);
		if (actionFlag)
		{
			if (order == 0)
			{
				tactics_state++;
			}
			order--;
		}
		break;
	case 6:
		tacticsFlag = true;
		tactics_state = 2;
		break;
	}
}
void BlockLineUpSearch::fullSearch(int count)
{
	int targetCost = 60;
	int cost = 0;
	int goalCost = 0;
	int moveCount = 1000;
	bingoFlag[cameraCircle[0]] = true;
	//if (count == 0) {	//進入場所のブロック対応　Lのみ
	//	stack[0][0] = start;
	//	goal = compareColorCircle(8, cameraColor[4]);
	//	routeSearch(goal, true, true);
	//	stack[0][1] = start;
	//	stackCircle[0] = goal;
	//	bingoFlag[stackCircle[0]] = true;	//設置済み判定
	//	stackDist[0] = fDist;
	//	fullSearchL(count + 1);
	//}
	//else {
	for (int i = 0; i < 8; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			cost = 0;
			goalCost = 0;
			if (block[startCircle[i]] == 1)
			{ //ブロックが交点サークルに設置されている
				//交点サークルへ
				routeSearch(startCircle[i], false, false);
				stack[count][0] = start;
				stackDist[count] = fDist; //部分距離
				if (fDist > 20)
				{
					//初期化
					if (count != 0)
					{
						start = stack[count - 1][1];
					}
					else
					{
						start = 11;
					}
					stack[count][0] = -1; //順序
					break;
				}
				//カラーサークルへ
				if (cameraColor[i] == 4)
				{ //黒のとき
					goal = cameraNum;
					routeSearch(goal, true, true);
					//printf("%d: %d - black \n", count, goal);
				}
				else
				{
					for (int j = 0; j < 4; j++)
					{ //運搬先を探す
						if (j == cameraColor[i])
						{
							goal = cCircle[j][k];
							break;
						}
					}
					if (bingoFlag[goal])
					{
						//初期化
						block[stack[count][0]] = 1; //ブロック位置
						for (int l = 0; l < dN; l++)
						{
							if (distance[l][stack[count][0]] == 1)
							{ //距離設定
								distance[l][stack[count][0]] = 10;
							}
						}
						if (count != 0)
						{
							start = stack[count - 1][1];
						}
						else
						{
							start = 11;
						}
						stack[count][0] = -1; //順序
						stack[count][1] = -1;
						stackCircle[count] = -1;
						continue;
					}
					routeSearch(goal, true, true);
					bingoFlag[goal] = true;
					//printf("%d: %d - true \n", count, goal);
				}
				if (fDist > 10)
				{
					//初期化
					block[stack[count][0]] = 1; //ブロック位置
					if (cameraColor[i] != 4)
					{
						bingoFlag[goal] = false;
						//printf("%d: %d - false1 \n", count, goal);
					}
					for (int l = 0; l < dN; l++)
					{
						if (distance[l][stack[count][0]] == 1)
						{ //距離設定
							distance[l][stack[count][0]] = 10;
						}
					}
					if (count != 0)
					{
						start = stack[count - 1][1];
					}
					else
					{
						start = 11;
					}
					stack[count][0] = -1; //順序
					stackCircle[count] = -1;
					continue;
				}
				stack[count][1] = start;
				stackCircle[count] = goal;
				stackDist[count] += fDist; //部分距離更新
				for (int j = 0; j <= count; j++)
				{ //現時点での距離
					cost += stackDist[j];
					//printf("%d -> %d -> ", stack[j][0], stack[j][1]);
				}
				//printf(" \ncost:%d\n", cost);
				//再帰判定
				routeSearch(11, false, false); //L
				//routeSearch(4, false, false);	//R
				goalCost = fDist + cost;
				//printf("cost:%d\n", (cost + goalCost));
				if (bingoCheck() >= 2 && (moveCount > count))
				{
					if ((fullCost > goalCost) && (goalCost <= targetCost))
					{
						//printf("ダブルビンゴ以上達成！＆最短更新！\n");
						for (int j = 0; j < 8; j++)
						{
							fullCost = goalCost;
							tacticsList[j][0] = stack[j][0];
							tacticsList[j][1] = stackCircle[j];
						}
						moveCount = count;
					}
				}
				else if (goalCost <= targetCost)
				{
					fullSearch(count + 1);
				}
				//初期化
				block[stack[count][0]] = 1; //ブロック位置
				if (cameraColor[i] != 4)
				{
					bingoFlag[stackCircle[count]] = false;
					//printf("%d: %d - false2 \n", count, goal);
				}
				for (int l = 0; l < dN; l++)
				{
					if (distance[l][stack[count][0]] == 1)
					{ //距離設定
						distance[l][stack[count][0]] = 10;
					}
				}
				if (count != 0)
				{
					start = stack[count - 1][1];
				}
				else
				{
					start = 11;
				}
				stack[count][0] = -1; //順序
				stack[count][1] = -1;
				stackCircle[count] = -1;
				stackDist[count] = 0;
			}
		}
	}
}
void BlockLineUpSearch::fullRouteTactics()
{
	tacticsFlag = false;
	switch (tactics_state)
	{
		//Rコースは全て
	case 0:
		goal = tacticsList[tacticsCount][0];
		//printf("-----\n取得経路 -> %d\n", goal);
		routeSearch(goal, false, false);
		tactics_state++;
		break;
	case 1:
		routeAction(false);
		if (actionFlag)
		{
			if (order == 0)
			{
				//printf("\n\n");
				tactics_state = 2;
			}
			order--;
		}
		break;
		//Lコースはここから
	case 2:
		goal = tacticsList[tacticsCount][1];
		//printf("-----\n運搬経路 -> %d\n", goal);
		routeSearch(goal, true, true);
		tactics_state++;
		break;
	case 3:
		routeAction(true);
		if (actionFlag)
		{
			if (order == 0)
			{
				tacticsCount++;
				tactics_state++;
			}
			order--;
		}
		break;
	case 4:
		goal = tacticsList[tacticsCount][0];
		if (goal == -1 || tacticsCount == 8)
		{
			//printf("-----\nゴール経路\n");
			//routeSearch(11, false, false);	//L = 11, R = 4
			routeSearch(4, false, false);
			tactics_state = 6;
		}
		else
		{
			//printf("-----\n取得経路 -> %d\n", goal);
			routeSearch(goal, false, false);
			tactics_state++;
		}
		break;
	case 5:
		routeAction(false);
		if (actionFlag)
		{
			if (order == 0)
			{
				//printf("\n\n");
				tactics_state = 2;
			}
			order--;
		}
		break;
	case 6:
		routeAction(false);
		if (actionFlag)
		{
			if (order == 0)
			{
				//printf("\n\n");
				tactics_state++;
			}
			order--;
		}
		break;
	case 7:
		tacticsFlag = true;
		//tactics_state = 0;
		break;
	}
}

int BlockLineUpSearch::isFinished()
{
	return flag;
}