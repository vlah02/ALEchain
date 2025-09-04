%{
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <limits>

#include "../inc/lexer.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"

Section *section = Section::extract("txt");

std::unordered_map<std::string, unsigned short> regs = {
        {"%r0",      0b0000},
        {"%status",  0b0000},
        {"%r1",      0b0001},
        {"%handler", 0b0001},
        {"%r2",      0b0010},
        {"%cause",   0b0010},
        {"%r3",      0b0011},
        {"%r4",      0b0100},
        {"%r5",      0b0101},
        {"%r6",      0b0110},
        {"%r7",      0b0111},
        {"%r8",      0b1000},
        {"%r9",      0b1001},
        {"%r10",     0b1010},
        {"%r11",     0b1011},
        {"%r12",     0b1100},
        {"%r13",     0b1101},
        {"%r14",     0b1110},
        {"%sp",      0b1110},
        {"%r15",     0b1111},
        {"%pc",      0b1111}
};

void yyerror(const char *s);

enum valtype {
    SYM,
    INT
};

std::vector<std::string> symblist;
std::vector<std::pair<std::string, valtype>> vallist;
std::vector<unsigned short> reglist;
%}

%union { const char *str; }
%token<str> dotGLOBAL dotSECTION dotWORD dotSKIP dotASCII dotEQU dotEND dotTYPE dotWEAK
%token<str> HALT INTERRUPT INTERRUPT_RETURN CALL RETURN JUMP BRANCH_EQUAL BRANCH_notEQUAL BRANCH_GREATER PUSH POP EXCHANGE ADD SUBTRACT MULTIPLY DIVIDE NOT AND OR XOR SHIFT_LEFT SHIFT_RIGHT LOAD STORE CSRRD CSRWR
%token<str> NEWLINE COMMENT STRING SYMBOL INTEGER REGISTER SYSTEM_REGISTER
%token<str> PLUS MINUS STAR SLASH COLON DOLLAR LBRACKET LCBRACKET RBRACKET RCBRACKET COMMA GRGR LSLS
%token<str> CATCH_ERROR

%%
program: | program line | program NEWLINE ;
line: label | instruction | label instruction | directive | label directive | terminate;
instruction: halt | int | call | jmp | beq | bne | bgt | xchg | add | sub | mul | div | not | and | or | xor | shl | shr | push | st | pop | ld | ret | iret | csrrd | csrwr;
directive: global | section | word | skip | ascii | equ | end | type | weak;
terminate: NEWLINE | COMMENT | ;

symblist: SYMBOL {
    symblist.push_back($1);
} | SYMBOL COMMA symblist {
    symblist.push_back($1);
};

vallist: SYMBOL {
    vallist.push_back({ $1, valtype::SYM });
} | INTEGER {
    vallist.push_back({ $1, valtype::INT });
} | MINUS INTEGER {
    std::string lit = "-" + std::string($2);
    vallist.push_back({ lit, valtype::INT });
} | SYMBOL PLUS INTEGER {
    std::string lit = std::string($1) + "+" + std::string($3);
    vallist.push_back({ lit, valtype::SYM });
} | SYMBOL MINUS INTEGER {
    std::string lit = std::string($1) + "-" + std::string($3);
    vallist.push_back({ lit, valtype::SYM });
} | vallist COMMA SYMBOL {
    vallist.push_back({ $3, valtype::SYM });
} | vallist COMMA INTEGER {
    vallist.push_back({ $3, valtype::INT });
} | vallist COMMA MINUS INTEGER {
    std::string lit = "-" + std::string($3);
    vallist.push_back({ lit, valtype::INT });
} | vallist COMMA SYMBOL PLUS INTEGER {
    std::string lit = std::string($3) + "+" + std::string($5);
    vallist.push_back({ lit, valtype::SYM });
} | vallist COMMA SYMBOL MINUS INTEGER {
    std::string lit = std::string($3) + "-" + std::string($5);
    vallist.push_back({ lit, valtype::SYM });
};

reglist: REGISTER {
    reglist.push_back(regs[$1]);
} | REGISTER COMMA reglist {
    reglist.push_back(regs[$1]);
}


label: SYMBOL COLON terminate {
    std::cerr << "Defining label: " << $1 << " at offset " << section->getSize() << std::endl;
    SymTab::add_definition($1, section->getName(), section->getSize());
};

global: dotGLOBAL symblist terminate {
    for (auto& symb : symblist)
        SymTab::globals.insert(symb);
    symblist.clear();
};

section: dotSECTION SYMBOL terminate {
    section = Section::extract($2);
};

word: dotWORD vallist terminate {
    for (auto it = vallist.rbegin(); it != vallist.rend(); ++it) {
        if (it->second == valtype::INT) {
            section->insertInt(std::stoul(it->first, nullptr, 0));
        } else if (it->second == valtype::SYM) {
            long equVal = SymTab::try_resolve_equ(it->first);
            if (equVal != std::numeric_limits<long>::min()) {
                section->insertInt(equVal);
            } else {
                std::string name = it->first;
                int addend = 0;
                auto plus = name.find('+');
                auto minus = name.find('-');
                if (plus != std::string::npos) {
                    addend = std::stoi(name.substr(plus + 1));
                    name = name.substr(0, plus);
                } else if (minus != std::string::npos) {
                    addend = -std::stoi(name.substr(minus + 1));
                    name = name.substr(0, minus);
                }
                section->add_literal(name, addend);
                section->insertInt(-1);
                SymTab::add_occurrence(name, section->getName(), section->getSize());
            }
        }
    }
    vallist.clear();
};

