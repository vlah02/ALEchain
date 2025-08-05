#include "../inc/assembler_section.hpp"
#include <bitset>
#include <iostream>

#include "../inc/assembler_symtab.hpp"

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

void Section::add_literal(const std::string& literal, int addend) {
    pool.push_back({data.size(), -1, literal, addend});
}

void Section::add_literal(int literal) {
    pool.push_back({data.size(), literal, ""});
}

Section* Section::extract(const std::string& name) {
    if (sections.find(name) == sections.end()) {
        sections[name] = new Section(name);
        section_order.push_back(name);
    }
    return sections[name];
}

void Section::dumpPool() {
    for (auto& pair : sections) {
        auto section = pair.second;
        auto section_name = pair.first;
        for (auto& literal : section->pool) {
            int lineToReplace = literal.line;
            int literalLine = section->data.size();
            int displacement = literalLine - lineToReplace - 4;
            section->data[lineToReplace + 3] = displacement & 0x0FF;
            section->data[lineToReplace + 2] |= displacement >> 8;

            if (!literal.symbol.empty())
                SymTab::add_occurrence(literal.symbol, section_name, section->data.size(), /*inPool=*/true);

            if (literal.value != -1) {
                section->insertInt(literal.value);
            } else if (!literal.symbol.empty()) {
                auto symEntryIt = SymTab::table.find(literal.symbol);
                if (symEntryIt != SymTab::table.end() && symEntryIt->second->line != -1) {
                    int symbolOffset = symEntryIt->second->line;
                    section->insertInt(symbolOffset + literal.addend);
                    fprintf(stderr, "[POOL] insertInt(%d) for LOCAL symbol=%s at section=%s, offset=%zu\n",
                            symbolOffset, literal.symbol.c_str(), section_name.c_str(), section->data.size());
                } else {
                    section->insertInt(0);
                    fprintf(stderr, "[POOL] insertInt(0) for EXTERNAL symbol=%s at section=%s, offset=%zu\n",
                            literal.symbol.c_str(), section_name.c_str(), section->data.size());
                }
            }

        }
    }
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

