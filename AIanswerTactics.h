#ifndef AIANSWER_TACTICS_H
#define AIANSWER_TACTICS_H

#define KP 1
#define KI 2
#define KD 3

#include "Tactics.h"
#include "BluetoothLogger.h"

class AIanswerTactics : public Tactics
{
  public:
	explicit AIanswerTactics();
	virtual ~AIanswerTactics();

	void execute();
	bool digitalfont();
	bool textbookFont();
	bool bitAnswer();
	bool isFinished();

  private:
	/* execute用 */
	//case文用
	int state = 0;
	int bitAnswerCount = 2;
	//20文字表示
	char buf[20];

	//白検知用変数
	int white;
	//黒検知用変数
	int black;
	//緑検知用
	int green;
	//直線検知時の角度保持
	double angleRetention;

	//デジタルフォント用
	//デジタルフォントcase文用
	int digital_state = 0;
	//デジタルアンサー回答　全ての分岐に該当しなかった場合０が答え
	int digital_answer = 0;
	//デジタルフォントの黒を読んだ回数
	int digital_blackCount = 0;

	//textbookfont用
	//textbook_case文用
	int textbook_state = 0;
	//教科書フォント読み取りに入るライントレースの補正を入れるための目標値
	//double target;
	//区間用変数
	int textbookCount = 0;
	//1～12区間の黒判定(true:区間内に黒が少しでも有る, false:区間内に黒がない)(上の区間内黒判定black_decisionを変更して実装すると良いかな？)
	bool black_flag1 = false;
	bool black_flag2 = false;
	bool black_flag3 = false;
	bool black_flag4 = false;
	bool black_flagM4 = false; //4の後半
	bool black_flag5 = false;
	bool black_flag6 = false;
	bool black_flag7 = false;
	bool black_flag8 = false;
	bool black_flag9 = false;
	bool black_flag10 = false;
	bool black_flag11 = false;
	bool black_flag12 = false;
	//1～12区間の白判定(true:区間内に白が少しでも有る, false:区間内に白がない)
	bool white_flag1 = false;
	bool white_flag2 = false;
	bool white_flag3 = false;
	bool white_flag4 = false;
	bool white_flagM4 = false; //4の後半
	bool white_flag5 = false;
	bool white_flag6 = false;
	bool white_flag7 = false;
	bool white_flag8 = false;
	bool white_flag9 = false;
	bool white_flag10 = false;
	bool white_flag11 = false;
	bool white_flag12 = false;
	//1回目の黒が読まれる区間の番号
	int firstTime_black = 0;
	//9区間の終わり10区間への変わり目で黒を読んだか（4か6の判定用）ただし区間の変わり目の一瞬の判定用なので輝度値にノイズが入ると精度が下がる気もする。その場合は9区間を2分割して対応すると良いかな？
	bool black_flag_endline_of9 = false;
	//黒カウント（2と3の判別の区間ので黒を誤検知した（ノイズか？）
	int blackCount1 = 0;
	int blackCount2 = 0;
	int blackCount3 = 0;
	int blackCount4 = 0;
	int blackCountM4 = 0; //4の後半
	int blackCount5 = 0;
	int blackCount6 = 0;
	int blackCount7 = 0;
	int blackCount8 = 0;
	int blackCount9 = 0;
	int blackCount10 = 0;
	int blackCount11 = 0;
	int blackCount12 = 0;

	//白カウント（6と4の判別の9区間の終わりで白を誤検知した（ノイズか？）ので実装してみたが他クラスで実装済みなら別の理由なので意味なしか
	int whiteCount1 = 0;
	int whiteCount2 = 0;
	int whiteCount3 = 0;
	int whiteCount4 = 0;
	int whiteCountM4 = 0; //4の後半
	int whiteCount5 = 0;
	int whiteCount6 = 0;
	int whiteCount7 = 0;
	int whiteCount8 = 0;
	int whiteCount9 = 0;
	int whiteCount10 = 0;
	int whiteCount11 = 0;
	int whiteCount12 = 0;
	//黒カウント textbookblackCountに変えてください
	int blackCount = 0;
	//教科書フォントアンサー回答　デフォルトは黒を読まない１に設定
	int textbook_answer = 1;

	/* bitAnswer用 */
	//switch文用変数
	int bitstate = 0;
	//基数変換用
	int i;
	int binary_digital[3];
	int binary_textbook[3];
	//ライントレース用スピード変数
	int line_trace_speed_HIGH = 20;
	int line_trace_speed_LOW = 13;
	//ランストレート用スピード変数
	int run_straight_speed = 13;
	//バック用スピード変数
	int back_speed = 15;
	//回転用スピード変数
	int rotate_speed = 18;
	//PIDパラメーター
	double PID[3] = {0.25, 0.0, 2.0}; //0.1, 0.0, 1.5(0.3, 0.0, 2.5)

	//動作完了フラグ(true:動作完了, false:動作中)
	bool flag = false;
};

#endif