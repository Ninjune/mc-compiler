// File: typechkr.cpp
#include "typechkr.h"


void TypeChecker::run()
{
    try {
        checkProgram();
    }
    catch (TypeError e) 
    {
        error(e.msg, e.line);
        err = true;
    }
}


void TypeChecker::checkProgram() 
{
    ftable.enter();

    // Pass 1: collect all signatures so calls can precede definitions.
    for (Node* f : prog->children) 
    {
        const std::string& name = idName(f->children[1]);
        Node* params = f->children[2];

        if (!ftable.bind(name, typeOf(f->children[0])))
            fail(f, "function '" + name + "' is already defined");

        if (params->children.size() > 8)
            fail(f, "function '" + name + "' has more than 8 parameters");

        std::vector<Type> pts;
        std::unordered_set<std::string> seenParams;

        for (Node* p : params->children) 
        {
            const std::string& pn = idName(p->children[1]);

            if (!seenParams.insert(pn).second)
                fail(p, "duplicate parameter '" + pn + "' in function '" + name + "'");

            pts.push_back(typeOf(p->children[0]));
        }
        paramTypes[name] = std::move(pts);
    }

    // main must exist: int main() with no parameters.
    auto mainRet = ftable.lookup("main");

    if (!mainRet || *mainRet != Type::INT || !paramTypes["main"].empty())
        fail(prog, "program must define 'int main()' with no parameters");

    // Pass 2: check bodies.
    for (Node* f : prog->children) 
        checkFun(f);
}

void TypeChecker::checkFun(Node* fun)
{
    currentReturn = typeOf(fun->children[0]);
    Node* params = fun->children[2];
    Node* body = fun->children[3];

    // Params and the body's outermost declarations share one scope.
    vtable.enter();

    for (Node* p : params->children)
        vtable.bind(idName(p->children[1]), typeOf(p->children[0]));

    for (Node* item : body->children) 
        checkStmt(item);

    vtable.exit();

    if (!alwaysReturns(body))
        fail(fun, "function '" + idName(fun->children[1]) + "' may reach the end without returning");
}

void TypeChecker::checkStmt(Node* stmt)
{
    if (auto* nt = std::get_if<NonTerminal>(&stmt->symbol)) 
    {
        switch (*nt) 
        {
        case Block:
            vtable.enter();
            for (Node* c : stmt->children) checkStmt(c);
            vtable.exit();
            return;
        case Decl: {
            Type declaredType = typeOf(stmt->children[0]);
            const std::string& x = idName(stmt->children[1]);
            Type initType = checkExp(stmt->children[2]); // before bind: x not in scope yet

            if (initType != declaredType)
                fail(stmt, std::string("cannot initialize ") + name(declaredType) +
                        " variable '" + x + "' with " + name(initType));

            if (!vtable.bind(x, declaredType))
                fail(stmt, "variable '" + x + "' is already declared in this block");

            return;
        }
        case Call: // call statement; result discarded
            checkExp(stmt);
            return;

        default: break;
        }
    } 
    else 
    {
        switch (std::get<Terminal>(stmt->symbol)) 
        {
        case ASSIGN: {
            const std::string& x = idName(stmt->children[0]);
            auto vt = vtable.lookup(x);

            if (!vt) 
                    fail(stmt, "undeclared variable '" + x + "'");

            Type rhs = checkExp(stmt->children[1]);

            if (rhs != *vt)
                fail(stmt, std::string("cannot assign ") + name(rhs) + " to " + name(*vt) +
                        " variable '" + x + "'");

            return;
            }
        case IF:
            expect(stmt->children[0], Type::BOOL, "if condition");
            checkStmt(stmt->children[1]);

            if (stmt->children.size() == 3)
                checkStmt(stmt->children[2]);

            return;
        case WHILE:
            expect(stmt->children[0], Type::BOOL, "while condition");
            checkStmt(stmt->children[1]);
            return;
        case RETURN:
            expect(stmt->children[0], currentReturn, "return value");
            return;
        case PRINT:
            expect(stmt->children[0], Type::INT, "print argument");
            return;
        default: break;
        }
    }

    fail(stmt, "internal error: unexpected statement node");
}

TypeChecker::Type TypeChecker::checkExp(Node* exp)
{
    if (isNT(exp, Call)) 
        return record(exp, checkCall(exp));

    auto* terminal = std::get_if<Terminal>(&exp->symbol);
    if (!terminal) fail(exp, "internal error: unexpected expression node");

    switch (*terminal) {
    case NUM:
    case READ:  return record(exp, Type::INT);
    case TRUE:
    case FALSE: return record(exp, Type::BOOL);
    case IDENT: {
        auto t = vtable.lookup(idName(exp));

        if (!t)
            fail(exp, "undeclared variable '" + idName(exp) + "'");

        return record(exp, *t);
    }
    case NOT:
        expect(exp->children[0], Type::BOOL, "operand of '!'");
        return record(exp, Type::BOOL);
    case MINUS:
        if (exp->children.size() == 1) 
        {
            expect(exp->children[0], Type::INT, "operand of unary '-'");
            return record(exp, Type::INT);
        }
        [[fallthrough]];
    case PLUS: case STAR: case SLASH: case PERCENT:
        expect(exp->children[0], Type::INT, "left operand of arithmetic operator");
        expect(exp->children[1], Type::INT, "right operand of arithmetic operator");
        return record(exp, Type::INT);
    case LT: case LE: case GT: case GE:
        expect(exp->children[0], Type::INT, "left operand of comparison");
        expect(exp->children[1], Type::INT, "right operand of comparison");
        return record(exp, Type::BOOL);
    case EQ: case NE: {
        Type l = checkExp(exp->children[0]);
        Type r = checkExp(exp->children[1]);

        if (l != r)
            fail(exp, std::string("cannot compare ") + name(l) + " with " + name(r));

        return record(exp, Type::BOOL);
    }
    case AND: case OR:
        expect(exp->children[0], Type::BOOL, "left operand of logical operator");
        expect(exp->children[1], Type::BOOL, "right operand of logical operator");
        return record(exp, Type::BOOL);
    default: break;
    }

    fail(exp, "internal error: unexpected expression node");
}

TypeChecker::Type TypeChecker::checkCall(Node* call)
{
    const std::string& f = idName(call->children[0]);

    auto ret = ftable.lookup(f);

    if (!ret) 
        fail(call, "undefined function '" + f + "'");

    const std::vector<Type>& params = paramTypes[f];

    unsigned int nargs = call->children.size() - 1;

    if (nargs != params.size())
        fail(call, "function '" + f + "' expects " + std::to_string(params.size()) +
                   " arguments but got " + std::to_string(nargs));

    for (unsigned int i = 0; i < nargs; i++) 
    {
        Node* arg = call->children[i + 1];
        Type got = checkExp(arg);

        if (got != params[i])
            fail(arg, "argument " + std::to_string(i + 1) + " of '" + f + "' must be " +
                      name(params[i]) + " but is " + name(got));
    }
    return *ret;

}

bool TypeChecker::alwaysReturns(const Node* s) 
{
    if (isT(s, RETURN)) 
        return true;

    if (isT(s, IF))
        return s->children.size() == 3 &&
            alwaysReturns(s->children[1]) && 
            alwaysReturns(s->children[2]);

    if (isNT(s, Block))
        return !s->children.empty() && alwaysReturns(s->children.back());

    return false;
}

void TypeChecker::fail(const Node* n, const std::string& msg)
{
    throw TypeError{n->lineNum, msg};
}
