#include "../inc/linker_section.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cstdint>
#include <map>
#include <set>

std::unordered_map<std::string, LinkerSection> LinkerSections::merged_sections;
std::vector<std::string> LinkerSections::section_order;

static bool is_bin8(const std::string& s) {
    if (s.size() != 8) return false;
    for (char c : s) if (c != '0' && c != '1') return false;
    return true;
}

static void parse_section_block(
    std::ifstream& file,
    const std::string& section_name,
    LinkerSection& section,
    const std::string& filename)
{
    size_t start_offset = section.data.size();
    std::string line;

    std::streampos prev_pos = file.tellg();

    while (true) {
        std::streampos here = file.tellg();
        if (!std::getline(file, line)) break;

        if (line.empty() || (!line.empty() && line[0] == '.')) {
            file.clear();
            file.seekg(here);
            break;
        }

        std::istringstream iss(line);
        std::string byte_str;
        while (iss >> byte_str) {
            if (!is_bin8(byte_str)) {
                std::cerr << "[WARN] Non-binary token '" << byte_str
                          << "' in " << filename << " section ." << section_name << "\n";
                continue;
            }
            unsigned char val = static_cast<unsigned char>(std::stoi(byte_str, nullptr, 2));
            section.data.push_back(val);
        }

        prev_pos = file.tellg();
    }

    section.file_offsets[filename] = start_offset;
}

void LinkerSections::load_sections(const std::vector<std::string>& filenames)
{
    merged_sections.clear();
    section_order.clear();
    for (const auto& fname : filenames) {
        std::ifstream file(fname);
        if (!file.is_open()) {
            std::cerr << "Failed to open assembler output: " << fname << "\n";
            exit(1);
        }
        std::string line;

        while (std::getline(file, line)) {
            if (line == ".sections") break;
        }

        while (true) {
            std::streampos here = file.tellg();
            if (!std::getline(file, line)) break;
            if (line.empty()) continue;
            if (line == ".symbols") {
                file.clear();
                file.seekg(here);
                break;
            }
            if (line[0] == '.') {
                std::string section_name = line.substr(1);
                if (merged_sections.find(section_name) == merged_sections.end()) {
                    merged_sections[section_name] = LinkerSection(section_name);
                    section_order.push_back(section_name);
                }
                parse_section_block(file, section_name, merged_sections[section_name], fname);
            }
        }
    }
}

void LinkerSections::merge_sections(const std::vector<std::pair<std::string, long>>& placements) {
    std::unordered_map<std::string, long> explicit_placement;
    for (const auto& p : placements)
        explicit_placement[p.first] = p.second;

    std::cerr << "Section order: ";
    for (auto& secname : section_order) {
        std::cerr << secname << "(" << merged_sections[secname].data.size() << ") ";
    }
    std::cerr << std::endl;

    long current_addr = 0;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];

        if (sec.data.empty()) {
            sec.base_addr = -1;
            continue;
        }

        if (explicit_placement.count(secname)) {
            sec.base_addr = explicit_placement[secname];
            current_addr = sec.base_addr + sec.data.size();
        } else {
            long next_explicit = -1;
            for (const auto& [ename, eaddr] : explicit_placement) {
                auto pos_cur = std::find(section_order.begin(), section_order.end(), secname);
                auto pos_exp = std::find(section_order.begin(), section_order.end(), ename);
                if (pos_exp != section_order.end() && pos_exp > pos_cur && eaddr >= current_addr) {
                    if (next_explicit == -1 || eaddr < next_explicit) {
                        next_explicit = eaddr;
                    }
                }
            }
            if (next_explicit != -1 && current_addr + (long)sec.data.size() > next_explicit) {
                current_addr = next_explicit;
            }
            sec.base_addr = current_addr;
            current_addr += sec.data.size();
        }
    }

    std::vector<std::pair<long, long>> ranges;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];
        if (sec.base_addr >= 0 && sec.data.size() > 0)
            ranges.push_back({sec.base_addr, sec.base_addr + (long)sec.data.size()});
    }
    std::sort(ranges.begin(), ranges.end());
    for (size_t i = 1; i < ranges.size(); ++i) {
        if (ranges[i-1].second > ranges[i].first) {
            std::cerr << "Section overlap in memory layout!\n";
            exit(2);
        }
    }
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];
        std::cerr << "[DEBUG] Section: " << secname
                  << " base=0x" << std::hex << sec.base_addr
                  << " size=" << std::dec << sec.data.size() << std::endl;
    }
}

int LinkerSections::get_offset(const std::string& section, const std::string& file) {
    auto it = merged_sections.find(section);
    if (it == merged_sections.end()) return 0;
    auto jt = it->second.file_offsets.find(file);
    if (jt == it->second.file_offsets.end()) return 0;
    return jt->second;
}
long LinkerSections::get_section_base(const std::string& section) {
    auto it = merged_sections.find(section);
    if (it == merged_sections.end()) return 0;
    return it->second.base_addr < 0 ? 0 : it->second.base_addr;
}

void LinkerSections::output_hex(std::ostream& out) {
    std::map<long, uint8_t> memory;
    std::set<long> all_lines;

    for (const auto& sec_name : section_order) {
        const auto& sec = merged_sections[sec_name];
        if (sec.base_addr < 0 || sec.data.empty()) continue;
        for (size_t i = 0; i < sec.data.size(); ++i) {
            long addr = sec.base_addr + i;
            memory[addr] = sec.data[i];
            all_lines.insert(addr & ~0x7);
        }
    }

    for (long line : all_lines) {
        out << std::hex << std::setw(8) << std::setfill('0') << line << ":";
        for (int j = 0; j < 8; ++j) {
            long addr = line + j;
            if (memory.count(addr))
                out << " " << std::setw(2) << std::setfill('0') << (int)memory[addr];
            else
                out << " 00";
        }
        out << std::endl;
    }
}

void LinkerSections::output_text(std::ostream& out)
{
    for (auto& sec_name : section_order) {
        auto& sec = merged_sections[sec_name];
        out << "." << sec.name << "\n";
        int i = 0;
        for (auto b : sec.data) {
            std::bitset<8> bin(b);
            out << bin.to_string() << (++i % 4 ? " " : "\n");
        }
        out << "\n";
    }
}
