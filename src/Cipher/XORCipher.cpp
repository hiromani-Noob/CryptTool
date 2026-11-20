
#include "Cipher/XORCipher.h"

#include <stdexcept>

namespace crypttool {

    std::string XORCipher::encrypt(
        const std::string& text,
        const std::string& key
    ) {

        if (key.empty()) {
            throw std::invalid_argument("XOR key is empty.");
        }

        constexpr char HEX[] = "0123456789ABCDEF";

        std::string result;

        for (std::size_t i = 0; i < text.size(); ++i) {

            unsigned char value =
                static_cast<unsigned char>(text[i]) ^
                static_cast<unsigned char>(key[i % key.size()]);

            result += HEX[value >> 4];
            result += HEX[value & 0x0F];
        }

        return result;
    }

    std::string XORCipher::decrypt(
        const std::string& hexText,
        const std::string& key
    ) {

        if (key.empty()) {
            throw std::invalid_argument("XOR key is empty.");
        }

        if (hexText.size() % 2 != 0) {
            throw std::invalid_argument("Invalid HEX length.");
        }

        std::string result;

        for (std::size_t i = 0; i < hexText.size(); i += 2) {

            auto hexValue = [](char c) -> int {

                if (c >= '0' && c <= '9') {
                    return c - '0';
                }

                if (c >= 'A' && c <= 'F') {
                    return c - 'A' + 10;
                }

                if (c >= 'a' && c <= 'f') {
                    return c - 'a' + 10;
                }

                return -1;
                };

            int high = hexValue(hexText[i]);
            int low = hexValue(hexText[i + 1]);

            if (high < 0 || low < 0) {
                throw std::invalid_argument("Invalid HEX character.");
            }

            unsigned char value =
                static_cast<unsigned char>((high << 4) | low);

            value ^= static_cast<unsigned char>(
                key[(i / 2) % key.size()]
                );

            result += static_cast<char>(value);
        }

        return result;
    }

}
