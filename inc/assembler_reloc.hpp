#pragma once
#include <string>
#include <vector>
#include <ostream>

struct AsmReloc {
    std::string section;
    int         offset;
    std::string type;
    std::string symbol;
    int         addend;
};

struct AsmRelocs {
    static void add(std::string sec, int off, std::string type, std::string sym, int addend);
    static void out(std::ostream& os);
    static void clear();

    static std::vector<AsmReloc> items;
};
