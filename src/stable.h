// File: stable.h
#pragma once
#include <list>
#include <optional>
#include <unordered_map>
#include <variant>
#include <string>


class STable {
public:
    enum Type {
        INT,
        BOOL
    };

    bool bind(std::string s, Type t);
    std::optional<Type> lookup(std::string symbol);

    void enter();
    // pop all until a marker
    void exit();
private:
    std::list<std::unordered_map<std::string, Type>> table;
};
