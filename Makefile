emulator:
	g++ -o emulator src/emulator_driver.cpp

linker:
	g++ -o linker src/linker.cpp src/linker_section.cpp src/linker_symtab.cpp

assembler: parser
	g++ -o assembler src/assembler.cpp src/assembler_section.cpp src/assembler_symtab.cpp src/lexer.cpp src/parser.cpp

parser: lexer
	bison -d -o src/parser.cpp --defines=inc/parser.hpp misc/parser.y
	sed -i 's/#include "parser.hpp"/#include "..\/inc\/parser.hpp"/' src/parser.cpp

lexer:
	flex --header-file=inc/lexer.hpp -o src/lexer.cpp misc/lexer.l

clean:
	rm -f inc/lexer.hpp inc/parser.hpp src/lexer.cpp src/parser.cpp \
			tests/levelA/*.o tests/levelA/*.hex \
			tests/levelB/*.o tests/levelB/*.hex \
			tests/levelC/*.o tests/levelC/*.hex \
			./assembler ./linker ./emulator

levela: clean assembler linker emulator
	./assembler -o tests/levelA/main.o tests/levelA/main.s
	./assembler -o tests/levelA/math.o tests/levelA/math.s
	./assembler -o tests/levelA/handler.o tests/levelA/handler.s
	./assembler -o tests/levelA/isr_timer.o tests/levelA/isr_timer.s
	./assembler -o tests/levelA/isr_terminal.o tests/levelA/isr_terminal.s
	./assembler -o tests/levelA/isr_software.o tests/levelA/isr_software.s
	./linker -hex \
		-place=my_code@0x40000000 -place=math@0xF0000000 \
		-o tests/levelA/program.hex \
		tests/levelA/handler.o tests/levelA/math.o tests/levelA/main.o tests/levelA/isr_terminal.o tests/levelA/isr_timer.o tests/levelA/isr_software.o
	./emulator tests/levelA/program.hex

levelb: clean assembler linker emulator
	./assembler -o tests/levelB/main.o tests/levelB/main.s
	./assembler -o tests/levelB/handler.o tests/levelB/handler.s
	./assembler -o tests/levelB/isr_terminal.o tests/levelB/isr_terminal.s
	./assembler -o tests/levelB/isr_timer.o tests/levelB/isr_timer.s
	./linker -hex \
    	-place=my_code@0x40000000 \
    	-o tests/levelB/program.hex \
		tests/levelB/main.o tests/levelB/isr_terminal.o tests/levelB/isr_timer.o tests/levelB/handler.o
	./emulator tests/levelB/program.hex

levelc: clean assembler linker emulator
	./assembler -o tests/levelC/main.o tests/levelC/main.s
	./assembler -o tests/levelC/handler.o tests/levelC/handler.s
	./assembler -o tests/levelC/isr_terminal.o tests/levelC/isr_terminal.s
	./assembler -o tests/levelC/isr_timer.o tests/levelC/isr_timer.s
	./linker -hex \
    	-place=code@0x40000000 \
    	-o tests/levelC/program.hex \
		tests/levelC/main.o tests/levelC/isr_terminal.o tests/levelC/isr_timer.o tests/levelC/handler.o
	./emulator tests/levelC/program.hex