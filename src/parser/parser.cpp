// File: parser.cpp
#include "parser.h"

BTNode* Parser::failGen(BTNode* root, const std::string& msg, int line)
{
    error(msg, line);
    delete root;
    while(!stack.empty()) stack.pop();
    return nullptr;
}

BTNode* Parser::genTree()
{
    Token a = NextToken(); // a := NextToken()
    BTNode* root = new BTNode(a.line, S);
    stack.push({S, root});

    StackEntry top = TopOfStack(); // X := TopOfStack()
    while(std::holds_alternative<NonTerminal>(top.sym) ||
          std::get<Terminal>(top.sym) != _EOF) // while (X ̸!= $)
    {
        // if X = a (* match *)
        if(std::holds_alternative<Terminal>(top.sym))
        {
            Terminal t = std::get<Terminal>(top.sym);
            if(t != a.kind)
                return failGen(root, "syntax error: expected " + symToStr(t) +
                            ", got " + symToStr(a.kind), a.line);

            // match: record the actual token on the leaf
            top.node->token = a;
            top.node->lineNum = a.line;

            // then Pop(); a := NextToken()
            Pop();
            a = NextToken();
        }
        else // else if M[X, a] = {α} (* predict *)
        {
            NonTerminal A = std::get<NonTerminal>(top.sym);
            const RuleRHS& rhs = parseTable.at(A).at(a.kind);

            if(rhs.empty())
                return failGen(root, "syntax error: unexpected " + symToStr(a.kind) +
                            " while parsing " + symToStr(A), a.line);

            const std::vector<Symbol>* symlist = &*rhs.begin();

            if(rhs.size() > 1) // dangling else resolution
            {
                if(A != ElsePart || a.kind != ELSE)
                    return failGen(root, "grammar error: not LL(1) at " + symToStr(A), a.line);

                for(const std::vector<Symbol>& sl : rhs)
                    if(!sl.empty())
                        symlist = &sl;
            }

            // then Pop(); Push(α)
            Pop();
            Push(*symlist, top.node, a.line);
        }
        top = TopOfStack(); // X := TopOfStack()
    }

    if(a.kind != _EOF)
        return failGen(root, "syntax error: trailing input after program", a.line);


    return CSTToAST(root);
}


BTNode* Parser::CSTToAST(BTNode* ast)
{
    // TODO
}


Token Parser::NextToken()
{
    if(input->empty())
        throw std::runtime_error("token input empty! cannot parse.");
    Token a = input->front();
    input->pop();
    return a;
}


void Parser::Push(const std::vector<Symbol>& sl, BTNode* parent, int line)
{
    // create children left to right and link them as first-child/next-sibling
    std::vector<BTNode*> kids;
    BTNode* prev = nullptr;
    for(const Symbol& sym : sl)
    {
        BTNode* kid = new BTNode(line, sym);
        if(prev == nullptr)
            parent->left = kid;
        else 
            prev->right = kid;
        prev = kid;
        kids.push_back(kid);
    }

    // push in reverse so the leftmost child is on top of the stack
    for(size_t i = sl.size(); i-- > 0; )
        stack.push({sl[i], kids[i]});
}
