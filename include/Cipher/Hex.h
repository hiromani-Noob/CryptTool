#pragma once

#include <string>

namespace crypttool {

    class Hex {
    public:

        static std::string encode(
            const std::string& text
        );

        static std::string decode(
            const std::string& hexText
        );
    };

}