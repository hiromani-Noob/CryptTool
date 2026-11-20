
#include "MainWindow.h"
#include "Cipher/CaesarCipher.h"
#include "Cipher/XORCipher.h"
#include <stdexcept>
#include "Cipher/Base64.h"
#include "Cipher/Hex.h"

#include <string>
#include <vector>
#include <cwchar>

namespace crypttool {

    namespace {

        constexpr wchar_t WINDOW_CLASS_NAME[] =
            L"CryptToolMainWindow";

        HWND hCipherCombo = nullptr;
        HWND hInputEdit = nullptr;
        HWND hOutputEdit = nullptr;
        HWND hShiftEdit = nullptr;
        HWND hXorKeyEdit = nullptr;

        std::string wideToUtf8(const std::wstring& text) {

            if (text.empty()) {
                return "";
            }

            int size = WideCharToMultiByte(
                CP_UTF8, 0, text.c_str(), -1,
                nullptr, 0, nullptr, nullptr
            );

            if (size <= 0) {
                return "";
            }

            std::string result(size, '\0');

            WideCharToMultiByte(
                CP_UTF8, 0, text.c_str(), -1,
                result.data(), size, nullptr, nullptr
            );

            result.pop_back();

            return result;
        }

        std::wstring utf8ToWide(const std::string& text) {

            if (text.empty()) {
                return L"";
            }

            int size = MultiByteToWideChar(
                CP_UTF8, 0, text.c_str(), -1,
                nullptr, 0
            );

            if (size <= 0) {
                return L"";
            }

            std::wstring result(size, L'\0');

            MultiByteToWideChar(
                CP_UTF8, 0, text.c_str(), -1,
                result.data(), size
            );

            result.pop_back();

            return result;
        }

        std::wstring getEditText(HWND hwnd) {

            int length = GetWindowTextLengthW(hwnd);

            std::vector<wchar_t> buffer(length + 1);

            GetWindowTextW(
                hwnd,
                buffer.data(),
                length + 1
            );

            return std::wstring(buffer.data());
        }

        void showError(HWND hwnd, const wchar_t* message) {

            MessageBoxW(
                hwnd,
                message,
                L"CryptTool Error",
                MB_OK | MB_ICONERROR
            );
        }

        bool getShiftValue(int& shift) {

            wchar_t buffer[64]{};

            GetWindowTextW(
                hShiftEdit,
                buffer,
                64
            );

            wchar_t* end = nullptr;

            long value = std::wcstol(
                buffer,
                &end,
                10
            );

            if (buffer[0] == L'\0' ||
                end == buffer ||
                *end != L'\0' ||
                value < -1000000 ||
                value > 1000000) {

                return false;
            }

            shift = static_cast<int>(value);

            return true;
        }


