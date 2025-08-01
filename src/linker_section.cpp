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

static void parse_section_block(
    std::ifstream& file,
    const std::string& section_name,
    LinkerSection& section,
    const std::string& filename)
{
    size_t start_offset = section.data.size();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '.') break;
        std::istringstream iss(line);
        std::string byte_str;
        while (iss >> byte_str) {
            unsigned char val = std::stoi(byte_str, nullptr, 2);
            section.data.push_back(val);
        }
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
        // Go to .sections block
        while (std::getline(file, line)) {
            if (line == ".sections") break;
        }
        // Read sections until .symbols or EOF
        while (std::getline(file, line)) {
            if (line == ".symbols" || line.empty()) break;
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

    // For debug
    std::cerr << "Section order: ";
    for (auto& secname : section_order) {
        std::cerr << secname << "(" << merged_sections[secname].data.size() << ") ";
    }
    std::cerr << std::endl;

    // Track where the next "free" byte in memory is.
    long current_addr = 0;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];

        if (sec.data.empty()) {
            sec.base_addr = -1; // skip empty sections
            continue;
        }

        // If section is explicitly placed
        if (explicit_placement.count(secname)) {
            sec.base_addr = explicit_placement[secname];
            current_addr = sec.base_addr + sec.data.size(); // move forward
        } else {
            // Not explicitly placed: place at the next available address (current_addr)
            // But, make sure not to overlap with any future explicit placement
            // Find next explicit placement address (if any)
            long next_explicit = -1;
            for (const auto& [ename, eaddr] : explicit_placement) {
                // Only consider explicit placements that come after this section in section_order
                // and whose address is after or equal to current_addr
                auto pos_cur = std::find(section_order.begin(), section_order.end(), secname);
                auto pos_exp = std::find(section_order.begin(), section_order.end(), ename);
                if (pos_exp != section_order.end() && pos_exp > pos_cur && eaddr >= current_addr) {
                    if (next_explicit == -1 || eaddr < next_explicit) {
                        next_explicit = eaddr;
                    }
                }
            }
            // If there is a next explicit section and placing this section would overlap, pad forward
            if (next_explicit != -1 && current_addr + (long)sec.data.size() > next_explicit) {
                current_addr = next_explicit;
            }
            sec.base_addr = current_addr;
            current_addr += sec.data.size();
        }
    }

    // Optional: check for overlaps (just in case)
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
    // Gather all section bytes into a memory map
    std::map<long, uint8_t> memory;
    std::set<long> all_lines; // stores start addresses of lines to print

    for (const auto& sec_name : section_order) {
        const auto& sec = merged_sections[sec_name];
        if (sec.base_addr < 0 || sec.data.empty()) continue;
        for (size_t i = 0; i < sec.data.size(); ++i) {
            long addr = sec.base_addr + i;
            memory[addr] = sec.data[i];
            all_lines.insert(addr & ~0x7); // add the line for this byte
        }
    }

    // Print only lines that have any actual section byte
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
