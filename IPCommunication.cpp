#include "IPCommunication.h"
#include "ev3api.h"
#include "app.h"
#include <syssvc/serial.h>
#include <stdlib.h>
#include "etroboc_ext.h"

#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (12 /*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (16 /*TODO: magic number*/)
#define MESSAGE_LEN 6

//static char data_log_buffer[32] = {0};
static char buf[1] = {0};
//static char buf_for_print[20] = {0};
static char message[MESSAGE_LEN] = {0};

// static bool loopS = false;
// static bool loopR = false;
//static bool loopI = true;

//static int size = 0;
static int change = 0;
static int blockcount = 0;
static int block = 0;
static int blockColor[12] = {};
static int last = 0;
static double few = 0;

static int cardnumber = 0;
static int blocknumber = 0;
static int blockspot[11] = {};

static bool flag = false;
static bool completeflag = false;
static bool flag_getCourceInfo = false;

/**
* 走行開始状態のフラグ
*/
bool IPCommunication::setFlag(char s)
{
	buf[0] = s;
	// カーネルのシリアルインターフェスドライバを使う場合
	// bufが指すバッファからBluetooth仮想シリアルポートへ最大lenバイトを書き込む
	serial_wri_dat(SIO_PORT_BT, buf, 1);
	return true;
}

// void IPCommunication::recieveData()
// {
// 	// for (int i = 0; i < 32; i++)
// 	// {
// 	// 	data_log_buffer[i] = 0;
// 	// }
// 	switch (state)
// 	{
// 	case 0:
// 		size = serial_rea_dat(SIO_PORT_BT, message, MESSAGE_LEN);
// 		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
// 		sprintf(buf, "message : %s", message);
// 		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);
// 		if (size > 0)
// 		{
// 			for (int i = 9; i >= 5; i--)
// 			{
// 				//block[i] = message[i] - '0';
// 			}
// 			state++;
// 		}
// 		break;
// 	case 1:
// 		buf[0] = 'l';
// 		serial_wri_dat(SIO_PORT_BT, buf, 1);
// 		flag = false;
// 		state++;
// 		break;
// 	case 2:
// 		size = serial_rea_dat(SIO_PORT_BT, message, MESSAGE_LEN);
// 		ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
// 		sprintf(buf, "message : %s", message);
// 		ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);
// 		if (size > 0)
// 		{
// 			for (int i = 4; i >= 0; i--)
// 			{
// 				//block[i] = message[i] - '0';
// 			}
// 			state++;
// 		}
// 		break;
// 	case 3:
// 		buf[0] = 'l';
// 		serial_wri_dat(SIO_PORT_BT, buf, 1);
// 		flag = false;
// 		completeflag = false;
// 		break;

// 	default:
// 		break;
// 	}

// 	送信されたデータをlcdに表示
// 		sprintf(buf, "red : %d", redblock);
// 	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);

// 	sprintf(buf, "green : %d", greenblock);
// 	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 4);

// 	sprintf(buf, "black : %d", blackblock);
// 	ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);
// }
bool IPCommunication::recieveData()
{
	int size = serial_rea_dat(SIO_PORT_BT, message, MESSAGE_LEN);
	// ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	// sprintf(buf, "size : %d", size);
	//ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);
	if (size > 0)
	{
		block = atoi(message);
		blockcount++;
		if (blockcount == 2)
		{
			completeflag = true;
		}
		return true;
	}
	return false;
	//block[blockcount] = atoi(message);
	//block[blockcount] = message - '0';
}
void IPCommunication::sendData()
{
	buf[0] = 'l';
	serial_wri_dat(SIO_PORT_BT, buf, 1);
}

/**
 * 以下のメソッドを呼ぶと数値を返します。
 * 配列に数値が入っています。
 * １＝赤、２＝青、３＝黄、４＝緑、５＝黒で値が格納される
 * ６＝認識していない
 * 黒とカラーブロックのボーナスサークルを呼ぶときは
 * ９以外の数値＝カラーサークルの番号
 * ９＝認識していない
 */

//数字情報
int IPCommunication::getNumber()
{
	return blockColor[0];
}
//１個目の色
int IPCommunication::getNo1()
{
	return blockColor[1];
}
//２個目の色
int IPCommunication::getNo2()
{
	return blockColor[2];
}
//３個目の色
int IPCommunication::getNo3()
{
	return blockColor[3];
}
//４個目の色
int IPCommunication::getNo4()
{
	return blockColor[4];
}
//５個目の色
int IPCommunication::getNo5()
{
	return blockColor[5];
}
//６個目の色
int IPCommunication::getNo6()
{
	return blockColor[7];
}
//７個目の色
int IPCommunication::getNo7()
{
	return blockColor[8];
}
//８個目の色
int IPCommunication::getNo8()
{
	return blockColor[9];
}
//黒ブロックのボーナスサークル、ブロックサークルの番号が格納される
int IPCommunication::getBlackBlock()
{
	return blockColor[10];
}
//カラーブロックのボーナスサークル
int IPCommunication::getColorSircle()
{
	return blockColor[11];
}
// void IPCommunication::getBlock()
// {
// 	return block;
// }
void IPCommunication::decode()
{
	if (blockcount == 1)
	{
		for (int i = 5; i >= 0; i--)
		{
			last = block % 10; //下一桁を求める
			few = block / 10;  //少数にする(double型)
			block = few;	   //double型をint型に入れることで整数部分だけになる
			blockColor[i] = last;
		}
	}
	else if (blockcount == 2)
	{
		for (int i = 11; i >= 6; i--)
		{
			last = block % 10; //下一桁を求める
			few = block / 10;  //少数にする(double型)
			block = few;	   //double型をint型に入れることで整数部分だけになる
			blockColor[i] = last;
		}
	}
}
int IPCommunication::returnmessage()
{
	change = atoi(message);
	return change;
}
// int IPCommunication::returnsize()
// {
// 	return size;
// }
//終了判定フラグ
bool IPCommunication::isFinished()
{
	return flag;
}
//通信タスク完了フラグ
bool IPCommunication::isCompleted()
{
	return completeflag;
}

/*
シミュレータAPIは数値（int型）を返す
数値に対応した文字コードがコース情報となっている
（ASCIIコード　文字に対応する10進数）
交点サークル
A<=>65 B<=>66 C<=>67 D<=>68
E<=>69 F<=>70 G<=>71 H<=>72
J<=>74 K<=>75 L<=>76 M<=>77
P<=>80 Q<=>81 R<=>82 S<=>83
ブロックサークル
1<=>49 2<=>50 3<=>51
4<=>52        5<=>53
6<=>54 7<=>55 8<=>56
*/

bool IPCommunication::getCourceInfo_sim()
{
	cardnumber = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_CARD_NUMBER);
	blockspot[1] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_BLACK1);
	blockspot[2] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_BLACK2);
	blockspot[3] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_RED1);
	blockspot[4] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_RED2);
	blockspot[5] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_YELLOW1);
	blockspot[6] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_YELLOW2);
	blockspot[7] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_BLUE1);
	blockspot[8] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_BLUE2);
	blockspot[9] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_GREEN1);
	blockspot[10] = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_POS_GREEN2);

	return flag_getCourceInfo = true;
}

int IPCommunication::getBlockSpot_sim(int colorblock)
{
	return blockspot[colorblock];
}

int IPCommunication::getCardNumber_sim()
{
	return cardnumber;
}

int IPCommunication::getBlockNumber_sim()
{
	blocknumber = ETRoboc_getCourceInfo(ETROBOC_COURSE_INFO_BLOCK_NUMBER);
	
	return blocknumber;
}
