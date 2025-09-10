#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct PoolItem {
    std::string section;
    int         site;
    long        value;
    std::string symbol;
    int         addend;
    bool        patchInPlace;
};

struct EquEntry {
    enum class Op { ADD, SUB };
    std::string dst;
    std::string lhs;
    std::string rhs;
    Op op;
};

struct AsmPool {
    static std::vector<EquEntry> pending_equs;
    static std::unordered_map<std::string, long> equs;

    static void enqueue_value(const std::string& sec, int site, int value, bool patchInPlace);
    static void enqueue_symbol(const std::string& sec, int site, const std::string& sym, int addend, bool patchInPlace);
    static void add_literal(const std::string& section, int site, const std::string& sym);
    static void flush();
    static void resolve_equs();
    static void check_weaks();
    static void clear();
};
