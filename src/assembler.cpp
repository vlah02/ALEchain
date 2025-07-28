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

    for(int i = 0; !input.eof(); i++) {
        std::string line;
        getline(input, line);
        yy_scan_string(line.c_str());
        if(yyparse() == 1) break;;
    }

    std::ofstream output;
    output.open(outputName);

    Section::dumpPool();
    Section::out(output);
    SymTab::out(output);

    return 0;
}



// flex --header-file=inc/lexer.hpp -o src/lexer.cpp misc/lexer.l
// bison -d -o src/parser.cpp misc/parser.y
// g++ -o assembler src/assembler.cpp src/assembler_section.cpp src/assembler_symtab.cpp src/lexer.cpp src/parser.cpp