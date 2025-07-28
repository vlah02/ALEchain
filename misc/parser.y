%{
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <unordered_map>

#include "../inc/lexer.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"

unsigned char word[8] = {};

Section *section = Section::extract("txt");

long toInt(const std::string &str);

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

%union {
    const char *str;
}

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
    vallist.push_back({$1, valtype::SYM});
 } | SYMBOL COMMA vallist {
    vallist.push_back({$1, valtype::SYM});
  } | INTEGER {
    vallist.push_back({$1, valtype::INT});
  } | INTEGER COMMA vallist {
    vallist.push_back({$1, valtype::INT});
  }

reglist: REGISTER {
    reglist.push_back(regs[$1]);
} | REGISTER COMMA reglist {
    reglist.push_back(regs[$1]);
}


label: SYMBOL COLON terminate {
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
            section->insertInt(toInt(it->first));
        } else if (it->second == valtype::SYM) {
            section->insertInt(-1);
            SymTab::add_occurrence(it->first, section->getName(), section->getSize());
        }
    }
    vallist.clear();
};

skip: dotSKIP INTEGER terminate {
    for (int i = 0; i < toInt($2); i++) {
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

equ: dotEQU SYMBOL COMMA COMMA terminate {

};

end: dotEND terminate {
    YYABORT;
};

type: dotTYPE SYMBOL COMMA SYMBOL terminate {
    SymTab::add_type($2, $4);
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
    section->add_literal($2);
    section->add_instruction(0b0010, 0b0001, 15);
} | CALL INTEGER terminate {
    section->add_literal(toInt($2));
    section->add_instruction(0b0010, 0b0001, 15);
}
jmp: JUMP SYMBOL terminate {
    section->add_literal($2);
    section->add_instruction(0b0011, 0b1000, 15);
} | JUMP INTEGER terminate {
    section->add_literal(toInt($2));
    section->add_instruction(0b0011, 0b1000, 15);
}

beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_literal($6);
    section->add_instruction(0b0011, 0b1001, 15, regs[$2], regs[$4]);
} | BRANCH_EQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(toInt($6));
    section->add_instruction(0b0011, 0b1001, 15, regs[$2], regs[$4]);
}
bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_literal($6);
    section->add_instruction(0b0011, 0b1010, 15, regs[$2], regs[$4]);
} | BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(toInt($6));
    section->add_instruction(0b0011, 0b1010, 15, regs[$2], regs[$4]);
}
bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA SYMBOL terminate {
    section->add_literal($6);
    section->add_instruction(0b0011, 0b1011, 15, regs[$2], regs[$4]);
} | BRANCH_GREATER REGISTER COMMA REGISTER COMMA INTEGER terminate {
    section->add_literal(toInt($6));
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
    section->add_instruction(0b0110, 0b0010, regs[$4], regs[$4], regs[$2]);
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
    section->add_literal(toInt($4));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[$2]);
} | STORE REGISTER COMMA SYMBOL terminate {
    section->add_literal($4);
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[$2]);
} | STORE REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0001, regs[$4], regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS INTEGER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2], toInt($7));
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS SYMBOL RBRACKET terminate {
    section->add_literal($7);
    section->add_instruction(0b1000, 0b0000, regs[$5], 0, regs[$2]);
} | STORE REGISTER COMMA LBRACKET REGISTER PLUS REGISTER RBRACKET terminate {
    section->add_instruction(0b1000, 0b0000, regs[$5], regs[$7], regs[$2]);
}

ld: LOAD DOLLAR INTEGER COMMA REGISTER terminate {
    section->add_literal(toInt($3));
    section->add_instruction(0b1001, 0b0010, regs[$5], 15);
} | LOAD DOLLAR SYMBOL COMMA REGISTER terminate {
    section->add_literal($3);
    section->add_instruction(0b1001, 0b0010, regs[$5], 15);
} | LOAD INTEGER COMMA REGISTER terminate {
    section->add_literal(toInt($2));
    section->add_instruction(0b1001, 0b0010, regs[$4], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[$4], regs[$4]);
} | LOAD SYMBOL COMMA REGISTER terminate {
    section->add_literal($2);
    section->add_instruction(0b1001, 0b0010, regs[$4], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[$4], regs[$4]);
} | LOAD REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0001, regs[$4], regs[$2]);
} | LOAD LBRACKET REGISTER RBRACKET COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0010, regs[$6], regs[$3]);
} | LOAD LBRACKET REGISTER PLUS INTEGER RBRACKET COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0010, regs[$8], regs[$3], toInt($5));
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
    section->add_instruction(0b1001, 0b0011, 0b0000, 0b1110, 0, 4);
    section->add_instruction(0b1001, 0b0011, 0b1111, 0b1110, 0, 4);
};

csrrd: CSRRD SYSTEM_REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0000, regs[$4], regs[$2]);
}
csrwr: CSRWR SYSTEM_REGISTER COMMA REGISTER terminate {
    section->add_instruction(0b1001, 0b0100, regs[$4], regs[$2]);
}

%%

void yyerror(const char *s) {
    fprintf(stderr, "ERROR (token=%d, text=%s)\n", yychar, yytext);
}

long toInt(const std::string &str) {
    int base = 10; // default base
    int start = 0;
    bool isNegative = false;

    if (str[0] == '-') {
        isNegative = true;
        start = 1;
    }

    if (str[start] == '0' && start + 1 < str.length()) {
        char nextChar = tolower(str[start + 1]);
        if (nextChar == 'x') {
            base = 16; // hexadecimal
            start += 2;
        } else if (nextChar == 'o') {
            base = 8; // octal
            start += 2;
        } else if (nextChar == 'b') {
            base = 2; // binary
            start += 2;
        }
    }

    long result = 0;

    for (int i = start; i < str.length(); ++i) {
        char c = tolower(str[i]);
        int digit = 0;

        // Handle conversion of digits or letters based on base
        if (isdigit(c)) {
            digit = c - '0';
        } else if (isalpha(c)) {
            digit = c - 'a' + 10; // convert a-f to 10-15
        } else {
            throw std::invalid_argument("Invalid character in input string");
        }

        // Check if digit is valid for the current base
        if (digit >= base) {
            throw std::invalid_argument("Digit out of range for base");
        }

        result = result * base + digit;
    }

    return isNegative ? -result : result;
}
