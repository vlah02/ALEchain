#include "../inc/linker_symtab.hpp"
#include "../inc/linker_section.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_set>

std::unordered_map<std::string, SymbolEntry> LinkerSymTab::symbols;
std::vector<RelocationEntry> LinkerSymTab::relocations;
std::unordered_map<std::string, int> LinkerSymTab::symbol_values;
std::vector<Occurrence> LinkerSymTab::occurrences;
std::unordered_map<std::string, int> LinkerSymTab::def_counts;
std::unordered_map<std::string, int> LinkerSymTab::strong_def_counts;
std::unordered_map<std::string, std::string> LinkerSymTab::first_non_notype;

void LinkerSymTab::parse_symbols_and_relocations(const std::vector<std::string>& filenames) {
    symbols.clear();
    relocations.clear();
    occurrences.clear();
    symbol_values.clear();
    def_counts.clear();
    strong_def_counts.clear();

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
            if (s.empty()) return;
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

                    std::vector<std::string> toks;
                    {
                        std::string t;
                        while (iss >> t) toks.push_back(t);
                    }

                    int numOcc = 0;
                    if (!toks.empty()) {
                        numOcc = std::stoi(toks.back());
                        toks.pop_back();
                    }

                    if (toks.empty()) {
                        std::cerr << "Error parsing symbols for '" << name << "': missing strength token\n";
                        exit(4);
                    }
                    const std::string strength = toks.front();
                    toks.erase(toks.begin());
                    if (strength == "weak") entry.is_weak = true;
                    else if (strength == "strong") entry.is_weak = false;
                    else {
                        std::cerr << "Error parsing symbols: expected 'weak' or 'strong' for symbol '"
                                  << name << "' but got '" << strength << "'\n";
                        exit(4);
                    }

                    entry.stype = toks.empty() ? std::string("NOTYPE") : toks[0];
                    if (!toks.empty()) toks.erase(toks.begin());

                    auto record_type_consistency = [&](const std::string& sym, const std::string& ty) {
                        if (!ty.empty() && ty != "NOTYPE") {
                            auto it = first_non_notype.find(sym);
                            if (it == first_non_notype.end()) first_non_notype[sym] = ty;
                            else if (it->second != ty) {
                                std::cerr << "Error: symbol '" << sym << "' has conflicting types across files: "
                                          << it->second << " vs " << ty << "\n";
                                exit(4);
                            }
                        }
                    };
                    record_type_consistency(name, entry.stype);

                    bool have_loc = (toks.size() >= 2);
                    std::string raw_section = have_loc ? toks[0] : "";
                    int raw_offset = have_loc ? std::stoi(toks[1], nullptr, 0) : 0;

                    bool is_defined_here = false;
                    if (have_loc) {
                        if (!(raw_section == "ABS" && raw_offset == -1)) {
                            is_defined_here = true;
                        }
                    }
                    entry.defined = is_defined_here;
                    entry.section = (raw_section == "ABS") ? "" : raw_section;
                    entry.offset  = raw_offset;

                    entry.file = fname;

                    if (entry.defined) {
                        def_counts[name] += 1;
                        if (!entry.is_weak) strong_def_counts[name] += 1;
                    }

                    auto it = symbols.find(name);
                    if (it == symbols.end()) {
                        symbols[name] = entry;
                    } else {
                        SymbolEntry& cur = it->second;
                        if (!cur.defined && entry.defined) {
                            cur = entry;
                        }
                        else if (cur.defined && entry.defined) {
                            if (cur.is_weak && !entry.is_weak) {
                                cur = entry;
                            }
                        }
                    }

                    for (int i = 0; i < numOcc; ++i) {
                        std::string occLine;
                        if (!std::getline(file, occLine)) break;
                        trim(occLine);
                        if (occLine.empty() || occLine[0] == '.') {
                            pending_line = occLine;
                            break;
                        }
                        std::istringstream occiss(occLine);
                        std::string sec;
                        int off = 0;
                        int inPoolInt = 0;
                        occiss >> sec >> off >> inPoolInt;

                        Occurrence occ;
                        occ.symbol = name;
                        occ.section = sec;
                        occ.file = fname;
                        occ.offset = off;
                        occ.inPool = (inPoolInt != 0);
                        occurrences.push_back(occ);
                    }
                    continue;
                }
            } else if (in_relocations) {
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
                symbol_values[name] = static_cast<int>(base) + file_offset + entry.offset;
            }
        }
    }
    std::cerr << "[DEBUG][SYMBOL_VALUES]\n";
    for (const auto& [name, val] : symbol_values) {
        std::cerr << "  '" << name << "' = 0x" << std::hex << val << std::dec << std::endl;
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

bool LinkerSymTab::check_no_multiple_definitions(bool die_on_error) {
    for (const auto& [name, cnt] : strong_def_counts) {
        if (cnt > 1) {
            std::cerr << "Error: symbol multiply defined (strong): " << name << "\n";
            if (die_on_error) return false;
        }
    }
    return true;
}

static int adjusted_symbol_offset(const SymbolEntry& e) {
    if (e.section.empty()) {
        return e.offset;
    }
    int file_off = LinkerSections::get_offset(e.section, e.file);
    return file_off + e.offset;
}

static int adjusted_occurrence_offset(const Occurrence& o) {
    int file_off = LinkerSections::get_offset(o.section, o.file);
    return file_off + o.offset;
}

void LinkerSymTab::output_relocatable(std::ostream& out) {
    std::unordered_map<std::string, std::vector<Occurrence>> occ_by_sym;
    for (const auto& occ : occurrences) {
        occ_by_sym[occ.symbol].push_back(occ);
    }

    std::vector<std::string> names;
    names.reserve(symbols.size());
    for (const auto& kv : symbols) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    names.erase(std::unique(names.begin(), names.end()), names.end());

    for (const auto& name : names) {
        const SymbolEntry& e = symbols.at(name);
        const bool is_defined = e.defined;

        std::string ty = e.stype.empty() ? std::string("NOTYPE") : e.stype;
        out << name << " "
            << (is_defined ? "defined" : "undefined") << " "
            << (e.is_weak ? "weak " : "strong ")
            << ty;

        const auto& occs = occ_by_sym[name];
        if (is_defined) {
            std::string section_out = e.section.empty() ? "ABS" : e.section;
            int offset_out = e.section.empty() ? e.offset : adjusted_symbol_offset(e);
            out << " " << section_out << " " << offset_out;
        }
        out << " " << occs.size() << "\n";

        for (const auto& oc : occs) {
            out << oc.section << "\t" << adjusted_occurrence_offset(oc) << "\t" << (oc.inPool ? 1 : 0) << "\n";
        }
    }
}
