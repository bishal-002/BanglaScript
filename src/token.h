#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    KEYWORD_DHORI,
    KEYWORD_JODI,
    KEYWORD_NAHOLE,
    KEYWORD_JOTOKKHON,
    KEYWORD_DEKHAO,

    TYPE_SONGKHA,
    TYPE_LEKHA,

    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    ASSIGN,

    GT,
    LT,
    GTE,
    LTE,
    EQ,
    NEQ,

    SEMICOLON,

    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    END_OF_FILE,
    UNKNOWN
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type,
          const std::string& lexeme,
          int line)
        : type(type),
          lexeme(lexeme),
          line(line)
    {
    }
};

#endif