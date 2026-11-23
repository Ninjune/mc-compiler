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
    bool bind(std::string s, T t)
    {
        if(table.front().count(s))
            return false;

        table.front()[s] = t;

        return true;
    }

    std::optional<T> lookup(std::string symbol)
    {
        for(auto& r : table)
        if(r.count(symbol))
            return r[symbol];

        return std::nullopt;
    }

    void enter()
    {
        table.push_front(std::unordered_map<std::string, T>());
    }

    void exit()
    {
        table.pop_front();
    }
private:
    std::list<std::unordered_map<std::string, T>> table;
};
