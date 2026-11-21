// File: stable.h
#pragma once
#include <list>
#include <optional>
#include <unordered_map>
#include <variant>
#include <string>


template<typename T>
class STable {
public:
    

    bool bind(std::string s, T t);
    std::optional<T> lookup(std::string symbol);

    void enter();
    void exit();
private:
    std::list<std::unordered_map<std::string, T>> table;
};
