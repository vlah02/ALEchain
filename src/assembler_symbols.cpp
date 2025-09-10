#include "../inc/assembler_symbols.hpp"
#include <iostream>

void Symbols::add_definition(const std::string& name, const std::string& section, int line) {
    std::cerr << "[SYMTAB] Defining symbol " << name
              << " at section " << section << " offset " << line << std::endl;
    auto& def = table[name];
    def.section = section;
    def.line    = line;
}

void Symbols::add_occurrence(const std::string& name,
                            const std::string& section,
                            int line, bool inPool) {
    auto& def = table[name];
    def.occurences.push_back({section, line, inPool});
}

void Symbols::out(std::ostream& os) const {
    os << ".symbols\n";
    for (const auto& [name, defPtr] : table) {
        const auto& def = defPtr;

        const bool isGlob   = (globals.find(name) != globals.end());
        const bool defined  = (def.line != -1);
        const std::string binding = isGlob ? (defined ? "defined" : "undefined") : "local";

        os << name << " " << binding << " "
           << (weaks.find(name) != weaks.end() ? "weak " : "strong ");

        auto itType = types.find(name);
        os << (itType != types.end() ? itType->second : std::string("NOTYPE"));

        if (binding != "undefined") {
            if (def.section.empty())
                os << " ABS " << def.line;
            else
                os << " " << def.section << " " << def.line;
        }

        os << " 0\n";
    }
    os << "\n";
}

void Symbols::clear() {
    table.clear();
    globals.clear();
    weaks.clear();
    types.clear();
}
