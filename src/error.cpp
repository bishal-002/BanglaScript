#include "error.h"

#include <iostream>

void ErrorReporter::report(
    ErrorType type,
    const std::string& message,
    int line)
{
    std::string typeName;

    switch (type)
    {
    case ErrorType::LEXICAL:
        typeName = "Lexical Error";
        break;

    case ErrorType::PARSER:
        typeName = "Parser Error";
        break;

    case ErrorType::SEMANTIC:
        typeName = "Semantic Error";
        break;
    }

    std::cout
        << typeName
        << " (Line "
        << line
        << "): "
        << message
        << '\n';
}