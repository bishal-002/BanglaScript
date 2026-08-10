#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

class Statement : public ASTNode
{
};

class Expression : public ASTNode
{
};

#endif