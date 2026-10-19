cmake_minimum_required(VERSION 4.1)
project(mc-compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp
    src/lexer.cpp
    )

# google ASAN
target_compile_options(main PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
target_link_options(main PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
