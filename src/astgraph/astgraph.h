// File: astgraph.h

//#define BUILD_GRAPHVIZ

#ifdef BUILD_GRAPHVIZ

#pragma once
#include <string>
#include <graphviz/gvc.h>
#include "../parser/parser.h"


// render the tree for testing
class ASTGraph {
public:
    // Write Graphviz source to `path`. Returns false on I/O failure.
    static bool writeDot(const Node* root, const std::string& path);

    static bool render(const Node* root, const std::string& path,
                       const char* format = "svg");
    static std::string labelFor(const Node* n);

    static bool isTerminal(const Node* n);

    static Agraph_t* build(const Node* root);
};

#endif
