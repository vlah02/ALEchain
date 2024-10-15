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
    HALT = 265,                    /* HALT  */
    INTERRUPT = 266,               /* INTERRUPT  */
    INTERRUPT_RETURN = 267,        /* INTERRUPT_RETURN  */
    CALL = 268,                    /* CALL  */
    RETURN = 269,                  /* RETURN  */
    JUMP = 270,                    /* JUMP  */
    BRANCH_EQUAL = 271,            /* BRANCH_EQUAL  */
    BRANCH_notEQUAL = 272,         /* BRANCH_notEQUAL  */
    BRANCH_GREATER = 273,          /* BRANCH_GREATER  */
    PUSH = 274,                    /* PUSH  */
    POP = 275,                     /* POP  */
    EXCHANGE = 276,                /* EXCHANGE  */
    ADD = 277,                     /* ADD  */
    SUBTRACT = 278,                /* SUBTRACT  */
    MULTIPLY = 279,                /* MULTIPLY  */
    DIVIDE = 280,                  /* DIVIDE  */
    NOT = 281,                     /* NOT  */
    AND = 282,                     /* AND  */
    OR = 283,                      /* OR  */
    XOR = 284,                     /* XOR  */
    SHIFT_LEFT = 285,              /* SHIFT_LEFT  */
    SHIFT_RIGHT = 286,             /* SHIFT_RIGHT  */
    LOAD = 287,                    /* LOAD  */
    STORE = 288,                   /* STORE  */
    CSRRD = 289,                   /* CSRRD  */
    CSRWR = 290,                   /* CSRWR  */
    NEWLINE = 291,                 /* NEWLINE  */
    COMMENT = 292,                 /* COMMENT  */
    STRING = 293,                  /* STRING  */
    SYMBOL = 294,                  /* SYMBOL  */
    INTEGER = 295,                 /* INTEGER  */
    REGISTER = 296,                /* REGISTER  */
    SYSTEM_REGISTER = 297,         /* SYSTEM_REGISTER  */
    PLUS = 298,                    /* PLUS  */
    MINUS = 299,                   /* MINUS  */
    STAR = 300,                    /* STAR  */
    SLASH = 301,                   /* SLASH  */
    COLON = 302,                   /* COLON  */
    DOLLAR = 303,                  /* DOLLAR  */
    LBRACKET = 304,                /* LBRACKET  */
    RBRACKET = 305,                /* RBRACKET  */
    COMMA = 306,                   /* COMMA  */
    CATCH_ERROR = 307              /* CATCH_ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 53 "misc/parser.y"

    const char *str;

#line 120 "inc/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_INC_PARSER_HPP_INCLUDED  */
