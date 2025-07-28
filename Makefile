assembler: parser
	g++ -o assembler src/assembler.cpp src/assembler_section.cpp src/assembler_symtab.cpp src/lexer.cpp src/parser.cpp

parser: lexer
	bison -d -o src/parser.cpp --defines=inc/parser.hpp misc/parser.y
	sed -i 's/#include "parser.hpp"/#include "..\/inc\/parser.hpp"/' src/parser.cpp

lexer:
	flex --header-file=inc/lexer.hpp -o src/lexer.cpp misc/lexer.l

clean:
	rm -f inc/lexer.hpp inc/parser.hpp src/lexer.cpp src/parser.cpp

asmtest: clean assembler
	 ./assembler -o tests/output.s tests/input.s