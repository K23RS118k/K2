#ifndef MUSIC_H
#define MUSIC_H

#include "ev3api.h"
#include <cstdint>
#include <stdio.h>

/**
 * @brief WAVファイルをロード・再生するクラス
 */
class Music {
public:
    /**
     * @brief コンストラクタ
     */
    Music();

    /**
     * @brief デストラクタ
     * ロードされているWAVファイルのメモリを解放します。
     */
    ~Music();

    /**
     * @brief 指定されたパスのWAVファイルをロードします。
     * @param filepath SDカード上のファイルへの絶対パス（例: "/kakekko1.wav"）
     * @return ロードに成功した場合 true、失敗した場合 false
     */
    bool load(const char* filepath);

    /**
     * @brief ロードしたWAVファイルを再生します。
     * @param volume 再生音量 (0〜100)
     * @param duration 再生時間 (ミリ秒) または SOUND_MANUAL_STOP (-1)
     * @return 再生開始に成功した場合 true、失敗した場合 false
     */
    bool play(int volume, int duration = SOUND_MANUAL_STOP);

private:
    memfile_t m_memfile; ///< ロードされたファイル情報を保持する構造体
    bool m_is_loaded;    ///< ファイルが正常にロードされたかを示すフラグ
};

#endif // MUSIC_H