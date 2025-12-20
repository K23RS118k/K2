#include "FaceDisplay.h"

FaceDisplay::FaceDisplay()
    : currentFace(0), loaded(false)
{
}

void FaceDisplay::setFace(int id) {
    currentFace = id;
}

const char* FaceDisplay::getFilename(int id) {
    switch(id) {
        case 0: return "/ev3rt/res/goalone.bmp";
        case 1: return "/ev3rt/res/aka.bmp";
        case 2: return "/ev3rt/res/ao.bmp";
        case 3: return "/ev3rt/res/kiiro.bmp";
        case 4: return "/ev3rt/res/midori.bmp";
        case 5: return "/ev3rt/res/haitatu.bmp";
        default:return "/ev3rt/res/goalone.bmp";
    }
}

// ★ 最初に一度だけ呼ぶ
void FaceDisplay::load() {
    if (loaded) return;

    for (int i = 0; i < 6; i++) {
        memfile_t memfile;
        ER ercd = ev3_memfile_load(getFilename(i), &memfile);
        if (ercd != E_OK) {
            syslog(LOG_ERROR, "Image load failed: %d", i);
            continue;
        }
        ev3_image_load(&memfile, &images[i]);
        // memfile は ev3_image_load 後に不要
        ev3_memfile_free(&memfile);  // ★超重要
    }

    loaded = true;
}

// ★ 描画だけ
void FaceDisplay::show() {
    if (!loaded) return;

    ev3_lcd_draw_image(&images[currentFace], 0, 0);
}
