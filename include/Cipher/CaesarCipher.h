
#pragma once

#include <string>

namespace crypttool {

    class CaesarCipher {
    public:

        static std::string encrypt(
            const std::string& text,
            int shift
        );

        static std::string decrypt(
            const std::string& text,
            int shift
        );

    private:

        static char shiftCharacter(
            char character,
            int shift
        );
    };

}
