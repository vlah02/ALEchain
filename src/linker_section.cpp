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
    // Build a map for fast lookup of explicit placements
    std::unordered_map<std::string, long> explicit_placement;
    for (const auto& p : placements)
        explicit_placement[p.first] = p.second;

    // 1. Assign addresses to all sections
    // - Placed sections get their user address.
    // - Unplaced sections start at 0, packed upwards, skipping over any occupied regions.
    //    e.g. If my_code is at 0x40000000, default sections fit at 0, 4, 8, ... etc.

    // Step 1: Collect all placed sections and sort by base_addr.
    struct SecRange { std::string name; long addr; long size; };
    std::vector<SecRange> placed;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];
        if (explicit_placement.count(secname)) {
            sec.base_addr = explicit_placement[secname];
            placed.push_back({secname, sec.base_addr, (long)sec.data.size()});
        } else {
            sec.base_addr = -1;
        }
    }
    std::sort(placed.begin(), placed.end(), [](const SecRange& a, const SecRange& b){ return a.addr < b.addr; });

    // Step 2: Pack all unplaced sections into available gaps, starting from 0.
    long next_free = 0;
    size_t pi = 0;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];
        if (sec.base_addr >= 0) continue; // already placed

        // If next placed section is at/after our current next_free, skip to there
        if (pi < placed.size() && next_free >= placed[pi].addr) {
            next_free = placed[pi].addr + placed[pi].size;
            ++pi;
        }
        sec.base_addr = next_free;
        next_free += (long)sec.data.size();
    }

    // 3. Check for overlaps: sort everything by address
    std::vector<SecRange> all_ranges;
    for (auto& secname : section_order) {
        auto& sec = merged_sections[secname];
        all_ranges.push_back({secname, sec.base_addr, (long)sec.data.size()});
    }
    std::sort(all_ranges.begin(), all_ranges.end(), [](const SecRange& a, const SecRange& b){ return a.addr < b.addr; });

    std::cout << "\n==== SECTION LAYOUT ====" << std::endl;
    for (auto& secname : section_order) {
        const auto& section = merged_sections[secname];
        std::cout << "[DEBUG] Section " << section.name
                  << " base=0x" << std::hex << section.base_addr
                  << " size=" << std::dec << section.data.size() << std::endl;
        for (const auto& [filename, offset] : section.file_offsets) {
            std::cout << "  [DEBUG] file=" << filename
                      << " offset_in_section=" << offset << std::endl;
        }
    }

    for (size_t i = 1; i < all_ranges.size(); ++i) {
        auto& prev = all_ranges[i-1];
        auto& curr = all_ranges[i];
        if (prev.addr + prev.size > curr.addr) {
            std::cerr << "Section overlap between " << prev.name << " and " << curr.name << "\n";
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
    // Collect all sections with base_addr >= 0
    struct SecRange {
        std::string name;
        long addr;
        const std::vector<unsigned char> *data;
    };
    std::vector<SecRange> ranges;
    for (const auto& sec_name : section_order) {
        const auto& sec = merged_sections[sec_name];
        if (sec.base_addr < 0) continue;
        if (sec.data.empty()) continue;
        ranges.push_back({sec_name, sec.base_addr, &sec.data});
    }
    std::sort(ranges.begin(), ranges.end(), [](const SecRange& a, const SecRange& b){
        return a.addr < b.addr;
    });

    const int line_bytes = 8;
    for (const auto& r : ranges) {
        long addr = r.addr;
        size_t size = r.data->size();
        const std::vector<unsigned char>& mem = *(r.data);

        for (size_t i = 0; i < size; i += line_bytes) {
            out << std::hex << std::setw(8) << std::setfill('0') << (addr + i) << ":";
            for (int j = 0; j < line_bytes && i + j < size; ++j) {
                out << " " << std::setw(2) << std::setfill('0') << (int)mem[i + j];
            }
            out << std::endl;
        }
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
