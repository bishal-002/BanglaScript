#include "parser.h"
#include "error.h"

#include <iostream>
#include <stdexcept>

// ==========================================
// Constructor
// ==========================================

Parser::Parser(
    const std::vector<Token>& tokens)
    : tokens(tokens),
      current(0)
{
}

// ==========================================
// Token Helper Functions
// ==========================================

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
           tokens[current].type ==
               TokenType::END_OF_FILE;
}

// ==========================================
// Main Parse Function
// ==========================================

std::shared_ptr<ProgramNode> Parser::parse()
{
    auto program =
        std::make_shared<ProgramNode>();

    while (!isAtEnd())
    {
        try
        {
            auto statement =
                parseStatement();

            if (statement)
            {
                program->statements.push_back(
                    statement);
            }
        }
        catch (const std::runtime_error& error)
        {
            ErrorReporter::report(
                ErrorType::PARSER,
                error.what(),
                peek().line);

            if (!isAtEnd())
            {
                advance();
            }
        }
    }

    return program;
}

// ==========================================
// Statement Parser
// ==========================================

std::shared_ptr<Statement>
Parser::parseStatement()
{
    // Declaration
    if (check(TokenType::KEYWORD_DHORI))
    {
        return parseDeclaration();
    }

    // If Else
    if (check(TokenType::KEYWORD_JODI))
    {
        return parseIfElse();
    }

    // While Loop
    if (check(TokenType::KEYWORD_JOTOKKHON))
    {
        return parseWhile();
    }

    // Print
    if (check(TokenType::KEYWORD_DEKHAO))
    {
        return parsePrint();
    }

    // Assignment
    if (check(TokenType::IDENTIFIER))
    {
        return parseAssignment();
    }

    throw std::runtime_error(
        "Unexpected token: " +
        peek().lexeme);
}

// ==========================================
// Declaration
// ==========================================

std::shared_ptr<Statement>
Parser::parseDeclaration()
{
    // Consume "ধরি"
    advance();

    // Accept সংখ্যা or লেখা
    if (!check(TokenType::TYPE_SONGKHA) &&
        !check(TokenType::TYPE_LEKHA))
    {
        throw std::runtime_error(
            "Expected type after declaration keyword.");
    }

    std::string variableType =
        advance().lexeme;

    // Identifier
    if (!check(TokenType::IDENTIFIER))
    {
        throw std::runtime_error(
            "Expected identifier in declaration.");
    }

    std::string variableName =
        advance().lexeme;

    // =
    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' in declaration.");
    }

    advance();

    // Value
    auto value =
        parseExpression();

    // ;
    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after declaration.");
    }

    advance();

    return std::make_shared<DeclarationNode>(
        variableType,
        variableName,
        value);
}

// ==========================================
// Assignment
// ==========================================

std::shared_ptr<Statement>
Parser::parseAssignment()
{
    std::string variableName =
        advance().lexeme;

    // =
    if (!check(TokenType::ASSIGN))
    {
        throw std::runtime_error(
            "Expected '=' after identifier.");
    }

    advance();

    // Value
    auto value =
        parseExpression();

    // ;
    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after assignment.");
    }

    advance();

    return std::make_shared<AssignmentNode>(
        variableName,
        value);
}

// ==========================================
// Print
// ==========================================

std::shared_ptr<Statement>
Parser::parsePrint()
{
    // Consume "দেখাও"
    advance();

    // (
    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after print keyword.");
    }

    advance();

    // Expression
    auto value =
        parseExpression();

    // )
    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after print expression.");
    }

    advance();

    // ;
    if (!check(TokenType::SEMICOLON))
    {
        throw std::runtime_error(
            "Expected ';' after print statement.");
    }

    advance();

    return std::make_shared<PrintNode>(
        value);
}

// ==========================================
// Expression
// ==========================================

std::shared_ptr<Expression>
Parser::parseExpression()
{
    auto left =
        parsePrimary();

    while (check(TokenType::PLUS) ||
           check(TokenType::MINUS) ||
           check(TokenType::STAR) ||
           check(TokenType::SLASH))
    {
        std::string op =
            advance().lexeme;

        auto right =
            parsePrimary();

        left =
            std::make_shared<BinaryExpressionNode>(
                op,
                left,
                right);
    }

    return left;
}

