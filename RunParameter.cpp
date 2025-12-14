#include "RunParameter.h"

/* 白の輝度値 */
void RunParameter::setWhite(int w)
{
	white = w;
}
int RunParameter::getWhite()
{
	return white;
}
/* 黒の輝度値 */
void RunParameter::setBlack(int b)
{
	black = b;
}
int RunParameter::getBlack()
{
	return black;
}
/* 目標輝度値 */
void RunParameter::setTargetBrightness(double t)
{
	target_brightness = t;
}
double RunParameter::getTargetBrightness()
{
	return target_brightness;
}
/* ライントレースの速度 */
void RunParameter::setLineTraceSpeed(int s)
{
	line_trace_speed = s;
}
int RunParameter::getLineTraceSpeed()
{
	return line_trace_speed;
}
/* KP, KI, KD */
void RunParameter::setKP(double kp)
{
	kp_gain = kp;
}
double RunParameter::getKP()
{
	return kp_gain;
}
void RunParameter::setKI(double ki)
{
	ki_gain = ki;
}
double RunParameter::getKI()
{
	return ki_gain;
}
void RunParameter::setKD(double kd)
{
	kd_gain = kd;
}
double RunParameter::getKD()
{
	return kd_gain;
}
/* 右エッジ走行フラグ */
void RunParameter::setRunRightEdgeFlag(bool flag)
{
	run_right_edge_flag = flag;
}
bool RunParameter::getRunRighEdgeFlag()
{
	return run_right_edge_flag;
}
/* 速度調整有効フラグ */
void RunParameter::setChangeSpeedFlag(bool flag)
{
	change_speed_flag = flag;
}
bool RunParameter::getChangeSpeedFlag()
{
	return change_speed_flag;
}
/* アームの角度 */
void RunParameter::setArmAngle(double a)
{
	arm_angle = a;
}
double RunParameter::getArmAngle()
{
	return arm_angle;
}
/* アーム動作速度 */
void RunParameter::setArmPower(int p)
{
	arm_power = p;
}
int RunParameter::getArmPower()
{
	return arm_power;
}
/* 直進するための速度 */
void RunParameter::setForwardSpeed(int s)
{
	forward_speed = s;
}

int RunParameter::getForwardSpeed()
{
	return forward_speed;
}
/* 直進走行時の角度初期化フラグ */
void RunParameter::setAngleResetFlag(bool flag)
{
	reset_straight_angle = flag;
}
bool RunParameter::getAngleResetFlag()
{
	return reset_straight_angle;
}
/* 直進走行時の目標角度 */
void RunParameter::setStraightAngle(double a)
{
	straight_target_angle = a;
	setTargetAngleFlag(true);
}
double RunParameter::getStraightAngle()
{
	return straight_target_angle;
}
/* 直進走行時の目標角度指定フラグ */
void RunParameter::setTargetAngleFlag(bool flag)
{
	set_target_angle = flag;
}
bool RunParameter::getTargetAngleFlag()
{
	return set_target_angle;
}
/* 回転速度 */
void RunParameter::setRotateSpeed(int s)
{
	rotate_speed = s;
}
int RunParameter::getRotateSpeed()
{
	return rotate_speed;
}
/* 回転角度 */
void RunParameter::setRotateAngle(double a)
{
	rotate_angel = a;
}
double RunParameter::getRotateAngle()
{
	return rotate_angel;
}
/* 減速式回転の角度 */
void RunParameter::setDecelerationRotaryAngle(double a)
{
	deceleration_rotary_angle = a;
}
double RunParameter::getDecelerationRotaryAngle()
{
	return deceleration_rotary_angle;
}
/* 減速式回転の回転速度 */
void RunParameter::setDecelerationRotarySpeed(double s)
{
	deceleration_rotary_speed = s;
}
double RunParameter::getDecelerationRotarySpeed()
{
	return deceleration_rotary_speed;
}
/* 減速式回転の回転速度と角度をを固定する */
void RunParameter::setUturnApporoachFlag(bool flag)
{
	uturn_apporoach_flag = flag;
}
double RunParameter::getUturnApporoachFlag()
{
	return uturn_apporoach_flag;
}
void RunParameter::setForwardTurnFlag(bool flag)
{
	forward_turn_flag = flag;
}
bool RunParameter::getForwardTurnFlag()
{
	return forward_turn_flag;
}
/* 黒線検知旋回モード */
void RunParameter::setDetectTurnMode(bool m)
{
	line_detect_turn_mode = m;
}
bool RunParameter::getDetectTurnMode()
{
	return line_detect_turn_mode;
}
/* 黒線検知旋回方向 */
void RunParameter::setDetectTurnDirection(bool d)
{
	line_detect_turn_direction = d;
}
bool RunParameter::getDetectTurnDirection()
{
	return line_detect_turn_direction;
}
/* 曲率走行速度 */
void RunParameter::setCurvatureRunSpeed(int s)
{
	curvature_run_speed = s;
}
int RunParameter::getCurvatureRunSpeed()
{
	return curvature_run_speed;
}
/* 曲率半径 */
void RunParameter::setCurvatureRadias(double r)
{
	curvature_radias = r;
}
double RunParameter::getCurvatureRadias()
{
	return curvature_radias;
}

/* ブロック初期位置	*/
int RunParameter::getOneBlock()
{
	return one_position;
}
void RunParameter::setOneBlock(int g)
{
	one_position = g;
}
int RunParameter::getTwoBlock()
{
	return two_position;
}
void RunParameter::setTwoBlock(int b)
{
	two_position = b;
}
int RunParameter::getThreeBlock()
{
	return three_position;
}
void RunParameter::setThreeBlock(int r)
{
	three_position = r;
}
int RunParameter::getFourBlock()
{
	return four_position;
}
void RunParameter::setFourBlock(int y)
{
	four_position = y;
}
int RunParameter::getPowerBlock()
{
	return power_position;
}
void RunParameter::setPowerBlock(int b)
{
	power_position = b;
}
