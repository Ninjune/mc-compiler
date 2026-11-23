#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "ir.h"

class IRInterp
{
public:
    // Resolves variable names to frame slots and labels to instruction indices.
    IRInterp(IRProgram& p, std::istream& in = std::cin,
             std::ostream& out = std::cout);

    // Runs main(); returns its value (the compiler ignores it).
    // Throws std::runtime_error on a run-time failure (e.g. division by zero).
    int run();
private:
    void resolve();
    int exec(int fn, const std::vector<int>& args);

    static int arith(BinOp op, int x, int y);
    static bool compare(RelOp op, int x, int y);
    static int slotOf(std::unordered_map<std::string, int>& slots,
                      const std::string& v);
    static void resolveAtom(std::unordered_map<std::string, int>& slots,
                            Atom& a);
    static int labelTarget(const std::unordered_map<std::string, int>& labels,
                           const std::string& l);
    static int atomValue(const std::vector<int>& loc, const Atom& a);

    IRProgram& prog;
    std::istream& is;
    std::ostream& os;
    int mainIdx = -1;
};
