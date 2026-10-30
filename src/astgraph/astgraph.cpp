// File: astgraph.cpp
#include "astgraph.h"
#include <graphviz/gvc.h>
#include <cstdio>
#include <stack>
#include <utility>

namespace {

std::string labelFor(const BTNode* n)
{
    std::string label = symToStr(n->symbol);
    if(n->token.has_value())
        label += "\\n" + n->token->value;
    return label;
}

bool isTerminal(const BTNode* n)
{
    return std::holds_alternative<Terminal>(n->symbol);
}

Agraph_t* build(const BTNode* root)
{
    Agraph_t* g = agopen(const_cast<char*>("ast"), Agdirected, nullptr);

    // declare attribute defaults before creating nodes
    agattr(g, AGRAPH, const_cast<char*>("ordering"), const_cast<char*>("out"));
    agattr(g, AGNODE, const_cast<char*>("label"),    const_cast<char*>(""));
    agattr(g, AGNODE, const_cast<char*>("shape"),    const_cast<char*>("ellipse"));
    agattr(g, AGNODE, const_cast<char*>("style"),    const_cast<char*>(""));

    int counter = 0;
    auto makeNode = [&](const BTNode* n) {
        std::string name = "n" + std::to_string(counter++);
        Agnode_t* gn = agnode(g, const_cast<char*>(name.c_str()), 1);
        agset(gn, const_cast<char*>("label"),
              const_cast<char*>(labelFor(n).c_str()));
        if(isTerminal(n))
        {
            agset(gn, const_cast<char*>("shape"), const_cast<char*>("box"));
            agset(gn, const_cast<char*>("style"), const_cast<char*>("filled"));
        }
        return gn;
    };

    std::stack<std::pair<const BTNode*, Agnode_t*>> work;
    work.push({root, makeNode(root)});

    while(!work.empty())
    {
        auto [node, gnode] = work.top();
        work.pop();

        // children = left, then the right-sibling chain; edges are created
        // in order so `ordering=out` keeps them left to right
        for(const BTNode* c = node->left; c != nullptr; c = c->right)
        {
            Agnode_t* gc = makeNode(c);
            agedge(g, gnode, gc, nullptr, 1);
            work.push({c, gc});
        }
    }
    return g;
}

} // namespace

bool ASTGraph::writeDot(const BTNode* root, const std::string& path)
{
    if(root == nullptr) return false;

    Agraph_t* g = build(root);
    FILE* f = std::fopen(path.c_str(), "w");
    if(f == nullptr) { agclose(g); return false; }

    int rc = agwrite(g, f);
    std::fclose(f);
    agclose(g);
    return rc == 0;
}

bool ASTGraph::render(const BTNode* root, const std::string& path, const char* format)
{
    if(root == nullptr) return false;

    GVC_t* gvc = gvContext();
    Agraph_t* g = build(root);

    int rc = gvLayout(gvc, g, "dot");
    if(rc == 0)
    {
        rc = gvRenderFilename(gvc, g, format, path.c_str());
        gvFreeLayout(gvc, g);
    }
    agclose(g);
    gvFreeContext(gvc);
    return rc == 0;
}
