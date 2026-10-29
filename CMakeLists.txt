cmake_minimum_required(VERSION 4.1)
project(mc-compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mcc src/main.cpp
    src/lexer/lexer.cpp
    src/token.cpp
    )

target_compile_options(mcc PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
target_link_options(mcc PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
