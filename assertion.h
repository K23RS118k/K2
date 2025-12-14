#ifndef ASSERTION_H
#define ASSERTION_H

/*下の1行をコメントアウトするとデバッグモード */
//#define NDEBUG

#include "assert.h"

/**
* assertの使い方
* デバッグしたい箇所にassert(条件文)を書いてください
* 条件文がfalseのときプログラムが停止しEV3コンソールにファイル名と行番号が表示されます
* 条件文がtrueのときはそのまま動作を継続します
* その箇所でtrueであるはずというような条件文を書いて下さい(例：white > black)
* もしくはswitch文などでassert(false)とするとstateの異常を検知できます
* assert(条件文 && "メッセージ")とするとコンソールにメッセージを表示したりもできます
*/

#endif
