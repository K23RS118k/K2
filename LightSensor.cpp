#include "LightSensor.h"
#include "BluetoothLogger.h"
#include <math.h>

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (12/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (16/*TODO: magic number*/)
#define N 10


/**
* コンストラクタ
* @param colorSensor カラーセンサ
*/
LightSensor::LightSensor(ev3api::ColorSensor& colorSensor, BluetoothLogger* bluetoothLogger)
: mColorSensor(colorSensor){
	
}

LightSensor::~LightSensor() {
}


//getValue 反射光(白黒)を取得する
int LightSensor::getValue() {
	return mColorSensor.getBrightness();
}

/*
//getETFloorColor 床(ET相撲)の反射光(色)を表示する
int  LightSensor::getETFloorColor() {		//get⇔disp
	char buf[20];

	mColorSensor.getRawColor(rgb);

	//色判別
	//////////////////////////
	//	red = 1				//
	//	green = 2			//
	//	yellow = 3			//
	//	blue = 4			//
	//	course_green = 5	//
	//	white = 11			//
	//	black 12			//
	//	gray = 13			//
	//	error = 0			//
	//////////////////////////


	// LCD画面表示
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	//red
	sprintf(buf,"r ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*1);
	sprintf(buf,"%d",rgb.r);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*1);
	//green
	sprintf(buf,"g ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*2);
	sprintf(buf,"%d",rgb.g);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*2);
	//blue
	sprintf(buf,"b ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*3);
	sprintf(buf,"%d",rgb.b);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*3);
	
////////////////////////////////////////////////////////////

//	BluetoothLogger::dataLogger(0, 0, (int16_t)rgb.r, (int16_t)rgb.g , (int16_t)rgb.b, 0);

	//ET相撲コースred判定
	if(sqrt(pow(etredaverage[0]-rgb.r,2)+pow(etredaverage[1]-rgb.g,2)+pow(etredaverage[2]-rgb.b,2))<=etredradius){
		sprintf(buf,"ETred");
		color = RED;
	//ET相撲コースgreen判定
	}else if(sqrt(pow(etgreenaverage[0]-rgb.r,2)+pow(etgreenaverage[1]-rgb.g,2)+pow(etgreenaverage[2]-rgb.b,2))<=etgreenradius){
		sprintf(buf,"ETgreen");               
		color = GREEN;
	//ET相撲コースyellow判定
	}else if(sqrt(pow(etyellowaverage[0]-rgb.r,2)+pow(etyellowaverage[1]-rgb.g,2)+pow(etyellowaverage[2]-rgb.b,2))<=etyellowradius){
		sprintf(buf,"ETyellow");
		color = YELLOW;
	//ET相撲コースblue判定
	}else if(sqrt(pow(etblueaverage[0]-rgb.r,2)+pow(etblueaverage[1]-rgb.g,2)+pow(etblueaverage[2]-rgb.b,2))<=etblueradius){
		sprintf(buf,"ETblue");
		color = BLUE;
	//ET相撲コースblack判定
	}else if(sqrt(pow(etblackaverage[0]-rgb.r,2)+pow(etblackaverage[1]-rgb.g,2)+pow(etblackaverage[2]-rgb.b,2))<=etblackradius){
		sprintf(buf,"ETblack");
		color = BLACK;
	//ET相撲コースwhite判定
	}else if(sqrt(pow(etwhiteaverage[0]-rgb.r,2)+pow(etwhiteaverage[1]-rgb.g,2)+pow(etwhiteaverage[2]-rgb.b,2))<=etwhiteradius){
		sprintf(buf,"ETwhite");
		color = WHITE;
	//???(上記に当てはまらない場合)
	}else{
		sprintf(buf,"etERROR");
		color = ERROR;
	}

	//表示時以外はコメントアウトする(他の表示部分が上書きされて消えることがあるため)
	//ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*5);

	return color;
}
*/

//getBlockFloorColor 床(ブロック並べ)の反射光(色)を表示する
int  LightSensor::getBlockFloorColor() {		//get⇔disp
	char buf[20];

	mColorSensor.getRawColor(rgb);

	//色判別
	//////////////////////////
	//	red = 1				//
	//	green = 2			//
	//	yellow = 3			//
	//	blue = 4			//
	//	course_green = 5	//
	//	white = 11			//
	//	black 12			//
	//	gray = 13			//
	//	error = 0			//
	//////////////////////////


	// LCD画面表示
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	//red
	sprintf(buf,"r ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*1);
	sprintf(buf,"%d",rgb.r);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*1);
	//green
	sprintf(buf,"g ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*2);
	sprintf(buf,"%d",rgb.g);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*2);
	//blue
	sprintf(buf,"b ");
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*3);
	sprintf(buf,"%d",rgb.b);
	ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*3);
	
