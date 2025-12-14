#pragma once

#include "Tactics.h"
#include "ColorID.h"
#include "ColorSensor.h"
#include "Motor.h"

class BlockLineUpBehavior : public Tactics
{
public:
  explicit BlockLineUpBehavior(ev3api::ColorSensor &colorSensor, ev3api::Motor &leftWheel, ev3api::Motor &rightWheel);
  virtual ~BlockLineUpBehavior();

  void execute();
  bool isFinished();
  bool Straight_X_axis(bool Avoidance);  // X軸での直進動作  [true = 回避する, false = 回避なし]
  bool Straight_Y_axis(bool Avoidance);  // Y軸での直進動作  [true = 回避する, false = 回避なし]
  bool TurnLeft_X_axis(bool Avoidance);  // X軸での左折動作  [true = 回避する, false = 回避なし]
  bool TurnLeft_Y_axis(bool Avoidance);  // Y軸での左折動作  [true = 回避する, false = 回避なし]
  bool TurnRight_X_axis(bool Avoidance); // X軸での右折動作  [true = 回避する, false = 回避なし]
  bool TurnRight_Y_axis(bool Avoidance); // Y軸での右折動作  [true = 回避する, false = 回避なし]
  bool Uturn();                          // Uターン(180°旋回動作)
  bool UturnApporoach_X_axis();          // X軸でのカラーブロックブロックの色判定の後に行うUターン動作
  bool UturnApporoach_Y_axis();          // Y軸でのカラーブロックブロックの色判定の後に行うUターン動作
  bool BlockPut_X_left(bool start);      // X軸での左直進からブロックをサークル内に置く動作
  bool BlockPut_X_Right(bool start);     // X軸での右直進からブロックをサークル内に置く動作
  bool BlockPut_Y_left(bool start);      // Y軸での左直進からブロックをサークル内に置く動作
  bool BlockPut_Y_Right(bool sart);      // Y軸での右直進からブロックをサークル内に置く動作
  bool BlockPut_X_Diagonal(bool start);  //右斜め45度にブロックをサークル内に設置する動作
  bool BlockPut_Y_Diagonal(bool start);  //左斜め45度にブロックをサークル内に設置する動作
  bool BlockPut_arm_Right(bool start);   //右斜め60度にブロックをサークル内に設置する動作
  bool BlockPut_arm_left(bool start);    //左斜め60度にブロックをサークル内に設置する動作
  bool BlockPut_X_Outline(bool start);
  bool GoGarage(bool directionRobot);       // ガレージへ向かう為の動作 [true = 緑 false = 黄] ※例）trueの場合は緑サークルから侵入した
  bool BlockSortArea_Intrusion(bool ArmUP); // ブロック並べエリアに侵入の為の動作
  bool CircleDetection();                   // 色識別用
  int ColorBlockReturn();                   // 何色のカラーブロックを取得したか
  void SetSwitchingParameter(bool PID);     // PIDパラメータを切り替え [true = ブロック所持, false = ブロックを持っていない]
  void getSwitchingParameter_LOW();         // PIDパラメータ[LOW]を切り替え (呼び出し用)
  void SwitchingMove(bool move);

  //使わないメソッド
  bool Apporoach();
  bool Apporoach_Not_Arm();

private:
  ev3api::ColorSensor &mColorSensor;
  ev3api::Motor &mLeftWheel;
  ev3api::Motor &mRightWheel;

  // 状態変数
  int state = 0;                  // executeの状態変数
  int Straight_X_State = 0;       // Straight_X_axisの状態変数
  int Straight_Y_State = 0;       // Straight_Y_axisの状態変数
  int TurnLeft_X_State = 0;       // TurnLeft_X_axisの状態変数
  int TurnLeft_Y_State = 0;       // TurnLeft_Y_axisの状態変数
  int TurnRight_X_State = 0;      // TurnRight_X_axisの状態変数
  int TurnRight_Y_State = 0;      // TurnRight_Y_axisの状態変数
  int UturnState = 0;             // Uturnの状態変数
  int UturnApporoach_X_State = 0; // UturnApporoach_X_axisの状態変数
  int UturnApporoach_Y_State = 0; // UturnApporoach_Y_axisの状態変数
  int BlockPut_X_State = 0;       // BlockPut_X_axisの状態変数
  int BlockPut_Y_State = 0;       // BlockPut_Y_axisの状態変数
  int BlockPut_arm_state = 0;
  int GoGarage_State = 0;                // GoGarageの状態変数
  int BlockDetectionState = 0;           // Apporoachの状態変数
  int BlockSortArea_Intrusion_State = 0; // BlockSortArea_Intrusionの状態変数

