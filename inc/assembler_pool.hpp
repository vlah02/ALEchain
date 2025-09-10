#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "assembler_sections.hpp"
#include "assembler_symbols.hpp"
#include "assembler_relocations.hpp"

class Pool {
public:
    struct EquEntry {
        enum class Op { ADD, SUB };
        std::string dst, lhs, rhs;
        Op op;
    };

    struct PoolItem {
        std::string section;
        int site;
        long value;
        std::string symbol;
        int addend;
        bool patchInPlace;
    };

    Pool() = default;
    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;

    void add_literal(const std::string& section, int site, const std::string& token);

    void enqueue_value(const std::string& sec, int site, int value, bool patchInPlace);
    void enqueue_symbol(const std::string& sec, int site, const std::string& sym, int addend, bool patchInPlace);

    void flush(Sections& sections, Symbols& symbols, Relocations& relocs);

    void clear();

    std::vector<PoolItem> items;
    std::unordered_map<std::string, long> equs;
    std::vector<EquEntry> pending_equs;

private:
    void resolve_equs(Symbols& symbols);
    void check_weaks(const Symbols& symbols) const;
};
