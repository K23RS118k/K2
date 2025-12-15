#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H

#include "ev3api.h" // ev3_memfile_load, ev3_image_load, ev3_lcd_draw_image, ev3_image_free のために必要

class FaceDisplay {
 public:
    // EV3ディスプレイに表示する顔のIDを定義
    enum FaceID {
        FACE_START = 0, // goalone.bmp（起動時）
        FACE_COLOR1,    // aka.bmp (赤)
        FACE_COLOR2,    // ao.bmp (青)
        FACE_COLOR3,    // kiiro.bmp (黄色)
        FACE_COLOR4,    // midori.bmp (緑)
        FACE_WALL       // haitatu.bmp (障害物/壁)
    };

    /**
     * コンストラクタ
     */
    FaceDisplay();

    /**
     * 表示する顔IDを設定する
     * @param id 設定する顔ID
     */
    void setFace(FaceID id);

    /**
     * 現在設定されている顔をEV3のLCDに表示する
     */
    void show();

 private:
    FaceID currentFace;

    /**
     * FaceIDに対応するファイル名を取得する
     * @param id FaceID
     * @return BMPファイル名へのポインタ
     */
    const char* getFilename(FaceID id);
};

#endif // FACEDISPLAY_H