#include "FaceDisplay.h"

// FaceDisplay クラスのコンストラクタ
FaceDisplay::FaceDisplay() {
    currentFace = FACE_START;
}

// 表示する顔IDを設定する
void FaceDisplay::setFace(FaceID id) {
    currentFace = id;
}

// FaceIDに対応するファイル名を取得する
const char* FaceDisplay::getFilename(FaceID id) {
    switch(id) {
        case FACE_START: return "ev3rt/res/goalone.bmp";
        case FACE_COLOR1: return "ev3rt/res/aka.bmp";
        case FACE_COLOR2: return "ev3rt/res/ao.bmp";
        case FACE_COLOR3: return "ev3rt/res/kiiro.bmp";
        case FACE_COLOR4: return "ev3rt/res/midori.bmp";
        case FACE_WALL: return "ev3rt/res/haitatu.bmp";
        default: return "ev3rt/res/goalone.bmp";
    }
}

// 現在設定されている顔をEV3のLCDに表示する
void FaceDisplay::show() {
    const char* filename = getFilename(currentFace);
    
    // メモリファイル構造体を準備
    memfile_t memfile;
    
    // BMPファイルをメモリにロード
    ER ercd = ev3_memfile_load(filename, &memfile);
    if (ercd != E_OK) {
        // ロード失敗時のエラー処理（必要に応じてログ出力などを追加）
        return;
    }

    // ロードしたメモリファイルからイメージをデコード
    image_t image;
    ev3_image_load(&memfile, &image);
    
    // LCDの座標(0, 0)にイメージを描画
    ev3_lcd_draw_image(&image, 0, 0);
    
    // イメージに使用したメモリを解放
    ev3_image_free(&image);
    
    // (オプション) メモリファイルを解放
    // ev3_memfile_free(&memfile); // ev3rtの実装に依存しますが、ロードしたファイルは通常自動または明示的に解放が必要です
}