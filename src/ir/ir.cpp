#include "ir.h"
#include <sstream>
#include <stdexcept>

namespace {

const char* binName(BinOp op)
{
    switch(op)
    {
        case BinOp::Add: return "+";
        case BinOp::Sub: return "-";
        case BinOp::Mul: return "*";
        case BinOp::Div: return "/";
        case BinOp::Rem: return "%";
    }
    return "?";
}

const char* relName(RelOp op)
{
    switch(op)
    {
        case RelOp::Eq: return "==";
        case RelOp::Ne: return "!=";
        case RelOp::Lt: return "<";
        case RelOp::Le: return "<=";
        case RelOp::Gt: return ">";
        case RelOp::Ge: return ">=";
    }
    return "?";
}

bool toBinOp(Terminal t, BinOp& op)
{
    switch(t)
    {
        case PLUS: op = BinOp::Add; return true;
        case MINUS: op = BinOp::Sub; return true;
        case STAR: op = BinOp::Mul; return true;
        case SLASH: op = BinOp::Div; return true;
        case PERCENT: op = BinOp::Rem; return true;
        default:      return false;
    }
}

bool toRelOp(Terminal t, RelOp& op)
{
    switch(t)
    {
        case EQ: op = RelOp::Eq; return true;
        case NE: op = RelOp::Ne; return true;
        case LT: op = RelOp::Lt; return true;
        case LE: op = RelOp::Le; return true;
        case GT: op = RelOp::Gt; return true;
        case GE: op = RelOp::Ge; return true;
        default: return false;
    }
}

} // namespace

std::string IRProgram::toString() const
{
    std::ostringstream os;

    for(const IRFunction& f : funcs)
    {
        os << "FUNCTION " << f.name << "(";
        for(size_t i = 0; i < f.params.size(); i++)
            os << (i ? ", " : "") << f.params[i];
        os << ")\n";

        for(const Instr& i : f.code)
        {
            switch(i.kind)
            {
                case InstrKind::Label:  os << "LABEL " << i.l1 << "\n"; break;
                case InstrKind::Move:   os << i.dst << " := " << i.a.str() << "\n"; break;
                case InstrKind::Neg:    os << i.dst << " := - " << i.a.str() << "\n"; break;
                case InstrKind::Bin:
                    os << i.dst << " := " << i.a.str() << " " << binName(i.bop)
                       << " " << i.b.str() << "\n";
                    break;
                case InstrKind::Call:
                    os << i.dst << " := CALL " << i.callee << "(";
                    for(size_t k = 0; k < i.args.size(); k++)
                        os << (k ? ", " : "") << i.args[k].str();
                    os << ")\n";
                    break;
                case InstrKind::If:
                    os << "IF " << i.a.str() << " " << relName(i.rop) << " " << i.b.str()
                       << " THEN " << i.l1 << " ELSE " << i.l2 << "\n";
                    break;
                case InstrKind::Goto:   os << "GOTO " << i.l1 << "\n"; break;
                case InstrKind::Return: os << "RETURN " << i.a.str() << "\n"; break;
            }
        }
        os << "END\n";
    }

    return os.str();
}


void IR::internalError(const Node* n, const std::string& msg)
{
    throw std::runtime_error("internal error: line " + std::to_string(n->lineNum) + ": " + msg);
}


Atom IR::toVar(Atom a)
{
    if(!a.isNum)
        return a;

    std::string t = newvar();
    write(Instr::move(t, a));

    return Atom::variable(t);
}

// translation
void IR::transProgram()
{
    for(Node* f : prog->children)
        transFun(f);
}

// FunDecl: children = { Type, IDENT, Params, Block }
void IR::transFun(Node* f)
{
    cur = IRFunction();
    cur.name = f->children[1]->token->value;

    vtable.enter();

    for(Node* p : f->children[2]->children) // Param: { Type, IDENT }
    {
        const std::string& var = p->children[1]->token->value;

        std::string irName = rename(var);
        vtable.bind(var, irName);
        cur.params.push_back(irName);
    }
    transStmt(f->children[3]);
    // The type checker guarantees every path returns, but a trailing LABEL
    // (e.g. the join point after an if/else that returns on both sides) would
    // otherwise be the last instruction. This keeps every function well formed.
    write(Instr::ret(Atom::num(0)));

    vtable.exit();

    out->funcs.push_back(std::move(cur));
}