////////////////////////////////////////////////////////////


//	BluetoothLogger::dataLogger(0, 0, (int16_t)rgb.r, (int16_t)rgb.g , (int16_t)rgb.b, 0);

	//ブロック並べコースred距離
	distance[0]=pow(redgravity[0]-rgb.r,2)+pow(redgravity[1]-rgb.g,2)+pow(redgravity[2]-rgb.b,2);
	//ブロック並べコースgreen距離
	distance[1]=pow(greengravity[0]-rgb.r,2)+pow(greengravity[1]-rgb.g,2)+pow(greengravity[2]-rgb.b,2);
	//ブロック並べコースyellow距離
	distance[2]=pow(yellowgravity[0]-rgb.r,2)+pow(yellowgravity[1]-rgb.g,2)+pow(yellowgravity[2]-rgb.b,2);
	//ブロック並べコースblue距離
	distance[3]=pow(bluegravity[0]-rgb.r,2)+pow(bluegravity[1]-rgb.g,2)+pow(bluegravity[2]-rgb.b,2);
	//ブロック並べコースblack
	distance[4]=pow(blackgravity[0]-rgb.r,2)+pow(blackgravity[1]-rgb.g,2)+pow(blackgravity[2]-rgb.b,2);
	//ブロック並べコースwhite距離
	distance[5]=pow(whitegravity[0]-rgb.r,2)+pow(whitegravity[1]-rgb.g,2)+pow(whitegravity[2]-rgb.b,2);
	//ブロック並べコースgray距離
	distance[6]=pow(graygravity[0]-rgb.r,2)+pow(graygravity[1]-rgb.g,2)+pow(graygravity[2]-rgb.b,2);

	min = distance[0];
	index = 0;
	for(int i = 0; i < 7; i++){
		if(distance[i] < min){
			min = distance[i];
			index = i;
		}
	}

	if(index == 0){
		sprintf(buf,"red");
		color = RED;
	}else if(index == 1){
			sprintf(buf,"green");               
			color = GREEN;
	}else if(index == 2){
			sprintf(buf,"yellow");
			color = YELLOW;
	}else if(index == 3){
			sprintf(buf,"blue");
			color = BLUE;
	}else if(index == 4){
			sprintf(buf,"black");
			color = BLACK;
	}else if(index == 5){
			sprintf(buf,"white");
			color = WHITE;
	}else if(index == 6){
			sprintf(buf,"gray");
			color = GRAY;
	}


	//表示時以外はコメントアウトする(他の表示部分が上書きされて消えることがあるため)
	//ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*5);

	return color;
}

