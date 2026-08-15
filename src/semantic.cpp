#include "semantic.h"

#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(){}

bool SemanticAnalyzer::analyzeExpression(
    const std::shared_ptr<Expression>& expression)
{
    if (!expression)
    {
        std::cout
            << "Semantic Error: Empty expression."
            << '\n';

        return false;
    }

    auto number =
        std::dynamic_pointer_cast<NumberNode>(
            expression
        );

    if (number)
    {
        return true;
    }

    auto identifier =
        std::dynamic_pointer_cast<IdentifierNode>(
            expression
        );

    if (identifier)
    {
        if (!symbolTable.exists(identifier->name))
        {
            std::cout
                << "Semantic Error: Variable '"
                << identifier->name
                << "' is not declared."
                << '\n';

            return false;
        }

        return true;
    }

    auto binary =
        std::dynamic_pointer_cast<BinaryExpressionNode>(
            expression
        );

    if (binary)
    {
        bool leftValid =
            analyzeExpression(binary->left);

        bool rightValid =
            analyzeExpression(binary->right);

        return leftValid && rightValid;
    }

    std::cout
        << "Semantic Error: Unknown expression."
        << '\n';

    return false;
}

bool SemanticAnalyzer::analyzeStatement(
    const std::shared_ptr<Statement>& statement)
{
    if (!statement)
    {
        std::cout
            << "Semantic Error: Empty statement."
            << '\n';

        return false;
    }

    auto declaration =
        std::dynamic_pointer_cast<DeclarationNode>(
            statement
        );

    if (declaration)
    {
        if (!analyzeExpression(
                declaration->value))
        {
            return false;
        }

        if (!symbolTable.declare(
                declaration->variableName,
                declaration->variableType))
        {
            std::cout
                << "Semantic Error: Variable '"
                << declaration->variableName
                << "' is already declared."
                << '\n';

            return false;
        }

        return true;
    }

    auto assignment =
        std::dynamic_pointer_cast<AssignmentNode>(
            statement
        );

    if (assignment)
    {
        if (!symbolTable.exists(
                assignment->variableName))
        {
            std::cout
                << "Semantic Error: Variable '"
                << assignment->variableName
                << "' is not declared."
                << '\n';

            return false;
        }
        return analyzeExpression(
            assignment->value
        );
    }

    auto ifElse =
        std::dynamic_pointer_cast<IfElseNode>(
            statement
        );

    if (ifElse)
    {
        if (!analyzeExpression(
                ifElse->condition))
        {
            return false;
        }

        for (const auto& bodyStatement :
             ifElse->ifBody)
        {
            if (!analyzeStatement(
                    bodyStatement))
            {
                return false;
            }
        }

        for (const auto& bodyStatement :
             ifElse->elseBody)
        {
            if (!analyzeStatement(
                    bodyStatement))
            {
                return false;
            }
        }

        return true;
    }

    std::cout
        << "Semantic Error: Unknown statement."
        << '\n';

    return false;
}

bool SemanticAnalyzer::analyze(
    const std::shared_ptr<ProgramNode>& program)
{
    if (!program)
    {
        std::cout
            << "Semantic Error: No program found."
            << '\n';

        return false;
    }

    for (const auto& statement :
         program->statements)
    {
        if (!analyzeStatement(statement))
        {
            return false;
        }
    }

    std::cout
        << "Semantic Analysis Successful"
        << '\n';

    return true;
}