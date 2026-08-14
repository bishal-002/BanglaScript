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

private:
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;

    Token advance();

    bool check(TokenType type) const;
    bool match(TokenType type);

    // Statements
    std::shared_ptr<Statement> parseStatement();

    std::shared_ptr<Statement> parseDeclaration();

    std::shared_ptr<Statement> parseAssignment();

    std::shared_ptr<Statement> parseIfElse();

    // Expressions
    std::shared_ptr<Expression> parseExpression();

    std::shared_ptr<Expression> parseComparison();

    std::shared_ptr<Expression> parseTerm();

    std::shared_ptr<Expression> parseFactor();

    std::shared_ptr<Expression> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens);

    std::shared_ptr<ProgramNode> parse();
};

#endif