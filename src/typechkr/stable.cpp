// File: stable.cpp
#include "stable.h"


std::optional<STable::Type> STable::lookup(std::string symbol)
{
    for(auto& r : table)
        if(r.count(symbol))
            return r[symbol];
    return std::nullopt;
}


void STable::enter()
{
    table.push_back(std::unordered_map<std::string, STable::Type>());
}


void STable::exit()
{
    table.pop_back();
}
