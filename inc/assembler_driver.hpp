#pragma once
#include <string>
#include "assembler_sections.hpp"
#include "assembler_symbols.hpp"
#include "assembler_relocations.hpp"
#include "assembler_pool.hpp"

class Assembler {
public:
    static Assembler& instance();

    int parse(const std::string& inputName, const std::string& outputName);

    Sections& sections() { return sections_; }
    Symbols& symbols() { return symbols_; }
    Relocations& relocs() { return relocs_; }
    Pool& pool() { return pool_; }

    const Sections& sections() const { return sections_; }
    const Symbols& symbols() const { return symbols_; }
    const Relocations& relocs() const { return relocs_; }
    const Pool& pool() const { return pool_; }

private:
    Assembler();
    ~Assembler() = default;
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;
    Assembler(Assembler&&) = delete;
    Assembler& operator=(Assembler&&) = delete;

    Sections sections_;
    Symbols symbols_;
    Relocations relocs_;
    Pool pool_;
};
