#include "parser.h"

#include <iostream>
#include <stdexcept>

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
    if (!isAtEnd())
    {
        current++;
    }

    return tokens[current - 1];
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd())
    {
        return type == TokenType::END_OF_FILE;
    }

    return peek().type == type;
}

bool Parser::isAtEnd() const
{
    return current >= tokens.size() ||
           tokens[current].type == TokenType::END_OF_FILE;
}

std::shared_ptr<ProgramNode> Parser::parse()
{
    auto program = std::make_shared<ProgramNode>();

    while (!isAtEnd())
    {
        try
        {
            auto statement = parseStatement();

            if (statement)
            {
                program->statements.push_back(statement);
            }
        }
        catch (const std::runtime_error& error)
        {
            std::cout
                << "Parser Error: "
                << error.what()
                << '\n';

            if (!isAtEnd())
            {
                advance();
            }
        }
    }

    return program;
}

std::shared_ptr<Statement> Parser::parseStatement()
{
    if (check(TokenType::KEYWORD_DHORI))
    {
        return parseDeclaration();
    }

    if (check(TokenType::KEYWORD_JODI))
    {
        return parseIfElse();
    }

    if (check(TokenType::IDENTIFIER))
    {
        return parseAssignment();
    }

    throw std::runtime_error(
        "Unexpected token: " + peek().lexeme
    );
}

std::shared_ptr<Statement> Parser::parseDeclaration()
{
    advance();

    if (!check(TokenType::TYPE_SONGKHA))
    {
        throw std::runtime_error(
            "Expected type after declaration keyword."
        );
    }

    std::string variableType = advance().lexeme;

    if (!check(TokenType::IDENTIFIER))
    {
        throw std::runtime_error(
            "Expected identifier in declaration."
        );
    }

    std::string variableName = advance().lexeme;

    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' in declaration."
        );
    }

    advance();

    auto value = parseExpression();

    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after declaration."
        );
    }

    advance();

    return std::make_shared<DeclarationNode>(
        variableType,
        variableName,
        value
    );
}

std::shared_ptr<Statement> Parser::parseAssignment()
{
    std::string variableName = advance().lexeme;

    // =
    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' after identifier."
        );
    }

    advance();

    auto value = parseExpression();

    // ;
    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after assignment."
        );
    }

    advance();

    return std::make_shared<AssignmentNode>(
        variableName,
        value
    );
}

std::shared_ptr<Expression> Parser::parseExpression()
{
    auto left = parsePrimary();

    while (check(TokenType::PLUS) ||
           check(TokenType::MINUS) ||
           check(TokenType::STAR) ||
           check(TokenType::SLASH))
    {
        std::string op = advance().lexeme;

        auto right = parsePrimary();

        left = std::make_shared<BinaryExpressionNode>(
            op,
            left,
            right
        );
    }

    return left;
}


std::shared_ptr<Expression> Parser::parsePrimary()
{
    if (check(TokenType::NUMBER))
    {
        int value = std::stoi(advance().lexeme);

        return std::make_shared<NumberNode>(value);
    }

    if (check(TokenType::IDENTIFIER))
    {
        std::string name = advance().lexeme;

        return std::make_shared<IdentifierNode>(name);
    }

    throw std::runtime_error(
        "Expected number or identifier in expression."
    );
}

std::shared_ptr<Expression> Parser::parseCondition()
{
    auto left = parsePrimary();

    if (!(check(TokenType::GT) ||
          check(TokenType::LT) ||
          check(TokenType::GTE) ||
          check(TokenType::LTE) ||
          check(TokenType::EQ) ||
          check(TokenType::NEQ)))
    {
        throw std::runtime_error(
            "Expected comparison operator in condition."
        );
    }

    std::string op = advance().lexeme;

    auto right = parsePrimary();

    return std::make_shared<BinaryExpressionNode>(
        op,
        left,
        right
    );
}

std::shared_ptr<Statement> Parser::parseIfElse()
{
    advance();

    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after if."
        );
    }

    advance();

    auto condition = parseCondition();

    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after condition."
        );
    }

    advance();

    if (!check(TokenType::LBRACE))
    {
        throw std::runtime_error(
            "Expected '{' before if body."
        );
    }

    advance();

    auto ifElseNode = std::make_shared<IfElseNode>();

    ifElseNode->condition = condition;

    while (!check(TokenType::RBRACE) &&
           !isAtEnd())
    {
        auto statement = parseStatement();

        if (statement)
        {
            ifElseNode->ifBody.push_back(statement);
        }
    }

    if (!check(TokenType::RBRACE))
    {
        throw std::runtime_error(
            "Expected '}' after if body."
        );
    }

    advance();

    if (check(TokenType::KEYWORD_NAHOLE))
    {
        advance();

        if (!check(TokenType::LBRACE))
        {
            throw std::runtime_error(
                "Expected '{' before else body."
            );
        }

        advance();

        while (!check(TokenType::RBRACE) &&
               !isAtEnd())
        {
            auto statement = parseStatement();

            if (statement)
            {
                ifElseNode->elseBody.push_back(statement);
            }
        }

        if (!check(TokenType::RBRACE))
        {
            throw std::runtime_error(
                "Expected '}' after else body."
            );
        }

        advance();
    }

    return ifElseNode;
}