skip: dotSKIP INTEGER terminate {
    for (int i = 0; i < std::stoul($2, nullptr, 0); i++) {
        section->insertByte(0);
    }
};

ascii: dotASCII STRING terminate {
    std::string raw($2);
    if (!raw.empty() && raw.front() == '"' && raw.back() == '"') {
        raw = raw.substr(1, raw.size() - 2);
    }
    for (char c : raw) {
        section->insertByte(static_cast<unsigned char>(c));
    }
};

equ: dotEQU SYMBOL COMMA INTEGER terminate {
    SymTab::equs[$2] = std::stoul($4, nullptr, 0);
    if (SymTab::globals.count($2)) {
        SymTab::add_definition($2, "", SymTab::equs[$2]);
    }
} | dotEQU SYMBOL COMMA SYMBOL MINUS SYMBOL terminate {
    SymTab::pending_equs.push_back(
        SymTab::EquEntry{ std::string($2), std::string($4), std::string($6), SymTab::EquEntry::Op::SUB }
    );
} | dotEQU SYMBOL COMMA SYMBOL PLUS SYMBOL terminate {
    SymTab::pending_equs.push_back(
        SymTab::EquEntry{ std::string($2), std::string($4), std::string($6), SymTab::EquEntry::Op::ADD }
    );
};

end: dotEND terminate {
    YYACCEPT;
};

type: dotTYPE SYMBOL COMMA SYMBOL terminate {
    std::string t($4);
    if (t != "FUNC" && t != "DATA" && t != "NOTYPE") {
        std::cerr << "ERROR: invalid .type for symbol '" << $2
                  << "': got '" << t
                  << "', expected one of: FUNC, DATA, NOTYPE\n";
        YYABORT;
    }
    SymTab::add_type($2, t);
};

weak: dotWEAK symblist terminate {
    for (auto& symb : symblist)
        SymTab::weaks.insert(symb);
    symblist.clear();
};

halt: HALT terminate {
    section->add_instruction();
};

int: INTERRUPT terminate {
    section->add_instruction(0b0001);
};

call: CALL SYMBOL terminate {
    section->add_symbol_or_equ_literal($2);
    section->add_instruction(0b0010, 0b0001, 15);
} | CALL INTEGER terminate {
    section->add_literal(std::stoul($2, nullptr, 0));
    section->add_instruction(0b0010, 0b0001, 15);
}
jmp: JUMP SYMBOL terminate {
    section->add_symbol_or_equ_literal($2);
    section->add_instruction(0b0011, 0b1000, 15);
} | JUMP INTEGER terminate {
    section->add_literal(std::stoul($2, nullptr, 0));
    section->add_instruction(0b0011, 0b1000, 15);
} | JUMP SYMBOL PLUS INTEGER terminate {
    section->add_symbol_or_equ_literal(std::string($2) + "+" + std::string($4));
    section->add_instruction(0b0011, 0b1000, 15);
} | JUMP SYMBOL MINUS INTEGER terminate {
    section->add_symbol_or_equ_literal(std::string($2) + "-" + std::string($4));
    section->add_instruction(0b0011, 0b1000, 15);
}

beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_symbol_or_equ_literal($6);
    section->add_instruction(0b0011, 0b1001, 15, regs[$2], regs[$4]);
} | BRANCH_EQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(std::stoul($6, nullptr, 0));
    section->add_instruction(0b0011, 0b1001, 15, regs[$2], regs[$4]);
}
bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_symbol_or_equ_literal($6);
    section->add_instruction(0b0011, 0b1010, 15, regs[$2], regs[$4]);
} | BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(std::stoul($6, nullptr, 0));
    section->add_instruction(0b0011, 0b1010, 15, regs[$2], regs[$4]);
}
bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_symbol_or_equ_literal($6);
    section->add_instruction(0b0011, 0b1011, 15, regs[$2], regs[$4]);
} | BRANCH_GREATER REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(std::stoul($6, nullptr, 0));
    section->add_instruction(0b0011, 0b1011, 15, regs[$2], regs[$4]);
}

xchg: EXCHANGE REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0100, 0b0000, 0, regs[$2], regs[$4]);
}

add: ADD REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0101, 0b0000, regs[$4], regs[$4], regs[$2]);
} | ADD REGISTER COMMA REGISTER GRGR INTEGER {

} | ADD REGISTER COMMA REGISTER LSLS INTEGER {

}
sub: SUBTRACT REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0101, 0b0001, regs[$4], regs[$4], regs[$2]);
}
mul: MULTIPLY REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0101, 0b0010, regs[$4], regs[$4], regs[$2]);
}
div: DIVIDE REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0101, 0b0011, regs[$4], regs[$4], regs[$2]);
}

