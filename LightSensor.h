#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "ColorSensor.h"
#include "ColorID.h"
#include "BluetoothLogger.h"

class LightSensor {
public:
	explicit LightSensor(ev3api::ColorSensor& colorSensor, BluetoothLogger* bluetoothLogger);
	virtual ~LightSensor();
	int getValue();
	int getETFloorColor();
	int getBlockFloorColor();
	int getBlockColor();
	int getBlockCount();
	int dispColor();
	
	//レプリカコース　ロボコン用レプリカコースの値
	
	//コース赤重心
	const int redgravity[3] = {76, 22, 16};
	//コース緑重心
	const int greengravity[3] = {17, 72, 30};
	//コース黄重心
	const int yellowgravity[3] = {78, 103, 14};
	//コース青重心
	const int bluegravity[3] = {10, 42, 81};
	//コース黒重心
	const int blackgravity[3] = {5, 6, 8};
	//コース白重心
	const int whitegravity[3] = {86, 123, 128};
	//コース灰色重心
	const int graygravity[3] = {42, 70, 73};

/*	
	//ET相撲コース赤平均
	const int etredaverage[3] = {310, 87, 59};
	//ET相撲コース緑平均
	const int etgreenaverage[3] = {62, 170, 87};
	//ET相撲コース黄平均
	const int etyellowaverage[3] = {340, 263, 65};
	//ET相撲コース青平均
	const int etblueaverage[3] = {57, 150, 265};
	//ET相撲コース黒平均
	const int etblackaverage[3] = {23, 26, 16};
	//ET相撲コース白平均
	const int etwhiteaverage[3] = {239, 296, 167};
*/


/*
	//ブロック赤1平均
	const int blockredaverage1[3] = {28, 3, 5};
	//ブロック赤2平均
	const int blockredaverage2[3] = {44, 3, 7};
	//ブロック赤3平均
	const int blockredaverage3[3] = {66, 8, 10};
	//ブロック赤4平均
	const int blockredaverage4[3] = {98, 11, 13};
	//ブロック赤5平均
	const int blockredaverage5[3] = {130, 14, 16};

	//ブロック赤1半径
	const int blockredradius1 = 6;
	//ブロック赤2半径
	const int blockredradius2 = 11;
	//ブック赤3半径
	const int blockredradius3 = 13;
	//ブロック赤4半径
	const int blockredradius4 = 20;
	//ブロック赤5半径
	const int blockredradius5 = 30;

	//ブロック緑1平均
	const int blockgreenaverage1[3] = {2, 8, 9};
	//ブロック緑2平均
	const int blockgreenaverage2[3] = {5, 16, 14};
	//ブロック緑3平均
	const int blockgreenaverage3[3] = {8, 26, 20};
	//ブロック緑4平均
	const int blockgreenaverage4[3] = {12, 40, 29};

	//ブロック緑1半径
	const int blockgreenradius1 = 6;
	//ブロック緑2半径
	const int blockgreenradius2 = 6;
	//ブロック緑3半径
	const int blockgreenradius3 = 8;
	//ブロック緑4半径
	const int blockgreenradius4 = 15;

	//ブロック黄1平均
	const int blockyellowaverage1[3] = {26, 14, 4};
	//ブロック黄2平均
	const int blockyellowaverage2[3] = {38, 28, 6};
	//ブロック黄3平均
	const int blockyellowaverage3[3] = {64, 38, 9};
	//ブロック黄4平均
	const int blockyellowaverage4[3] = {97, 59, 12};
	//ブロック黄5平均
	const int blockyellowaverage5[3] = {129, 79, 15};

	//ブロック黄1半径
	const int blockyellowradius1 = 5;
	//ブロック黄2半径
	const int blockyellowradius2 = 14;
	//ブロック黄3半径
	const int blockyellowradius3 = 17;
	//ブロック黄4半径
	const int blockyellowradius4 = 23;
	//ブロック黄5半径
	const int blockyellowradius5 = 30;

	//ブロック青1平均
	const int blockblueaverage1[3] = {1, 8, 42};
	//ブロック青2平均
	const int blockblueaverage2[3] = {3, 17, 64};
	//ブロック青3平均c
	const int blockblueaverage3[3] = {6, 28, 93};
	//ブロック青4平均
	const int blockblueaverage4[3] = {9, 44, 136};

	//ブロック青1半径
	const int blockblueradius1 = 10;
	//ブロック青2半径
	const int blockblueradius2 = 15;
	//ブロック青3半径
	const int blockblueradius3 = 20;
	//ブロック青4半径
	const int blockblueradius4 = 30;
	
*/

///////////////////////////////////

private:

	int min;
	int index;
	int distance[7];

	void initializeCount();

	ev3api::ColorSensor& mColorSensor;
	rgb_raw_t rgb;
	int color = 0;
	int count = 0;

	int redCount = 0;
	int greenCount = 0;
	int yellowCount = 0;
	int blueCount = 0;

	int returnColor = ERROR;
	int blockColor;
};

#endif
