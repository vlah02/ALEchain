/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_INC_PARSER_HPP_INCLUDED
# define YY_YY_INC_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    dotGLOBAL = 258,               /* dotGLOBAL  */
    dotSECTION = 259,              /* dotSECTION  */
    dotWORD = 260,                 /* dotWORD  */
    dotSKIP = 261,                 /* dotSKIP  */
    dotASCII = 262,                /* dotASCII  */
    dotEQU = 263,                  /* dotEQU  */
    dotEND = 264,                  /* dotEND  */
    dotTYPE = 265,                 /* dotTYPE  */
    dotWEAK = 266,                 /* dotWEAK  */
    HALT = 267,                    /* HALT  */
    INTERRUPT = 268,               /* INTERRUPT  */
    INTERRUPT_RETURN = 269,        /* INTERRUPT_RETURN  */
    CALL = 270,                    /* CALL  */
    RETURN = 271,                  /* RETURN  */
    JUMP = 272,                    /* JUMP  */
    BRANCH_EQUAL = 273,            /* BRANCH_EQUAL  */
    BRANCH_notEQUAL = 274,         /* BRANCH_notEQUAL  */
    BRANCH_GREATER = 275,          /* BRANCH_GREATER  */
    PUSH = 276,                    /* PUSH  */
    POP = 277,                     /* POP  */
    EXCHANGE = 278,                /* EXCHANGE  */
    ADD = 279,                     /* ADD  */
    SUBTRACT = 280,                /* SUBTRACT  */
    MULTIPLY = 281,                /* MULTIPLY  */
    DIVIDE = 282,                  /* DIVIDE  */
    NOT = 283,                     /* NOT  */
    AND = 284,                     /* AND  */
    OR = 285,                      /* OR  */
    XOR = 286,                     /* XOR  */
    SHIFT_LEFT = 287,              /* SHIFT_LEFT  */
    SHIFT_RIGHT = 288,             /* SHIFT_RIGHT  */
    LOAD = 289,                    /* LOAD  */
    STORE = 290,                   /* STORE  */
    CSRRD = 291,                   /* CSRRD  */
    CSRWR = 292,                   /* CSRWR  */
    NEWLINE = 293,                 /* NEWLINE  */
    COMMENT = 294,                 /* COMMENT  */
    STRING = 295,                  /* STRING  */
    SYMBOL = 296,                  /* SYMBOL  */
    INTEGER = 297,                 /* INTEGER  */
    REGISTER = 298,                /* REGISTER  */
    SYSTEM_REGISTER = 299,         /* SYSTEM_REGISTER  */
    PLUS = 300,                    /* PLUS  */
    MINUS = 301,                   /* MINUS  */
    STAR = 302,                    /* STAR  */
    SLASH = 303,                   /* SLASH  */
    COLON = 304,                   /* COLON  */
    DOLLAR = 305,                  /* DOLLAR  */
    LBRACKET = 306,                /* LBRACKET  */
    LCBRACKET = 307,               /* LCBRACKET  */
    RBRACKET = 308,                /* RBRACKET  */
    RCBRACKET = 309,               /* RCBRACKET  */
    COMMA = 310,                   /* COMMA  */
    CATCH_ERROR = 311              /* CATCH_ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 54 "misc/parser.y"

    const char *str;

#line 124 "inc/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_INC_PARSER_HPP_INCLUDED  */
