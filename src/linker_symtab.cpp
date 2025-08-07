#include "../inc/linker_symtab.hpp"
#include "../inc/linker_section.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, SymbolEntry> LinkerSymTab::symbols;
std::vector<RelocationEntry> LinkerSymTab::relocations;
std::unordered_map<std::string, int> LinkerSymTab::symbol_values;
std::vector<Occurrence> LinkerSymTab::occurrences;

void LinkerSymTab::parse_symbols_and_relocations(const std::vector<std::string>& filenames) {
    symbols.clear();
    relocations.clear();
    for (const auto& fname : filenames) {
        std::ifstream file(fname);
        if (!file.is_open()) {
            std::cerr << "Failed to open assembler output: " << fname << "\n";
            exit(1);
        }

        std::string line;
        std::string pending_line;
        bool in_symbols = false, in_relocations = false;

        auto trim = [](std::string& s) {
            s.erase(s.find_last_not_of(" \n\r\t") + 1);
            s.erase(0, s.find_first_not_of(" \n\r\t"));
        };

        while (true) {
            if (!pending_line.empty()) {
                line = pending_line;
                pending_line.clear();
            } else if (!std::getline(file, line)) {
                break;
            }
            trim(line);

            if (line.empty()) continue;
            if (line == ".symbols") {
                in_symbols = true;
                in_relocations = false;
                continue;
            }
            if (line == ".relocations") {
                in_symbols = false;
                in_relocations = true;
                continue;
            }

            if (in_symbols) {
                std::istringstream iss(line);
                std::string name, binding;
                iss >> name >> binding;
                if (binding == "defined" || binding == "local" || binding == "global" || binding == "undefined") {
                    SymbolEntry entry;
                    entry.name = name;
                    entry.binding = binding;
                    if (binding == "defined" || binding == "local" || binding == "global") {
                        std::string strength, notyp;
                        iss >> strength >> notyp
                            >> entry.section
                            >> entry.offset;
                        if (entry.section == "ABS") entry.section = "";
                        entry.defined = true;
                        entry.file = fname;
                    } else {
                        std::string maybe_section;
                        iss >> maybe_section >> entry.offset;
                        entry.defined = false;
                        entry.file = "";
                    }
                    if (binding == "defined" || symbols.find(name) == symbols.end()) {
                        symbols[name] = entry;
                    }

                    std::cerr << "[DEBUG][SYMBOL] name='" << name << "' binding='" << binding
                              << "' section='" << entry.section << "' offset=" << entry.offset << std::endl;

                    while (std::getline(file, line)) {
                        trim(line);
                        if (line.empty()) continue;
                        if (line[0] == '.') {
                            pending_line = line;
                            break;
                        }
                        std::istringstream peek(line);
                        std::string first, second, third;
                        peek >> first >> second >> third;
                        if (second == "defined"
                            || second == "local"
                            || second == "global"
                            || second == "undefined") {
                            pending_line = line;
                            break;
                        }
                        std::istringstream occiss(line);
                        std::string sec;
                        int off;
                        int inPoolInt = 0;
                        occiss >> sec >> off >> inPoolInt;

                        Occurrence occ;
                        occ.symbol = name;
                        occ.section = sec;
                        occ.file = fname;
                        occ.offset = off;
                        occ.inPool = (inPoolInt != 0);
                        LinkerSymTab::occurrences.push_back(occ);
                    }
                    continue;
                }
            }
            else if (in_relocations) {
                std::istringstream iss(line);
                RelocationEntry rel;
                iss >> rel.section >> rel.offset >> rel.type >> rel.symbol >> rel.addend;
                rel.file = fname;
                relocations.push_back(rel);
            }
        }
    }
}

void LinkerSymTab::resolve_symbols() {
    symbol_values.clear();
    for (auto& [name, entry] : symbols) {
        if ((entry.binding == "defined" || entry.binding == "local" || entry.binding == "global")) {
            if (entry.section.empty() || LinkerSections::merged_sections.find(entry.section) == LinkerSections::merged_sections.end()) {
                symbol_values[name] = entry.offset;
            } else {
                long base = LinkerSections::get_section_base(entry.section);
                int file_offset = LinkerSections::get_offset(entry.section, entry.file);
                symbol_values[name] = base + file_offset + entry.offset;
            }
        }
    }
    std::cerr << "[DEBUG][SYMBOL_VALUES]\n";
    for (const auto& [name, val] : symbol_values) {
        std::cerr << "  '" << name << "' = 0x" << std::hex << val << std::endl;
    }
}

void LinkerSymTab::patch_occurrences() {
    for (const auto& occ : LinkerSymTab::occurrences) {
        if (!symbol_values.count(occ.symbol)) {
            std::cerr << "[SKIP] Occurrence: " << occ.symbol << " not found in symbol values!\n";
            continue;
        }
        int value = symbol_values[occ.symbol];
        auto sec_it = LinkerSections::merged_sections.find(occ.section);
        if (sec_it == LinkerSections::merged_sections.end()) {
            std::cerr << "[SKIP] Section not found for occurrence: " << occ.section << "\n";
            continue;
        }
        auto& data = sec_it->second.data;
        int patch_offset = occ.offset + LinkerSections::get_offset(occ.section, occ.file);

        std::cerr << "[PATCH_OCC] "
                  << "symbol='" << occ.symbol << "' "
                  << "section='" << occ.section << "' "
                  << "file='" << occ.file << "' "
                  << "offset_in_section=" << occ.offset << " "
                  << "resolved_addr=0x" << std::hex << value << " "
                  << "patch_offset=0x" << std::hex << patch_offset
                  << std::dec << "\n";
        if (patch_offset + 3 < (int)data.size()) {
            int oldval = data[patch_offset] | (data[patch_offset+1] << 8)
                       | (data[patch_offset+2] << 16) | (data[patch_offset+3] << 24);
            std::cerr << "    Old value at patch: 0x" << std::hex << oldval << std::dec << "\n";
            data[patch_offset + 0] = (value & 0xFF);
            data[patch_offset + 1] = (value >> 8) & 0xFF;
            data[patch_offset + 2] = (value >> 16) & 0xFF;
            data[patch_offset + 3] = (value >> 24) & 0xFF;
            int newval = data[patch_offset] | (data[patch_offset+1] << 8)
                       | (data[patch_offset+2] << 16) | (data[patch_offset+3] << 24);
            std::cerr << "    New value at patch: 0x" << std::hex << newval << std::dec << "\n";
        } else {
            std::cerr << "[ERROR] Patch offset out of range for symbol '" << occ.symbol
                      << "' at offset " << patch_offset << " (data size: " << data.size() << ")\n";
        }
    }
}
