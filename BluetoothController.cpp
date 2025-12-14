#include "BluetoothController.h"
#include "ev3api.h"
#include <syssvc/serial.h>
#include "BluetoothLogger.h"

// 初期化
char BluetoothController::receiveBuf[BT_RCV_BUF_SIZE] = {0};
unsigned int BluetoothController::len = 0;
signed char BluetoothController::cmd_forward = 0;
signed char BluetoothController::cmd_turn = 0;

/**
 * NXT GamePadから1まとまり（34バイト）を受信します。
 * データの順序は、0:デーサイズ、1:謎の値、2:コントローラその1（アナログ左）、3:コントローラその2（アナログ右）、あと不明です。
 * このread()関数を実行してからgetForward()やgetTurn()を呼んでください。
 */
unsigned int BluetoothController::read() {
	len = serial_rea_dat(SIO_PORT_BT, receiveBuf, BT_RCV_BUF_SIZE);	// 34バイト受信する
	cmd_forward = -(signed char)receiveBuf[2];	// 前進値を更新する
	cmd_turn = (signed char)receiveBuf[3];		// 旋回値を更新する
	return len;	// 受信サイズを返す
}

/**
 * 前進値を返します。
 */
signed char BluetoothController::getForward() {
	return cmd_forward;
}

/**
 * 旋回値を返します。
 */
signed char BluetoothController::getTurn() {
	return cmd_turn;
}
