#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

struct SymbolOccurrence {
    std::string section;
    int offset;
};

struct SymbolEntry {
    std::string name;
    std::string section;
    int offset;
    std::string binding; // "global", "local", "undefined"
    bool defined;
    std::vector<SymbolOccurrence> uses;
    std::string file; // File where defined (for file offsets)
};

struct RelocationEntry {
    std::string section;
    int offset;
    std::string symbol;
    int addend;
    std::string type;
    std::string file;
};

class LinkerSymTab {
public:
    static void parse_symbols_and_relocations(const std::vector<std::string>& filenames);
    static void resolve_symbols();
    static void apply_relocations();
    static void output_symbols(std::ostream& out);

    static std::unordered_map<std::string, SymbolEntry> symbols;
    static std::vector<RelocationEntry> relocations;
    static std::unordered_map<std::string, int> symbol_values;
};
