#include "../inc/assembler_section.hpp"
#include <bitset>
#include <iostream>
#include <limits>

#include "../inc/assembler_symtab.hpp"
#include "../inc/assembler_reloc.hpp"
#include "../inc/assembler_pool.hpp"

std::unordered_map<std::string, Section*> Section::sections;
static std::vector<std::string> section_order;

Section* Section::extract(const std::string& name) {
    if (sections.find(name) == sections.end()) {
        sections[name] = new Section(name);
        section_order.push_back(name);
    }
    return sections[name];
}

void Section::add_instruction(unsigned char OC, unsigned char MOD,
                              unsigned char RegA, unsigned char RegB,
                              unsigned char RegC, signed short Disp) {
    fprintf(stderr, "add_instruction: OC=0x%X MOD=0x%X RegA=0x%X RegB=0x%X RegC=0x%X Disp=%d | Offset=%zu\n",
        OC, MOD, RegA, RegB, RegC, Disp, data.size());

    fprintf(stderr, "   Will emit at instruction index %zu\n", data.size() / 4);

    this->data.push_back((unsigned char)((OC << 4) | (MOD & 0x0F)));
    this->data.push_back((unsigned char)((RegA << 4) | (RegB & 0x0F)));
    this->data.push_back((unsigned char)((RegC << 4) | ((Disp >> 8) & 0x0F)));
    this->data.push_back((unsigned char)(Disp & 0xFF));

    fprintf(stderr, "   Bytes: %02X %02X %02X %02X\n",
            data[data.size()-4], data[data.size()-3], data[data.size()-2], data[data.size()-1]);
}

void Section::insertInt(int number) {
    unsigned char b0 = (unsigned char)(number & 0xFF);
    this->data.push_back(b0);
    unsigned char b1 = (unsigned char)((number >> 8) & 0xFF);
    this->data.push_back(b1);
    unsigned char b2 = (unsigned char)((number >> 16) & 0xFF);
    this->data.push_back(b2);
    unsigned char b3 = (unsigned char)((number >> 24) & 0xFF);
    this->data.push_back(b3);
}

void Section::insertByte(int number) {
    this->data.push_back((unsigned char)number);
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
