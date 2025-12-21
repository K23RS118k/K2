#include "Music.h"
#include <cstdint>
#include <stdio.h>
// ev3api.h は Music.h でインクルード済み

Music::Music() : m_is_loaded(false) {}

Music::~Music() {
    if (m_is_loaded) {
        ev3_memfile_free(&m_memfile);
        m_is_loaded = false;
    }
}



bool Music::load(const char* filepath) {
    // 既にロードされている場合は、一度解放します (念のため)
    if (m_is_loaded) {
        ev3_memfile_free(&m_memfile);
        m_is_loaded = false;
    }

    // ev3_memfile_loadを使ってWAVファイルをロードします。E_OK (0) は成功。
    if (ev3_memfile_load(filepath, &m_memfile) == E_OK) {
        m_is_loaded = true;
        ev3_memfile_free(&m_memfile); 
        return true;
    }

    // ロード失敗
    return false;
}

bool Music::play(int volume, int duration) {
    if (!m_is_loaded) {
        // ファイルがロードされていない場合は再生できません
        ev3_lcd_draw_string("memfile load error", 0, 0);
        return false;
    }

    // 1. 音量を設定します
    // 音量を0-100の範囲に制限 (クランプ) すると安全です
    int actual_volume = (volume < 0) ? 0 : ((volume > 100) ? 100 : volume);
    ev3_speaker_set_volume(actual_volume);

    // 2. ロードしたファイルを再生します。E_OK (0) は成功。
    if (ev3_speaker_play_file(&m_memfile, duration) == E_OK) {
        return true;
    }

    return false;
}