#include <fstream>
#include <iostream>
#include <sstream>
#include "../inc/lexer.hpp"
#include "../inc/parser.hpp"
#include "../inc/assembler_driver.hpp"

Assembler& Assembler::instance() {
    static Assembler inst;
    return inst;
}

Assembler::Assembler() {
    sections_.extract("txt");
}

int Assembler::parse(const std::string& inputName, const std::string& outputName) {
    std::ifstream input(inputName);
    if (!input) {
        std::cerr << "Unable to open file '" << inputName << "'\n";
        return -1;
    }

    std::stringstream ss; ss << input.rdbuf();
    const std::string source = ss.str();

    YY_BUFFER_STATE buf = yy_scan_bytes(source.data(), (int)source.size());
    int parse_rc = yyparse();
    yy_delete_buffer(buf);
    yylex_destroy();

    if (parse_rc != 0) {
        std::cerr << "Assembly failed. No output written.\n";
        return 1;
    }

    pool_.flush(sections_, symbols_, relocs_);

    std::ofstream out(outputName);
    if (!out) {
        std::cerr << "Unable to open output file '" << outputName << "' for writing\n";
        return -3;
    }

    sections_.out(out);
    symbols_.out(out);
    relocs_.out(out);
    return 0;
}

int main(int argc, char** argv) {
    std::string inputName, outputName;

    if (argc == 2) {
        inputName  = argv[1];
        outputName = inputName;
        outputName.back() = 'o';
    } else if (argc == 4 && std::string(argv[1]) == "-o") {
        outputName = argv[2];
        inputName  = argv[3];
    } else {
        return -2;
    }

    return Assembler::instance().parse(inputName, outputName);
}