not: NOT REGISTER terminate {
    section->add_instruction(0b0110, 0b0000, regs[$2], regs[$2]);
}
and: AND REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0110, 0b0001, regs[$4], regs[$4], regs[$2]);
}
or: OR REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0110, 0b0010, regs[$4], regs[$4], regs[$2]);
}
xor: XOR REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0110, 0b0011, regs[$4], regs[$4], regs[$2]);
}

shl: SHIFT_LEFT REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0111, 0b0000, regs[$4], regs[$4], regs[$2]);
}
shr: SHIFT_RIGHT REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b0111, 0b0001, regs[$4], regs[$4], regs[$2]);
}

push: PUSH REGISTER terminate {
    section->add_instruction(0b1000, 0b0001, 14, 0, regs[$2], -4);
} | PUSH LCBRACKET reglist RCBRACKET terminate {
    for (auto it = reglist.rbegin(); it != reglist.rend(); ++it) {
        section->add_instruction(0b1000, 0b0001, 14, 0, *it, -4);
    }
    reglist.clear();
}

pop: POP REGISTER terminate {
    section->add_instruction(0b1001, 0b0011, regs[$2], 14, 0, 4);
}

st: STORE REGISTER COMMA INTEGER terminate {
    section->add_literal(std::stoul($4, nullptr, 0));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[$2]);
} | STORE REGISTER COMMA SYMBOL terminate {
    section->add_symbol_or_equ_literal($4);
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[$2]);
} | STORE REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0001, regs[$4], regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS INTEGER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2], std::stoul($7, nullptr, 0));
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS SYMBOL RBRACKET terminate {
    section->add_symbol_or_equ_literal($7);
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS REGISTER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], regs[$7], regs[$2]);
}

ld: LOAD DOLLAR INTEGER COMMA REGISTER terminate {
    section->add_literal(std::stoul($3, nullptr, 0));
    section->add_instruction(0b1001, 0b0010, regs[$5], 15);
} | LOAD DOLLAR SYMBOL COMMA REGISTER terminate {
    section->add_symbol_or_equ_literal($3);
    section->add_instruction(0b1001, 0b0010, regs[$5], 15);
} | LOAD INTEGER COMMA REGISTER terminate {
    bool alt = (std::string($4) == "%r1");
    unsigned char scratch = alt ? 2 : 1;
    section->add_instruction(0b1000, 0b0001, 14, 0, scratch, -1);
    section->add_literal(std::stoul($2, nullptr, 0));
    section->add_instruction(0b1001, 0b0010, scratch, 15);
    section->add_instruction(0b1001, 0b0010, regs[$4], scratch);
    section->add_instruction(0b1001, 0b0011, scratch, 14, 0, 1);
} | LOAD SYMBOL COMMA REGISTER terminate {
    bool alt = (std::string($4) == "%r1");
    unsigned char scratch = alt ? 2 : 1;
    section->add_instruction(0b1000, 0b0001, 14, 0, scratch, -1);
    section->add_symbol_or_equ_literal($2);
    section->add_instruction(0b1001, 0b0010, scratch, 15);
    section->add_instruction(0b1001, 0b0010, regs[$4], scratch);
    section->add_instruction(0b1001, 0b0011, scratch, 14, 0, 1);
} | LOAD REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0001, regs[$4], regs[$2]);
} | LOAD LBRACKET REGISTER RBRACKET COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0010, regs[$6], regs[$3]);
} | LOAD LBRACKET REGISTER PLUS INTEGER RBRACKET COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0010, regs[$8], regs[$3], 0, std::stoul($5, nullptr, 0));
} | LOAD LBRACKET REGISTER PLUS SYMBOL RBRACKET COMMA REGISTER terminate {
    SymTab::add_occurrence($5, section->getName(), section->getSize(), false);
    section->add_instruction(0b1001, 0b0010, regs[$8], regs[$3]);
} | LOAD LBRACKET REGISTER PLUS REGISTER RBRACKET COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0010, regs[$8], regs[$3], regs[$5]);
}

ret: RETURN terminate {
    section->add_instruction(0b1001, 0b0011, 15, 14, 0, 4);
}

iret: INTERRUPT_RETURN terminate {
    section->add_instruction(0b1001, 0b0100, 0b0010);
    section->add_instruction(0b1001, 0b0111, 0b0000, 0b1110, 0, 4);
    section->add_instruction(0b1001, 0b0011, 0b1111, 0b1110, 0, 4);
};

csrrd: CSRRD SYSTEM_REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0000, regs[$4], regs[$2]);
}
csrwr: CSRWR REGISTER COMMA SYSTEM_REGISTER terminate {
    section->add_instruction(0b1001, 0b0100, regs[$4], regs[$2]);
}

%%

void yyerror(const char *s) {
    fprintf(stderr, "ERROR (token=%d, text=%s) (last symbol='%s')\n", yychar, yytext, yylval.str);
}
