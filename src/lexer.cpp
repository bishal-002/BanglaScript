#include "lexer.h"

#include <cctype>
#include <iostream>
#include <unordered_map>

static bool isBanglaDigitAt(
    const std::string &source,
    size_t pos)
{
    static const std::string digits[] =
        {
            "০", "১", "২", "৩", "৪",
            "৫", "৬", "৭", "৮", "৯"};

    for (const auto &digit : digits)
    {
        if (source.compare(pos, digit.size(), digit) == 0)
        {
            return true;
        }
    }

    return false;
}

static char banglaDigitToAscii(
    const std::string &digit)
{
    if (digit == "০")
        return '0';
    if (digit == "১")
        return '1';
    if (digit == "২")
        return '2';
    if (digit == "৩")
        return '3';
    if (digit == "৪")
        return '4';
    if (digit == "৫")
        return '5';
    if (digit == "৬")
        return '6';
    if (digit == "৭")
        return '7';
    if (digit == "৮")
        return '8';
    if (digit == "৯")
        return '9';

    return '?';
}

Lexer::Lexer(const std::string &source)
    : source(source), position(0), line(1)
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (position < source.length())
    {
        char current = source[position];

        // Ignore spaces and tabs
        if (current == ' ' || current == '\t' || current == '\r')
        {
            position++;
            continue;
        }

        // New line
        if (current == '\n')
        {
            line++;
            position++;
            continue;
        }

        // Identifier / keyword
        if (std::isalpha(static_cast<unsigned char>(current)) ||
            static_cast<unsigned char>(current) >= 128)
        {
            std::string value;

            while (position < source.length())
            {
                unsigned char c =
                    static_cast<unsigned char>(source[position]);

                if (std::isalnum(c) || c >= 128)
                {
                    value += source[position];
                    position++;
                }
                else
                {
                    break;
                }
            }

            static const std::unordered_map<std::string, TokenType> keywords =
                {
                    {"ধরি", TokenType::KEYWORD_DHORI},
                    {"যদি", TokenType::KEYWORD_JODI},
                    {"নাহলে", TokenType::KEYWORD_NAHOLE},
                    {"সংখ্যা", TokenType::TYPE_SONGKHA}};

            auto it = keywords.find(value);

            if (it != keywords.end())
            {
                tokens.emplace_back(it->second, value, line);
            }
            else
            {
                tokens.emplace_back(
                    TokenType::IDENTIFIER,
                    value,
                    line);
            }

            continue;
        }

        // Bangla Number
        if (isBanglaDigitAt(source, position))
        {
            std::string number;

            while (position < source.length())
            {
                bool foundDigit = false;

                static const std::string digits[] =
                    {
                        "০", "১", "২", "৩", "৪",
                        "৫", "৬", "৭", "৮", "৯"};

                for (const auto &digit : digits)
                {
                    if (source.compare(
                            position,
                            digit.size(),
                            digit) == 0)
                    {
                        number += banglaDigitToAscii(digit);

                        position += digit.size();

                        foundDigit = true;
                        break;
                    }
                }

                if (!foundDigit)
                {
                    break;
                }
            }

            tokens.emplace_back(
                TokenType::NUMBER,
                number,
                line);

            continue;
        }

        if (std::isdigit(
                static_cast<unsigned char>(current)))
        {
            throw std::runtime_error(
                "English numerals are not allowed. "
                "Use Bangla numerals (০-৯).");
        }

        // Two-character operators
        if (position + 1 < source.length())
        {
            std::string twoChars =
                source.substr(position, 2);

            if (twoChars == ">=")
            {
                tokens.emplace_back(
                    TokenType::GTE,
                    twoChars,
                    line);

                position += 2;
                continue;
            }

            if (twoChars == "<=")
            {
                tokens.emplace_back(
                    TokenType::LTE,
                    twoChars,
                    line);

                position += 2;
                continue;
            }

            if (twoChars == "==")
            {
                tokens.emplace_back(
                    TokenType::EQ,
                    twoChars,
                    line);

                position += 2;
                continue;
            }

            if (twoChars == "!=")
            {
                tokens.emplace_back(
                    TokenType::NEQ,
                    twoChars,
                    line);

                position += 2;
                continue;
            }
        }

        // Single-character tokens
        switch (current)
        {
        case '+':
            tokens.emplace_back(TokenType::PLUS, "+", line);
            break;

        case '-':
            tokens.emplace_back(TokenType::MINUS, "-", line);
            break;

        case '*':
            tokens.emplace_back(TokenType::STAR, "*", line);
            break;

        case '/':
            tokens.emplace_back(TokenType::SLASH, "/", line);
            break;

        case '=':
            tokens.emplace_back(TokenType::ASSIGN, "=", line);
            break;

        case '>':
            tokens.emplace_back(TokenType::GT, ">", line);
            break;

        case '<':
            tokens.emplace_back(TokenType::LT, "<", line);
            break;

        case ';':
            tokens.emplace_back(TokenType::SEMICOLON, ";", line);
            break;

        case '(':
            tokens.emplace_back(TokenType::LPAREN, "(", line);
            break;

        case ')':
            tokens.emplace_back(TokenType::RPAREN, ")", line);
            break;

        case '{':
            tokens.emplace_back(TokenType::LBRACE, "{", line);
            break;

        case '}':
            tokens.emplace_back(TokenType::RBRACE, "}", line);
            break;

        default:
            tokens.emplace_back(
                TokenType::UNKNOWN,
                std::string(1, current),
                line);
            break;
        }

        position++;
    }

    tokens.emplace_back(
        TokenType::END_OF_FILE,
        "EOF",
        line);

    return tokens;
}