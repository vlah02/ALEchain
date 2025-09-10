#include "../inc/assembler_symtab.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>

std::unordered_map<std::string, SymTab::symDefinition*> SymTab::table;
std::unordered_set<std::string> SymTab::globals;
std::unordered_set<std::string> SymTab::weaks;
std::unordered_map<std::string, std::string> SymTab::types;

void SymTab::add_definition(const std::string& name, const std::string& section, int line) {
    std::cerr << "[SYMTAB] Defining symbol " << name << " at section " << section << " offset " << line << std::endl;
    if (table.find(name) == table.end())
        table[name] = new symDefinition();
    table[name]->section = section;
    table[name]->line = line;
}

void SymTab::add_occurrence(const std::string& name, const std::string& section, int line, bool inPool) {
    if (table.find(name) == table.end())
        table[name] = new symDefinition();
    table[name]->occurences.push_back({section, line, inPool});
}




void SymTab::out(std::ostream& os) {
    os << ".symbols\n";
    for (auto& row : table) {
        std::string binding = (globals.find(row.first) != globals.end()
                               ? (row.second->line == -1 ? "undefined" : "defined")
                               : "local");
        os << row.first << " "
           << binding << " ";

        if (weaks.find(row.first) != weaks.end())
            os << "weak ";
        else
            os << "strong ";

        if (types.find(row.first) != types.end())
            os << types[row.first];
        else
            os << "NOTYPE";

        if (binding != std::string("undefined")) {
            if (row.second->section.empty())
                os << " ABS " << row.second->line;
            else
                os << " " << row.second->section << " " << row.second->line;
        }

        os << " 0\n";
    }
}


