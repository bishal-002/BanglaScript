#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"

class Lexer
{
private:
    std::string source;
    size_t position;
    int line;

public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();
};

#endif