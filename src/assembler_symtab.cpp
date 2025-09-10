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

void SymTab::resolve_pending_equs() {
    auto lookup = [&](const std::string& sym, long& out) -> bool {
        if (auto it = equs.find(sym); it != equs.end()) {
            out = it->second;
            return true;
        }
        if (auto it = table.find(sym); it != table.end() && it->second->line != -1) {
            out = static_cast<long>(it->second->line);
            return true;
        }
        return false;
    };

    for (auto& e : pending_equs) {
        long L = 0, R = 0;
        bool okL = lookup(e.lhs, L);
        bool okR = lookup(e.rhs, R);

        long val = 0;
        if (!okL || !okR) {
            std::cerr << "ERROR: undefined symbol in deferred .equ: "
                      << (!okL ? e.lhs : "") << ((!okL && !okR) ? " and " : "")
                      << (!okR ? e.rhs : "") << "\n";
        } else {
            val = (e.op == SymTab::EquEntry::Op::ADD) ? (L + R) : (L - R);
        }

        equs[e.dst] = val;
        add_definition(e.dst, "", static_cast<int>(val));
    }

    pending_equs.clear();
}

void SymTab::validate_weaks_or_die() {
    for (const auto& w : weaks) {
        auto it = table.find(w);
        if (it == table.end()) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
        auto* def = it->second;
        bool hasDef = (def->line != -1);
        bool hasOcc = !def->occurences.empty();
        if (!hasDef && !hasOcc) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
    }
}

void SymTab::out(std::ostream& os) {
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


