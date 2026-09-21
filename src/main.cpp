#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <clocale>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

// ==========================================
// Token Type to String
// ==========================================

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

    case TokenType::KEYWORD_JOTOKKHON:
        return "KEYWORD_JOTOKKHON";

    case TokenType::KEYWORD_DEKHAO:
        return "KEYWORD_DEKHAO";

    case TokenType::TYPE_SONGKHA:
        return "TYPE_SONGKHA";

    case TokenType::TYPE_LEKHA:
        return "TYPE_LEKHA";

    case TokenType::IDENTIFIER:
        return "IDENTIFIER";

    case TokenType::NUMBER:
        return "NUMBER";

    case TokenType::STRING_LITERAL:
        return "STRING_LITERAL";

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

// ==========================================
// Print Expression
// ==========================================

void printExpression(
    const std::shared_ptr<Expression>& expression,
    int indent = 0)
{
    std::string spaces(
        indent,
        ' ');

    auto number =
        std::dynamic_pointer_cast<NumberNode>(
            expression);

    if (number)
    {
        std::cout
            << spaces
            << "NumberNode: "
            << number->value
            << '\n';

        return;
    }

    auto string =
        std::dynamic_pointer_cast<StringNode>(
            expression);

    if (string)
    {
        std::cout
            << spaces
            << "StringNode: "
            << string->value
            << '\n';

        return;
    }

    auto identifier =
        std::dynamic_pointer_cast<IdentifierNode>(
            expression);

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
        std::dynamic_pointer_cast<
            BinaryExpressionNode>(
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
            << "  Left:"
            << '\n';

        printExpression(
            binary->left,
            indent + 4);

        std::cout
            << spaces
            << "  Right:"
            << '\n';

        printExpression(
            binary->right,
            indent + 4);
    }
}

// ==========================================
// Print Statement
// ==========================================

void printStatement(
    const std::shared_ptr<Statement>& statement,
    int indent = 2)
{
    std::string spaces(
        indent,
        ' ');

    // Declaration
    auto declaration =
        std::dynamic_pointer_cast<
            DeclarationNode>(
            statement);

    if (declaration)
    {
        std::cout
            << spaces
            << "DeclarationNode"
            << '\n';

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
            << "  Value:"
            << '\n';

        printExpression(
            declaration->value,
            indent + 4);

        return;
    }

    // Assignment
    auto assignment =
        std::dynamic_pointer_cast<
            AssignmentNode>(
            statement);

    if (assignment)
    {
        std::cout
            << spaces
            << "AssignmentNode"
            << '\n';

        std::cout
            << spaces
            << "  Name: "
            << assignment->variableName
            << '\n';

        std::cout
            << spaces
            << "  Value:"
            << '\n';

        printExpression(
            assignment->value,
            indent + 4);

        return;
    }

    // Print
    auto print =
        std::dynamic_pointer_cast<
            PrintNode>(
            statement);

    if (print)
    {
        std::cout
            << spaces
            << "PrintNode"
            << '\n';

        std::cout
            << spaces
            << "  Value:"
            << '\n';

        printExpression(
            print->value,
            indent + 4);

        return;
    }

    // If Else
    auto ifElse =
        std::dynamic_pointer_cast<
            IfElseNode>(
            statement);

    if (ifElse)
    {
        std::cout
            << spaces
            << "IfElseNode"
            << '\n';

        std::cout
            << spaces
            << "  Condition:"
            << '\n';

        printExpression(
            ifElse->condition,
            indent + 4);

        std::cout
            << spaces
            << "  If Body:"
            << '\n';

        for (const auto& bodyStatement :
             ifElse->ifBody)
        {
            printStatement(
                bodyStatement,
                indent + 4);
        }

        if (!ifElse->elseBody.empty())
        {
            std::cout
                << spaces
                << "  Else Body:"
                << '\n';

            for (const auto& bodyStatement :
                 ifElse->elseBody)
            {
                printStatement(
                    bodyStatement,
                    indent + 4);
            }
        }

        return;
    }

    // While
    auto whileNode =
        std::dynamic_pointer_cast<
            WhileNode>(
            statement);

    if (whileNode)
    {
        std::cout
            << spaces
            << "WhileNode"
            << '\n';

        std::cout
            << spaces
            << "  Condition:"
            << '\n';

        printExpression(
            whileNode->condition,
            indent + 4);

        std::cout
            << spaces
            << "  Body:"
            << '\n';

        for (const auto& bodyStatement :
             whileNode->body)
        {
            printStatement(
                bodyStatement,
                indent + 4);
        }

        return;
    }
}

// ==========================================
// Main
// ==========================================

int main(int argc, char* argv[])
{
    std::setlocale(
        LC_ALL,
        "bn_BD.UTF-8");

    // ==========================================
    // Check Input File
    // ==========================================

    if (argc < 2)
    {
        std::cout
            << "Usage: BanglaScript.exe <file.bscript>"
            << '\n';

        return 1;
    }

    // ==========================================
    // Read BanglaScript File
    // ==========================================

    std::ifstream inputFile(
        argv[1]);

    if (!inputFile.is_open())
    {
        std::cout
            << "Error: Could not open file."
            << '\n';

        return 1;
    }

    std::string source(
        (std::istreambuf_iterator<char>(
            inputFile)),
        std::istreambuf_iterator<char>()
    );

    inputFile.close();

    std::cout
        << "Compiling: "
        << argv[1]
        << '\n';

    // ==========================================
    // 1. LEXICAL ANALYSIS
    // ==========================================

    Lexer lexer(source);

    std::vector<Token> tokens =
        lexer.tokenize();

    std::cout
        << "\n========== TOKENS =========="
        << '\n';

    for (const auto& token :
         tokens)
    {
        std::cout
            << "Line "
            << token.line
            << " | "
            << tokenTypeToString(
                   token.type)
            << " | "
            << token.lexeme
            << '\n';
    }

    // ==========================================
    // 2. PARSING
    // ==========================================

    Parser parser(tokens);

    auto program =
        parser.parse();

    std::cout
        << "\n========== AST =========="
        << '\n';

    std::cout
        << "ProgramNode"
        << '\n';

    for (const auto& statement :
         program->statements)
    {
        printStatement(statement);
    }

    // ==========================================
    // 3. SEMANTIC ANALYSIS
    // ==========================================

    std::cout
        << "\n========== SEMANTIC ANALYSIS =========="
        << '\n';

    SemanticAnalyzer semanticAnalyzer;

    bool semanticResult =
        semanticAnalyzer.analyze(
            program);

    if (!semanticResult)
    {
        std::cout
            << "Program failed semantic analysis."
            << '\n';

        return 1;
    }

    std::cout
        << "Program passed semantic analysis."
        << '\n';

    // ==========================================
    // 4. CODE GENERATION
    // ==========================================

    std::cout
        << "\n========== CODE GENERATION =========="
        << '\n';

    CodeGenerator codeGenerator;

    std::string generatedCode =
        codeGenerator.generate(
            program);

    std::cout
        << generatedCode;

    // ==========================================
    // 5. WRITE GENERATED CODE TO output.py
    // ==========================================

    std::ofstream outputFile(
        "output.py");

    if (!outputFile.is_open())
    {
        std::cout
            << "\nError: Could not create output.py"
            << '\n';

        return 1;
    }

    outputFile
        << generatedCode;

    outputFile.close();

    std::cout
        << "\nGenerated Python code saved to output.py"
        << '\n';

    return 0;
}