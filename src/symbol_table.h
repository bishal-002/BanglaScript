#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

class SymbolTable
{
private:
    std::unordered_map<std::string, std::string> symbols;

public:
    bool declare(
        const std::string& name,
        const std::string& type
    );

    bool exists(
        const std::string& name
    ) const;

    std::string getType(
        const std::string& name
    ) const;
};

#endif