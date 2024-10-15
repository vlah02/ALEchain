#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include "../inc/lexer.hpp"
#include "../inc/parser.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"

int main() {
    std::ifstream input;
    input.open("tests/input.s");
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

    Section::dumpPool();

    Section::out(std::cout);

    SymTab::out(std::cout);

    return 0;
}



// flex --header-file=inc/lexer.hpp -o src/lexer.cpp misc/lexer.l
// bison -d -o src/parser.cpp misc/parser.y
// g++ -o assembler src/assembler.cpp src/assembler_section.cpp src/assembler_symtab.cpp src/lexer.cpp src/parser.cpp