        void processCipher(HWND hwnd, bool encrypt) {

            int selected = static_cast<int>(
                SendMessageW(
                    hCipherCombo,
                    CB_GETCURSEL,
                    0,
                    0
                )
                );

            // ROT13
            if (selected == 1) {

                std::wstring input = getEditText(hInputEdit);
                std::string inputUtf8 = wideToUtf8(input);

                std::string result = CaesarCipher::encrypt(
                    inputUtf8,
                    13
                );

                std::wstring output = utf8ToWide(result);

                SetWindowTextW(
                    hOutputEdit,
                    output.c_str()
                );

                return;
            }

            // XOR
            if (selected == 2) {

                std::wstring input = getEditText(hInputEdit);
                std::string inputUtf8 = wideToUtf8(input);

                std::wstring key = getEditText(hXorKeyEdit);
                std::string keyUtf8 = wideToUtf8(key);

                if (keyUtf8.empty()) {

                    showError(
                        hwnd,
                        L"Please enter an XOR key."
                    );

                    return;
                }

                std::string result;

                try {

                    if (encrypt) {

                        result = XORCipher::encrypt(
                            inputUtf8,
                            keyUtf8
                        );

                    }
                    else {

                        result = XORCipher::decrypt(
                            inputUtf8,
                            keyUtf8
                        );

                    }

                }
                catch (const std::invalid_argument&) {

                    showError(
                        hwnd,
                        L"Invalid HEX input. Check the encrypted text."
                    );

                    return;
                }

                std::wstring output = utf8ToWide(result);

                SetWindowTextW(
                    hOutputEdit,
                    output.c_str()
                );

                return;
            }

            // Base64
            if (selected == 3) {

                std::wstring input = getEditText(hInputEdit);
                std::string inputUtf8 = wideToUtf8(input);

                std::string result;

                try {

                    if (encrypt) {

                        result = Base64::encode(inputUtf8);

                    }
                    else {

                        result = Base64::decode(inputUtf8);

                    }

                }
                catch (const std::invalid_argument&) {

                    showError(
                        hwnd,
                        L"Invalid Base64 input."
                    );

                    return;
                }

                std::wstring output = utf8ToWide(result);

                SetWindowTextW(
                    hOutputEdit,
                    output.c_str()
                );

                return;
            }


            // Hex
            if (selected == 4) {

                std::wstring input = getEditText(hInputEdit);
                std::string inputUtf8 = wideToUtf8(input);

                std::string result;

                try {

                    if (encrypt) {

                        result = Hex::encode(inputUtf8);

                    }
                    else {

                        result = Hex::decode(inputUtf8);

                    }

                }
                catch (const std::invalid_argument&) {

                    showError(
                        hwnd,
                        L"Invalid HEX input."
                    );

                    return;
                }

                std::wstring output = utf8ToWide(result);

                SetWindowTextW(
                    hOutputEdit,
                    output.c_str()
                );

                return;
            }


            // 未実装の暗号方式
            if (selected != 0) {

                showError(
                    hwnd,
                    L"This cipher has not been implemented yet."
                );

                return;
            }

            // Caesar Cipher
            int shift = 0;

            if (!getShiftValue(shift)) {

                showError(
                    hwnd,
                    L"Enter a valid shift number."
                );

                return;
            }

            std::wstring input = getEditText(hInputEdit);
            std::string inputUtf8 = wideToUtf8(input);

            std::string result;

            if (encrypt) {

                result = CaesarCipher::encrypt(
                    inputUtf8,
                    shift
                );

            }
            else {

                result = CaesarCipher::decrypt(
                    inputUtf8,
                    shift
                );

            }

            std::wstring output = utf8ToWide(result);

            SetWindowTextW(
                hOutputEdit,
                output.c_str()
            );
        }


        void createControls(HWND hwnd) {

            CreateWindowExW(
                0,
                L"STATIC",
                L"Cipher Method",
                WS_CHILD | WS_VISIBLE,
                30, 25, 200, 25,
                hwnd,
                nullptr,
                nullptr,
                nullptr
            );

            hCipherCombo = CreateWindowExW(
                0,
                L"COMBOBOX",
                nullptr,
                WS_CHILD | WS_VISIBLE |
                CBS_DROPDOWNLIST | WS_VSCROLL,
                30, 55, 300, 200,
                hwnd,
                reinterpret_cast<HMENU>(ID_COMBO_CIPHER),
                nullptr,
                nullptr
            );

            SendMessageW(hCipherCombo, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(L"Caesar Cipher"));

            SendMessageW(hCipherCombo, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(L"ROT13"));

            SendMessageW(hCipherCombo, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(L"XOR"));

            SendMessageW(hCipherCombo, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(L"Base64"));

            SendMessageW(hCipherCombo, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(L"Hex"));

            SendMessageW(hCipherCombo, CB_SETCURSEL, 0, 0);

            // シフト数
            CreateWindowExW(
                0,
                L"STATIC",
                L"Shift Number",
                WS_CHILD | WS_VISIBLE,
                370, 25, 150, 25,
                hwnd,
                nullptr,
                nullptr,
                nullptr
            );

            hShiftEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"3",
                WS_CHILD | WS_VISIBLE | ES_NUMBER,
                370, 55, 120, 30,
                hwnd,
                reinterpret_cast<HMENU>(ID_EDIT_SHIFT),
                nullptr,
                nullptr
            );

            // XOR Key
            CreateWindowExW(
                0,
                L"STATIC",
                L"XOR Key",
                WS_CHILD | WS_VISIBLE,
                530, 25, 150, 25,
                hwnd,
                nullptr,
                nullptr,
                nullptr
            );

            hXorKeyEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"123",
                WS_CHILD | WS_VISIBLE,
                530, 55, 120, 30,
                hwnd,
                reinterpret_cast<HMENU>(ID_EDIT_XOR_KEY),
                nullptr,
                nullptr
            );

            // 入力欄
            CreateWindowExW(
                0,
                L"STATIC",
                L"Input Text",
                WS_CHILD | WS_VISIBLE,
                30, 105, 200, 25,
                hwnd,
                nullptr,
                nullptr,
                nullptr
            );

            hInputEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE |
                ES_MULTILINE | ES_AUTOVSCROLL |
                WS_VSCROLL,
                30, 135, 720, 120,
                hwnd,
                reinterpret_cast<HMENU>(ID_EDIT_INPUT),
                nullptr,
                nullptr
            );

            // 暗号化
            CreateWindowExW(
                0,
                L"BUTTON",
                L"Encrypt",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                30, 275, 120, 40,
                hwnd,
                reinterpret_cast<HMENU>(ID_BUTTON_ENCRYPT),
                nullptr,
                nullptr
            );

            // 復号
            CreateWindowExW(
                0,
                L"BUTTON",
                L"Decrypt",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                170, 275, 120, 40,
                hwnd,
                reinterpret_cast<HMENU>(ID_BUTTON_DECRYPT),
                nullptr,
                nullptr
            );

            // 出力欄
            CreateWindowExW(
                0,
                L"STATIC",
                L"Output Result",
                WS_CHILD | WS_VISIBLE,
                30, 345, 200, 25,
                hwnd,
                nullptr,
                nullptr,
                nullptr
            );

            hOutputEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE |
                ES_MULTILINE | ES_AUTOVSCROLL |
                ES_READONLY | WS_VSCROLL,
                30, 375, 720, 120,
                hwnd,
                reinterpret_cast<HMENU>(ID_EDIT_OUTPUT),
                nullptr,
                nullptr
            );

            // コピー
            CreateWindowExW(
                0,
                L"BUTTON",
                L"Copy Result",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                30, 515, 140, 40,
                hwnd,
                reinterpret_cast<HMENU>(ID_BUTTON_COPY),
                nullptr,
                nullptr
            );
        }

        LRESULT CALLBACK windowProc(
            HWND hwnd,
            UINT message,
            WPARAM wParam,
            LPARAM lParam
        ) {

            switch (message) {

            case WM_CREATE:
                createControls(hwnd);
                return 0;

            case WM_COMMAND:

                switch (LOWORD(wParam)) {

                case ID_BUTTON_ENCRYPT:
                    processCipher(hwnd, true);
                    return 0;

                case ID_BUTTON_DECRYPT:
                    processCipher(hwnd, false);
                    return 0;

                case ID_BUTTON_COPY: {

                    std::wstring output = getEditText(hOutputEdit);

                    if (output.empty()) {
                        showError(
                            hwnd,
                            L"There is no result to copy."
                        );
                        return 0;
                    }

                    if (!OpenClipboard(hwnd)) {
                        showError(
                            hwnd,
                            L"Could not open clipboard."
                        );
                        return 0;
                    }

                    EmptyClipboard();

                    SIZE_T size =
                        (output.size() + 1) * sizeof(wchar_t);

                    HGLOBAL memory = GlobalAlloc(
                        GMEM_MOVEABLE,
                        size
                    );

                    if (!memory) {
                        CloseClipboard();
                        showError(
                            hwnd,
                            L"Memory allocation failed."
                        );
                        return 0;
                    }

                    wchar_t* buffer = static_cast<wchar_t*>(
                        GlobalLock(memory)
                        );

                    if (!buffer) {
                        GlobalFree(memory);
                        CloseClipboard();
                        showError(
                            hwnd,
                            L"Could not access clipboard memory."
                        );
                        return 0;
                    }

                    wcscpy_s(
                        buffer,
                        output.size() + 1,
                        output.c_str()
                    );

                    GlobalUnlock(memory);

                    if (!SetClipboardData(CF_UNICODETEXT, memory)) {
                        GlobalFree(memory);
                        CloseClipboard();
                        showError(
                            hwnd,
                            L"Could not copy the result."
                        );
                        return 0;
                    }

                    CloseClipboard();

                    MessageBoxW(
                        hwnd,
                        L"Result copied to clipboard.",
                        L"CryptTool",
                        MB_OK | MB_ICONINFORMATION
                    );

                    return 0;
                }
                }

                break;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            default:
                break;
            }

            return DefWindowProcW(
                hwnd,
                message,
                wParam,
                lParam
            );
        }

    }

    HWND createMainWindow(
        HINSTANCE hInstance,
        int nCmdShow
    ) {

        WNDCLASSEXW wc{};

        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = windowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = WINDOW_CLASS_NAME;
        wc.hCursor = LoadCursorW(
            nullptr,
            IDC_ARROW
        );
        wc.hbrBackground = reinterpret_cast<HBRUSH>(
            COLOR_WINDOW + 1
            );

        if (!RegisterClassExW(&wc)) {
            return nullptr;
        }

        HWND hwnd = CreateWindowExW(
            0,
            WINDOW_CLASS_NAME,
            L"CryptTool",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            650,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        if (!hwnd) {
            return nullptr;
        }

        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);

        return hwnd;
    }

}
