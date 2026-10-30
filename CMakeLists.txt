cmake_minimum_required(VERSION 4.1)
project(mc-compiler LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GVC REQUIRED libgvc libcgraph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mcc src/main.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/astgraph/astgraph.cpp
    src/token.cpp
    )

target_include_directories(mcc PRIVATE ${GVC_INCLUDE_DIRS})
target_link_libraries(mcc PRIVATE ${GVC_LIBRARIES})

target_compile_options(mcc PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
target_link_options(mcc PRIVATE -fsanitize=address -Wall -Wextra -O2 -g)
