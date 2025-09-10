#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../inc/linker_section.hpp"
#include "../inc/linker_symtab.hpp"

int main(int argc, char** argv) {
    std::string output_file = "linked.hex";
    std::vector<std::string> input_files;
    std::vector<std::pair<std::string, long>> placements;
    bool output_hex = false;
    bool output_reloc = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i+1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-hex") {
            output_hex = true;
        } else if (arg == "-relocatable") {
            output_reloc = true;
        } else if (arg.rfind("-place=", 0) == 0) {
            auto at = arg.find('@');
            if (at == std::string::npos) {
                std::cerr << "Invalid -place argument!\n";
                return 2;
            }
            std::string section = arg.substr(7, at - 7);
            std::string addrstr = arg.substr(at+1);
            long addr = std::stol(addrstr, nullptr, 0);
            placements.push_back({section, addr});
        } else {
            input_files.push_back(arg);
        }
    }

    if (input_files.empty()) {
        std::cerr << "Usage: ./linker (-hex | -relocatable) -o <out> [-place=.section@0x1234] input1.o input2.o ...\n";
        return 1;
    }
    if (output_hex == output_reloc) {
        std::cerr << "Error: exactly one of -hex or -relocatable must be specified.\n";
        return 2;
    }

    LinkerSections::load_sections(input_files);
    LinkerSymTab::parse_symbols_and_relocations(input_files);

    if (output_hex) {
        LinkerSections::merge_sections(placements);
        LinkerSymTab::resolve_symbols();
        LinkerSymTab::patch_relocations();

        std::ofstream out(output_file);
        LinkerSections::output_hex(out);
        out.close();

        std::cout << "Linking complete. Output written to: " << output_file << "\n";
        return 0;
    }

    if (!LinkerSymTab::check_no_multiple_definitions(/*die_on_error=*/true)) {
        return 3;
    }

    std::ofstream out(output_file);
    out << ".sections\n";
    LinkerSections::output_text(out);
    out << ".symbols\n";
    LinkerSymTab::output_relocatable(out);
    out.close();

    std::cout << "Relocatable object written to: " << output_file << "\n";
    return 0;
}
