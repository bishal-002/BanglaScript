#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>

#include "token.h"
#include "ast.h"

class Parser
{
private:
    std::vector<Token> tokens;
    size_t current;

public:
    Parser(const std::vector<Token>& tokens);

    std::shared_ptr<ProgramNode> parse();

    Token peek() const;
    Token advance();
    bool check(TokenType type) const;

    void parseDeclaration();
    void parseAssignment();
    void parseExpression();

    void parseIfElse();
    void parseCondition();
};

#endif