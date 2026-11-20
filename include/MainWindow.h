
#pragma once

#include <windows.h>

namespace crypttool {

    // GUIコントロールのID
    constexpr int ID_COMBO_CIPHER = 1001;
    constexpr int ID_EDIT_INPUT = 1002;
    constexpr int ID_BUTTON_ENCRYPT = 1003;
    constexpr int ID_BUTTON_DECRYPT = 1004;
    constexpr int ID_EDIT_OUTPUT = 1005;
    constexpr int ID_BUTTON_COPY = 1006;
    constexpr int ID_EDIT_SHIFT = 1007;
    constexpr int ID_EDIT_XOR_KEY = 1008;

    // メインウィンドウ生成
    HWND createMainWindow(
        HINSTANCE hInstance,
        int nCmdShow
    );

}
