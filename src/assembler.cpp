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
        outputName[outputName.size() - 1] = 'o';
    } else if (argc == 4 && std::string(argv[1]) == "-o") {
        inputName = argv[3];
        outputName = argv[2];
    } else {
        return -2;
    }

    std::ifstream input;
    input.open(inputName);
    if (!input) {
        std::cerr << "Unable to open file " << std::endl;
        return -1;
    }
    std::stringstream buffer;
    buffer << input.rdbuf();
    yy_scan_string(buffer.str().c_str());
    yyparse();

    std::ofstream output;
    output.open(outputName);

    Section::dumpPool();
    output << ".sections" << std::endl;
    Section::out(output);
    output << ".symbols" << std::endl;
    SymTab::out(output);

    return 0;
}
