#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include "../inc/lexer.hpp"
#include "../inc/parser.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"

int main(int argc, char **argv) {
    std::string inputName;
    std::string outputName;

    if (argc == 2) {
        inputName = argv[1];
        outputName = argv[1];
        if (outputName.empty() || outputName.back() != 's') {
            std::cerr << "Usage: assembler [-o out.o] input.s\n";
            return -2;
        }
        outputName.back() = 'o';
    } else if (argc == 4 && std::string(argv[1]) == "-o") {
        outputName = argv[2];
        inputName  = argv[3];
    } else {
        std::cerr << "Usage: assembler [-o out.o] input.s\n";
        return -2;
    }

    std::ifstream input(inputName);
    if (!input) {
        std::cerr << "Unable to open file '" << inputName << "'\n";
        return -1;
    }

    std::stringstream ss;
    ss << input.rdbuf();
    const std::string source = ss.str();

    YY_BUFFER_STATE buf = yy_scan_bytes(source.data(),
                                        static_cast<int>(source.size()));

    int parse_rc = yyparse();

    yy_delete_buffer(buf);
    yylex_destroy();

    if (parse_rc != 0) {
        std::cerr << "Assembly failed. No output written.\n";
        return 1;
    }

    Section::dumpPool();

    std::ofstream output(outputName);
    if (!output) {
        std::cerr << "Unable to open output file '" << outputName << "' for writing\n";
        return -3;
    }

    output << ".sections\n";
    Section::out(output);
    output << ".symbols\n";
    SymTab::out(output);

    return 0;
}
