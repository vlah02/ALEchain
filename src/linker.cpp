#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../inc/linker_section.hpp"
#include "../inc/linker_symtab.hpp"

// Usage: ./linker -hex -o out.hex [-place=.section@0x1234] input1.o input2.o ...

int main(int argc, char** argv) {
    std::string output_file = "linked.hex";
    std::vector<std::string> input_files;
    std::vector<std::pair<std::string, long>> placements;
    bool output_hex = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i+1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-hex") {
            output_hex = true;
        } else if (arg.rfind("-place=", 0) == 0) {
            // Format: -place=.section@0x1234
            auto at = arg.find('@');
            if (at == std::string::npos) {
                std::cerr << "Invalid -place argument!\n";
                return 2;
            }
            std::string section = arg.substr(7, at - 7); // skip "-place="
            std::string addrstr = arg.substr(at+1);
            long addr = std::stol(addrstr, nullptr, 0);
            placements.push_back({section, addr});
        } else {
            input_files.push_back(arg);
        }
    }
    if (input_files.empty()) {
        std::cerr << "Usage: ./linker -hex -o out.hex [-place=.section@0x1234] input1.o input2.o ...\n";
        return 1;
    }

    // 1. Load sections
    LinkerSections::load_sections(input_files);

    // 2. Merge sections with placements (Level B)
    LinkerSections::merge_sections(placements);

    // 3. Parse symbols/relocations
    LinkerSymTab::parse_symbols_and_relocations(input_files);
    std::cout << "Loaded " << LinkerSymTab::relocations.size() << " relocations.\n";
    for (const auto& rel : LinkerSymTab::relocations) {
        std::cout << "Relocation: section=" << rel.section << " offset=" << rel.offset
                  << " symbol=" << rel.symbol << " file=" << rel.file << "\n";
    }

    // 4. Resolve symbol addresses
    LinkerSymTab::resolve_symbols();

    // ------------------------------
    // ADD THIS DEBUG PRINT HERE!
    std::cout << "SYMBOL VALUES:\n";
    for (const auto& [name, addr] : LinkerSymTab::symbol_values)
        std::cout << "  " << name << " = " << addr << "\n";
    // ------------------------------

    // 5. Patch relocations
    LinkerSymTab::apply_relocations();

    // 6. Output
    std::ofstream out(output_file);
    if (output_hex)
        LinkerSections::output_hex(out);
    else
        LinkerSections::output_text(out);
    out.close();

    std::cout << "Linking complete. Output written to: " << output_file << "\n";
    return 0;
}
