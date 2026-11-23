// File: interp.cpp
#include "interp.h"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <unordered_map>


IRInterp::IRInterp(IRProgram& p, std::istream& in, std::ostream& out)
    : prog(p), is(in), os(out)
{
    resolve();
}


int IRInterp::run()
{
    int r = exec(mainIdx, {});

    os.flush();
    return r;
}


void IRInterp::resolve()
{
    std::unordered_map<std::string, int> ftable;

    for (size_t i = 0; i < prog.funcs.size(); i++)
        ftable[prog.funcs[i].name] = static_cast<int>(i);

    if (!ftable.count("main"))
        throw std::runtime_error("no main function");

    mainIdx = ftable["main"];

    for (IRFunction& f : prog.funcs)
    {
        std::unordered_map<std::string, int> slots;

        for (const std::string& p : f.params) // parameters occupy slots 0..n-1
            slotOf(slots, p);

        std::unordered_map<std::string, int> labels;
        for (size_t i = 0; i < f.code.size(); i++)
            if (f.code[i].kind == InstrKind::Label)
                labels[f.code[i].l1] = static_cast<int>(i);

        for (Instr& i : f.code)
        {
            switch (i.kind)
            {
            case InstrKind::Label: break;
            case InstrKind::Move:
            case InstrKind::Neg:
                i.dstSlot = slotOf(slots, i.dst);
                resolveAtom(slots, i.a);
                break;
            case InstrKind::Bin:
                i.dstSlot = slotOf(slots, i.dst);
                resolveAtom(slots, i.a);
                resolveAtom(slots, i.b);
                break;
            case InstrKind::Call:
                i.dstSlot = slotOf(slots, i.dst);
                for (Atom& a : i.args)
                    resolveAtom(slots, a);
                if (i.callee == "print")
                    i.calleeIdx = -1;
                else if (i.callee == "read")
                    i.calleeIdx = -2;
                else
                {
                    auto it = ftable.find(i.callee);
                    if (it == ftable.end())
                        throw std::runtime_error("undefined function " +
                                                 i.callee);
                    i.calleeIdx = it->second;
                }
                break;
            case InstrKind::If:
                resolveAtom(slots, i.a);
                resolveAtom(slots, i.b);
                i.jump1 = labelTarget(labels, i.l1);
                i.jump2 = labelTarget(labels, i.l2);
                break;
            case InstrKind::Goto:   i.jump1 = labelTarget(labels, i.l1); break;
            case InstrKind::Return: resolveAtom(slots, i.a); break;
            }
        }
        f.numSlots = static_cast<int>(slots.size());
    }
}


int IRInterp::exec(int fn, const std::vector<int>& args)
{
    const IRFunction& f = prog.funcs[fn];
    std::vector<int> loc(f.numSlots, 0);
    for (size_t i = 0; i < args.size(); i++)
        loc[i] = args[i];

    size_t pc = 0;
    while (pc < f.code.size())
    {
        const Instr& i = f.code[pc];
        switch (i.kind)
        {
        case InstrKind::Label: pc++; break;
        case InstrKind::Move:
            loc[i.dstSlot] = atomValue(loc, i.a);
            pc++;
            break;
        case InstrKind::Neg:
            loc[i.dstSlot] =
                static_cast<int>(0u - static_cast<uint>(atomValue(loc, i.a)));
            pc++;
            break;
        case InstrKind::Bin:
            loc[i.dstSlot] =
                arith(i.bop, atomValue(loc, i.a), atomValue(loc, i.b));
            pc++;
            break;
        case InstrKind::Call:
        {
            int result = 0;
            if (i.calleeIdx == -1)
                os << atomValue(loc, i.args[0]) << '\n';
            else if (i.calleeIdx == -2)
            {
                if (!(is >> result))
                    throw std::runtime_error("read past end of input");
            }
            else
            {
                std::vector<int> av;
                av.reserve(i.args.size());
                for (const Atom& a : i.args)
                    av.push_back(atomValue(loc, a)); // first to last
                result = exec(i.calleeIdx, av);
            }
            loc[i.dstSlot] = result;
            pc++;
            break;
        }
        case InstrKind::If:
            pc = compare(i.rop, atomValue(loc, i.a), atomValue(loc, i.b))
                     ? i.jump1
                     : i.jump2;
            break;
        case InstrKind::Goto:   pc = i.jump1; break;
        case InstrKind::Return: return atomValue(loc, i.a);
        }
    }
    return 0;
}


// Signed overflow is undefined in C++, so + - * and negation go through uint.
// INT_MIN / -1 and INT_MIN % -1 trap on x86-64, so a divisor of -1 is handled
// apart.
int IRInterp::arith(BinOp op, int x, int y)
{
    uint ux = static_cast<uint>(x), uy = static_cast<uint>(y);
    switch (op)
    {
    case BinOp::Add: return static_cast<int>(ux + uy);
    case BinOp::Sub: return static_cast<int>(ux - uy);
    case BinOp::Mul: return static_cast<int>(ux * uy);
    case BinOp::Div:
        if (y == 0)
            throw std::runtime_error("division by zero");
        if (y == -1)
            return -x;
        return x / y;
    case BinOp::Rem:
        if (y == 0)
            throw std::runtime_error("division by zero");
        if (y == -1)
            return 0;
        return x % y;
    }
    return 0;
}


bool IRInterp::compare(RelOp op, int x, int y)
{
    switch (op)
    {
    case RelOp::Eq: return x == y;
    case RelOp::Ne: return x != y;
    case RelOp::Lt: return x < y;
    case RelOp::Le: return x <= y;
    case RelOp::Gt: return x > y;
    case RelOp::Ge: return x >= y;
    }
    return false;
}


int IRInterp::slotOf(std::unordered_map<std::string, int>& slots,
                     const std::string& v)
{
    auto it = slots.find(v);

    if (it != slots.end())
        return it->second;
    int s = static_cast<int>(slots.size());
    slots[v] = s;

    return s;
}


void IRInterp::resolveAtom(std::unordered_map<std::string, int>& slots, Atom& a)
{
    if (!a.isNum)
        a.slot = slotOf(slots, a.var);
}


int IRInterp::labelTarget(const std::unordered_map<std::string, int>& labels,
                          const std::string& l)
{
    auto it = labels.find(l);

    if (it == labels.end())
        throw std::runtime_error("undefined label " + l);

    return it->second;
}


int IRInterp::atomValue(const std::vector<int>& loc, const Atom& a)
{
    return a.isNum ? a.value : loc[a.slot];
}
