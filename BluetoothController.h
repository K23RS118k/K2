#ifndef BLUETOOTH_CONTROLLER_H
#define BLUETOOTH_CONTROLLER_H

#define BT_RCV_BUF_SIZE (34) /* NXT GamePadは送受信ともに34バイト*/

class BluetoothController {
	
public:
	static unsigned int read();			// コントローラから34バイトのデータを受信し、forward（前進値）とturn（旋回値）を更新する
	static signed char getForward();	// コントローラからの前進値を取得する
	static signed char getTurn();		// コントローラからの旋回値を取得する

private:
	static char receiveBuf[];		// 受信バッファ
	static unsigned int len;		// 受信バイト数
	static signed char cmd_forward;	// 前進値
	static signed char cmd_turn;	// 旋回値
};

#endif
