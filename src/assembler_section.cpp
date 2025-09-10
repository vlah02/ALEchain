#include "../inc/assembler_section.hpp"
#include <bitset>
#include <iostream>
#include <limits>

#include "../inc/assembler_symtab.hpp"
#include "../inc/assembler_reloc.hpp"

std::unordered_map<std::string, Section*> Section::sections;
static std::vector<std::string> section_order;

void Section::add_instruction(unsigned char OC, unsigned char MOD, unsigned char RegA, unsigned char RegB, unsigned char RegC, signed short Disp) {
    fprintf(stderr, "add_instruction: OC=0x%X MOD=0x%X RegA=0x%X RegB=0x%X RegC=0x%X Disp=%d | Offset=%zu\n",
        OC, MOD, RegA, RegB, RegC, Disp, data.size());

    fprintf(stderr, "   Will emit at instruction index %zu\n", data.size() / 4);

    this->data.push_back(OC << 4 | MOD & 0x0F);
    this->data.push_back(RegA << 4 | RegB & 0x0F);
    this->data.push_back(RegC << 4 | (Disp >> 8) & 0x0F);
    this->data.push_back(Disp & 0xFF);

    fprintf(stderr, "   Bytes: %02X %02X %02X %02X\n", data[data.size()-4], data[data.size()-3], data[data.size()-2], data[data.size()-1]);
}

void Section::add_literal(const std::string& literal, int addend, bool patchInPlace) {
    fprintf(stderr, "add_literal(sym='%s', addend=%d, patchInPlace=%d)\n", literal.c_str(), addend, patchInPlace);
    pool.push_back({data.size(), -1, literal, addend, patchInPlace});
}
void Section::add_literal(int literal, bool patchInPlace) {
    fprintf(stderr, "add_literal(val=%d, patchInPlace=%d)\n", literal, patchInPlace);
    pool.push_back({data.size(), literal, "", 0, patchInPlace});
}

void Section::add_symbol_or_equ_literal(const std::string& sym) {
    long equVal = SymTab::try_resolve_equ(sym);
    if (equVal != std::numeric_limits<long>::min()) {
        this->add_literal(static_cast<int>(equVal), true);
    } else {
        this->add_literal(sym, 0, false);
    }
}

Section* Section::extract(const std::string& name) {
    if (sections.find(name) == sections.end()) {
        sections[name] = new Section(name);
        section_order.push_back(name);
    }
    return sections[name];
}

void Section::dumpPool() {
    SymTab::resolve_pending_equs();

    for (auto& pair : sections) {
        auto section = pair.second;
        auto section_name = pair.first;
        for (auto& literal : section->pool) {
            int lineToReplace = literal.line;

            int literalLine = (int)section->data.size();
            int displacement = literalLine - lineToReplace - 4;
            section->data[lineToReplace + 3] = displacement & 0x0FF;
            section->data[lineToReplace + 2] |= (displacement >> 8) & 0x0F;

            if (!literal.symbol.empty()) {
                SymTab::add_occurrence(literal.symbol, section_name, (int)section->data.size(), /*inPool=*/true);
            }

            if (literal.value != -1) {
                section->insertInt(literal.value);
                continue;
            }

            if (!literal.symbol.empty()) {
                int patch_off = (int)section->data.size();

                int valueToInsert = 0;
                auto symEntryIt = SymTab::table.find(literal.symbol);
                bool isLocalAndDefined = (symEntryIt != SymTab::table.end() && symEntryIt->second->line != -1);
                if (isLocalAndDefined) {
                    valueToInsert = symEntryIt->second->line + literal.addend;
                    if (literal.patchInPlace) {
                        section->data[lineToReplace + 2] =
                            ((valueToInsert >> 8) & 0x0F) | (section->data[lineToReplace + 2] & 0xF0);
                        section->data[lineToReplace + 3] = valueToInsert & 0xFF;
                    }
                    section->insertInt(valueToInsert);
                    fprintf(stderr, "[POOL] insertInt(%d) for LOCAL symbol=%s at section=%s, offset=%zu\n",
                            valueToInsert, literal.symbol.c_str(), section_name.c_str(), section->data.size());
                } else {
                    if (literal.patchInPlace) {
                        section->data[lineToReplace + 2] &= 0xF0;
                        section->data[lineToReplace + 3]  = 0;
                    }
                    section->insertInt(0);
                    fprintf(stderr, "[POOL] insertInt(0) for EXTERNAL symbol=%s at section=%s, offset=%zu\n",
                            literal.symbol.c_str(), section_name.c_str(), section->data.size());
                }

                AsmRelocs::add(section_name, patch_off, "R_ABS32", literal.symbol, literal.addend);
            }
        }
    }

    SymTab::validate_weaks_or_die();
}

void Section::out(std::ostream& out) {
    for (const auto& section_name : section_order) {
        Section& section = *sections[section_name];
        out << "." << section.getName() << "\n";
        int i = 1;
        for (auto byte : section.data) {
            std::bitset<8> binary(byte);
            out << binary.to_string() << (i++ % 4 ? " " : "\n");
        }
        out << "\n";
    }
}

void Section::insertInt(int number) {
    unsigned char b0 = number & 0xFF;
    this->data.push_back(b0);
    unsigned char b1 = (number >> 8) & 0xFF;
    this->data.push_back(b1);
    unsigned char b2 = (number >> 16) & 0xFF;
    this->data.push_back(b2);
    unsigned char b3 = (number >> 24) & 0xFF;
    this->data.push_back(b3);
}

void Section::insertByte(int number) {
    this->data.push_back(number);
}

