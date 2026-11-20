
#include "Cipher/Hex.h"

#include <stdexcept>

namespace crypttool {

    namespace {

        constexpr char HEX_TABLE[] =
            "0123456789ABCDEF";

        int hexValue(char c) {

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
        }

    }

    std::string Hex::encode(
        const std::string& text
    ) {

        std::string result;

        for (unsigned char c : text) {

            result += HEX_TABLE[c >> 4];
            result += HEX_TABLE[c & 0x0F];

        }

        return result;
    }

    std::string Hex::decode(
        const std::string& hexText
    ) {

        if (hexText.size() % 2 != 0) {
            throw std::invalid_argument("Invalid HEX length.");
        }

        std::string result;

        for (std::size_t i = 0; i < hexText.size(); i += 2) {

            int high = hexValue(hexText[i]);
            int low = hexValue(hexText[i + 1]);

            if (high < 0 || low < 0) {
                throw std::invalid_argument("Invalid HEX character.");
            }

            unsigned char value =
                static_cast<unsigned char>((high << 4) | low);

            result += static_cast<char>(value);
        }

        return result;
    }

}
