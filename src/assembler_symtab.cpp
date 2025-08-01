#include "../inc/assembler_symtab.hpp"

#include <iostream>
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

void SymTab::add_type(const std::string& symbol, const std::string& type) {
    types.insert({symbol, type});
}

void SymTab::out(std::ostream& os) {
    for (auto& row : table) {
        std::string type = (globals.find(row.first) != globals.end() ? (row.second->line == -1 ? "undefined" : "defined") : "local");
        os << row.first << " "
           << type << " ";
        if (weaks.find(row.first) != weaks.end())
            os << "weak ";
        else
            os << "strong ";
        if (types.find(row.first) != types.end())
            os << types[row.first];
        else
            os << "notyp";
        if (type != std::string("undefined"))
            os << " " << row.second->section << " " << row.second->line;
        os << " " << row.second->occurences.size();
        os << std::endl;

        for (auto& occurence : row.second->occurences) {
            os << occurence.section << "\t" << occurence.line << "\t" << occurence.inPool << std::endl;
        }
    }
}


