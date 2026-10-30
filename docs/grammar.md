# Grammar

## Original grammar

The grammar is given in EBNF: { . . . } means zero or more repetitions, [ . . . ] means optional, and
terminals are quoted. ID and NUM are identifier and integer tokens.

Program ::= FunDecl { FunDecl }
FunDecl ::= Type ID "(" [ Params ] ")" Block
Params ::= Param { "," Param }
Param ::= Type ID
Type ::= "int" | "bool"
Block ::= "{" { Item } "}"
Item ::= Decl | Stmt -- declarations only in blocks
Decl ::= Type ID "=" Exp ";" -- declaration
Stmt ::= ID "=" Exp ";" -- assignment
    | ID "(" [ Args ] ")" ";" -- call statement
    | "if" "(" Exp ")" Stmt [ "else" Stmt ]
    | "while" "(" Exp ")" Stmt
    | "return" Exp ";"
    | "print" "(" Exp ")" ";"
    | Block
Exp ::= AndExp { "||" AndExp }
AndExp ::= EqExp { "&&" EqExp }
EqExp ::= RelExp [ ("==" | "!=") RelExp ]
RelExp ::= AddExp [ ("<" | "<=" | ">" | ">=") AddExp ]
AddExp ::= MulExp { ("+" | "-") MulExp }
MulExp ::= Unary { ("*" | "/" | "%") Unary }
Unary ::= ("-" | "!") Unary | Primary
Primary ::= NUM | "true" | "false"
    | ID | ID "(" [ Args ] ")"
    | "read" "(" ")"
    | "(" Exp ")"
Args ::= Exp { "," Exp }

## Rewritten LL(1) grammar:

S → Program $
Program → FunDecls
FunDecls → FunDecl FunDecls | ε
FunDecl → Type ID ( ParamsOpt ) Block
ParamsOpt → Params | ε
Params → Param ParamsTail
ParamsTail → , Param ParamsTail | ε
Param → Type ID
Type → int | bool
Block → { Items }
Items → Item Items | ε
Item → Decl | Stmt
Decl → Type ID = Exp ;
Stmt → ID StmtRest | if ( Exp ) Stmt ElsePart | while ( Exp ) Stmt
    | return Exp ; | print ( Exp ) ; | Block
StmtRest → = Exp ; | ( ArgsOpt ) ;
ElsePart → else Stmt | ε
ArgsOpt → Args | ε
Args → Exp ArgsTail
ArgsTail → , Exp ArgsTail | ε
Exp → AndExp ExpTail
ExpTail → || AndExp ExpTail | ε
AndExp → EqExp AndTail
AndTail → && EqExp AndTail | ε
EqExp → RelExp EqTail
EqTail → == RelExp | != RelExp | ε
RelExp → AddExp RelTail
RelTail → < AddExp | <= AddExp | > AddExp | >= AddExp | ε
AddExp → MulExp AddTail
AddTail → + MulExp AddTail | - MulExp AddTail | ε
MulExp → Unary MulTail
MulTail → * Unary MulTail | / Unary MulTail
    | % Unary MulTail | ε
Unary → - Unary | ! Unary | Primary
Primary → NUM | true | false | ID CallRest | read ( ) | ( Exp )
CallRest → ( ArgsOpt ) | ε

## generated tables for the above grammar:

Using mcc --tables a

