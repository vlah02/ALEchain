#include "../inc/assembler_symtab.hpp"

#include <ostream>

std::unordered_map<std::string, SymTab::symDefinition*> SymTab::table;
std::unordered_set<std::string> SymTab::globals;

void SymTab::add_definition(const std::string& name, const std::string& section, int line) {
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
    for (auto& row : table) {
        std::string type = (globals.find(row.first) != globals.end() ? (row.second->line == -1 ? "undefined" : "defined") : "local");
        os << row.first << " "
           << type << " "
           << row.second->occurences.size()
           << " ";
        if (type != std::string("undefined"))
            os << row.second->section << " " << row.second->line;
        os << std::endl;

        for (auto& occurence : row.second->occurences) {
            os << occurence.section << " " << occurence.line << " " << occurence.inPool << std::endl;
        }
    }
}