  // スピード
  int straightSpeed_HIGH = 30; // 直進走行のスピード
  int straightSpeed_LOW = 15;  // 直進走行のスピード（減速用）
  // 回転速度
  int rotateSpeed = 13;
  
  /* Uターン用定数 */
  const int BLACK_DETECT = true;
  const int WHITE_DETECT = false;
  int RIGHT_TURN = true;
  int LEFT_TURN = false;

  // PIDパラメータ {速度, KP, KI, KD}
  double PID_LOW[4] = {23, 0.45, 0, 8.0};      // 立石 = {23, 0.1, 0, 4.3} //カメラ無し　={15, 0.1, 0, 3.0}
  double PID_Bolck_LOW[4] = {25, 0.2, 0, 4.0}; //ブロックを持っている状態のライントレースパラメータ 立石 = {20, 0.1, 0, 4.0}
  double PID_Circle[4] = {20, 0.15, 0, 3.0};   //置き場検知じのライントレースパラメータ  {10, 0.08, 0, 0.2}
  // double PID_UturnApporoach[4] = {10, 0.2, 0, 1.8}; //Apporoach内のライントレースパラメータ

  // 色関連のパラメータ
  int REDCount = 0;                // 赤ブロックを読んだ回数
  int GREENCount = 0;              // 緑ブロックを読んだ回数
  int YELLOWCount = 0;             // 黄ブロックを読んだ回数
  int BLUECount = 0;               // 青ブロックを読んだ回数
  int ColorBlockDetectedCount = 3; // 同じカラーブロックの連続検知回数を調べる変数

  int target_Brightness = 48; // 正規化された黒と白の中間値（決め打ち）

  int Block_Color_Result; // ブロックの色を格納

  int black;
  int color;

  // 移動距離のパラメータ
  double Circle_Front_Distance = 2.0;                               // サークル手前までのライントレース距離{X軸, Y軸}
  double Straight_middle_Distans[2] = {10.5, 10.5};                 // 直進時の中間地点までの移動{X軸, Y軸}
  double Turn_middle_Distans[2] = {12.5, 12.5};                     // 右左折時の中間地点までの移動{X軸, Y軸}
  double Apporoach_middle_Distans[2] = {17, 15};                    // ブロックを持ってUターン時の中間地点までの移動{X軸, Y軸}
  double BlockPut_middle_Distans[4] = {-14.8, -12.8, -10.9, -11.4}; // ブロック設置後に中間地点までの移動{X軸(緑or青), X軸(赤or黄), Y軸(緑or青), Y軸(赤or黄)}

  double Straight_Before_Rotation_Distance_X[2] = {7.7, 7.7}; // 回転調節用直進動作の距離{左エッジ, 右エッジ}
  double Straight_Before_Rotation_Distance_Y[2] = {6.8, 6.8}; // 回転調節用直進動作の距離{左エッジ, 右エッジ}

  double Circle_Distance[2] = {8.8, 9.0};        // 右左折をする為のサークル内の移動距離{青, 赤or黄or緑}
  double Circle_Distance_left[2] = {13.0, 12.5}; // 右左折をする為のサークル内の移動距離{青, 赤or黄or緑}

  int Avoidance_Distance = 15; // 回避時の黒線誤検知を回避するための距離

  // 回転角度のパラメータ
  int Straight_Angle_X[2] = {42, 83};   // 直進回避{１回目, ２回目}
  int Straight_Angle_Y[2] = {49, 97};   // 直進回避{１回目, ２回目}
  int Turn_Left_Angle_X[2] = {39, 39};  // 左折回避{右エッジ, 左エッジ}
  int Turn_Left_Angle_Y[2] = {39, 39};  // 左折回避{右エッジ, 左エッジ}
  int Turn_Right_Angle_X[2] = {39, 39}; // 右折回避{右エッジ, 左エッジ}
  int Turn_Right_Angle_Y[2] = {39, 39}; // 右折回避{右エッジ, 左エッジ}

  int rotation_angle = 90; // 右左折回避なしの回転角度
  int rotation_angle_BLUE = 90;

  // 角度の保持
  int straightAngle;                    // 直進時の目標角度の保持
  int retentionAngle_Uturn[2] = {0, 0}; // Uturn()の相対角度保持{過去, 現在}

  // フラグ
  bool Block_Hold = false;   // ブロックを所持しているかどうか
  bool Block_Set = false;    //ブロックを置いたかどうか
  bool flag = false;         // 終了フラグ
  bool TimeOut_flag = false; //カラーブロック検知で色が読めなかった時のフラグ
  bool ColorFlag_1 = false;
  bool ColorFlag_2 = false;

  // その他パラメータ設定
  char buf[20]; // ディスプレイに２０文字表示

  // Y軸の置き場の間にある黒線の距離 = 30cm
  // X軸の置き場の間にある黒線の距離 = 35cm
};
