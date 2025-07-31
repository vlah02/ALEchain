#include "../inc/linker_section.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>

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
    for (auto& pair : section.file_offsets)
        std::cout << "Section " << section.name << " file_offset for " << pair.first << " = " << pair.second << "\n";
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
        // 1. Go to .sections block
        while (std::getline(file, line)) {
            if (line == ".sections") break;
        }
        // 2. Read sections until .symbols or EOF
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

// Handles Level B: placements and layout
void LinkerSections::merge_sections(const std::vector<std::pair<std::string, long>>& placements)
{
    long next_free = 0;
    // Assign base_addr for each section, respecting placements and preventing overlaps
    for (const auto& sec : section_order) {
        auto it = std::find_if(placements.begin(), placements.end(),
                               [&](const std::pair<std::string, long>& p) { return p.first == sec; });
        if (it != placements.end()) {
            merged_sections[sec].base_addr = it->second;
        } else {
            merged_sections[sec].base_addr = next_free;
        }
        next_free = merged_sections[sec].base_addr + merged_sections[sec].data.size();
    }
    // Overlap check:
    for (size_t i = 0; i + 1 < section_order.size(); ++i) {
        auto& s1 = merged_sections[section_order[i]];
        auto& s2 = merged_sections[section_order[i+1]];
        if (s1.base_addr + (long)s1.data.size() > s2.base_addr) {
            std::cerr << "Section overlap between " << s1.name << " and " << s2.name << "\n";
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

void LinkerSections::output_hex(std::ostream& out)
{
    // Find minimum and maximum address for all sections (for continuous output)
    long min_addr = -1, max_addr = -1;
    for (const auto& sec_name : section_order) {
        const auto& sec = merged_sections[sec_name];
        if (sec.base_addr < 0) continue;
        long start = sec.base_addr;
        long end = sec.base_addr + sec.data.size();
        if (min_addr == -1 || start < min_addr) min_addr = start;
        if (max_addr == -1 || end > max_addr) max_addr = end;
    }
    if (min_addr == -1) return; // nothing to output

    const int line_bytes = 8; // or 16 if you want

    // Prepare a memory image
    std::vector<unsigned char> mem(max_addr - min_addr, 0);
    for (const auto& sec_name : section_order) {
        const auto& sec = merged_sections[sec_name];
        if (sec.base_addr < 0) continue;
        for (size_t i = 0; i < sec.data.size(); ++i) {
            mem[sec.base_addr - min_addr + i] = sec.data[i];
        }
    }

    // Output in proper format
    for (long addr = 0; addr < (long)mem.size(); addr += line_bytes) {
        out << std::hex << std::setw(8) << std::setfill('0') << (min_addr + addr) << ":";
        for (int j = 0; j < line_bytes && addr + j < (long)mem.size(); ++j) {
            out << " " << std::setw(2) << std::setfill('0') << (int)mem[addr + j];
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
