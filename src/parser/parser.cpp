#include "parser.h"


std::string Parser::spell(Terminal k)
{
    return "'" + symToStr(k) + "'";
}


std::string Parser::spell(const Token& t)
{
    if(t.kind == IDENT || t.kind == NUM) 
        return "'" + t.value + "'";
    return spell(t.kind);
}


// driver / helpers 
std::string Parser::nodeToStr(Node* n)
{
    if(std::holds_alternative<NonTerminal>(n->symbol))
    {
        switch(std::get<NonTerminal>(n->symbol))
        {
            case Program:  return "program";
            case FunDecl:  return "fun";
            case Params:   return "params";
            case Param:    return "";
            case Block:    return "block";
            case Decl:     return "decl";
            case Args:     return "args";
            case Call:     return "call";
            default:       return symToStr(n->symbol);
        }
    }

    if(std::get<Terminal>(n->symbol) == MINUS && n->children.size() == 1)
        return "neg";

    if(n->token.has_value() && !n->token->value.empty())
        return n->token->value;

    return symToStr(std::get<Terminal>(n->symbol));
}

std::string Parser::getString(Node* node)
{
    if(node == nullptr)
        return "";
    if(holdsLeaf(node))
        return nodeToStr(node);

    std::string out = "(" + nodeToStr(node);

    bool first = true;
    for(Node* c : node->children)
    {
        if(std::holds_alternative<NonTerminal>(node->symbol) &&
            std::get<NonTerminal>(node->symbol) == Param)
        {
            if(first)
                out += getString(c);
            else
                out += " " + getString(c);
        }
        else
            out += " " + getString(c);
        first = false;
    }

    return out + ")";
}

Node* Parser::parse()
{
    try { return parseS(); }
    catch(const SyntaxError& e)
    {
        error(e.msg, e.line); // e.g. "expected ';' but found 'else'" on line 4
        return nullptr;       // partial tree is leaked; the program exits anyway
    }
}

Token Parser::next()
{
    Token t = input->front();
    if(t.kind != _EOF)
        input->pop(); // never run past the end marker
    return t;
}

void Parser::fail(const std::string& expected)
{
    throw SyntaxError{"expected " + expected + " but found " + spell(peek()), peek().line};
}

Token Parser::expect(Terminal k)
{
    if(peek().kind != k)
        fail(spell(k));
    return next();
}

Node* Parser::leaf(const Token& t)
{
    Node* n = new Node(t.line, t.kind);
    n->token = t;
    return n;
}

// Makes a node with the given children (null entries are skipped).
Node* Parser::node(NonTerminal nt, int line, const std::vector<Node*>& kids)
{
    Node* n = new Node(line, nt);
    for(Node* k : kids)
        if(k) n->children.push_back(k);
    return n;
}

// Operator token becomes the node itself, operands become its children.
static Node* opNode(Node* op, Node* lhs, Node* rhs)
{
    op->children = {lhs, rhs};
    return op;
}

// recursive descent parser
// S -> Program _EOF (returns the Program node. _EOF is dropped)
Node* Parser::parseS()
{
    Node* prog = parseProgram();
    expect(_EOF);
    return prog;
}

/* Program -> FunDecls

          Program
        /    |    \
   FunDecl FunDecl ...
*/
Node* Parser::parseProgram()
{
    Node* prog = new Node(peek().line, Program);
    parseFunDecls(prog);
    return prog;
}

// FunDecls -> FunDecl FunDecls | eps      (FOLLOW = { _EOF })
// appends each FunDecl to parent's children
void Parser::parseFunDecls(Node* parent)
{
    if(peek().kind != INT && peek().kind != BOOL)
        return;
    parent->children.push_back(parseFunDecl());
    parseFunDecls(parent);
}

/* FunDecl -> Type IDENT ( ParamsOpt ) Block

               FunDecl
          /      |      |      \
       Type   IDENT   Params   Block
*/
Node* Parser::parseFunDecl()
{
    Node* type = parseType();
    Node* name = leaf(expect(IDENT));
    expect(LPAREN);
    Node* params = parseParamsOpt();
    expect(RPAREN);
    Node* body = parseBlock();
    return node(FunDecl, name->lineNum, {type, name, params, body});
}

/* 
ParamsOpt -> Params | eps (FOLLOW = { ) })
The Params node always exists, even with no parameters.

        Params                 Params (empty)
       /   |   \
   Param Param ...
*/
Node* Parser::parseParamsOpt()
{
    Node* params = new Node(peek().line, Params);
    if(peek().kind == INT || peek().kind == BOOL)
        parseParams(params);
    return params;
}

// Params -> Param ParamsTail
void Parser::parseParams(Node* parent)
{
    parent->children.push_back(parseParam());
    parseParamsTail(parent);
}

