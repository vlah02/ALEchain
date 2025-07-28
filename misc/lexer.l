%{
#include "../inc/parser.hpp"
#include <iostream>
%}

digit [0-9]
decimal {digit}+
hexadigit [0-9]|[A-F]|[a-f]
hexa "0x"({hexadigit})+
octadigit [0-7]
octal "0o"({octadigit})+
bindigit [0-1]
binary "0b"({bindigit})+
lowercase [a-z]
uppercase [A-Z]
letter {lowercase}|{uppercase}|"_"
alphanum ({digit}|{letter})+
operator "+"|"-"|"*"|"/"



%%

".global"|".extern" { yylval.str = strdup(yytext); return dotGLOBAL; }
".section" { yylval.str = strdup(yytext); return dotSECTION; }
".word" { yylval.str = strdup(yytext); return dotWORD; }
".skip" { yylval.str = strdup(yytext); return dotSKIP; }
".ascii" { yylval.str = strdup(yytext); return dotASCII; }
".equ" { yylval.str = strdup(yytext); return dotEQU; }
".end" { yylval.str = strdup(yytext); return dotEND; }
".type" { yylval.str = strdup(yytext); return dotTYPE; }
".weak" { yylval.str = strdup(yytext); return dotWEAK; }


"halt" { yylval.str = strdup(yytext); return HALT; }
"int" { yylval.str = strdup(yytext); return INTERRUPT; }
"iret" { yylval.str = strdup(yytext); return INTERRUPT_RETURN; }
"call" { yylval.str = strdup(yytext); return CALL; }
"ret" { yylval.str = strdup(yytext); return RETURN; }
"jmp" { yylval.str = strdup(yytext); return JUMP; }
"beq" { yylval.str = strdup(yytext); return BRANCH_EQUAL; }
"bne" { yylval.str = strdup(yytext); return BRANCH_notEQUAL; }
"bgt" { yylval.str = strdup(yytext); return BRANCH_GREATER; }
"push" { yylval.str = strdup(yytext); return PUSH; }
"pop" { yylval.str = strdup(yytext); return POP; }
"xchg" { yylval.str = strdup(yytext); return EXCHANGE; }
"add" { yylval.str = strdup(yytext); return ADD; }
"sub" { yylval.str = strdup(yytext); return SUBTRACT; }
"mul" { yylval.str = strdup(yytext); return MULTIPLY; }
"div" { yylval.str = strdup(yytext); return DIVIDE; }
"not" { yylval.str = strdup(yytext); return NOT; }
"and" { yylval.str = strdup(yytext); return AND; }
"or" { yylval.str = strdup(yytext); return OR; }
"xor" { yylval.str = strdup(yytext); return XOR; }
"shl" { yylval.str = strdup(yytext); return SHIFT_LEFT; }
"shr" { yylval.str = strdup(yytext); return SHIFT_RIGHT; }
"ld" { yylval.str = strdup(yytext); return LOAD; }
"st" { yylval.str = strdup(yytext); return STORE; }
"csrrd" { yylval.str = strdup(yytext); return CSRRD; }
"csrwr" { yylval.str = strdup(yytext); return CSRWR; }


[\n] { yylval.str = strdup(yytext); return NEWLINE; }
"#"(.)* { yylval.str = strdup(yytext); return COMMENT; }
\"([^\"]*)\"   { yylval.str = strdup(yytext); return STRING; }
{letter}({alphanum})* { yylval.str = strdup(yytext); return SYMBOL; }
"-"?{decimal}|{hexa}|{octal}|{binary} { yylval.str = strdup(yytext); return INTEGER; }
"%"(("r"{digit}[0-5]?)|"pc"|"sp") { yylval.str = strdup(yytext); return REGISTER; }
"%"("status"|"handler"|"cause") { yylval.str = strdup(yytext); return SYSTEM_REGISTER; }

"+" { yylval.str = strdup(yytext); return PLUS; }
"-" { yylval.str = strdup(yytext); return MINUS; }
"*" { yylval.str = strdup(yytext); return STAR; }
"/" { yylval.str = strdup(yytext); return SLASH; }
":" { yylval.str = strdup(yytext); return COLON; }
"$" { yylval.str = strdup(yytext); return DOLLAR; }
"[" { yylval.str = strdup(yytext); return LBRACKET; }
"]" { yylval.str = strdup(yytext); return RBRACKET; }
"," { yylval.str = strdup(yytext); return COMMA; }
"{" { yylval.str = strdup(yytext); return LCBRACKET; }
"}" { yylval.str = strdup(yytext); return RCBRACKET; }
"<<" { yylval.str = strdup(yytext); return LSLS; }
">>" { yylval.str = strdup(yytext); return GRGR; }

[ \t]+ {}

. { yylval.str = strdup(yytext); return CATCH_ERROR; }

%%



int yywrap() {
    return 1;
}