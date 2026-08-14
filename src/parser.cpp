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
    if(current >= tokens.size())
    {
        return false;
    }

    return peek().type == type;
}

void Parser::parseExpression()
{
    if(check(TokenType::NUMBER))
    {
        advance();
    }
    else if(check(TokenType::IDENTIFIER))
    {
        advance();

        if(check(TokenType::PLUS) ||
           check(TokenType::MINUS) ||
           check(TokenType::STAR) ||
           check(TokenType::SLASH))
        {
            advance();

            if(check(TokenType::NUMBER) ||
               check(TokenType::IDENTIFIER))
            {
                advance();
            }
            else
            {
                std::cout
                    << "Expected value after operator\n";
            }
        }
    }
    else
    {
        std::cout << "Invalid expression\n";
    }
}

void Parser::parseCondition()
{
    if(!check(TokenType::IDENTIFIER))
    {
        std::cout
            << "Expected identifier in condition\n";
        return;
    }

    advance();

    if(!(check(TokenType::GT)  ||
         check(TokenType::LT)  ||
         check(TokenType::GTE) ||
         check(TokenType::LTE) ||
         check(TokenType::EQ)  ||
         check(TokenType::NEQ)))
    {
        std::cout
            << "Expected comparison operator\n";
        return;
    }

    advance();

    if(!(check(TokenType::NUMBER) ||
         check(TokenType::IDENTIFIER)))
    {
        std::cout
            << "Expected comparison value\n";
        return;
    }

    advance();
}

void Parser::parseDeclaration()
{
    advance();

    if(!check(TokenType::TYPE_SONGKHA))
    {
        std::cout << "Expected সংখ্যা\n";
        return;
    }

    advance();

    if(!check(TokenType::IDENTIFIER))
    {
        std::cout << "Expected Identifier\n";
        return;
    }

    advance();

    if(!check(TokenType::ASSIGN))
    {
        std::cout << "Expected =\n";
        return;
    }

    advance();

    parseExpression();

    if(!check(TokenType::SEMICOLON))
    {
        std::cout << "Expected ;\n";
        return;
    }

    advance();

    std::cout
        << "Declaration Parsed Successfully\n";
}

void Parser::parseAssignment()
{
    advance();

    if(!check(TokenType::ASSIGN))
    {
        std::cout << "Expected =\n";
        return;
    }

    advance();

    parseExpression();

    if(!check(TokenType::SEMICOLON))
    {
        std::cout << "Expected ;\n";
        return;
    }

    advance();

    std::cout
        << "Assignment Parsed Successfully\n";
}

void Parser::parseIfElse()
{
    advance(); // যদি

    if(!check(TokenType::LPAREN))
    {
        std::cout
            << "Expected (\n";
        return;
    }

    advance();

    parseCondition();

    if(!check(TokenType::RPAREN))
    {
        std::cout
            << "Expected )\n";
        return;
    }

    advance();

    if(!check(TokenType::LBRACE))
    {
        std::cout
            << "Expected {\n";
        return;
    }

    advance();

    while(check(TokenType::IDENTIFIER))
    {
        parseAssignment();
    }

    if(!check(TokenType::RBRACE))
    {
        std::cout
            << "Expected }\n";
        return;
    }

    advance();

    if(!check(TokenType::KEYWORD_NAHOLE))
    {
        std::cout
            << "Expected নাহলে\n";
        return;
    }

    advance();

    if(!check(TokenType::LBRACE))
    {
        std::cout
            << "Expected {\n";
        return;
    }

    advance();

    while(check(TokenType::IDENTIFIER))
    {
        parseAssignment();
    }

    if(!check(TokenType::RBRACE))
    {
        std::cout
            << "Expected }\n";
        return;
    }

    advance();

    std::cout
        << "If-Else Parsed Successfully\n";
}

std::shared_ptr<ProgramNode> Parser::parse()
{
    while(current < tokens.size() &&
          tokens[current].type != TokenType::END_OF_FILE)
    {
        if(check(TokenType::KEYWORD_DHORI))
        {
            parseDeclaration();
        }
        else if(check(TokenType::KEYWORD_JODI))
        {
            parseIfElse();
        }
        else if(check(TokenType::IDENTIFIER))
        {
            parseAssignment();
        }
        else
        {
            std::cout
                << "Unexpected token: "
                << peek().lexeme
                << "\n";

            advance();
        }
    }

    return std::make_shared<ProgramNode>();
}