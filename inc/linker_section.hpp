#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct LinkerSection {
    std::string name;
    std::vector<unsigned char> data;
    std::unordered_map<std::string, size_t> file_offsets;
    long base_addr = -1;

    LinkerSection() {}
    LinkerSection(const std::string& n) : name(n) {}
};

struct LinkerSections {
    static std::unordered_map<std::string, LinkerSection> merged_sections;
    static std::vector<std::string> section_order;

    static void load_sections(const std::vector<std::string>& filenames);
    static void merge_sections(const std::vector<std::pair<std::string, long>>& placements);

    static int get_offset(const std::string& section, const std::string& file);
    static long get_section_base(const std::string& section);

    static void output_hex(std::ostream& out);
    static void output_text(std::ostream& out);
};
