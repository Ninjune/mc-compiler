// File: astgraph.h

#pragma once
#include <string>
#include "../parser/parser.h"


// render the tree for testing
class ASTGraph {
public:
    // Write Graphviz source to `path`. Returns false on I/O failure.
    static bool writeDot(const BTNode* root, const std::string& path);

    // Optional: lay out with gvc and render directly (format: "png", "svg", "pdf", ...).
    static bool render(const BTNode* root, const std::string& path,
                       const char* format = "svg");
};
