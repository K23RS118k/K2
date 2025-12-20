#include "Calibration.h"

#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_HEIGHT (16)

Calibration::Calibration(
    ev3api::TouchSensor &touchSensor,
    RunParameter *runParameter,
    TimerJudgement *timerJudgement,
    EV3ColorSensor *ev3ColorSensor,
    RearMotor *rearMotor,
    CalcCurrentLocation *calcCurrentLocation)
    : mTouchSensor(touchSensor),
      mRunParameter(runParameter),
      mTimerJudgement(timerJudgement),
      mEV3ColorSensor(ev3ColorSensor),
      mRearMotor(rearMotor),
      mCalcCurrentLocation(calcCurrentLocation)
{
}

Calibration::~Calibration()
{
}

/**
 * キャリブレーション処理
 * return true : 完了
 * return false: 継続
 */
bool Calibration::calibrate()
{
    switch (state_b)
    {
    case 0: // 初期表示
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
        sprintf(buf, "Calibration");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);
        mEV3ColorSensor->getColorBrightness();
        mTimerJudgement->setTime(1000);
        mTimerJudgement->start();
        state_b = 10;
        break;

    case 10: // 白測定待ち
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
        sprintf(buf, "Put on WHITE");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);
        sprintf(buf, "ENTER to measure");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);

        if (ev3_button_is_pressed(ENTER_BUTTON))
        {
            white_brightness = mEV3ColorSensor->getColorBrightness();
            ev3_speaker_play_tone(NOTE_E5, 100);

            mTimerJudgement->setTime(300);
            mTimerJudgement->start();
            state_b = 11;
        }
        break;

    case 11: // ENTERボタン離し待ち
        if (!ev3_button_is_pressed(UP_BUTTON))
        {
            state_b = 20;
        }
        break;

    case 20: // 黒測定待ち
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
        sprintf(buf, "Put on BLACK");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);
        sprintf(buf, "ENTER to measure");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);

        if (ev3_button_is_pressed(DOWN_BUTTON))
        {
            black_brightness = mEV3ColorSensor->getColorBrightness();
            ev3_speaker_play_tone(NOTE_G4, 200);

            mTimerJudgement->setTime(300);
            mTimerJudgement->start();
            state_b = 21;
        }
        break;

    case 21: // ENTERボタン離し待ち
        if (!ev3_button_is_pressed(DOWN_BUTTON))
        {
            state_b = 30;
        }
        break;

    case 30: // 目標輝度計算
        target_brightness = (white_brightness + black_brightness) / 2;
        target_brightness+=20;//目標値＋２０で青対策

        mRunParameter->setWhite(white_brightness);
        mRunParameter->setBlack(black_brightness);
        mRunParameter->setTargetBrightness(target_brightness);

        state_b = 31;
        break;

    case 31: // 結果表示
        ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);

        sprintf(buf, "Calibration Done");
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 1);

        sprintf(buf, "white  : %d", white_brightness);
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 3);

        sprintf(buf, "black  : %d", black_brightness);
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 5);

        sprintf(buf, "target : %d", target_brightness);
        ev3_lcd_draw_string(buf, 0, CALIB_FONT_HEIGHT * 7);

        mTimerJudgement->setTime(1000);
        mTimerJudgement->start();
        state_b = 40;
        break;

    case 40: // 初期化して終了
        mRearMotor->resetEncoder();
        mCalcCurrentLocation->setPointX(0);
        mCalcCurrentLocation->setPointY(0);
        mCalcCurrentLocation->setAngle(0);
        return true;
    }

    return false;
}

/**
 * 初期位置コード解析（現状未使用）
 */
void Calibration::decode()
{
    // 今後使用する場合に実装
}

/**
 * タイマー再スタート（安全版）
 */
void Calibration::startTimer()
{
    mTimerJudgement->stop();
    mTimerJudgement->setTime(300);
    mTimerJudgement->start();
}
