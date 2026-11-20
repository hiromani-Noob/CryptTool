
cmake_minimum_required(VERSION 3.20)

project(CryptTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CryptTool WIN32
    src/main.cpp
    src/MainWindow.cpp
    src/Cipher/CaesarCipher.cpp
    include/MainWindow.h
    include/Cipher/CaesarCipher.h
    src/Cipher/XORCipher.cpp
    include/Cipher/XORCipher.h
    src/Cipher/Base64.cpp
    include/Cipher/Base64.h
    src/Cipher/Hex.cpp
    include/Cipher/Hex.h
 "include/Cipher/CaesarCipher.h" "src/Cipher/CaesarCipher.cpp" "include/Cipher/XORCipher.h" "src/Cipher/XORCipher.cpp" "include/Cipher/Base64.h" "src/Cipher/Base64.cpp" "include/Cipher/Hex.h" "src/Cipher/Hex.cpp")

target_include_directories(CryptTool PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(CryptTool PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

if(MSVC)
    target_compile_options(CryptTool PRIVATE /W4 /utf-8)
endif()
