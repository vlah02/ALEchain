#include "../inc/linker_symtab.hpp"
#include "../inc/linker_section.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, SymbolEntry> LinkerSymTab::symbols;
std::vector<RelocationEntry> LinkerSymTab::relocations;
std::unordered_map<std::string, int> LinkerSymTab::symbol_values;

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
            // Use pending_line if present, otherwise read next line
            if (!pending_line.empty()) {
                line = pending_line;
                pending_line.clear();
            } else if (!std::getline(file, line)) {
                break;
            }
            trim(line);
            std::cout << "[DEBUG] Line: '" << line << "'" << std::endl;

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
                    if (binding == "defined") {
                        // skip strength and notyp
                        std::string strength, notyp;
                        iss >> strength >> notyp
                            >> entry.section
                            >> entry.offset;
                        entry.defined = true;
                        entry.file = fname;
                    } else {
                        // undefined/local/global without real definition
                        std::string maybe_section;
                        iss >> maybe_section >> entry.offset;
                        entry.defined = false;
                        entry.file = "";
                    }
                    // Only overwrite existing if this is a true definition
                    if (binding == "defined" || symbols.find(name) == symbols.end()) {
                        symbols[name] = entry;
                    }

                    // Skip any occurrence lines until the next symbol or section header
                    while (std::getline(file, line)) {
                        trim(line);
                        if (line.empty()) continue;
                        if (line[0] == '.') {
                            pending_line = line;
                            break;
                        }
                        std::istringstream peek(line);
                        std::string first, second;
                        peek >> first >> second;
                        if (second == "defined"
                         || second == "local"
                         || second == "global"
                         || second == "undefined") {
                            // This is actually the next symbol line
                            pending_line = line;
                            break;
                        }
                        // else it's just an occurrence—skip it
                    }
                    continue;
                }
                // otherwise, ignore non-symbol lines
            }
            else if (in_relocations) {
                std::istringstream iss(line);
                RelocationEntry rel;
                iss >> rel.section >> rel.offset >> rel.type >> rel.symbol >> rel.addend;
                rel.file = fname;
                relocations.push_back(rel);
            }
            std::cout << "OUTSIDE!!!!" << std::endl;
            // else: outside of symbols/relocations, ignore
        }
    }

    std::cout << "Loaded " << relocations.size() << " relocations.\n";
    for (const auto& rel : relocations) {
        std::cout << "Relocation: section=" << rel.section
                  << " offset=" << rel.offset
                  << " symbol=" << rel.symbol
                  << " file=" << rel.file << "\n";
    }
}

void LinkerSymTab::resolve_symbols() {
    symbol_values.clear();
    for (auto& [name, entry] : symbols) {
        // Add both "defined" and local symbols with section/offset info
        if ((entry.binding == "defined" || entry.binding == "local" || entry.binding == "global") && !entry.section.empty()) {
            long base = LinkerSections::get_section_base(entry.section);
            int file_offset = LinkerSections::get_offset(entry.section, entry.file);
            symbol_values[name] = base + file_offset + entry.offset;
        }
    }
    std::cout << "SYMBOL TABLE ENTRIES:\n";
    for (auto& [name, entry] : symbols) {
        std::cout << "  name=" << name << " binding=" << entry.binding
                  << " section=" << entry.section
                  << " offset=" << entry.offset
                  << " file=" << entry.file
                  << " defined=" << entry.defined << std::endl;
    }
}

void LinkerSymTab::apply_relocations() {
    for (const auto& rel : relocations) {
        auto sec_it = LinkerSections::merged_sections.find(rel.section);
        if (sec_it == LinkerSections::merged_sections.end()) {
            std::cerr << "Section not found: " << rel.section << std::endl;
            continue;
        }
        auto& data = sec_it->second.data;
        int value = symbol_values.count(rel.symbol) ? symbol_values[rel.symbol] + rel.addend : rel.addend;

        int patch_offset = rel.offset + LinkerSections::get_offset(rel.section, rel.file);

        std::cout << "[DEBUG] Reloc: " << rel.section << "@" << patch_offset << " = " << std::hex << value << std::dec << " (for symbol " << rel.symbol << " from file " << rel.file << ")\n";

        if (patch_offset + 3 < (int)data.size()) {
            std::cout << "[DEBUG] Before patch: "
                << std::hex
                << (int)data[patch_offset] << " "
                << (int)data[patch_offset+1] << " "
                << (int)data[patch_offset+2] << " "
                << (int)data[patch_offset+3] << std::dec << "\n";
            data[patch_offset + 0] = (value & 0xFF);
            data[patch_offset + 1] = (value >> 8) & 0xFF;
            data[patch_offset + 2] = (value >> 16) & 0xFF;
            data[patch_offset + 3] = (value >> 24) & 0xFF;
            std::cout << "[DEBUG] After patch: "
                << std::hex
                << (int)data[patch_offset] << " "
                << (int)data[patch_offset+1] << " "
                << (int)data[patch_offset+2] << " "
                << (int)data[patch_offset+3] << std::dec << "\n";
        } else {
            std::cerr << "Relocation patch out of bounds for " << rel.symbol << " at " << patch_offset << " (data size: " << data.size() << ")\n";
        }
    }
}

void LinkerSymTab::output_symbols(std::ostream& out) {
    for (const auto& [name, entry] : symbols) {
        out << name << " " << entry.binding << " " << entry.section << " " << entry.offset << "\n";
    }
}