// ==========================================
// Primary Expression
// ==========================================

std::shared_ptr<Expression>
Parser::parsePrimary()
{
    // Number
    if (check(TokenType::NUMBER))
    {
        int value =
            std::stoi(
                advance().lexeme);

        return std::make_shared<NumberNode>(
            value);
    }

    // String
    if (check(TokenType::STRING_LITERAL))
    {
        std::string value =
            advance().lexeme;

        return std::make_shared<StringNode>(
            value);
    }

    // Identifier
    if (check(TokenType::IDENTIFIER))
    {
        std::string name =
            advance().lexeme;

        return std::make_shared<IdentifierNode>(
            name);
    }

    throw std::runtime_error(
        "Expected number, string, or identifier "
        "in expression.");
}

// ==========================================
// Condition
// ==========================================

std::shared_ptr<Expression>
Parser::parseCondition()
{
    auto left =
        parsePrimary();

    if (!(check(TokenType::GT) ||
          check(TokenType::LT) ||
          check(TokenType::GTE) ||
          check(TokenType::LTE) ||
          check(TokenType::EQ) ||
          check(TokenType::NEQ)))
    {
        throw std::runtime_error(
            "Expected comparison operator in condition.");
    }

    std::string op =
        advance().lexeme;

    auto right =
        parsePrimary();

    return std::make_shared<BinaryExpressionNode>(
        op,
        left,
        right);
}

// ==========================================
// If Else
// ==========================================

std::shared_ptr<Statement>
Parser::parseIfElse()
{
    // Consume "যদি"
    advance();

    // (
    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after if.");
    }

    advance();

    // Condition
    auto condition =
        parseCondition();

    // )
    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after condition.");
    }

    advance();

    // {
    if (!check(TokenType::LBRACE))
    {
        throw std::runtime_error(
            "Expected '{' before if body.");
    }

    advance();

    auto ifElseNode =
        std::make_shared<IfElseNode>();

    ifElseNode->condition =
        condition;

    // IF body
    while (!check(TokenType::RBRACE) &&
           !isAtEnd())
    {
        auto statement =
            parseStatement();

        if (statement)
        {
            ifElseNode->ifBody.push_back(
                statement);
        }
    }

    // }
    if (!check(TokenType::RBRACE))
    {
        throw std::runtime_error(
            "Expected '}' after if body.");
    }

    advance();

    // Else
    if (check(TokenType::KEYWORD_NAHOLE))
    {
        advance();

        // {
        if (!check(TokenType::LBRACE))
        {
            throw std::runtime_error(
                "Expected '{' before else body.");
        }

        advance();

        // ELSE body
        while (!check(TokenType::RBRACE) &&
               !isAtEnd())
        {
            auto statement =
                parseStatement();

            if (statement)
            {
                ifElseNode->elseBody.push_back(
                    statement);
            }
        }

        // }
        if (!check(TokenType::RBRACE))
        {
            throw std::runtime_error(
                "Expected '}' after else body.");
        }

        advance();
    }

    return ifElseNode;
}

// ==========================================
// While Loop
// ==========================================

std::shared_ptr<Statement>
Parser::parseWhile()
{
    // Consume "যতক্ষণ"
    advance();

    // (
    if (!check(TokenType::LPAREN))
    {
        throw std::runtime_error(
            "Expected '(' after while.");
    }

    advance();

    // Condition
    auto condition =
        parseCondition();

    // )
    if (!check(TokenType::RPAREN))
    {
        throw std::runtime_error(
            "Expected ')' after while condition.");
    }

    advance();

    // {
    if (!check(TokenType::LBRACE))
    {
        throw std::runtime_error(
            "Expected '{' before while body.");
    }

    advance();

    auto whileNode =
        std::make_shared<WhileNode>(
            condition);

    // While body
    while (!check(TokenType::RBRACE) &&
           !isAtEnd())
    {
        auto statement =
            parseStatement();

        if (statement)
        {
            whileNode->body.push_back(
                statement);
        }
    }

    // }
    if (!check(TokenType::RBRACE))
    {
        throw std::runtime_error(
            "Expected '}' after while body.");
    }

    advance();

    return whileNode;
}