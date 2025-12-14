#ifndef EV3_COLOR_SENSOR_H
#define EV3_COLOR_SENSOR_H
 
#include "ev3api.h"
#include "ColorSensor.h"
 
class EV3ColorSensor
{
  public:
    explicit EV3ColorSensor(ev3api::ColorSensor &colorSensor);
    virtual ~EV3ColorSensor();
 
    int getBrightness();               //輝度値を取得する
    double getColorBrightness();       //色の輝度値をとる
    void getEncodeHSV();             //RGB値をHSV値に変換
    float getHue();
    float getSaturation();
    float getBright();
    bool isColor_BLUE();
    bool blue_counter2();
    bool isColor_RED();
    bool isColor_GREEN();
    bool isColor_YELLOW();
    bool isColor_WHITE();
    bool isColor_BLACK();
    bool isColor_PRESENT();
    bool isColor_OBSTACLE();
    bool isColor();
 
  private:
    ev3api::ColorSensor &mColorSensor;
    int log1;
    int log2;
    int log3;
    float Hue;
    float Saturation;
    float Brightness;
    int hue;
    int saturation;
    int brightness;
    int min;
    int max;
    double rgb_brightness;
    int Bcounter = 0;
    int xm;
 
    rgb_raw_t rgb; //rgB値
 
};
 
#endif