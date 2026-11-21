// File: ir.h
#pragma once
#include <string>


class IR
{
    static std::string newvar() { static int x = 0; x += 1; return "t" + std::to_string(x); };
};
