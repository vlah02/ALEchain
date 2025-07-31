#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

struct LinkerSection {
    std::string name;
    std::vector<unsigned char> data;
    std::unordered_map<std::string, int> file_offsets; // where this file's chunk starts in data
    long base_addr = -1; // Used for Level B placement

    LinkerSection() : name("") {}
    LinkerSection(const std::string& n) : name(n) {}
};

class LinkerSections {
public:
    static void load_sections(const std::vector<std::string>& filenames);
    static void merge_sections(const std::vector<std::pair<std::string, long>>& placements);
    static void patch_relocations();
    static int get_offset(const std::string& section, const std::string& file);
    static long get_section_base(const std::string& section);
    static void output_hex(std::ostream& out);
    static void output_text(std::ostream& out);
    static std::unordered_map<std::string, LinkerSection> merged_sections;
    static std::vector<std::string> section_order;
};