First: 
MulExp: {LPAREN, NUM, IDENT, READ, FALSE, TRUE, MINUS, NOT}
RPAREN: {RPAREN}
AddTail: {MINUS, PLUS, ε}
LPAREN: {LPAREN}
AddExp: {NOT, MINUS, TRUE, FALSE, READ, IDENT, NUM, LPAREN}
ASSIGN: {ASSIGN}
RelTail: {GT, LE, LT, GE, ε}
NOT: {NOT}
RelExp: {LPAREN, NUM, IDENT, READ, FALSE, TRUE, MINUS, NOT}
OR: {OR}
EqTail: {NE, EQ, ε}
AND: {AND}
NE: {NE}
EqExp: {NOT, MINUS, TRUE, FALSE, READ, IDENT, NUM, LPAREN}
EQ: {EQ}
AndTail: {AND, ε}
GE: {GE}
AndExp: {LPAREN, NUM, IDENT, READ, FALSE, TRUE, MINUS, NOT}
GT: {GT}
ExpTail: {OR, ε}
LE: {LE}
Exp: {NOT, MINUS, TRUE, FALSE, READ, IDENT, NUM, LPAREN}
LT: {LT}
ArgsTail: {COMMA, ε}
PERCENT: {PERCENT}
Args: {LPAREN, NUM, IDENT, READ, FALSE, TRUE, MINUS, NOT}
SLASH: {SLASH}
ArgsOpt: {NOT, MINUS, TRUE, READ, IDENT, NUM, LPAREN, FALSE, ε}
STAR: {STAR}
ElsePart: {ELSE, ε}
MINUS: {MINUS}
StmtRest: {LPAREN, ASSIGN}
INT: {INT}
Program: {INT, BOOL, ε}
BOOL: {BOOL}
FunDecls: {BOOL, INT, ε}
IF: {IF}
FunDecl: {INT, BOOL}
ELSE: {ELSE}
ParamsOpt: {INT, BOOL, ε}
WHILE: {WHILE}
Params: {BOOL, INT}
RETURN: {RETURN}
ParamsTail: {COMMA, ε}
TRUE: {TRUE}
Param: {INT, BOOL}
FALSE: {FALSE}
Type: {BOOL, INT}
PRINT: {PRINT}
Block: {LBRACE}
READ: {READ}
Items: {INT, BOOL, IDENT, PRINT, RETURN, WHILE, IF, LBRACE, ε}
IDENT: {IDENT}
Item: {LBRACE, IF, WHILE, RETURN, PRINT, IDENT, BOOL, INT}
NUM: {NUM}
Decl: {INT, BOOL}
PLUS: {PLUS}
Stmt: {IDENT, PRINT, RETURN, WHILE, IF, LBRACE}
MulTail: {PERCENT, SLASH, STAR, ε}
LBRACE: {LBRACE}
Unary: {NOT, MINUS, TRUE, FALSE, READ, IDENT, NUM, LPAREN}
RBRACE: {RBRACE}
Primary: {LPAREN, NUM, IDENT, READ, FALSE, TRUE}
COMMA: {COMMA}
CallRest: {LPAREN, ε}
SEMI: {SEMI}
ε: {ε}
EOF: {EOF}
S: {EOF, BOOL, INT}

Follow: 
CallRest: {GE, SEMI, GT, STAR, SLASH, RPAREN, PERCENT, NE, OR, AND, PLUS, MINUS, EQ, LT, LE, COMMA}
Primary: {NE, OR, AND, PERCENT, SLASH, RPAREN, STAR, PLUS, MINUS, EQ, LT, LE, COMMA, GT, SEMI, GE}
Unary: {PLUS, MINUS, EQ, STAR, SLASH, RPAREN, PERCENT, LT, LE, COMMA, GT, SEMI, GE, AND, OR, NE}
MulTail: {PLUS, MINUS, EQ, NE, OR, RPAREN, AND, GE, SEMI, GT, COMMA, LE, LT}
MulExp: {LT, LE, COMMA, GT, SEMI, GE, AND, RPAREN, OR, NE, EQ, MINUS, PLUS}
Decl: {INT, BOOL, IF, RBRACE, WHILE, RETURN, PRINT, IDENT, LBRACE}
Item: {LBRACE, IDENT, PRINT, RETURN, RBRACE, WHILE, IF, BOOL, INT}
Items: {RBRACE}
Block: {ELSE, LBRACE, IDENT, EOF, PRINT, RETURN, RBRACE, WHILE, IF, INT, BOOL}
Type: {IDENT}
Param: {RPAREN, COMMA}
ParamsTail: {RPAREN}
Params: {RPAREN}
ParamsOpt: {RPAREN}
FunDecl: {EOF, BOOL, INT}
FunDecls: {EOF}
Program: {EOF}
S: {EOF}
Stmt: {INT, BOOL, IF, RBRACE, WHILE, RETURN, PRINT, IDENT, LBRACE, ELSE}
StmtRest: {IDENT, PRINT, RETURN, RBRACE, WHILE, IF, BOOL, INT, LBRACE, ELSE}
ElsePart: {IDENT, PRINT, RETURN, RBRACE, WHILE, IF, BOOL, INT, LBRACE, ELSE}
ArgsOpt: {RPAREN}
Args: {RPAREN}
ArgsTail: {RPAREN}
Exp: {COMMA, SEMI, RPAREN}
ExpTail: {RPAREN, SEMI, COMMA}
AndExp: {RPAREN, SEMI, COMMA, OR}
AndTail: {OR, COMMA, SEMI, RPAREN}
EqExp: {OR, COMMA, SEMI, RPAREN, AND}
EqTail: {AND, RPAREN, SEMI, COMMA, OR}
RelExp: {AND, RPAREN, SEMI, COMMA, OR, NE, EQ}
RelTail: {EQ, NE, OR, COMMA, SEMI, RPAREN, AND}
AddExp: {EQ, NE, OR, RPAREN, AND, GE, SEMI, GT, COMMA, LE, LT}
AddTail: {LT, LE, COMMA, GT, SEMI, GE, AND, RPAREN, OR, NE, EQ}

