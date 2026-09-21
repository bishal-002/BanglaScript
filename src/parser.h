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

private:
    Token peek() const;
    Token advance();
    bool check(TokenType type) const;
    bool isAtEnd() const;

    std::shared_ptr<Statement> parseStatement();

    std::shared_ptr<Statement> parseDeclaration();
    std::shared_ptr<Statement> parseAssignment();
    std::shared_ptr<Statement> parseIfElse();
    std::shared_ptr<Statement> parseWhile();

    std::shared_ptr<Expression> parseExpression();
    std::shared_ptr<Expression> parsePrimary();
    std::shared_ptr<Expression> parseCondition();
};

#endif