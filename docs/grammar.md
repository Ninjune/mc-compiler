
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

Rewritten:

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
