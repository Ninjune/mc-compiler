#pragma once
#include <string>
#include <utility>
#include <vector>
#include "../node.h"
#include "../stable.h"

// An atom is a variable or an integer constant.
struct Atom
{
    bool isNum = true;
    int value = 0;
    std::string var;
    int slot = -1; // frame slot, filled in by the interpreter

    static Atom num(int v)
    {
        Atom a;
        a.isNum = true;
        a.value = v;
        return a;
    }

    static Atom variable(const std::string& n)
    {
        Atom a;
        a.isNum = false;
        a.var = n;
        return a;
    }

    [[nodiscard]] std::string str() const
    {
        return isNum ? std::to_string(value) : var;
    }
};

enum class BinOp { Add, Sub, Mul, Div, Rem };
enum class RelOp { Eq, Ne, Lt, Le, Gt, Ge };
enum class InstrKind { Label, Move, Neg, Bin, Call, If, Goto, Return };

struct Instr
{
    Instr(InstrKind kind)
        : kind(kind)
    {}
    InstrKind kind = InstrKind::Label;
    std::string dst; // Move, Neg, Bin, Call
    Atom a, b;       // operands (a is always a variable for Bin and If)
    BinOp bop = BinOp::Add;
    RelOp rop = RelOp::Eq;
    std::string l1, l2;     // Label/Goto: l1.  If: l1 = THEN, l2 = ELSE
    std::string callee;     // Call
    std::vector<Atom> args; // Call

    // Resolved by the interpreter before running.
    int dstSlot = -1;
    int jump1 = -1, jump2 = -1;
    int calleeIdx = -1; // >= 0 user function, -1 print, -2 read

    static Instr label(const std::string& l)
    {
        Instr i(InstrKind::Label);
        i.l1 = l;
        return i;
    }

    static Instr move(const std::string& x, Atom a)
    {
        Instr i(InstrKind::Move);
        i.dst = x;
        i.a = std::move(a);
        return i;
    }

    static Instr neg(const std::string& x, Atom a)
    {
        Instr i(InstrKind::Neg);
        i.dst = x;
        i.a = std::move(a);
        return i;
    }

    static Instr bin(const std::string& x, Atom y, BinOp op, Atom z)
    {
        Instr i(InstrKind::Bin);
        i.dst = x;
        i.a = std::move(y);
        i.bop = op;
        i.b = std::move(z);
        return i;
    }

    static Instr call(const std::string& x, const std::string& f,
                      std::vector<Atom> args)
    {
        Instr i(InstrKind::Call);
        i.dst = x;
        i.callee = f;
        i.args = std::move(args);
        return i;
    }

    static Instr cond(Atom x, RelOp op, Atom y, const std::string& t,
                      const std::string& f)
    {
        Instr i(InstrKind::If);
        i.a = std::move(x);
        i.rop = op;
        i.b = std::move(y);
        i.l1 = t;
        i.l2 = f;
        return i;
    }

    static Instr jump(const std::string& l)
    {
        Instr i(InstrKind::Goto);
        i.l1 = l;
        return i;
    }

    static Instr ret(Atom a)
    {
        Instr i(InstrKind::Return);
        i.a = std::move(a);
        return i;
    }
};

struct IRFunction
{
    std::string name;
    std::vector<std::string> params;
    std::vector<Instr> code;
    int numSlots = 0; // filled in by the interpreter
};

struct IRProgram
{
    std::vector<IRFunction> funcs;

    std::string toString() const;
};

class IR
{
public:
    explicit IR(Node* in)
        : prog(in), out(new IRProgram())
    {
        run();
    }

    IRProgram* program() { return out; }
private:
    void run() { transProgram(); }

    [[noreturn]] static void internalError(const Node* n,
                                           const std::string& msg);

    std::string newvar() { return "t" + std::to_string(++tmpCount); }
    std::string newlabel() { return "l" + std::to_string(++lblCount); }
    // translates a var to ir name
    std::string rename(const std::string& src)
    {
        return src + "_" + std::to_string(++varCount);
    }

    void write(Instr i) { cur.code.push_back(std::move(i)); }
    Atom toVar(Atom a); // makes sure an atom is a variable (needed on the left
                        // of binop / relop)

    void transProgram();
    void transFun(Node* f);
    void transStmt(Node* s);
    Atom transExp(Node* e);
    Atom transCall(Node* c);
    void transCond(Node* e, const std::string& lt, const std::string& lf);

    Node* prog;
    IRProgram* out;
    IRFunction cur;             // function being built
    STable<std::string> vtable; // source variable -> IR name
    int tmpCount = 0, lblCount = 0, varCount = 0;
};
