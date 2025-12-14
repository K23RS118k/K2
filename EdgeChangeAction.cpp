#include "EdgeChangeAction.h"

EdgeChangeAction::EdgeChangeAction(){
}
EdgeChangeAction::~EdgeChangeAction(){
}

/**
* 動作開始
*/
void EdgeChangeAction::start(){
	switch (state) {
		case 0:
			updateParameter();	//パラメータを更新(念のため)
			distance = mCalcCurrentLocation -> getDistance();	//現在の走行距離を取得
			state++;
			break;
		case 1:
			mLineTraceAction -> start();	//ライントレース開始
			if(mCalcCurrentLocation -> getDistance() - distance > 10){//一定距離ライントレース
				ev3_speaker_play_tone(NOTE_C5, 100);
				mLineTraceAction -> stop();
				count = 0;
				state++;
			}
			break;
		case 2:
			if(edge){//右エッジ
				mRearMotor -> setSpeed(10, 20);
			}
			else{//左エッジ
				mRearMotor -> setSpeed(20, 10);
			}
			if(count > 25){//0.1秒経過
				ev3_speaker_play_tone(NOTE_D5, 100);
				state++;
			}
			count++;
			break;
		case 3:
			//TODO 条件文は要検討
			if(mEV3ColorSensor -> getBrightness() < target){//黒を見たら
				ev3_speaker_play_tone(NOTE_C5, 100);
				count = 0;
				state++;
			}
			break;

		case 4:
			if(mEV3ColorSensor -> getBrightness() > target){//白を見たら
				ev3_speaker_play_tone(NOTE_C5, 100);
				mRunParameter -> setRunRightEdgeFlag(!edge);	//エッジを切替
				mLineTraceAction -> updateParameter();			//ライントレースのパラメータ更新
				distance = mCalcCurrentLocation -> getDistance();	//現在の距離を取得
				state++;
			}
			break;
		case 5:
			if(edge){//右エッジ
				mRearMotor -> setSpeed(20, 0);
			}
			else{//左エッジ
				mRearMotor -> setSpeed(0, 20);
			}
			if(count < 25){//0.1秒経過
				ev3_speaker_play_tone(NOTE_D5, 100);
				state++;
			}
			count++;
			break;
		case 6:
			mLineTraceAction -> start();
			if(mCalcCurrentLocation -> getDistance() - distance > 20){//一定距離ライントレース
				mLineTraceAction -> stop();
				ev3_speaker_play_tone(NOTE_D5, 100);
				state++;
			}
			break;
		case 7:
			flag = true;	//動作完了フラグを立てる
			break;
	}
}
/**
* 動作終了
*/
void EdgeChangeAction::stop(){
	flag = false;
	state = 0;
	count = 0;
	mRearMotor -> setSpeed(0, 0);

}
/**
* パラメータ更新
*/
void EdgeChangeAction::updateParameter(){
	target = mRunParameter -> getTargetBrightness();
	edge = mRunParameter -> getRunRighEdgeFlag();
	/* ライントレース用パラメータの設定 */
	mRunParameter -> setLineTraceSpeed(20);
	mRunParameter -> setKP(0.3);
	mRunParameter -> setKI(0);
	mRunParameter -> setKD(0);
}

/**
* 動作終了判定
*/
bool EdgeChangeAction::isFinished(){
	return flag;
}
