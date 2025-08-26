%option noyywrap
%option nodefault
%option nounput
%option noinput
%option yylineno

%{
#include <cstdlib>
#include <cstring>
#include "../inc/parser.hpp"

static inline char* yy_strndup(const char* s, size_t n) {
    char* p = (char*)std::malloc(n + 1);
    if (!p) return nullptr;
    std::memcpy(p, s, n);
    p[n] = '\0';
    return p;
}
#define RET(tok) do { yylval.str = yy_strndup(yytext, (size_t)yyleng); return tok; } while(0)
%}

DIGIT        [0-9]
DECIMAL      {DIGIT}+
HEXADIGIT    [0-9A-Fa-f]
HEXA         0[xX]{HEXADIGIT}+
OCTADIGIT    [0-7]
OCTAL        0[oO]{OCTADIGIT}+
BINDIGIT     [01]
BINARY       0[bB]{BINDIGIT}+

LETTER       [A-Za-z_]
ALPHANUM     ({LETTER}|{DIGIT})+

REGNUM       (1[0-5]|[0-9])
REGISTER_RX  %r{REGNUM}
REGISTER_SP  %sp
REGISTER_PC  %pc

WS           [ \t]+
CRLF         \r?\n

%%

".global"                    { RET(dotGLOBAL); }
".extern"                    { RET(dotGLOBAL); }
".section"                   { RET(dotSECTION); }
".word"                      { RET(dotWORD); }
".skip"                      { RET(dotSKIP); }
".ascii"                     { RET(dotASCII); }
".equ"                       { RET(dotEQU); }
".end"                       { RET(dotEND); }
".type"                      { RET(dotTYPE); }
".weak"                      { RET(dotWEAK); }

halt                         { RET(HALT); }
int                          { RET(INTERRUPT); }
iret                         { RET(INTERRUPT_RETURN); }
call                         { RET(CALL); }
ret                          { RET(RETURN); }
jmp                          { RET(JUMP); }
beq                          { RET(BRANCH_EQUAL); }
bne                          { RET(BRANCH_notEQUAL); }
bgt                          { RET(BRANCH_GREATER); }
push                         { RET(PUSH); }
pop                          { RET(POP); }
xchg                         { RET(EXCHANGE); }
add                          { RET(ADD); }
sub                          { RET(SUBTRACT); }
mul                          { RET(MULTIPLY); }
div                          { RET(DIVIDE); }
not                          { RET(NOT); }
and                          { RET(AND); }
or                           { RET(OR); }
xor                          { RET(XOR); }
shl                          { RET(SHIFT_LEFT); }
shr                          { RET(SHIFT_RIGHT); }
ld                           { RET(LOAD); }
st                           { RET(STORE); }
csrrd                        { RET(CSRRD); }
csrwr                        { RET(CSRWR); }

"<<"                         { RET(LSLS); }
">>"                         { RET(GRGR); }
"+"                          { RET(PLUS); }
"-"                          { RET(MINUS); }
"*"                          { RET(STAR); }
"/"                          { RET(SLASH); }
":"                          { RET(COLON); }
"$"                          { RET(DOLLAR); }
"["                          { RET(LBRACKET); }
"]"                          { RET(RBRACKET); }
","                          { RET(COMMA); }
"{"                          { RET(LCBRACKET); }
"}"                          { RET(RCBRACKET); }

\"([^\"]*)\"                 { RET(STRING); }
{HEXA}                       { RET(INTEGER); }
{OCTAL}                      { RET(INTEGER); }
{BINARY}                     { RET(INTEGER); }
{DECIMAL}                    { RET(INTEGER); }
{LETTER}{ALPHANUM}*          { RET(SYMBOL); }

({REGISTER_RX}|{REGISTER_PC}|{REGISTER_SP})  { RET(REGISTER); }
%(status|handler|cause)      { RET(SYSTEM_REGISTER); }

{CRLF}                       { RET(NEWLINE); }
\#.*                         { RET(COMMENT); }
{WS}                         { /* skip */ }

.                            { RET(CATCH_ERROR); }

%%
