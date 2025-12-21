#ifndef SECTION_CONTROL_TACTICS_H
#define SECTION_CONTROL_TACTICS_H

#include "Tactics.h"
#include "EV3SonarSensor.h"

class SectionControlTactics : public Tactics
{
public:
  explicit SectionControlTactics(ev3api::ColorSensor &colorSensor,EV3SonarSensor* ev3sonarSensor);
  virtual ~SectionControlTactics();

  void execute(); //戦術を実行する
  bool isFinished();
  void cycle_set(double cycle);   // ★追加
  bool cycle_start();

private:
  ev3api::ColorSensor &mColorSensor;
  EV3SonarSensor* mEV3SonarSensor;
  /* パラメータ指定用の添字 */
  const int SPEED = 0;
  const int KP = 1;
  const int KI = 2;
  const int KD = 3;

  double target;
  int cycle_count = 0;    // ★追加
  int cycle_time  = 0;

  double retentionAngle[2];

  //灰色検知お試し
  /*
	int getcolor;
	int getkidoti;
	char buf[20];
	*/
  bool flag = false;
  /**
	* 区間ごとのパラメータ
	* section = {speed, KP, KI, KD}
	*/
  /*地区大会用21秒*/



  //まあまあ早い　20fps以上ないと脱線してしまう
  double sectionFC2[4] = {15, 0.05, 0, 2.5};
  double section0[4] = {35, 0.05, 0, 5.5};    //スタート
  double section1[4] = {20, 0.4, 0, 6.0};  //カーブ1入り
  double section2[4] = {15, 0.1, 0, 6.7};     //フリーエリア色接近
  double section3[4] = {25, 0.1, 0, 6.7};    //lap通過後
  double section4[4] = {15, 0.5, 0, 5.0};     //直線2
  double section5[4] = {40, 0.95, 0, 12.0};   //カーブ3
  double section6[4] = {10, 0.015, 0, 0};    //フリーエリア//////////////////////
  double section7[4] = {20, 0.8, 0, 6.0};   //フリーエリア関数
  double section8[4] = {40, 0.55, 0, 6.0};   //直線4
  double section9[4] = {40, 0.85, 0, 12.0};   //カーブ5
  double section10[4] = {40, 0.55, 0, 6.0}; //直線5
  double section11[4] = {40, 0.85, 0, 13.5};  //カーブ6
  double section12[4] = {40, 0.5, 0, 5.5};  //直線6
  double section13[4] = {40, 0.8, 0, 12.0};  //カーブ7
  double section14[4] = {40, 0.62, 0, 6.5};  //直線7
  double section15[4] = {40, 0.85, 0, 10.5};  //ゴール
  double section16[4] = {40, 0.55, 0, 6.0};  //カーブ6-2

  //これより下は現在未使用
  
  double section17[4] = {35, 0.4, 0, 7.0};    //直線6:ゴールまで
  double section18[4] = {55, 0.4, 0, 10.0};   //ゴールからカーブ直前まで
  double section19[4] = {45, 0.6, 0, 10.0};   //カーブ7前半
  double section20[4] = {25, 0.5, 0, 8.0};    //カーブ7後半*/
};

#endif