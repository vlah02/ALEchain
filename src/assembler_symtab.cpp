#include "../inc/assembler_symtab.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>

std::unordered_map<std::string, SymTab::symDefinition*> SymTab::table;
std::unordered_set<std::string> SymTab::globals;
std::unordered_set<std::string> SymTab::weaks;
std::unordered_map<std::string, std::string> SymTab::types;
std::unordered_map<std::string, long> SymTab::equs;
std::vector<SymTab::EquEntry> SymTab::pending_equs;

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

long SymTab::get_label_value_if_known(const std::string& name) {
    auto it = SymTab::table.find(name);
    if (it != SymTab::table.end() && it->second->line != -1) {
        return it->second->line;
    }
    return std::numeric_limits<long>::min();
}

long SymTab::value_of(const std::string& name) {
    auto eq = SymTab::equs.find(name);
    if (eq != SymTab::equs.end()) return eq->second;

    long lab = SymTab::get_label_value_if_known(name);
    if (lab != std::numeric_limits<long>::min()) return lab;

    return std::numeric_limits<long>::min();
}

void SymTab::resolve_pending_equs() {
    for (auto& e : pending_equs) {
        long L = value_of(e.lhs);
        long R = value_of(e.rhs);

        long val;
        if (L == std::numeric_limits<long>::min() || R == std::numeric_limits<long>::min()) {
            std::cerr << "ERROR: undefined symbol in deferred .equ: "
                      << e.lhs << " or " << e.rhs << std::endl;
            val = 0;
        } else {
            val = (e.op == SymTab::EquEntry::Op::ADD) ? (L + R) : (L - R);
        }

        equs[e.dst] = val;

        add_definition(e.dst, "", (int)val);
    }
    pending_equs.clear();
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
        if (type != std::string("undefined")) {
            if (row.second->section.empty())
                os << " ABS " << row.second->line;
            else
                os << " " << row.second->section << " " << row.second->line;
        }
        os << " " << row.second->occurences.size();
        os << std::endl;

        for (auto& occurence : row.second->occurences) {
            os << occurence.section << "\t" << occurence.line << "\t" << occurence.inPool << std::endl;
        }
    }
}