void IR::transStmt(Node* s)
{
    if(s->isSym(Block))
    {
        vtable.enter();

        for(Node* c : s->children)
            transStmt(c);

        vtable.exit();

        return;
    }

    if(s->isSym(Decl)) // { Type, IDENT, init }
    {
        // Translate the initializer BEFORE binding: in `int x = x + 1;` the
        // right-hand x is the outer one (scope starts at the next statement).
        Atom init = transExp(s->children[2]);

        const std::string& var = s->children[1]->token->value;
        std::string irName = rename(var);

        write(Instr::move(irName, init));
        vtable.bind(var, irName);

        return;
    }

    if(s->isSym(Call)) // call statement: result is discarded
    {
        transCall(s);

        return;
    }

    if(!std::holds_alternative<Terminal>(s->symbol))
        internalError(s, "unexpected statement node");

    switch(std::get<Terminal>(s->symbol))
    {
        case ASSIGN: // { IDENT, Exp }
        {
            Atom v = transExp(s->children[1]);

            auto target = vtable.lookup(s->children[0]->token->value);

            if(!target) 
                internalError(s, "assignment to unbound variable");

            write(Instr::move(*target, v));

            return;
        }
        case IF: // { cond, then, [else] }
        {
            std::string lt = newlabel();

            if(s->children.size() == 2)
            {
                std::string lend = newlabel();

                transCond(s->children[0], lt, lend);
                write(Instr::label(lt));

                transStmt(s->children[1]);
                write(Instr::label(lend));
            }
            else
            {
                std::string lf = newlabel(), lend = newlabel();

                transCond(s->children[0], lt, lf);
                write(Instr::label(lt));

                transStmt(s->children[1]);
                write(Instr::jump(lend));
                write(Instr::label(lf));

                transStmt(s->children[2]);
                write(Instr::label(lend));
            }
            return;
        }
        case WHILE: // { cond, body }
        {
            std::string ltest = newlabel(), lbody = newlabel(), lend = newlabel();

            write(Instr::label(ltest));
            transCond(s->children[0], lbody, lend);

            write(Instr::label(lbody));
            transStmt(s->children[1]);

            write(Instr::jump(ltest));
            write(Instr::label(lend));
            return;
        }
        case RETURN:
            write(Instr::ret(transExp(s->children[0])));
            return;
        case PRINT:
        {
            Atom v = transExp(s->children[0]);

            write(Instr::call(newvar(), "print", {v})); // result is a dummy variable
            return;
        }
        default:
            internalError(s, "unexpected statement node");
    }
}

// Call: children = { IDENT, args... }. Arguments are evaluated first to last.
Atom IR::transCall(Node* c)
{
    std::vector<Atom> args;

    for(size_t i = 1; i < c->children.size(); i++)
        args.push_back(transExp(c->children[i]));

    std::string x = newvar();
    write(Instr::call(x, c->children[0]->token->value, std::move(args)));

    return Atom::variable(x);
}

Atom IR::transExp(Node* e)
{
    if(e->isSym(Call))
        return transCall(e);

    if(!std::holds_alternative<Terminal>(e->symbol))
        internalError(e, "unexpected expression node");

    Terminal t = std::get<Terminal>(e->symbol);
    switch(t)
    {
        case NUM:
            return Atom::num(std::stoi(e->token->value));
        case TRUE: 
            return Atom::num(1);
        case FALSE: 
            return Atom::num(0);
        case IDENT:
        {
            auto v = vtable.lookup(e->token->value);

            if(!v) internalError(e, "unbound variable '" + e->token->value + "'");

            return Atom::variable(*v);
        }
        case READ:
        {
            std::string x = newvar();
            write(Instr::call(x, "read", {}));
            return Atom::variable(x);
        }
        default: break;
    }

    // unary minus
    if(t == MINUS && e->children.size() == 1)
    {
        Atom a = transExp(e->children[0]);
        std::string x = newvar();

        write(Instr::neg(x, a));

        return Atom::variable(x);
    }

    // arithmetic
    BinOp bop;

    if(toBinOp(t, bop))
    {
        Atom a = toVar(transExp(e->children[0])); // left operand first
        Atom b = transExp(e->children[1]);

        std::string x = newvar();

        write(Instr::bin(x, a, bop, b));

        return Atom::variable(x);
    }

    // A bool-valued expression used as a value must produce 1 or 0:
    //   x := 1; <cond -> ltrue / lfalse>; LABEL lfalse; x := 0; LABEL ltrue
    RelOp rop;
    if(t == NOT || t == AND || t == OR || toRelOp(t, rop))
    {
        std::string x = newvar();
        std::string ltrue = newlabel(), lfalse = newlabel();

        write(Instr::move(x, Atom::num(1)));
        transCond(e, ltrue, lfalse);

        write(Instr::label(lfalse));
        write(Instr::move(x, Atom::num(0)));
        write(Instr::label(ltrue));

        return Atom::variable(x);
    }

    internalError(e, "unexpected expression node");
}

// Jumps to lt if e is true, to lf if false; && || ! short-circuit (§6.6.1).
void IR::transCond(Node* e, const std::string& lt, const std::string& lf)
{
    if(std::holds_alternative<Terminal>(e->symbol))
    {
        Terminal t = std::get<Terminal>(e->symbol);
        RelOp rop;

        switch(t)
        {
            case TRUE: 
                write(Instr::jump(lt)); return;
            case FALSE: 
                write(Instr::jump(lf)); return;
            case NOT:   
                transCond(e->children[0], lf, lt); return;
            case AND:
            {
                std::string mid = newlabel();

                transCond(e->children[0], mid, lf);
                write(Instr::label(mid));
                transCond(e->children[1], lt, lf);
                return;
            }
            case OR:
            {
                std::string mid = newlabel();

                transCond(e->children[0], lt, mid);
                write(Instr::label(mid));
                transCond(e->children[1], lt, lf);
                return;
            }
            default:
                if(toRelOp(t, rop))
                {
                    Atom a = toVar(transExp(e->children[0]));
                    Atom b = transExp(e->children[1]);

                    write(Instr::cond(a, rop, b, lt, lf));

                    return;
                }
                break;
        }
    }

    // bool variable or call: compare against 0
    Atom a = toVar(transExp(e));
    write(Instr::cond(a, RelOp::Ne, Atom::num(0), lt, lf));
}
