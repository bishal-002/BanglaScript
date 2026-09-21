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
    // Declaration
    if (check(TokenType::KEYWORD_DHORI))
    {
        return parseDeclaration();
    }

    // If / Else
    if (check(TokenType::KEYWORD_JODI))
    {
        return parseIfElse();
    }

    // While Loop
    if (check(TokenType::KEYWORD_JOTOKKHON))
    {
        return parseWhile();
    }

    // Assignment
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
    // Consume "ধরি"
    advance();

    if (!check(TokenType::TYPE_SONGKHA))
    {
        throw std::runtime_error(
            "Expected type after declaration keyword."
        );
    }

    // Get variable type
    std::string variableType = advance().lexeme;

    if (!check(TokenType::IDENTIFIER))
    {
        throw std::runtime_error(
            "Expected identifier in declaration."
        );
    }

    // Get variable name
    std::string variableName = advance().lexeme;

    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' in declaration."
        );
    }

    // Consume "="
    advance();

    // Parse value
    auto value = parseExpression();

    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after declaration."
        );
    }

    // Consume ";"
    advance();

    return std::make_shared<DeclarationNode>(
        variableType,
        variableName,
        value
    );
}

std::shared_ptr<Statement> Parser::parseAssignment()
{
    // Get variable name
    std::string variableName = advance().lexeme;

    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' after identifier."
        );
    }

    // Consume "="
    advance();

    // Parse value
    auto value = parseExpression();

    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after assignment."
        );
    }

    // Consume ";"
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
    // Number
    if (check(TokenType::NUMBER))
    {
        int value = std::stoi(advance().lexeme);

        return std::make_shared<NumberNode>(value);
    }

    // Identifier
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
    // Consume "যদি"
    advance();

    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after if."
        );
    }

    // Consume "("
    advance();

    // Parse condition
    auto condition = parseCondition();

    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after condition."
        );
    }

    // Consume ")"
    advance();

    if (!check(TokenType::LBRACE))
    {
        throw std::runtime_error(
            "Expected '{' before if body."
        );
    }

    // Consume "{"
    advance();

    auto ifElseNode = std::make_shared<IfElseNode>();

    ifElseNode->condition = condition;

    // Parse IF body
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

    // Consume "}"
    advance();

    // Check for "নাহলে"
    if (check(TokenType::KEYWORD_NAHOLE))
    {
        // Consume "নাহলে"
        advance();

        if (!check(TokenType::LBRACE))
        {
            throw std::runtime_error(
                "Expected '{' before else body."
            );
        }

        // Consume "{"
        advance();

        // Parse ELSE body
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

        // Consume "}"
        advance();
    }

    return ifElseNode;
}

std::shared_ptr<Statement> Parser::parseWhile()
{
    // Consume "যতক্ষণ"
    advance();

    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after while."
        );
    }

    // Consume "("
    advance();

    // Parse condition
    auto condition = parseCondition();

    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after while condition."
        );
    }

    // Consume ")"
    advance();

    if (!check(TokenType::LBRACE))
    {
        throw std::runtime_error(
            "Expected '{' before while body."
        );
    }

    // Consume "{"
    advance();

    // Create WhileNode
    auto whileNode =
        std::make_shared<WhileNode>(condition);

    // Parse statements inside while body
    while (!check(TokenType::RBRACE) &&
           !isAtEnd())
    {
        auto statement = parseStatement();

        if (statement)
        {
            whileNode->body.push_back(statement);
        }
    }

    if (!check(TokenType::RBRACE))
    {
        throw std::runtime_error(
            "Expected '}' after while body."
        );
    }

    // Consume "}"
    advance();

    return whileNode;
}