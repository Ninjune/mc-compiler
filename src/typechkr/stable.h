// File: stable.h
#pragma once
#include <optional>
#include <stack>
#include <unordered_map>
#include <variant>
#include <string>
typedef bool Marker;


class STable {
public:
    enum Type {
        INT,
        BOOL
    };
    void bind();
    std::optional<Type> lookup();
    enter();

    // pop all until a marker
    exit();
private:
    std::stack<std::unordered_map<std::string, Type>> table;
};
