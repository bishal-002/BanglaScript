#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <memory>

#include "ast.h"
#include "symbol_table.h"

class SemanticAnalyzer
{
private:
    SymbolTable symbolTable;

    bool analyzeStatement(
        const std::shared_ptr<Statement>& statement
    );

    bool analyzeExpression(
        const std::shared_ptr<Expression>& expression
    );

public:
    SemanticAnalyzer();

    bool analyze(
        const std::shared_ptr<ProgramNode>& program
    );
};

#endif