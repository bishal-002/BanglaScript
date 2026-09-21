#include "semantic.h"
#include "error.h"

#include <iostream>

SemanticAnalyzer::SemanticAnalyzer()
{
}

bool SemanticAnalyzer::analyzeExpression(
    const std::shared_ptr<Expression>& expression)
{
    if (!expression)
    {
        ErrorReporter::report(
            ErrorType::SEMANTIC,
            "Empty expression.",
            0
        );

        return false;
    }

    auto number =
        std::dynamic_pointer_cast<NumberNode>(
            expression);

    if (number)
    {
        return true;
    }

    auto identifier =
        std::dynamic_pointer_cast<IdentifierNode>(
            expression);

    if (identifier)
    {
        if (!symbolTable.exists(identifier->name))
        {
            ErrorReporter::report(
                ErrorType::SEMANTIC,
                "Variable '" +
                identifier->name +
                "' is not declared.",
                0
            );

            return false;
        }

        return true;
    }

    auto binary =
        std::dynamic_pointer_cast<BinaryExpressionNode>(
            expression);

    if (binary)
    {
        bool leftValid =
            analyzeExpression(binary->left);

        bool rightValid =
            analyzeExpression(binary->right);

        return leftValid && rightValid;
    }

    ErrorReporter::report(
        ErrorType::SEMANTIC,
        "Unknown expression.",
        0
    );

    return false;
}

bool SemanticAnalyzer::analyzeStatement(
    const std::shared_ptr<Statement>& statement)
{
    if (!statement)
    {
        ErrorReporter::report(
            ErrorType::SEMANTIC,
            "Empty statement.",
            0
        );

        return false;
    }

    // =========================
    // Declaration
    // =========================

    auto declaration =
        std::dynamic_pointer_cast<DeclarationNode>(
            statement);

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
            ErrorReporter::report(
                ErrorType::SEMANTIC,
                "Variable '" +
                declaration->variableName +
                "' is already declared.",
                0
            );

            return false;
        }

        return true;
    }

    // =========================
    // Assignment
    // =========================

    auto assignment =
        std::dynamic_pointer_cast<AssignmentNode>(
            statement);

    if (assignment)
    {
        if (!symbolTable.exists(
                assignment->variableName))
        {
            ErrorReporter::report(
                ErrorType::SEMANTIC,
                "Variable '" +
                assignment->variableName +
                "' is not declared.",
                0
            );

            return false;
        }

        return analyzeExpression(
            assignment->value);
    }

    // =========================
    // If Else
    // =========================

    auto ifElse =
        std::dynamic_pointer_cast<IfElseNode>(
            statement);

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

    // =========================
    // While Loop
    // =========================

    auto whileNode =
        std::dynamic_pointer_cast<WhileNode>(
            statement);

    if (whileNode)
    {
        if (!analyzeExpression(
                whileNode->condition))
        {
            return false;
        }

        for (const auto& bodyStatement :
             whileNode->body)
        {
            if (!analyzeStatement(
                    bodyStatement))
            {
                return false;
            }
        }

        return true;
    }

    ErrorReporter::report(
        ErrorType::SEMANTIC,
        "Unknown statement.",
        0
    );

    return false;
}

bool SemanticAnalyzer::analyze(
    const std::shared_ptr<ProgramNode>& program)
{
    if (!program)
    {
        ErrorReporter::report(
            ErrorType::SEMANTIC,
            "No program found.",
            0
        );

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