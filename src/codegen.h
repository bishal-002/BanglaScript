#ifndef CODEGEN_H
#define CODEGEN_H

#include <memory>
#include <string>

#include "ast.h"

class CodeGenerator
{
private:
    std::string generateExpression(
        const std::shared_ptr<Expression>& expression
    );

    std::string generateStatement(
        const std::shared_ptr<Statement>& statement,
        int indent = 0
    );

public:
    std::string generate(
        const std::shared_ptr<ProgramNode>& program
    );
};

#endif