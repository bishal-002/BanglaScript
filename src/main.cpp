#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "parser.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::KEYWORD_DHORI:
        return "KEYWORD_DHORI";
    case TokenType::KEYWORD_JODI:
        return "KEYWORD_JODI";
    case TokenType::KEYWORD_NAHOLE:
        return "KEYWORD_NAHOLE";
    case TokenType::TYPE_SONGKHA:
        return "TYPE_SONGKHA";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::STAR:
        return "STAR";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::GT:
        return "GT";
    case TokenType::LT:
        return "LT";
    case TokenType::GTE:
        return "GTE";
    case TokenType::LTE:
        return "LTE";
    case TokenType::EQ:
        return "EQ";
    case TokenType::NEQ:
        return "NEQ";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::LBRACE:
        return "LBRACE";
    case TokenType::RBRACE:
        return "RBRACE";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    case TokenType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

int main()
{
    std::string source =
        "ধরি সংখ্যা ক = 10;\n"
        "ক = ক + 20;\n"
        "যদি (ক > 10) {\n"
        "    ক = ক - 1;\n"
        "}\n"
        "নাহলে {\n"
        "    ক = 0;\n"
        "}";

    Lexer lexer(source);

    std::vector<Token> tokens = lexer.tokenize();

    for (const Token &token : tokens)
    {
        std::cout
            << "Line " << token.line
            << " | "
            << tokenTypeToString(token.type)
            << " | "
            << token.lexeme
            << '\n';
    }

    std::cout << "\n========== PARSER ==========\n";

    Parser parser(tokens);

    parser.parse();

    return 0;
}