// ParamsTail -> , Param ParamsTail | eps      (FOLLOW = { ) })
void Parser::parseParamsTail(Node* parent)
{
    if(peek().kind != COMMA)
        return;
    next();
    parent->children.push_back(parseParam());
    parseParamsTail(parent);
}

/*
Param -> Type IDENT

        Param
        /   \
     Type   IDENT
*/
Node* Parser::parseParam()
{
    Node* type = parseType();
    Node* name = leaf(expect(IDENT));
    return node(Param, name->lineNum, {type, name});
}

/*
Type -> int | bool          (no node of its own: the keyword token is the node)

     INT  (or BOOL)
*/
Node* Parser::parseType()
{
    if(peek().kind != INT && peek().kind != BOOL)
        fail("'int' or 'bool'");
    return leaf(next());
}

/*
Block -> { Items }

           Block
         /   |   \
      Item Item  ...      (each Item is a Decl or a statement)
*/
Node* Parser::parseBlock()
{
    Node* block = new Node(expect(LBRACE).line, Block);
    parseItems(block);
    expect(RBRACE);
    return block;
}

// Items -> Item Items | eps          (FOLLOW = { } })
void Parser::parseItems(Node* parent)
{
    if(peek().kind == RBRACE)
        return;
    parent->children.push_back(parseItem());
    parseItems(parent);
}

// Item -> Decl | Stmt          (FIRST(Decl) = { int, bool }; no node of its own)
Node* Parser::parseItem()
{
    if(peek().kind == INT || peek().kind == BOOL)
        return parseDecl();
    return parseStmt();
}

/*
Decl -> Type IDENT = Exp ;

              Decl
          /     |     \
       Type   IDENT   init: Exp
*/
Node* Parser::parseDecl()
{
    Node* type = parseType();
    Node* name = leaf(expect(IDENT));
    expect(ASSIGN);
    Node* init = parseExp();
    expect(SEMI);
    return node(Decl, name->lineNum, {type, name, init});
}

// Stmt -> IDENT StmtRest | IF ( Exp ) Stmt ElsePart | WHILE ( Exp ) Stmt
//       | RETURN Exp ; | PRINT ( Exp ) ; | Block
Node* Parser::parseStmt()
{
    switch(peek().kind)
    {
        case IDENT:
            return parseStmtRest(leaf(next()));
        case IF: // IF ( Exp ) Stmt ElsePart
        {
            /*
                         IF
                    /    |     \
                 Exp    Stmt   Stmt     (else only if present)
            */
            Node* keyword = leaf(next());
            expect(LPAREN);
            Node* cond = parseExp();
            expect(RPAREN);
            Node* then = parseStmt();
            Node* els = parseElsePart(); // nullptr if there is no else
            keyword->children = {cond, then};
            if(els) keyword->children.push_back(els);
            return keyword;
        }
        case WHILE: // WHILE ( Exp ) Stmt
        {
            /*
                     WHILE
                    /     \
                  Exp    Stmt
            */
            Node* keyword = leaf(next());
            expect(LPAREN);
            Node* cond = parseExp();
            expect(RPAREN);
            Node* body = parseStmt();
            return opNode(keyword, cond, body);
        }
        case RETURN: // RETURN Exp ;
        {
            /*
                  RETURN
                     |
                    Exp
            */
            Node* keyword = leaf(next());
            keyword->children.push_back(parseExp());
            expect(SEMI);
            return keyword;
        }
        case PRINT: // PRINT ( Exp ) ;
        {
            /*
                  PRINT
                    |
                   Exp
            */
            Node* keyword = leaf(next());
            expect(LPAREN);
            keyword->children.push_back(parseExp());
            expect(RPAREN);
            expect(SEMI);
            return keyword;
        }
        case LBRACE: // Block (same tree as parseBlock)
            return parseBlock();
        default:
            fail("statement");
    }
}

// StmtRest -> = Exp ; | ( ArgsOpt ) ; (IDENT was already consumed: `name`)
Node* Parser::parseStmtRest(Node* name)
{
    if(peek().kind == ASSIGN)
    {
        /*
                  =
                /   \
             IDENT  Exp
        */
        Node* eq = leaf(next());
        Node* rhs = parseExp();
        expect(SEMI);
        return opNode(eq, name, rhs);
    }
    if(peek().kind == LPAREN)
    {
        Node* call = parseCall(name); // call statement; tree is shown there
        expect(SEMI);
        return call;
    }
    fail("'=' or '('");
}

// ElsePart -> ELSE Stmt | eps
// Taking ELSE whenever it is next is what binds it to the nearest IF.
Node* Parser::parseElsePart()
{
    if(peek().kind != ELSE)
        return nullptr;
    next();
    return parseStmt();
}

/*
ArgsOpt -> Args | eps

           parent
         /  |  \
      Exp  Exp  ...
*/
void Parser::parseArgsOpt(Node* parent)
{
    if(peek().kind != RPAREN)
        parseArgs(parent);
}