/*
//getFloorColor ブロックの反射光(色)を表示する
int  LightSensor::getBlockColor() {			//get⇔disp
	char buf[20];

	if(count >= 65){
		
		mColorSensor.getRawColor(rgb);
		
		//色判別
		//////////////////////////
		//	red = 1				//
		//	green = 2			//
		//	yellow = 3			//
		//	blue = 4			//
		//	course_green = 5	//
		//	white = 11			//
		//	black 12			//
		//	gray = 13			//
		//	error = 0			//
		//////////////////////////


		// LCD画面表示
		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		//red
		sprintf(buf,"r ");
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*1);
		sprintf(buf,"%d",rgb.r);
		ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*1);
		//green
		sprintf(buf,"g ");
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*2);
		sprintf(buf,"%d",rgb.g);
		ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*2);
		//blue
		sprintf(buf,"b ");
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*3);
		sprintf(buf,"%d",rgb.b);
		ev3_lcd_draw_string(buf, CALIB_FONT_HEIGHT*2, CALIB_FONT_HEIGHT*3);


////////////////////////////////////////////////////////////

	//	BluetoothLogger::dataLogger(0, 0, (int16_t)rgb.r, (int16_t)rgb.g , (int16_t)rgb.b, 0);

	//ブロックred1判定
	if(sqrt(pow(blockredaverage1[0]-rgb.r,2)+pow(blockredaverage1[1]-rgb.g,2)+pow(blockredaverage1[2]-rgb.b,2))<=blockredradius1){
		sprintf(buf,"BLOCKred1");
		color = RED;
	//ブロックgreen1判定
	}else if(sqrt(pow(blockgreenaverage1[0]-rgb.r,2)+pow(blockgreenaverage1[1]-rgb.g,2)+pow(blockgreenaverage1[2]-rgb.b,2))<=blockgreenradius1){
		sprintf(buf,"BLOCKgreen1");
		color = GREEN;
	//ブロックyellow1判定
	}else if(sqrt(pow(blockyellowaverage1[0]-rgb.r,2)+pow(blockyellowaverage1[1]-rgb.g,2)+pow(blockyellowaverage1[2]-rgb.b,2))<=blockyellowradius1){
		sprintf(buf,"BLOCKyellow1");
		color = YELLOW;
	//ブロックblue1判定
	}else if(sqrt(pow(blockblueaverage1[0]-rgb.r,2)+pow(blockblueaverage1[1]-rgb.g,2)+pow(blockblueaverage1[2]-rgb.b,2))<=blockblueradius1){
		sprintf(buf,"BLOCKblue1");
		color = BLUE;
	//ブロックred2判定
	}else if(sqrt(pow(blockredaverage2[0]-rgb.r,2)+pow(blockredaverage2[1]-rgb.g,2)+pow(blockredaverage2[2]-rgb.b,2))<=blockredradius2){
		sprintf(buf,"BLOCKred2");
		color = RED;
	//ブロックgreen2判定
	}else if(sqrt(pow(blockgreenaverage2[0]-rgb.r,2)+pow(blockgreenaverage2[1]-rgb.g,2)+pow(blockgreenaverage2[2]-rgb.b,2))<=blockgreenradius2){
		sprintf(buf,"BLOCKgreen2");
		color = GREEN;
	//ブロックyellow2判定
	}else if(sqrt(pow(blockyellowaverage2[0]-rgb.r,2)+pow(blockyellowaverage2[1]-rgb.g,2)+pow(blockyellowaverage2[2]-rgb.b,2))<=blockyellowradius2){
		sprintf(buf,"BLOCKyellow2");
		color = YELLOW;
	//ブロックblue2判定
	}else if(sqrt(pow(blockblueaverage2[0]-rgb.r,2)+pow(blockblueaverage2[1]-rgb.g,2)+pow(blockblueaverage2[2]-rgb.b,2))<=blockblueradius2){
		sprintf(buf,"BLOCKblue2");
		color = BLUE;
	//ブロックred3判定
	}else if(sqrt(pow(blockredaverage3[0]-rgb.r,2)+pow(blockredaverage3[1]-rgb.g,2)+pow(blockredaverage3[2]-rgb.b,2))<=blockredradius3){
		sprintf(buf,"BLOCKred3");
		color = RED;
	//ブロックgreen3判定
	}else if(sqrt(pow(blockgreenaverage3[0]-rgb.r,2)+pow(blockgreenaverage3[1]-rgb.g,2)+pow(blockgreenaverage3[2]-rgb.b,2))<=blockgreenradius3){
		sprintf(buf,"BLOCKgreen3");
		color = GREEN;
	//ブロックyellow3判定
	}else if(sqrt(pow(blockyellowaverage3[0]-rgb.r,2)+pow(blockyellowaverage3[1]-rgb.g,2)+pow(blockyellowaverage3[2]-rgb.b,2))<=blockyellowradius3){
		sprintf(buf,"BLOCKyellow3");
		color = YELLOW;
	//ブロックblue3判定
	}else if(sqrt(pow(blockblueaverage3[0]-rgb.r,2)+pow(blockblueaverage3[1]-rgb.g,2)+pow(blockblueaverage3[2]-rgb.b,2))<=blockblueradius3){
		sprintf(buf,"BLOCKblue3");
		color = BLUE;
	//ブロックred4判定
	}else if(sqrt(pow(blockredaverage4[0]-rgb.r,2)+pow(blockredaverage4[1]-rgb.g,2)+pow(blockredaverage4[2]-rgb.b,2))<=blockredradius4){
		sprintf(buf,"BLOCKred4");
		color = RED;
	//ブロックgreen4判定
	}else if(sqrt(pow(blockgreenaverage4[0]-rgb.r,2)+pow(blockgreenaverage4[1]-rgb.g,2)+pow(blockgreenaverage4[2]-rgb.b,2))<=blockgreenradius4){
		sprintf(buf,"BLOCKgreen4");
		color = GREEN;
	//ブロックyellow4判定
	}else if(sqrt(pow(blockyellowaverage4[0]-rgb.r,2)+pow(blockyellowaverage4[1]-rgb.g,2)+pow(blockyellowaverage4[2]-rgb.b,2))<=blockyellowradius4){
		sprintf(buf,"BLOCKyellow4");
		color = YELLOW;
	//ブロックblue4判定
	}else if(sqrt(pow(blockblueaverage4[0]-rgb.r,2)+pow(blockblueaverage4[1]-rgb.g,2)+pow(blockblueaverage4[2]-rgb.b,2))<=blockblueradius4){
		sprintf(buf,"BLOCKblue4");
		color = BLUE;
	//ブロックred5判定
	}else if(sqrt(pow(blockredaverage5[0]-rgb.r,2)+pow(blockredaverage5[1]-rgb.g,2)+pow(blockredaverage5[2]-rgb.b,2))<=blockredradius5){
		sprintf(buf,"BLOCKred5");
		color = RED;
	//ブロックyellow5判定
	}else if(sqrt(pow(blockyellowaverage5[0]-rgb.r,2)+pow(blockyellowaverage5[1]-rgb.g,2)+pow(blockyellowaverage5[2]-rgb.b,2))<=blockyellowradius5){
		sprintf(buf,"BLOCKyellow5");
		color = YELLOW;
	//???(上記にあてはまらない場合)
	}else{
		sprintf(buf,"BLOCKERROR");
		color = ERROR;
	}

		//表示時以外はコメントアウトする(他の表示部分が上書きされて消えることがあるため)
		//ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT*5);
		
		count = 0;
	}
	count++;
	
	return color;
}
*/

/*
//ブロックの色を見て最初に10回出た色を表示する
int  LightSensor::getBlockCount() {

	blockColor = getBlockColor();

	if(blockColor == RED){
		redCount++;
	}else if(blockColor == GREEN){
		greenCount++;
	}else if(blockColor == YELLOW){
		yellowCount++;
	}else if(blockColor == BLUE){
		blueCount++;
	}else{
		returnColor = ERROR;
	}

	if(redCount >= 10){
		returnColor = RED;
		initializeCount();
	}else if(greenCount >= 10){
		returnColor = GREEN;
		initializeCount();
	}else if(yellowCount >= 10){
		returnColor = YELLOW;
		initializeCount();
	}else if(blueCount >= 10){
		returnColor = BLUE;
		initializeCount();
	}
	else{
		returnColor = ERROR;
	}
	
	return returnColor;
}

void LightSensor::initializeCount(){
	redCount = 0;
	greenCount = 0;
	yellowCount = 0;
	blueCount = 0;
	blockColor = ERROR;
	returnColor = ERROR;
}
*/