Parse table: 
M(CallRest, SEMI): {[ε]}
M(CallRest, COMMA): {[ε]}
M(CallRest, PLUS): {[ε]}
M(CallRest, MINUS): {[ε]}
M(CallRest, STAR): {[ε]}
M(CallRest, SLASH): {[ε]}
M(CallRest, PERCENT): {[ε]}
M(CallRest, LT): {[ε]}
M(CallRest, LE): {[ε]}
M(CallRest, GT): {[ε]}
M(CallRest, GE): {[ε]}
M(CallRest, EQ): {[ε]}
M(CallRest, NE): {[ε]}
M(CallRest, AND): {[ε]}
M(CallRest, OR): {[ε]}
M(CallRest, LPAREN): {[LPAREN ArgsOpt RPAREN]}
M(CallRest, RPAREN): {[ε]}
M(Primary, NUM): {[NUM]}
M(Primary, IDENT): {[IDENT CallRest]}
M(Primary, READ): {[READ LPAREN RPAREN]}
M(Primary, FALSE): {[FALSE]}
M(Primary, TRUE): {[TRUE]}
M(Primary, LPAREN): {[LPAREN Exp RPAREN]}
M(Unary, NUM): {[Primary]}
M(Unary, IDENT): {[Primary]}
M(Unary, READ): {[Primary]}
M(Unary, FALSE): {[Primary]}
M(Unary, TRUE): {[Primary]}
M(Unary, MINUS): {[MINUS Unary]}
M(Unary, NOT): {[NOT Unary]}
M(Unary, LPAREN): {[Primary]}
M(MulTail, SEMI): {[ε]}
M(MulTail, COMMA): {[ε]}
M(MulTail, PLUS): {[ε]}
M(MulTail, MINUS): {[ε]}
M(MulTail, STAR): {[STAR Unary MulTail]}
M(MulTail, SLASH): {[SLASH Unary MulTail]}
M(MulTail, PERCENT): {[PERCENT Unary MulTail]}
M(MulTail, LT): {[ε]}
M(MulTail, LE): {[ε]}
M(MulTail, GT): {[ε]}
M(MulTail, GE): {[ε]}
M(MulTail, EQ): {[ε]}
M(MulTail, NE): {[ε]}
M(MulTail, AND): {[ε]}
M(MulTail, OR): {[ε]}
M(MulTail, RPAREN): {[ε]}
M(MulExp, NUM): {[Unary MulTail]}
M(MulExp, IDENT): {[Unary MulTail]}
M(MulExp, READ): {[Unary MulTail]}
M(MulExp, FALSE): {[Unary MulTail]}
M(MulExp, TRUE): {[Unary MulTail]}
M(MulExp, MINUS): {[Unary MulTail]}
M(MulExp, NOT): {[Unary MulTail]}
M(MulExp, LPAREN): {[Unary MulTail]}
M(Decl, BOOL): {[Type IDENT ASSIGN Exp SEMI]}
M(Decl, INT): {[Type IDENT ASSIGN Exp SEMI]}
M(Item, LBRACE): {[Stmt]}
M(Item, IDENT): {[Stmt]}
M(Item, PRINT): {[Stmt]}
M(Item, RETURN): {[Stmt]}
M(Item, WHILE): {[Stmt]}
M(Item, IF): {[Stmt]}
M(Item, BOOL): {[Decl]}
M(Item, INT): {[Decl]}
M(Items, RBRACE): {[ε]}
M(Items, LBRACE): {[Item Items]}
M(Items, IDENT): {[Item Items]}
M(Items, PRINT): {[Item Items]}
M(Items, RETURN): {[Item Items]}
M(Items, WHILE): {[Item Items]}
M(Items, IF): {[Item Items]}
M(Items, BOOL): {[Item Items]}
M(Items, INT): {[Item Items]}
M(Block, LBRACE): {[LBRACE Items RBRACE]}
M(Type, BOOL): {[BOOL]}
M(Type, INT): {[INT]}
M(Param, BOOL): {[Type IDENT]}
M(Param, INT): {[Type IDENT]}
M(ParamsTail, COMMA): {[COMMA Param ParamsTail]}
M(ParamsTail, RPAREN): {[ε]}
M(Params, BOOL): {[Param ParamsTail]}
M(Params, INT): {[Param ParamsTail]}
M(ParamsOpt, BOOL): {[Params]}
M(ParamsOpt, INT): {[Params]}
M(ParamsOpt, RPAREN): {[ε]}
M(FunDecl, BOOL): {[Type IDENT LPAREN ParamsOpt RPAREN Block]}
M(FunDecl, INT): {[Type IDENT LPAREN ParamsOpt RPAREN Block]}
M(FunDecls, EOF): {[ε]}
M(FunDecls, BOOL): {[FunDecl FunDecls]}
M(FunDecls, INT): {[FunDecl FunDecls]}
M(Program, EOF): {[FunDecls]}
M(Program, BOOL): {[FunDecls]}
M(Program, INT): {[FunDecls]}
M(S, EOF): {[Program EOF]}
M(S, BOOL): {[Program EOF]}
M(S, INT): {[Program EOF]}
M(Stmt, LBRACE): {[Block]}
M(Stmt, IDENT): {[IDENT StmtRest]}
M(Stmt, PRINT): {[PRINT LPAREN Exp RPAREN SEMI]}
M(Stmt, RETURN): {[RETURN Exp SEMI]}
M(Stmt, WHILE): {[WHILE LPAREN Exp RPAREN Stmt]}
M(Stmt, IF): {[IF LPAREN Exp RPAREN Stmt ElsePart]}
M(StmtRest, ASSIGN): {[ASSIGN Exp SEMI]}
M(StmtRest, LPAREN): {[LPAREN ArgsOpt RPAREN SEMI]}
M(ElsePart, RBRACE): {[ε]}
M(ElsePart, LBRACE): {[ε]}
M(ElsePart, IDENT): {[ε]}
M(ElsePart, PRINT): {[ε]}
M(ElsePart, RETURN): {[ε]}
M(ElsePart, WHILE): {[ε]}
M(ElsePart, ELSE): {[ε], [ELSE Stmt]}
M(ElsePart, IF): {[ε]}
M(ElsePart, BOOL): {[ε]}
M(ElsePart, INT): {[ε]}
M(ArgsOpt, NUM): {[Args]}
M(ArgsOpt, IDENT): {[Args]}
M(ArgsOpt, READ): {[Args]}
M(ArgsOpt, FALSE): {[Args]}
M(ArgsOpt, TRUE): {[Args]}
M(ArgsOpt, MINUS): {[Args]}
M(ArgsOpt, NOT): {[Args]}
M(ArgsOpt, LPAREN): {[Args]}
M(ArgsOpt, RPAREN): {[ε]}
M(Args, NUM): {[Exp ArgsTail]}
M(Args, IDENT): {[Exp ArgsTail]}
M(Args, READ): {[Exp ArgsTail]}
M(Args, FALSE): {[Exp ArgsTail]}
M(Args, TRUE): {[Exp ArgsTail]}
M(Args, MINUS): {[Exp ArgsTail]}
M(Args, NOT): {[Exp ArgsTail]}
M(Args, LPAREN): {[Exp ArgsTail]}
M(ArgsTail, COMMA): {[COMMA Exp ArgsTail]}
M(ArgsTail, RPAREN): {[ε]}
M(Exp, NUM): {[AndExp ExpTail]}
M(Exp, IDENT): {[AndExp ExpTail]}
M(Exp, READ): {[AndExp ExpTail]}
M(Exp, FALSE): {[AndExp ExpTail]}
M(Exp, TRUE): {[AndExp ExpTail]}
M(Exp, MINUS): {[AndExp ExpTail]}
M(Exp, NOT): {[AndExp ExpTail]}
M(Exp, LPAREN): {[AndExp ExpTail]}
M(ExpTail, SEMI): {[ε]}
M(ExpTail, COMMA): {[ε]}
M(ExpTail, OR): {[OR AndExp ExpTail]}
M(ExpTail, RPAREN): {[ε]}
M(AndExp, NUM): {[EqExp AndTail]}
M(AndExp, IDENT): {[EqExp AndTail]}
M(AndExp, READ): {[EqExp AndTail]}
M(AndExp, FALSE): {[EqExp AndTail]}
M(AndExp, TRUE): {[EqExp AndTail]}
M(AndExp, MINUS): {[EqExp AndTail]}
M(AndExp, NOT): {[EqExp AndTail]}
M(AndExp, LPAREN): {[EqExp AndTail]}
M(AndTail, SEMI): {[ε]}
M(AndTail, COMMA): {[ε]}
M(AndTail, AND): {[AND EqExp AndTail]}
M(AndTail, OR): {[ε]}
M(AndTail, RPAREN): {[ε]}
M(EqExp, NUM): {[RelExp EqTail]}
M(EqExp, IDENT): {[RelExp EqTail]}
M(EqExp, READ): {[RelExp EqTail]}
M(EqExp, FALSE): {[RelExp EqTail]}
M(EqExp, TRUE): {[RelExp EqTail]}
M(EqExp, MINUS): {[RelExp EqTail]}
M(EqExp, NOT): {[RelExp EqTail]}
M(EqExp, LPAREN): {[RelExp EqTail]}
M(EqTail, SEMI): {[ε]}
M(EqTail, COMMA): {[ε]}
M(EqTail, EQ): {[EQ RelExp]}
M(EqTail, NE): {[NE RelExp]}
M(EqTail, AND): {[ε]}
M(EqTail, OR): {[ε]}
M(EqTail, RPAREN): {[ε]}
M(RelExp, NUM): {[AddExp RelTail]}
M(RelExp, IDENT): {[AddExp RelTail]}
M(RelExp, READ): {[AddExp RelTail]}
M(RelExp, FALSE): {[AddExp RelTail]}
M(RelExp, TRUE): {[AddExp RelTail]}
M(RelExp, MINUS): {[AddExp RelTail]}
M(RelExp, NOT): {[AddExp RelTail]}
M(RelExp, LPAREN): {[AddExp RelTail]}
M(RelTail, SEMI): {[ε]}
M(RelTail, COMMA): {[ε]}
M(RelTail, LT): {[LT AddExp]}
M(RelTail, LE): {[LE AddExp]}
M(RelTail, GT): {[GT AddExp]}
M(RelTail, GE): {[GE AddExp]}
M(RelTail, EQ): {[ε]}
M(RelTail, NE): {[ε]}
M(RelTail, AND): {[ε]}
M(RelTail, OR): {[ε]}
M(RelTail, RPAREN): {[ε]}
M(AddExp, NUM): {[MulExp AddTail]}
M(AddExp, IDENT): {[MulExp AddTail]}
M(AddExp, READ): {[MulExp AddTail]}
M(AddExp, FALSE): {[MulExp AddTail]}
M(AddExp, TRUE): {[MulExp AddTail]}
M(AddExp, MINUS): {[MulExp AddTail]}
M(AddExp, NOT): {[MulExp AddTail]}
M(AddExp, LPAREN): {[MulExp AddTail]}
M(AddTail, SEMI): {[ε]}
M(AddTail, COMMA): {[ε]}
M(AddTail, PLUS): {[PLUS MulExp AddTail]}
M(AddTail, MINUS): {[MINUS MulExp AddTail]}
M(AddTail, LT): {[ε]}
M(AddTail, LE): {[ε]}
M(AddTail, GT): {[ε]}
M(AddTail, GE): {[ε]}
M(AddTail, EQ): {[ε]}
M(AddTail, NE): {[ε]}
M(AddTail, AND): {[ε]}
M(AddTail, OR): {[ε]}
M(AddTail, RPAREN): {[ε]}
