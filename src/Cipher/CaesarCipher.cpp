
#include "Cipher/CaesarCipher.h"

namespace crypttool {

    char CaesarCipher::shiftCharacter(
        char character,
        int shift
    ) {

        if (character >= 'A' && character <= 'Z') {

            return static_cast<char>(
                'A' + (character - 'A' + shift + 26) % 26
                );
        }

        if (character >= 'a' && character <= 'z') {

            return static_cast<char>(
                'a' + (character - 'a' + shift + 26) % 26
                );
        }

        return character;
    }

    std::string CaesarCipher::encrypt(
        const std::string& text,
        int shift
    ) {

        shift = ((shift % 26) + 26) % 26;

        std::string result;

        for (char character : text) {
            result += shiftCharacter(character, shift);
        }

        return result;
    }

    std::string CaesarCipher::decrypt(
        const std::string& text,
        int shift
    ) {

        shift = ((shift % 26) + 26) % 26;

        return encrypt(text, 26 - shift);
    }

}
