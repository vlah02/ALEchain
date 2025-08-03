#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct SymbolEntry {
    std::string name, binding, section, file;
    int offset = 0;
    bool defined = false;
};

struct RelocationEntry {
    std::string section, symbol, file, type;
    int offset = 0;
    int addend = 0;
};

struct Occurrence {
    std::string symbol;
    std::string section;
    std::string file;
    int offset;
    bool inPool;

};

struct LinkerSymTab {
    static std::unordered_map<std::string, SymbolEntry> symbols;
    static std::vector<RelocationEntry> relocations;
    static std::vector<Occurrence> occurrences;
    static std::unordered_map<std::string, int> symbol_values;

    static void parse_symbols_and_relocations(const std::vector<std::string>& filenames);
    static void resolve_symbols();
    static void apply_relocations();
    static void patch_occurrences();
};
