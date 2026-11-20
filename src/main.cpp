
#include "MainWindow.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
) {
    HWND hwnd = crypttool::createMainWindow(
        hInstance,
        nCmdShow
    );

    if (!hwnd) {
        MessageBoxW(
            nullptr,
            L"Failed to create the main window.",
            L"CryptTool Error",
            MB_OK | MB_ICONERROR
        );

        return 1;
    }

    MSG msg{};

    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
