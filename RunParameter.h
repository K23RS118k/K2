#ifndef RUN_PARAMETER_H
#define RUN_PARAMETER_H

class RunParameter
{
  public:
	/**
	* 各パラメータへのセッター・ゲッター
	*/
	void setWhite(int w); //白の輝度値
	int getWhite();
	void setBlack(int b); //黒の輝度値
	int getBlack();
	void setTargetBrightness(double b); //目標輝度値
	double getTargetBrightness();
	void setLineTraceSpeed(int s); //ライントレースの速度
	int getLineTraceSpeed();
	void setKP(double kp); //KP
	double getKP();
	void setKI(double ki); //KI
	double getKI();
	void setKD(double kd); //KD
	double getKD();
	void setRunRightEdgeFlag(bool flag); //走行エッジ true:右, false:左
	void setRunLeftEdgeFlag(bool flag);
	bool getRunRighEdgeFlag();
	bool getRunlefEdgeFlag();
	void setChangeSpeedFlag(bool flag); //速度調整クラス true:有効, false:無効
	bool getChangeSpeedFlag();
	void setArmAngle(double a); //アームの角度
	double getArmAngle();
	void setArmPower(int p); //アーム動作速度
	int getArmPower();
	void setForwardSpeed(int s); //直進するための速度
	int getForwardSpeed();
	void setAngleResetFlag(bool flag); //直進走行時の角度初期化フラグ
	bool getAngleResetFlag();
	void setStraightAngle(double a); //直進走行時の目標角度
	double getStraightAngle();
	void setTargetAngleFlag(bool flag); //直進走行時の角度初期化フラグ
	bool getTargetAngleFlag();
	void setRotateSpeed(int s); //回転速度
	int getRotateSpeed();
	void setRotateAngle(double a); //回転角度
	double getRotateAngle();
	void setDecelerationRotaryAngle(double a); //減速式回転の角度設定
	double getDecelerationRotaryAngle();
	void setDecelerationRotarySpeed(double s); //減速式回転の回転速度設定
	double getDecelerationRotarySpeed();
	void setUturnApporoachFlag(bool flag); //減速式回転の回転速度と角度をを固定する　true:固定する（180度、25）, flase:設定できる
	double getUturnApporoachFlag();
	void setForwardTurnFlag(bool flag); //ブロックを押しながら旋回する
	bool getForwardTurnFlag();
	void setDetectTurnMode(bool m); //黒線検知旋回モード
	bool getDetectTurnMode();
	void setDetectTurnDirection(bool d); //黒線検知旋回方向
	bool getDetectTurnDirection();
	void setCurvatureRunSpeed(int s); //曲率走行速度
	int getCurvatureRunSpeed();
	void setCurvatureRadias(double r); //曲率半径
	double getCurvatureRadias();
	/* ブロック初期位置	*/
	int getOneBlock();
	void setOneBlock(int g);
	int getTwoBlock();
	void setTwoBlock(int b);
	int getThreeBlock();
	void setThreeBlock(int r);
	int getFourBlock();
	void setFourBlock(int y);
	int getPowerBlock();
	void setPowerBlock(int b);

  private:
	/**
	* 走行パラメータ
	*/
	int white;						 //白の輝度値
	int black;						 //黒の輝度値
	double target_brightness;		 //ライントレースの目標輝度値
	int line_trace_speed;			 //ライントレースの速度
	double kp_gain;					 //KPゲイン
	double ki_gain;					 //KIゲイン
	double kd_gain;					 //KDゲイン
	bool run_right_edge_flag;		 //右エッジ走行フラグ
	bool run_left_edge_flag;		 //左エッジ走行フラグ
	bool change_speed_flag;			 //速度調整有効フラグ
	int arm_angle;					 //アームの角度
	int arm_power;					 //アーム動作速度
	int forward_speed;				 //直進するための速度
	bool reset_straight_angle;		 //直進走行時の角初期化フラグ
	double straight_target_angle;	//直進走行時の目標角度
	bool set_target_angle;			 //直進走行角度が設定された
	int rotate_speed;				 //回転速度
	int rotate_angel;				 //回転角度
	int deceleration_rotary_angle;   //減速式回転の角度
	int deceleration_rotary_speed;   //減速式回転の回転速度
	bool uturn_apporoach_flag;		 //減速式回転の回転速度と角度をを固定する
	bool forward_turn_flag;			 //ブロック運搬時回転
	bool line_detect_turn_mode;		 //黒線検知旋回モード
	bool line_detect_turn_direction; //黒線検知旋回方向
	int curvature_run_speed;		 //曲率走行速度
	double curvature_radias;		 //曲率半径
	int one_position;				 //緑ブロックの位置
	int two_position;				 //黒ブロックの位置
	int three_position;				 //赤ブロックの位置
	int four_position;				 //黄ブロックの位置
	int power_position;				 //青ブロックの位置
									 /*	int red_position;					//赤ブロックの位置
	int green_position;					//緑ブロックの位置
	int yellow_position;				//黄ブロックの位置
	int blue_position;					//青ブロックの位置
	int black_position;					//黒ブロックの位置*/
};

#endif
