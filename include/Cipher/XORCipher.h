
#pragma once

#include <string>

namespace crypttool {

    class XORCipher {
    public:

        static std::string encrypt(
            const std::string& text,
            const std::string& key
        );

        static std::string decrypt(
            const std::string& hexText,
            const std::string& key
        );
    };

}
