#pragma once
#include <string>

struct PoolItem {
    std::string section;
    int         site;
    long        value;
    std::string symbol;
    int         addend;
    bool        patchInPlace;
};

struct AsmPool {
    static void enqueue_value(const std::string& sec, int site, int value, bool patchInPlace);
    static void enqueue_symbol(const std::string& sec, int site, const std::string& sym, int addend, bool patchInPlace);
    static void flush();
    static void clear();
};
