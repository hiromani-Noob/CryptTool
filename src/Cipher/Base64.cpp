
#include "Cipher/Base64.h"

#include <stdexcept>
#include <cstdint>
#include <string>

namespace crypttool {

    namespace {

        constexpr char BASE64_TABLE[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        int decodeCharacter(char c) {

            for (int i = 0; i < 64; ++i) {

                if (BASE64_TABLE[i] == c) {
                    return i;
                }
            }

            return -1;
        }

    }

    std::string Base64::encode(
        const std::string& text
    ) {

        std::string result;

        std::uint32_t buffer = 0;
        int bits = 0;

        for (unsigned char c : text) {

            buffer = (buffer << 8) | c;
            bits += 8;

            while (bits >= 6) {

                bits -= 6;

                result += BASE64_TABLE[
                    (buffer >> bits) & 0x3F
                ];
            }
        }

        if (bits > 0) {

            result += BASE64_TABLE[
                (buffer << (6 - bits)) & 0x3F
            ];
        }

        while (result.size() % 4 != 0) {
            result += '=';
        }

        return result;
    }

    std::string Base64::decode(
        const std::string& encodedText
    ) {

        if (encodedText.size() % 4 != 0) {
            throw std::invalid_argument("Invalid Base64 length.");
        }

        std::size_t padding = 0;

        if (!encodedText.empty() &&
            encodedText.back() == '=') {

            padding = 1;

            if (encodedText.size() >= 2 &&
                encodedText[encodedText.size() - 2] == '=') {

                padding = 2;
            }
        }

        std::size_t dataLength =
            encodedText.size() - padding;

        if ((padding == 1 && dataLength % 4 != 3) ||
            (padding == 2 && dataLength % 4 != 2)) {

            throw std::invalid_argument("Invalid Base64 padding.");
        }

        std::string result;

        std::uint32_t buffer = 0;
        int bits = 0;

        for (std::size_t i = 0; i < dataLength; ++i) {

            int value = decodeCharacter(encodedText[i]);

            if (value < 0) {
                throw std::invalid_argument("Invalid Base64 character.");
            }

            buffer = (buffer << 6) |
                static_cast<std::uint32_t>(value);

            bits += 6;

            if (bits >= 8) {

                bits -= 8;

                result += static_cast<char>(
                    (buffer >> bits) & 0xFF
                    );

                if (bits == 0) {
                    buffer = 0;
                }
                else {
                    buffer &= (1u << bits) - 1;
                }
            }
        }

        if (bits > 0 && buffer != 0) {
            throw std::invalid_argument("Invalid Base64 data.");
        }

        return result;
    }

}
