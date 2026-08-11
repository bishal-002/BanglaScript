#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    // Keywords
    KEYWORD_DHORI,
    KEYWORD_JODI,
    KEYWORD_NAHOLE,

    // Types
    TYPE_SONGKHA,

    // Identifiers & Literals
    IDENTIFIER,
    NUMBER,

    // Arithmetic Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,

    // Assignment
    ASSIGN,

    // Comparison Operators
    GT,
    LT,
    GTE,
    LTE,
    EQ,
    NEQ,

    // Delimiters
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Special
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