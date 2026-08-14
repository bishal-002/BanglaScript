#include "symbol_table.h"

bool SymbolTable::declare(
    const std::string& name,
    const std::string& type)
{
    if(exists(name))
    {
        return false;
    }

    symbols[name] = type;

    return true;
}

bool SymbolTable::exists(
    const std::string& name) const
{
    return symbols.find(name)
           != symbols.end();
}