#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H

#include "ev3api.h"

class FaceDisplay {
public:
    // EV3ディスプレイに表示する顔のID
    enum FaceID {
        FACE_START = 0, // goalone.bmp（起動時）
        FACE_COLOR1,    // aka.bmp（赤）
        FACE_COLOR2,    // ao.bmp（青）
        FACE_COLOR3,    // kiiro.bmp（黄色）
        FACE_COLOR4,    // midori.bmp（緑）
        FACE_WALL,      // haitatu.bmp（壁）
        FACE_MAX        // ★ 配列サイズ用（必ず最後）
    };

    /**
     * コンストラクタ
     */
    FaceDisplay();

    /**
     * 表示する顔IDを設定する
     */
    void setFace(int id);

    /**
     * すべての画像をメモリにロードする（起動時に1回だけ呼ぶ）
     */
    void load();

    /**
     * 現在設定されている顔をEV3のLCDに表示する
     */
    void show();

private:
    int currentFace;
    bool loaded;

    image_t images[FACE_MAX];  // ★ 全画像を保持

    /**
     * FaceIDに対応するBMPファイル名を取得
     */
    const char* getFilename(int id);
};

#endif // FACEDISPLAY_H
