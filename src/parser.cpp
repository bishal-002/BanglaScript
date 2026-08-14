#include "parser.h"

#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens),
      current(0)
{
}

Token Parser::peek() const
{
    return tokens[current];
}

Token Parser::advance()
{
    current++;
    return tokens[current - 1];
}

bool Parser::check(TokenType type) const
{
    return peek().type == type;
}

void Parser::parseDeclaration()
{
    if (!check(TokenType::KEYWORD_DHORI))
    {
        std::cout << "Expected ধরি\n";
        return;
    }

    advance();

    if (!check(TokenType::TYPE_SONGKHA))
    {
        std::cout << "Expected সংখ্যা\n";
        return;
    }

    advance();

    if (!check(TokenType::IDENTIFIER))
    {
        std::cout << "Expected Identifier\n";
        return;
    }

    advance();

    if (!check(TokenType::ASSIGN))
    {
        std::cout << "Expected =\n";
        return;
    }

    advance();

    if (!check(TokenType::NUMBER))
    {
        std::cout << "Expected Number\n";
        return;
    }

    advance();

    if (!check(TokenType::SEMICOLON))
    {
        std::cout << "Expected ;\n";
        return;
    }

    std::cout
        << "Declaration Parsed Successfully\n";
}

std::shared_ptr<ProgramNode> Parser::parse()
{
    parseDeclaration();

    return std::make_shared<ProgramNode>();
}