#ifdef __cplusplus
extern "C"
{
#endif

    /*	ここをコメントアウトするとassertは実行されない */
    //#define NDEBUG

#include "ev3api.h"
#include "assert.h"

/*
 *  各タスクの優先度の定義
 */
#define MAIN_PRIORITY TMIN_APP_TPRI + 1 /* メインタスクの優先度 */
#define RUN_PRIORITY TMIN_APP_TPRI + 2
#define CALIBRATION_PRIORITY TMIN_APP_TPRI + 3
#define CAMERA_PRIORITY TMIN_APP_TPRI + 4

/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef STACK_SIZE
#define STACK_SIZE 4096 /* タスクのスタックサイズ */
#endif                  /* STACK_SIZE */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

    extern void main_task(intptr_t exinf);
    extern void run_task(intptr_t exinf);
    extern void calibration_task(intptr_t exinf);
    extern void camera_task(intptr_t exinf);

    extern void ev3_cyc_run(intptr_t exinf);
    extern void ev3_cyc_calibration(intptr_t exinf);
    extern void ev3_cyc_camera(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