// Args -> Exp ArgsTail
void Parser::parseArgs(Node* parent)
{
    parent->children.push_back(parseExp());
    parseArgsTail(parent);
}

// ArgsTail -> , Exp ArgsTail | eps
void Parser::parseArgsTail(Node* parent)
{
    if(peek().kind != COMMA)
        return;
    next();
    parent->children.push_back(parseExp());
    parseArgsTail(parent);
}


Node* Parser::parseExp()
{
    return parseExpTail(parseAndExp());
}

// ExpTail -> OR AndExp ExpTail | eps
Node* Parser::parseExpTail(Node* lhs)
{
    if(peek().kind != OR)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseAndExp();
    return parseExpTail(opNode(op, lhs, rhs));
}

// AndExp -> EqExp AndTail
Node* Parser::parseAndExp()
{
    return parseAndTail(parseEqExp());
}

// AndTail -> AND EqExp AndTail | eps
Node* Parser::parseAndTail(Node* lhs)
{
    if(peek().kind != AND)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseEqExp();
    return parseAndTail(opNode(op, lhs, rhs));
}

// EqExp -> RelExp EqTail
Node* Parser::parseEqExp()
{
    return parseEqTail(parseRelExp());
}

// EqTail -> EQ RelExp | NE RelExp | eps (at most one; not chained)
Node* Parser::parseEqTail(Node* lhs)
{
    if(peek().kind != EQ && peek().kind != NE)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseRelExp();
    return opNode(op, lhs, rhs);
}

// RelExp -> AddExp RelTail
Node* Parser::parseRelExp()
{
    return parseRelTail(parseAddExp());
}

// RelTail -> LT AddExp | LE AddExp | GT AddExp | GE AddExp | eps
Node* Parser::parseRelTail(Node* lhs)
{
    if(peek().kind != LT && peek().kind != LE &&
       peek().kind != GT && peek().kind != GE)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseAddExp();
    return opNode(op, lhs, rhs);
}

// AddExp -> MulExp AddTail
Node* Parser::parseAddExp()
{
    return parseAddTail(parseMulExp());
}

// AddTail -> PLUS MulExp AddTail | MINUS MulExp AddTail | eps
Node* Parser::parseAddTail(Node* lhs)
{
    if(peek().kind != PLUS && peek().kind != MINUS)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseMulExp();
    return parseAddTail(opNode(op, lhs, rhs));
}

// MulExp -> Unary MulTail
Node* Parser::parseMulExp()
{
    return parseMulTail(parseUnary());
}

// MulTail -> STAR Unary MulTail | SLASH Unary MulTail | PERCENT Unary MulTail | eps
Node* Parser::parseMulTail(Node* lhs)
{
    if(peek().kind != STAR && peek().kind != SLASH && peek().kind != PERCENT)
        return lhs;
    Node* op = leaf(next());
    Node* rhs = parseUnary();
    return parseMulTail(opNode(op, lhs, rhs));
}

/* 
Unary -> MINUS Unary | NOT Unary | Primary

     MINUS  (or NOT)
        |
     operand
*/
Node* Parser::parseUnary()
{
    if(peek().kind == MINUS || peek().kind == NOT)
    {
        Node* op = leaf(next());
        op->children.push_back(parseUnary());
        return op;
    }
    return parsePrimary();
}

// Primary -> NUM | TRUE | FALSE | IDENT Call | READ ( ) | ( Exp )
Node* Parser::parsePrimary()
{
    switch(peek().kind)
    {
        case NUM: case TRUE: case FALSE: // a single leaf
            return leaf(next());
        case IDENT:
            return parseCall(leaf(next())); // plain variable or a call
        case READ: // READ ( ) a single READ node with no children
        {
            Node* keyword = leaf(next());
            expect(LPAREN);
            expect(RPAREN);
            return keyword;
        }
        case LPAREN: // ( Exp ) grouping only, no node of its own
        {
            next();
            Node* e = parseExp();
            expect(RPAREN);
            return e;
        }
        default:
            fail("expression");
    }
}

/*
Call -> ( ArgsOpt ) | eps (IDENT was already consumed: `name`)
with no '(' the identifier is just a variable and comes back unchanged.

              Call
         /      |  \     \
       IDENT   Exp  Exp  ...
*/
Node* Parser::parseCall(Node* name)
{
    if(peek().kind != LPAREN)
        return name;
    next();
    Node* callrest = node(Call, name->lineNum, {name});
    parseArgsOpt(callrest);
    expect(RPAREN);
    return callrest;
}


bool Parser::holdsLeaf(const Node* n)
{
    return n->children.empty() &&
           std::holds_alternative<Terminal>(n->symbol) &&
           std::get<Terminal>(n->symbol) != READ;
}
