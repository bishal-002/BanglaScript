#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

// Base AST Node
class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

// Base Classes
class Statement : public ASTNode
{
};

class Expression : public ASTNode
{
};

// Program Node
class ProgramNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<Statement>> statements;
};

// Expression Nodes
class NumberNode : public Expression
{
public:
    int value;

    NumberNode(int value)
        : value(value)
    {
    }
};

class IdentifierNode : public Expression
{
public:
    std::string name;

    IdentifierNode(const std::string& name)
        : name(name)
    {
    }
};

class BinaryExpressionNode : public Expression
{
public:
    std::string op;

    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;

    BinaryExpressionNode(
        const std::string& op,
        std::shared_ptr<Expression> left,
        std::shared_ptr<Expression> right)
        : op(op),
          left(left),
          right(right)
    {
    }
};

// Statement Nodes
class DeclarationNode : public Statement
{
public:
    std::string variableType;
    std::string variableName;

    std::shared_ptr<Expression> value;

    DeclarationNode(
        const std::string& variableType,
        const std::string& variableName,
        std::shared_ptr<Expression> value)
        : variableType(variableType),
          variableName(variableName),
          value(value)
    {
    }
};

class AssignmentNode : public Statement
{
public:
    std::string variableName;

    std::shared_ptr<Expression> value;

    AssignmentNode(
        const std::string& variableName,
        std::shared_ptr<Expression> value)
        : variableName(variableName),
          value(value)
    {
    }
};

class IfElseNode : public Statement
{
public:
    std::shared_ptr<Expression> condition;

    std::vector<std::shared_ptr<Statement>> ifBody;
    std::vector<std::shared_ptr<Statement>> elseBody;
};

#endif