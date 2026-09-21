#include "codegen.h"

#include <sstream>

std::string CodeGenerator::generateExpression(
    const std::shared_ptr<Expression>& expression)
{
    if (!expression)
    {
        return "";
    }

    auto number =
        std::dynamic_pointer_cast<NumberNode>(
            expression);

    if (number)
    {
        return std::to_string(
            number->value);
    }

    auto identifier =
        std::dynamic_pointer_cast<IdentifierNode>(
            expression);

    if (identifier)
    {
        return identifier->name;
    }

    auto binary =
        std::dynamic_pointer_cast<BinaryExpressionNode>(
            expression);

    if (binary)
    {
        return "(" +
               generateExpression(binary->left) +
               " " +
               binary->op +
               " " +
               generateExpression(binary->right) +
               ")";
    }

    return "";
}

std::string CodeGenerator::generateStatement(
    const std::shared_ptr<Statement>& statement,
    int indent)
{
    std::string spaces(
        indent * 4,
        ' ');

    std::stringstream output;

    auto declaration =
        std::dynamic_pointer_cast<DeclarationNode>(
            statement);

    if (declaration)
    {
        output
            << spaces
            << declaration->variableName
            << " = "
            << generateExpression(
                   declaration->value)
            << "\n";

        return output.str();
    }

    auto assignment =
        std::dynamic_pointer_cast<AssignmentNode>(
            statement);

    if (assignment)
    {
        output
            << spaces
            << assignment->variableName
            << " = "
            << generateExpression(
                   assignment->value)
            << "\n";

        return output.str();
    }

    auto ifElse =
        std::dynamic_pointer_cast<IfElseNode>(
            statement);

    if (ifElse)
    {
        output
            << spaces
            << "if "
            << generateExpression(
                   ifElse->condition)
            << ":\n";

        for (const auto& bodyStatement :
             ifElse->ifBody)
        {
            output
                << generateStatement(
                       bodyStatement,
                       indent + 1);
        }

        if (!ifElse->elseBody.empty())
        {
            output
                << spaces
                << "else:\n";

            for (const auto& bodyStatement :
                 ifElse->elseBody)
            {
                output
                    << generateStatement(
                           bodyStatement,
                           indent + 1);
            }
        }

        return output.str();
    }

    auto whileNode =
        std::dynamic_pointer_cast<WhileNode>(
            statement);

    if (whileNode)
    {
        output
            << spaces
            << "while "
            << generateExpression(
                   whileNode->condition)
            << ":\n";

        for (const auto& bodyStatement :
             whileNode->body)
        {
            output
                << generateStatement(
                       bodyStatement,
                       indent + 1);
        }

        return output.str();
    }

    return "";
}

std::string CodeGenerator::generate(
    const std::shared_ptr<ProgramNode>& program)
{
    std::stringstream output;

    for (const auto& statement :
         program->statements)
    {
        output
            << generateStatement(
                   statement,
                   0);
    }

    return output.str();
}