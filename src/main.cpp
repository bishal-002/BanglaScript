#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::KEYWORD_DHORI:
        return "KEYWORD_DHORI";

    case TokenType::KEYWORD_JODI:
        return "KEYWORD_JODI";

    case TokenType::KEYWORD_NAHOLE:
        return "KEYWORD_NAHOLE";

    case TokenType::TYPE_SONGKHA:
        return "TYPE_SONGKHA";

    case TokenType::IDENTIFIER:
        return "IDENTIFIER";

    case TokenType::NUMBER:
        return "NUMBER";

    case TokenType::PLUS:
        return "PLUS";

    case TokenType::MINUS:
        return "MINUS";

    case TokenType::STAR:
        return "STAR";

    case TokenType::SLASH:
        return "SLASH";

    case TokenType::ASSIGN:
        return "ASSIGN";

    case TokenType::GT:
        return "GT";

    case TokenType::LT:
        return "LT";

    case TokenType::GTE:
        return "GTE";

    case TokenType::LTE:
        return "LTE";

    case TokenType::EQ:
        return "EQ";

    case TokenType::NEQ:
        return "NEQ";

    case TokenType::SEMICOLON:
        return "SEMICOLON";

    case TokenType::LPAREN:
        return "LPAREN";

    case TokenType::RPAREN:
        return "RPAREN";

    case TokenType::LBRACE:
        return "LBRACE";

    case TokenType::RBRACE:
        return "RBRACE";

    case TokenType::END_OF_FILE:
        return "END_OF_FILE";

    case TokenType::UNKNOWN:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

void printExpression(
    const std::shared_ptr<Expression> &expression,
    int indent = 0)
{
    std::string spaces(indent, ' ');

    if (!expression)
    {
        std::cout << spaces << "NULL Expression\n";
        return;
    }
    auto number =
        std::dynamic_pointer_cast<NumberNode>(expression);

    if (number)
    {
        std::cout
            << spaces
            << "NumberNode: "
            << number->value
            << '\n';

        return;
    }
    auto identifier =
        std::dynamic_pointer_cast<IdentifierNode>(expression);

    if (identifier)
    {
        std::cout
            << spaces
            << "IdentifierNode: "
            << identifier->name
            << '\n';

        return;
    }
    auto binary =
        std::dynamic_pointer_cast<BinaryExpressionNode>(
            expression);

    if (binary)
    {
        std::cout
            << spaces
            << "BinaryExpressionNode: "
            << binary->op
            << '\n';

        std::cout
            << spaces
            << "  Left:\n";

        printExpression(
            binary->left,
            indent + 4);

        std::cout
            << spaces
            << "  Right:\n";

        printExpression(
            binary->right,
            indent + 4);

        return;
    }

    std::cout
        << spaces
        << "Unknown Expression\n";
}

void printStatement(
    const std::shared_ptr<Statement> &statement,
    int indent = 2)
{
    std::string spaces(indent, ' ');

    if (!statement)
    {
        std::cout
            << spaces
            << "NULL Statement\n";

        return;
    }

    auto declaration =
        std::dynamic_pointer_cast<DeclarationNode>(
            statement);

    if (declaration)
    {
        std::cout
            << spaces
            << "DeclarationNode\n";

        std::cout
            << spaces
            << "  Type: "
            << declaration->variableType
            << '\n';

        std::cout
            << spaces
            << "  Name: "
            << declaration->variableName
            << '\n';

        std::cout
            << spaces
            << "  Value:\n";

        printExpression(
            declaration->value,
            indent + 4);

        return;
    }

    auto assignment =
        std::dynamic_pointer_cast<AssignmentNode>(
            statement);

    if (assignment)
    {
        std::cout
            << spaces
            << "AssignmentNode\n";

        std::cout
            << spaces
            << "  Name: "
            << assignment->variableName
            << '\n';

        std::cout
            << spaces
            << "  Value:\n";

        printExpression(
            assignment->value,
            indent + 4);

        return;
    }

    auto ifElse =
        std::dynamic_pointer_cast<IfElseNode>(
            statement);

    if (ifElse)
    {
        std::cout
            << spaces
            << "IfElseNode\n";

        std::cout
            << spaces
            << "  Condition:\n";

        printExpression(
            ifElse->condition,
            indent + 4);

        std::cout
            << spaces
            << "  IF BODY:\n";

        for (const auto &bodyStatement :
             ifElse->ifBody)
        {
            printStatement(
                bodyStatement,
                indent + 4);
        }

        std::cout
            << spaces
            << "  ELSE BODY:\n";

        for (const auto &bodyStatement :
             ifElse->elseBody)
        {
            printStatement(
                bodyStatement,
                indent + 4);
        }

        return;
    }

    std::cout
        << spaces
        << "Unknown Statement\n";
}

int main()
{
    std::string source =
        "ধরি সংখ্যা ক = 10;\n"
        "ক = ক + 20;\n"
        "যদি (ক > 10) {\n"
        "    ক = ক - 1;\n"
        "}\n"
        "নাহলে {\n"
        "    ক = 0;\n"
        "}";

    Lexer lexer(source);

    std::vector<Token> tokens =
        lexer.tokenize();

    std::cout
        << "========== TOKENS ==========\n";

    for (const Token &token : tokens)
    {
        std::cout
            << "Line "
            << token.line
            << " | "
            << tokenTypeToString(token.type)
            << " | "
            << token.lexeme
            << '\n';
    }

    std::cout
        << "\n========== PARSER ==========\n";

    Parser parser(tokens);

    auto program = parser.parse();

    std::cout
        << "\n========== AST ==========\n";

    if (program)
    {
        std::cout
            << "ProgramNode\n";

        for (const auto &statement :
             program->statements)
        {
            printStatement(statement);
        }
    }
    else
    {
        std::cout
            << "AST construction failed.\n";
    }

    std::cout
        << "\n========== SEMANTIC ANALYSIS ==========\n";

    SemanticAnalyzer semanticAnalyzer;

    bool semanticResult =
        semanticAnalyzer.analyze(program);

    if (semanticResult)
    {
        std::cout
            << "Program passed semantic analysis."
            << '\n';
    }
    else
    {
        std::cout
            << "Program failed semantic analysis."
            << '\n';
    }

    return 0;
}