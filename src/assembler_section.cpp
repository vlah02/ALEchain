#include "../inc/assembler_section.hpp"
#include <bitset>
#include <iostream>

#include "../inc/assembler_symtab.hpp"

std::unordered_map<std::string, Section*> Section::sections;
std::vector<RelocationEntry> Section::relocations;

void Section::add_instruction(unsigned char OC, unsigned char MOD, unsigned char RegA, unsigned char RegB, unsigned char RegC, signed short Disp) {
    // Print debug info before pushing
    fprintf(stderr, "add_instruction: OC=0x%X MOD=0x%X RegA=0x%X RegB=0x%X RegC=0x%X Disp=%d | Offset=%zu\n",
        OC, MOD, RegA, RegB, RegC, Disp, data.size());

    // Show next instruction address (helps match reference)
    fprintf(stderr, "   Will emit at instruction index %zu\n", data.size() / 4);

    // Actually emit
    this->data.push_back(OC << 4 | MOD & 0x0F);
    this->data.push_back(RegA << 4 | RegB & 0x0F);
    this->data.push_back(RegC << 4 | (Disp >> 8) & 0x0F);
    this->data.push_back(Disp & 0xFF);

    // Optionally print raw bytes
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

            if (literal.value != -1) {
                // This is a direct integer literal
                fprintf(stderr, "[POOL] insertInt(%ld) at section=%s, offset=%zu\n",
                    literal.value, section_name.c_str(), section->data.size());
                section->insertInt(literal.value);
            } else if (!literal.symbol.empty()) {
                // This is a symbol (possibly with an addend)
                auto def = SymTab::table.find(literal.symbol);
                if (def != SymTab::table.end() && def->second->line != -1) {
                    int value = def->second->line + literal.addend;
                    fprintf(stderr, "[POOL] insertInt(symbol=%s value=%d addend=%d) at section=%s, offset=%zu\n",
                        literal.symbol.c_str(), value, literal.addend, section_name.c_str(), section->data.size());
                    section->insertInt(value);
                } else {
                    fprintf(stderr, "[POOL] insertInt(0) for undefined symbol=%s at section=%s, offset=%zu\n",
                        literal.symbol.c_str(), section_name.c_str(), section->data.size());
                    Section::relocations.push_back(
                        RelocationEntry(section_name, lineToReplace, "ABS", literal.symbol, literal.addend)
                    );
                    section->insertInt(0);
                }
            }

            if (literal.value == -1)
                SymTab::add_occurrence(literal.symbol, section_name, literal.line);
        }
    }
}

void Section::out(std::ostream& out) {
    for (auto &pair : sections) {
        Section &section = *pair.second;

        out << std::string(".") << std::string(section.getName()) << std::string("\n");

        int i = 1;
        for (auto byte : section.data) {
            std::bitset<8> binary(byte);
            out << binary.to_string() << (i++ % 4 ? std::string(" ") : std::string("\n"));
        }

        out << std::string("\n");
    }
}

void Section::outRelocations(std::ostream& out) {
    for (const auto& r : relocations) {
        if (!r.symbol.empty()) {
            out << r.section << " " << r.offset << " " << r.type << " " << r.symbol << " " << r.addend << "\n";
        }
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

