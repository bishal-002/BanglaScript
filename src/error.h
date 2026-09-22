#ifndef ERROR_H
#define ERROR_H

#include <string>

enum class ErrorType
{
    LEXICAL,
    PARSER,
    SEMANTIC
};

class ErrorReporter
{
public:
    static void report(
        ErrorType type,
        const std::string& message,
        int line
    );
};

#endif