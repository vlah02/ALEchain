/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "misc/parser.y"

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

#line 124 "src/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "../inc/parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_dotGLOBAL = 3,                  /* dotGLOBAL  */
  YYSYMBOL_dotSECTION = 4,                 /* dotSECTION  */
  YYSYMBOL_dotWORD = 5,                    /* dotWORD  */
  YYSYMBOL_dotSKIP = 6,                    /* dotSKIP  */
  YYSYMBOL_dotASCII = 7,                   /* dotASCII  */
  YYSYMBOL_dotEQU = 8,                     /* dotEQU  */
  YYSYMBOL_dotEND = 9,                     /* dotEND  */
  YYSYMBOL_dotTYPE = 10,                   /* dotTYPE  */
  YYSYMBOL_dotWEAK = 11,                   /* dotWEAK  */
  YYSYMBOL_HALT = 12,                      /* HALT  */
  YYSYMBOL_INTERRUPT = 13,                 /* INTERRUPT  */
  YYSYMBOL_INTERRUPT_RETURN = 14,          /* INTERRUPT_RETURN  */
  YYSYMBOL_CALL = 15,                      /* CALL  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_JUMP = 17,                      /* JUMP  */
  YYSYMBOL_BRANCH_EQUAL = 18,              /* BRANCH_EQUAL  */
  YYSYMBOL_BRANCH_notEQUAL = 19,           /* BRANCH_notEQUAL  */
  YYSYMBOL_BRANCH_GREATER = 20,            /* BRANCH_GREATER  */
  YYSYMBOL_PUSH = 21,                      /* PUSH  */
  YYSYMBOL_POP = 22,                       /* POP  */
  YYSYMBOL_EXCHANGE = 23,                  /* EXCHANGE  */
  YYSYMBOL_ADD = 24,                       /* ADD  */
  YYSYMBOL_SUBTRACT = 25,                  /* SUBTRACT  */
  YYSYMBOL_MULTIPLY = 26,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 27,                    /* DIVIDE  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_AND = 29,                       /* AND  */
  YYSYMBOL_OR = 30,                        /* OR  */
  YYSYMBOL_XOR = 31,                       /* XOR  */
  YYSYMBOL_SHIFT_LEFT = 32,                /* SHIFT_LEFT  */
  YYSYMBOL_SHIFT_RIGHT = 33,               /* SHIFT_RIGHT  */
  YYSYMBOL_LOAD = 34,                      /* LOAD  */
  YYSYMBOL_STORE = 35,                     /* STORE  */
  YYSYMBOL_CSRRD = 36,                     /* CSRRD  */
  YYSYMBOL_CSRWR = 37,                     /* CSRWR  */
  YYSYMBOL_NEWLINE = 38,                   /* NEWLINE  */
  YYSYMBOL_COMMENT = 39,                   /* COMMENT  */
  YYSYMBOL_STRING = 40,                    /* STRING  */
  YYSYMBOL_SYMBOL = 41,                    /* SYMBOL  */
  YYSYMBOL_INTEGER = 42,                   /* INTEGER  */
  YYSYMBOL_REGISTER = 43,                  /* REGISTER  */
  YYSYMBOL_SYSTEM_REGISTER = 44,           /* SYSTEM_REGISTER  */
  YYSYMBOL_PLUS = 45,                      /* PLUS  */
  YYSYMBOL_MINUS = 46,                     /* MINUS  */
  YYSYMBOL_STAR = 47,                      /* STAR  */
  YYSYMBOL_SLASH = 48,                     /* SLASH  */
  YYSYMBOL_COLON = 49,                     /* COLON  */
  YYSYMBOL_DOLLAR = 50,                    /* DOLLAR  */
  YYSYMBOL_LBRACKET = 51,                  /* LBRACKET  */
  YYSYMBOL_LCBRACKET = 52,                 /* LCBRACKET  */
  YYSYMBOL_RBRACKET = 53,                  /* RBRACKET  */
  YYSYMBOL_RCBRACKET = 54,                 /* RCBRACKET  */
  YYSYMBOL_COMMA = 55,                     /* COMMA  */
  YYSYMBOL_GRGR = 56,                      /* GRGR  */
  YYSYMBOL_LSLS = 57,                      /* LSLS  */
  YYSYMBOL_CATCH_ERROR = 58,               /* CATCH_ERROR  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_line = 61,                      /* line  */
  YYSYMBOL_instruction = 62,               /* instruction  */
  YYSYMBOL_directive = 63,                 /* directive  */
  YYSYMBOL_terminate = 64,                 /* terminate  */
  YYSYMBOL_symblist = 65,                  /* symblist  */
  YYSYMBOL_vallist = 66,                   /* vallist  */
  YYSYMBOL_reglist = 67,                   /* reglist  */
  YYSYMBOL_label = 68,                     /* label  */
  YYSYMBOL_global = 69,                    /* global  */
  YYSYMBOL_section = 70,                   /* section  */
  YYSYMBOL_word = 71,                      /* word  */
  YYSYMBOL_skip = 72,                      /* skip  */
  YYSYMBOL_ascii = 73,                     /* ascii  */
  YYSYMBOL_equ = 74,                       /* equ  */
  YYSYMBOL_end = 75,                       /* end  */
  YYSYMBOL_type = 76,                      /* type  */
  YYSYMBOL_weak = 77,                      /* weak  */
  YYSYMBOL_halt = 78,                      /* halt  */
  YYSYMBOL_int = 79,                       /* int  */
  YYSYMBOL_call = 80,                      /* call  */
  YYSYMBOL_jmp = 81,                       /* jmp  */
  YYSYMBOL_beq = 82,                       /* beq  */
  YYSYMBOL_bne = 83,                       /* bne  */
  YYSYMBOL_bgt = 84,                       /* bgt  */
  YYSYMBOL_xchg = 85,                      /* xchg  */
  YYSYMBOL_add = 86,                       /* add  */
  YYSYMBOL_sub = 87,                       /* sub  */
  YYSYMBOL_mul = 88,                       /* mul  */
  YYSYMBOL_div = 89,                       /* div  */
  YYSYMBOL_not = 90,                       /* not  */
  YYSYMBOL_and = 91,                       /* and  */
  YYSYMBOL_or = 92,                        /* or  */
  YYSYMBOL_xor = 93,                       /* xor  */
  YYSYMBOL_shl = 94,                       /* shl  */
  YYSYMBOL_shr = 95,                       /* shr  */
  YYSYMBOL_push = 96,                      /* push  */
  YYSYMBOL_pop = 97,                       /* pop  */
  YYSYMBOL_st = 98,                        /* st  */
  YYSYMBOL_ld = 99,                        /* ld  */
  YYSYMBOL_ret = 100,                      /* ret  */
  YYSYMBOL_iret = 101,                     /* iret  */
  YYSYMBOL_csrrd = 102,                    /* csrrd  */
  YYSYMBOL_csrwr = 103                     /* csrwr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  299

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    66,    66,    66,    66,    67,    67,    67,    67,    67,
      67,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    70,    70,    70,    72,
      74,    79,    81,    83,    88,    93,    98,   100,   102,   107,
     112,   119,   121,   126,   130,   136,   140,   164,   170,   180,
     184,   188,   192,   199,   203,   207,   210,   214,   217,   220,
     223,   228,   231,   235,   238,   242,   245,   250,   254,   256,
     258,   261,   264,   267,   271,   274,   277,   280,   284,   287,
     291,   293,   300,   304,   307,   310,   312,   314,   316,   319,
     323,   326,   329,   333,   337,   339,   341,   343,   346,   350,
     354,   360,   363
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "dotGLOBAL",
  "dotSECTION", "dotWORD", "dotSKIP", "dotASCII", "dotEQU", "dotEND",
  "dotTYPE", "dotWEAK", "HALT", "INTERRUPT", "INTERRUPT_RETURN", "CALL",
  "RETURN", "JUMP", "BRANCH_EQUAL", "BRANCH_notEQUAL", "BRANCH_GREATER",
  "PUSH", "POP", "EXCHANGE", "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE",
  "NOT", "AND", "OR", "XOR", "SHIFT_LEFT", "SHIFT_RIGHT", "LOAD", "STORE",
  "CSRRD", "CSRWR", "NEWLINE", "COMMENT", "STRING", "SYMBOL", "INTEGER",
  "REGISTER", "SYSTEM_REGISTER", "PLUS", "MINUS", "STAR", "SLASH", "COLON",
  "DOLLAR", "LBRACKET", "LCBRACKET", "RBRACKET", "RCBRACKET", "COMMA",
  "GRGR", "LSLS", "CATCH_ERROR", "$accept", "program", "line",
  "instruction", "directive", "terminate", "symblist", "vallist",
  "reglist", "label", "global", "section", "word", "skip", "ascii", "equ",
  "end", "type", "weak", "halt", "int", "call", "jmp", "beq", "bne", "bgt",
  "xchg", "add", "sub", "mul", "div", "not", "and", "or", "xor", "shl",
  "shr", "push", "pop", "st", "ld", "ret", "iret", "csrrd", "csrwr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-40)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -40,     6,   -40,   -39,   -33,    45,     4,    16,     9,    32,
      36,   -39,    32,    32,    32,    68,    32,    70,    39,    51,
      55,    29,    61,    77,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   136,    12,   137,    44,   138,   -40,   -40,
      64,   -40,   -40,   -40,   -40,   119,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,    63,    32,    32,    69,   -40,   140,    13,    32,    32,
     110,   -40,   -40,   142,    32,   -40,   -40,   -40,    32,    32,
     -40,    19,    32,   143,   144,   145,    32,   161,    32,   152,
     153,   154,   155,   156,    32,   157,   158,   159,   160,   162,
     163,   164,   165,    75,   173,   166,   167,   168,    32,   -40,
     -40,   -39,   -40,   -40,   182,   183,   -40,    54,   -40,   -40,
     -40,   171,   186,   -40,   -40,   -40,   187,   188,   -40,   -40,
     185,   189,   190,   -40,   176,   184,   -40,   193,   194,   196,
     197,   198,   -40,   199,   200,   201,   202,   203,   204,   211,
     212,   205,   207,    30,    18,   213,   214,   -40,   -40,   -40,
     -40,   112,   -40,   215,    32,    32,    32,    32,   209,   210,
     216,   161,    32,    32,    10,    32,    32,    32,    32,    32,
      32,    32,    32,    32,    32,    32,   223,   224,    60,   217,
      32,    32,    32,   225,    32,    32,   227,   228,   -40,   -40,
     -40,   -40,   -40,   118,   120,   122,   -40,   -40,   -40,   231,
     232,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,    32,    32,   206,   222,   233,   234,   -40,
     -40,   -40,    31,   -40,   -40,   -40,   -40,    32,    32,    32,
      32,    32,    32,   -40,   -40,   -40,   -40,   221,   229,   230,
      32,    65,    32,   -40,   -40,   -40,   -40,   -40,   -40,   244,
     245,   246,   -40,   237,   238,   239,   -40,    32,    32,    32,
      32,    32,    32,   -40,   -40,   -40,   -40,   -40,   -40
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    48,    48,    48,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,    47,
       0,     3,     6,     8,    10,     5,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    31,    30,    32,    33,    34,    35,
      36,    49,    48,    48,    51,    52,     0,    48,    48,    48,
       0,    46,    70,     0,    48,    73,    74,   120,    48,    48,
     119,    48,    48,     0,     0,     0,    48,     0,    48,     0,
       0,     0,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     7,
       9,     0,    64,    65,     0,     0,    53,     0,    66,    67,
      68,     0,     0,    72,    75,    76,     0,     0,    77,    78,
       0,     0,     0,   100,    61,     0,   102,     0,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    50,    54,
      55,    56,    57,     0,    48,    48,    48,    48,     0,     0,
       0,     0,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,     0,     0,     0,     0,
      48,    48,    48,     0,    48,    48,     0,     0,    58,    69,
      71,    79,    80,     0,     0,     0,    62,   101,    87,     0,
       0,    88,    91,    92,    93,    95,    96,    97,    98,    99,
     113,   112,   114,    48,    48,     0,     0,     0,     0,   104,
     103,   105,     0,   121,   122,    59,    60,    48,    48,    48,
      48,    48,    48,    89,    90,   111,   110,     0,     0,     0,
      48,     0,    48,    81,    82,    83,    84,    85,    86,     0,
       0,     0,   115,     0,     0,     0,   106,    48,    48,    48,
      48,    48,    48,   117,   116,   118,   108,   107,   109
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -40,   -40,   -40,   248,   249,    -9,   -10,   -40,   104,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    41,    42,    43,    44,    82,    87,   155,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,    94,    81,    95,    96,    97,     2,   100,    83,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    88,    40,    91,    39,
      90,    91,    39,   120,   121,   122,    89,    91,    39,   210,
     211,   212,   123,   124,   146,   147,   229,   230,   137,   213,
      91,    39,   106,   132,   133,   208,   271,    93,   138,   139,
     140,   107,   103,   209,   272,   143,    84,    85,   126,   144,
     145,    86,   148,   149,   104,   181,   182,   153,   105,   156,
     183,   245,   246,   247,   108,   162,   283,   284,   285,    98,
      99,   101,   102,   128,   134,   135,   171,   172,   131,   177,
     109,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,   216,   217,   257,
     258,   259,   260,   261,   262,   141,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   219,   220,   221,   222,   119,
     125,   127,   136,   227,   228,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   142,   150,   151,
     152,   249,   250,   251,   154,   253,   254,   157,   158,   159,
     160,   161,   163,   164,   165,   166,   173,   167,   168,   169,
     170,   174,   175,   176,   179,   180,   184,   185,   188,   186,
     187,   191,   189,   190,   265,   266,   193,   194,   192,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   273,   274,
     275,   276,   277,   278,   204,   205,   214,   218,   215,   267,
     206,   282,   207,   286,   223,   224,   243,   244,   252,   255,
     256,   225,   248,   263,   264,   268,   279,   270,   293,   294,
     295,   296,   297,   298,   280,   281,   269,   287,   288,   289,
     290,   291,   292,   129,   130,   226
};

static const yytype_int16 yycheck[] =
{
       9,    11,    41,    12,    13,    14,     0,    16,    41,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    42,    41,    38,    39,
      41,    38,    39,    41,    42,    43,    40,    38,    39,    41,
      42,    43,    50,    51,    45,    46,    56,    57,    55,    51,
      38,    39,    43,    82,    83,    45,    45,    41,    87,    88,
      89,    52,    43,    53,    53,    94,    41,    42,    44,    98,
      99,    46,   101,   102,    43,    41,    42,   106,    43,   108,
      46,    41,    42,    43,    43,   114,    41,    42,    43,    41,
      42,    41,    42,    49,    45,    46,    41,    42,    55,   128,
      43,   131,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    45,    46,    41,
      42,    41,    42,    41,    42,    55,    43,    43,    43,    43,
      43,    43,    43,    43,    43,   184,   185,   186,   187,    43,
      43,    43,    42,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,    55,    55,    55,
      55,   210,   211,   212,    43,   214,   215,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    43,    55,    55,    55,
      55,    55,    55,    55,    42,    42,    55,    41,    43,    42,
      42,    55,    43,    43,   243,   244,    43,    43,    54,    43,
      43,    43,    43,    43,    43,    43,    43,    43,   257,   258,
     259,   260,   261,   262,    43,    43,    43,    42,    44,    53,
      55,   270,    55,   272,    55,    55,    43,    43,    43,    42,
      42,    55,    55,    42,    42,    53,    55,    43,   287,   288,
     289,   290,   291,   292,    55,    55,    53,    43,    43,    43,
      53,    53,    53,    45,    45,   191
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    60,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      41,    61,    62,    63,    64,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    41,    65,    41,    41,    42,    46,    66,    42,    40,
      41,    38,    64,    41,    65,    64,    64,    64,    41,    42,
      64,    41,    42,    43,    43,    43,    43,    52,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    43,    43,
      41,    42,    43,    50,    51,    43,    44,    43,    49,    62,
      63,    55,    64,    64,    45,    46,    42,    55,    64,    64,
      64,    55,    55,    64,    64,    64,    45,    46,    64,    64,
      55,    55,    55,    64,    43,    67,    64,    55,    55,    55,
      55,    55,    64,    55,    55,    55,    55,    55,    55,    55,
      55,    41,    42,    43,    55,    55,    55,    64,    65,    42,
      42,    41,    42,    46,    55,    41,    42,    42,    43,    43,
      43,    55,    54,    43,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    55,    55,    45,    53,
      41,    42,    43,    51,    43,    44,    45,    46,    42,    64,
      64,    64,    64,    55,    55,    55,    67,    64,    64,    56,
      57,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    43,    43,    41,    42,    43,    55,    64,
      64,    64,    43,    64,    64,    42,    42,    41,    42,    41,
      42,    41,    42,    42,    42,    64,    64,    53,    53,    53,
      43,    45,    53,    64,    64,    64,    64,    64,    64,    55,
      55,    55,    64,    41,    42,    43,    64,    43,    43,    43,
      53,    53,    53,    64,    64,    64,    64,    64,    64
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    60,    61,    61,    61,    61,    61,
      61,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    64,    64,    64,    65,
      65,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    80,    81,    81,    81,
      81,    82,    82,    83,    83,    84,    84,    85,    86,    86,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    96,    97,    98,    98,    98,    98,    98,    98,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,   100,
     101,   102,   103
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       3,     1,     1,     2,     3,     3,     3,     3,     4,     5,
       5,     1,     3,     3,     3,     3,     3,     3,     3,     5,
       2,     5,     3,     2,     2,     3,     3,     3,     3,     5,
       5,     7,     7,     7,     7,     7,     7,     5,     5,     6,
       6,     5,     5,     5,     3,     5,     5,     5,     5,     5,
       3,     5,     3,     5,     5,     5,     7,     9,     9,     9,
       6,     6,     5,     5,     5,     7,     9,     9,     9,     2,
       2,     5,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 49: /* symblist: SYMBOL  */
#line 72 "misc/parser.y"
                 {
    symblist.push_back((yyvsp[0].str));
}
#line 1410 "src/parser.cpp"
    break;

  case 50: /* symblist: SYMBOL COMMA symblist  */
#line 74 "misc/parser.y"
                          {
    symblist.push_back((yyvsp[-2].str));
}
#line 1418 "src/parser.cpp"
    break;

  case 51: /* vallist: SYMBOL  */
#line 80 "misc/parser.y"
        { vallist.push_back({ (yyvsp[0].str), valtype::SYM }); }
#line 1424 "src/parser.cpp"
    break;

  case 52: /* vallist: INTEGER  */
#line 82 "misc/parser.y"
        { vallist.push_back({ (yyvsp[0].str), valtype::INT }); }
#line 1430 "src/parser.cpp"
    break;

  case 53: /* vallist: MINUS INTEGER  */
#line 84 "misc/parser.y"
        {
            std::string lit = "-" + std::string((yyvsp[0].str));
            vallist.push_back({ lit, valtype::INT });
        }
#line 1439 "src/parser.cpp"
    break;

  case 54: /* vallist: SYMBOL PLUS INTEGER  */
#line 89 "misc/parser.y"
        {
            std::string lit = std::string((yyvsp[-2].str)) + "+" + std::string((yyvsp[0].str));
            vallist.push_back({ lit, valtype::SYM });
        }
#line 1448 "src/parser.cpp"
    break;

  case 55: /* vallist: SYMBOL MINUS INTEGER  */
#line 94 "misc/parser.y"
        {
            std::string lit = std::string((yyvsp[-2].str)) + "-" + std::string((yyvsp[0].str));
            vallist.push_back({ lit, valtype::SYM });
        }
#line 1457 "src/parser.cpp"
    break;

  case 56: /* vallist: vallist COMMA SYMBOL  */
#line 99 "misc/parser.y"
        { vallist.push_back({ (yyvsp[0].str), valtype::SYM }); }
#line 1463 "src/parser.cpp"
    break;

  case 57: /* vallist: vallist COMMA INTEGER  */
#line 101 "misc/parser.y"
        { vallist.push_back({ (yyvsp[0].str), valtype::INT }); }
#line 1469 "src/parser.cpp"
    break;

  case 58: /* vallist: vallist COMMA MINUS INTEGER  */
#line 103 "misc/parser.y"
        {
            std::string lit = "-" + std::string((yyvsp[-1].str));
            vallist.push_back({ lit, valtype::INT });
        }
#line 1478 "src/parser.cpp"
    break;

  case 59: /* vallist: vallist COMMA SYMBOL PLUS INTEGER  */
#line 108 "misc/parser.y"
        {
            std::string lit = std::string((yyvsp[-2].str)) + "+" + std::string((yyvsp[0].str));
            vallist.push_back({ lit, valtype::SYM });
        }
#line 1487 "src/parser.cpp"
    break;

  case 60: /* vallist: vallist COMMA SYMBOL MINUS INTEGER  */
#line 113 "misc/parser.y"
        {
            std::string lit = std::string((yyvsp[-2].str)) + "-" + std::string((yyvsp[0].str));
            vallist.push_back({ lit, valtype::SYM });
        }
#line 1496 "src/parser.cpp"
    break;

  case 61: /* reglist: REGISTER  */
#line 119 "misc/parser.y"
                  {
    reglist.push_back(regs[(yyvsp[0].str)]);
}
#line 1504 "src/parser.cpp"
    break;

  case 62: /* reglist: REGISTER COMMA reglist  */
#line 121 "misc/parser.y"
                           {
    reglist.push_back(regs[(yyvsp[-2].str)]);
}
#line 1512 "src/parser.cpp"
    break;

  case 63: /* label: SYMBOL COLON terminate  */
#line 126 "misc/parser.y"
                              {
    SymTab::add_definition((yyvsp[-2].str), section->getName(), section->getSize());
}
#line 1520 "src/parser.cpp"
    break;

  case 64: /* global: dotGLOBAL symblist terminate  */
#line 130 "misc/parser.y"
                                     {
    for (auto& symb : symblist)
        SymTab::globals.insert(symb);
    symblist.clear();
}
#line 1530 "src/parser.cpp"
    break;

  case 65: /* section: dotSECTION SYMBOL terminate  */
#line 136 "misc/parser.y"
                                     {
    section = Section::extract((yyvsp[-1].str));
}
#line 1538 "src/parser.cpp"
    break;

  case 66: /* word: dotWORD vallist terminate  */
#line 140 "misc/parser.y"
                                {
    for (auto it = vallist.rbegin(); it != vallist.rend(); ++it) {
        if (it->second == valtype::INT) {
            section->insertInt(toInt(it->first));
        } else if (it->second == valtype::SYM) {
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
    vallist.clear();
}
#line 1566 "src/parser.cpp"
    break;

  case 67: /* skip: dotSKIP INTEGER terminate  */
#line 164 "misc/parser.y"
                                {
    for (int i = 0; i < toInt((yyvsp[-1].str)); i++) {
        section->insertByte(0);
    }
}
#line 1576 "src/parser.cpp"
    break;

  case 68: /* ascii: dotASCII STRING terminate  */
#line 170 "misc/parser.y"
                                 {
    std::string raw((yyvsp[-1].str));
    if (!raw.empty() && raw.front() == '"' && raw.back() == '"') {
        raw = raw.substr(1, raw.size() - 2);
    }
    for (char c : raw) {
        section->insertByte(static_cast<unsigned char>(c));
    }
}
#line 1590 "src/parser.cpp"
    break;

  case 69: /* equ: dotEQU SYMBOL COMMA COMMA terminate  */
#line 180 "misc/parser.y"
                                         {

}
#line 1598 "src/parser.cpp"
    break;

  case 70: /* end: dotEND terminate  */
#line 184 "misc/parser.y"
                      {
    YYABORT;
}
#line 1606 "src/parser.cpp"
    break;

  case 71: /* type: dotTYPE SYMBOL COMMA SYMBOL terminate  */
#line 188 "misc/parser.y"
                                            {
    SymTab::add_type((yyvsp[-3].str), (yyvsp[-1].str));
}
#line 1614 "src/parser.cpp"
    break;

  case 72: /* weak: dotWEAK symblist terminate  */
#line 192 "misc/parser.y"
                                 {
    for (auto& symb : symblist)
        SymTab::weaks.insert(symb);
    symblist.clear();
}
#line 1624 "src/parser.cpp"
    break;

  case 73: /* halt: HALT terminate  */
#line 199 "misc/parser.y"
                     {
    section->add_instruction();
}
#line 1632 "src/parser.cpp"
    break;

  case 74: /* int: INTERRUPT terminate  */
#line 203 "misc/parser.y"
                         {
    section->add_instruction(0b0001);
}
#line 1640 "src/parser.cpp"
    break;

  case 75: /* call: CALL SYMBOL terminate  */
#line 207 "misc/parser.y"
                            {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0010, 0b0001, 15);
}
#line 1649 "src/parser.cpp"
    break;

  case 76: /* call: CALL INTEGER terminate  */
#line 210 "misc/parser.y"
                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0010, 0b0001, 15);
}
#line 1658 "src/parser.cpp"
    break;

  case 77: /* jmp: JUMP SYMBOL terminate  */
#line 214 "misc/parser.y"
                           {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1667 "src/parser.cpp"
    break;

  case 78: /* jmp: JUMP INTEGER terminate  */
#line 217 "misc/parser.y"
                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1676 "src/parser.cpp"
    break;

  case 79: /* jmp: JUMP SYMBOL PLUS INTEGER terminate  */
#line 220 "misc/parser.y"
                                       {
    section->add_literal(std::string((yyvsp[-3].str)) + "+" + std::string((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1685 "src/parser.cpp"
    break;

  case 80: /* jmp: JUMP SYMBOL MINUS INTEGER terminate  */
#line 223 "misc/parser.y"
                                        {
    section->add_literal(std::string((yyvsp[-3].str)) + "-" + std::string((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1694 "src/parser.cpp"
    break;

  case 81: /* beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 228 "misc/parser.y"
                                                                 {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1001, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1703 "src/parser.cpp"
    break;

  case 82: /* beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 231 "misc/parser.y"
                                                                 {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1001, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1712 "src/parser.cpp"
    break;

  case 83: /* bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 235 "misc/parser.y"
                                                                    {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1010, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1721 "src/parser.cpp"
    break;

  case 84: /* bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 238 "misc/parser.y"
                                                                    {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1010, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1730 "src/parser.cpp"
    break;

  case 85: /* bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 242 "misc/parser.y"
                                                                   {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1011, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1739 "src/parser.cpp"
    break;

  case 86: /* bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 245 "misc/parser.y"
                                                                   {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1011, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1748 "src/parser.cpp"
    break;

  case 87: /* xchg: EXCHANGE REGISTER COMMA REGISTER terminate  */
#line 250 "misc/parser.y"
                                                 {
    section->add_instruction(0b0100, 0b0000, 0, regs[(yyvsp[-3].str)], regs[(yyvsp[-1].str)]);
}
#line 1756 "src/parser.cpp"
    break;

  case 88: /* add: ADD REGISTER COMMA REGISTER terminate  */
#line 254 "misc/parser.y"
                                           {
    section->add_instruction(0b0101, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1764 "src/parser.cpp"
    break;

  case 89: /* add: ADD REGISTER COMMA REGISTER GRGR INTEGER  */
#line 256 "misc/parser.y"
                                             {

}
#line 1772 "src/parser.cpp"
    break;

  case 90: /* add: ADD REGISTER COMMA REGISTER LSLS INTEGER  */
#line 258 "misc/parser.y"
                                             {

}
#line 1780 "src/parser.cpp"
    break;

  case 91: /* sub: SUBTRACT REGISTER COMMA REGISTER terminate  */
#line 261 "misc/parser.y"
                                                {
    section->add_instruction(0b0101, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1788 "src/parser.cpp"
    break;

  case 92: /* mul: MULTIPLY REGISTER COMMA REGISTER terminate  */
#line 264 "misc/parser.y"
                                                {
    section->add_instruction(0b0101, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1796 "src/parser.cpp"
    break;

  case 93: /* div: DIVIDE REGISTER COMMA REGISTER terminate  */
#line 267 "misc/parser.y"
                                              {
    section->add_instruction(0b0101, 0b0011, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1804 "src/parser.cpp"
    break;

  case 94: /* not: NOT REGISTER terminate  */
#line 271 "misc/parser.y"
                            {
    section->add_instruction(0b0110, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1812 "src/parser.cpp"
    break;

  case 95: /* and: AND REGISTER COMMA REGISTER terminate  */
#line 274 "misc/parser.y"
                                           {
    section->add_instruction(0b0110, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1820 "src/parser.cpp"
    break;

  case 96: /* or: OR REGISTER COMMA REGISTER terminate  */
#line 277 "misc/parser.y"
                                         {
    section->add_instruction(0b0110, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1828 "src/parser.cpp"
    break;

  case 97: /* xor: XOR REGISTER COMMA REGISTER terminate  */
#line 280 "misc/parser.y"
                                           {
    section->add_instruction(0b0110, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1836 "src/parser.cpp"
    break;

  case 98: /* shl: SHIFT_LEFT REGISTER COMMA REGISTER terminate  */
#line 284 "misc/parser.y"
                                                  {
    section->add_instruction(0b0111, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1844 "src/parser.cpp"
    break;

  case 99: /* shr: SHIFT_RIGHT REGISTER COMMA REGISTER terminate  */
#line 287 "misc/parser.y"
                                                   {
    section->add_instruction(0b0111, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1852 "src/parser.cpp"
    break;

  case 100: /* push: PUSH REGISTER terminate  */
#line 291 "misc/parser.y"
                              {
    section->add_instruction(0b1000, 0b0001, 14, 0, regs[(yyvsp[-1].str)], -4);
}
#line 1860 "src/parser.cpp"
    break;

  case 101: /* push: PUSH LCBRACKET reglist RCBRACKET terminate  */
#line 293 "misc/parser.y"
                                               {
    for (auto it = reglist.rbegin(); it != reglist.rend(); ++it) {
        section->add_instruction(0b1000, 0b0001, 14, 0, *it, -4);
    }
    reglist.clear();
}
#line 1871 "src/parser.cpp"
    break;

  case 102: /* pop: POP REGISTER terminate  */
#line 300 "misc/parser.y"
                            {
    section->add_instruction(0b1001, 0b0011, regs[(yyvsp[-1].str)], 14, 0, 4);
}
#line 1879 "src/parser.cpp"
    break;

  case 103: /* st: STORE REGISTER COMMA INTEGER terminate  */
#line 304 "misc/parser.y"
                                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[(yyvsp[-3].str)]);
}
#line 1888 "src/parser.cpp"
    break;

  case 104: /* st: STORE REGISTER COMMA SYMBOL terminate  */
#line 307 "misc/parser.y"
                                          {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[(yyvsp[-3].str)]);
}
#line 1897 "src/parser.cpp"
    break;

  case 105: /* st: STORE REGISTER COMMA REGISTER terminate  */
#line 310 "misc/parser.y"
                                            {
    section->add_instruction(0b1001, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1905 "src/parser.cpp"
    break;

  case 106: /* st: STORE REGISTER COMMA LBRACKET REGISTER RBRACKET terminate  */
#line 312 "misc/parser.y"
                                                              {
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-2].str)], 0, regs[(yyvsp[-5].str)]);
}
#line 1913 "src/parser.cpp"
    break;

  case 107: /* st: STORE REGISTER COMMA LBRACKET REGISTER PLUS INTEGER RBRACKET terminate  */
#line 314 "misc/parser.y"
                                                                           {
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-4].str)], 0, regs[(yyvsp[-7].str)], toInt((yyvsp[-2].str)));
}
#line 1921 "src/parser.cpp"
    break;

  case 108: /* st: STORE REGISTER COMMA LBRACKET REGISTER PLUS SYMBOL RBRACKET terminate  */
#line 316 "misc/parser.y"
                                                                          {
    section->add_literal((yyvsp[-2].str));
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-4].str)], 0, regs[(yyvsp[-7].str)]);
}
#line 1930 "src/parser.cpp"
    break;

  case 109: /* st: STORE REGISTER COMMA LBRACKET REGISTER PLUS REGISTER RBRACKET terminate  */
#line 319 "misc/parser.y"
                                                                            {
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-4].str)], regs[(yyvsp[-2].str)], regs[(yyvsp[-7].str)]);
}
#line 1938 "src/parser.cpp"
    break;

  case 110: /* ld: LOAD DOLLAR INTEGER COMMA REGISTER terminate  */
#line 323 "misc/parser.y"
                                                 {
    section->add_literal(toInt((yyvsp[-3].str)));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 15);
}
#line 1947 "src/parser.cpp"
    break;

  case 111: /* ld: LOAD DOLLAR SYMBOL COMMA REGISTER terminate  */
#line 326 "misc/parser.y"
                                                {
    section->add_literal((yyvsp[-3].str));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 15);
}
#line 1956 "src/parser.cpp"
    break;

  case 112: /* ld: LOAD INTEGER COMMA REGISTER terminate  */
#line 329 "misc/parser.y"
                                          {
    section->add_literal(toInt((yyvsp[-3].str)));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1966 "src/parser.cpp"
    break;

  case 113: /* ld: LOAD SYMBOL COMMA REGISTER terminate  */
#line 333 "misc/parser.y"
                                         {
    section->add_literal((yyvsp[-3].str));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1976 "src/parser.cpp"
    break;

  case 114: /* ld: LOAD REGISTER COMMA REGISTER terminate  */
#line 337 "misc/parser.y"
                                           {
    section->add_instruction(0b1001, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1984 "src/parser.cpp"
    break;

  case 115: /* ld: LOAD LBRACKET REGISTER RBRACKET COMMA REGISTER terminate  */
#line 339 "misc/parser.y"
                                                             {
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-4].str)]);
}
#line 1992 "src/parser.cpp"
    break;

  case 116: /* ld: LOAD LBRACKET REGISTER PLUS INTEGER RBRACKET COMMA REGISTER terminate  */
#line 341 "misc/parser.y"
                                                                          {
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-6].str)], toInt((yyvsp[-4].str)));
}
#line 2000 "src/parser.cpp"
    break;

  case 117: /* ld: LOAD LBRACKET REGISTER PLUS SYMBOL RBRACKET COMMA REGISTER terminate  */
#line 343 "misc/parser.y"
                                                                         {
    SymTab::add_occurrence((yyvsp[-4].str), section->getName(), section->getSize(), false);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-6].str)]);
}
#line 2009 "src/parser.cpp"
    break;

  case 118: /* ld: LOAD LBRACKET REGISTER PLUS REGISTER RBRACKET COMMA REGISTER terminate  */
#line 346 "misc/parser.y"
                                                                           {
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-6].str)], regs[(yyvsp[-4].str)]);
}
#line 2017 "src/parser.cpp"
    break;

  case 119: /* ret: RETURN terminate  */
#line 350 "misc/parser.y"
                      {
    section->add_instruction(0b1001, 0b0011, 15, 14, 0, 4);
}
#line 2025 "src/parser.cpp"
    break;

  case 120: /* iret: INTERRUPT_RETURN terminate  */
#line 354 "misc/parser.y"
                                 {
    section->add_instruction(0b1001, 0b0100, 0b0010);
    section->add_instruction(0b1001, 0b0011, 0b0000, 0b1110, 0, 4);
    section->add_instruction(0b1001, 0b0011, 0b1111, 0b1110, 0, 4);
}
#line 2035 "src/parser.cpp"
    break;

  case 121: /* csrrd: CSRRD SYSTEM_REGISTER COMMA REGISTER terminate  */
#line 360 "misc/parser.y"
                                                      {
    section->add_instruction(0b1001, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 2043 "src/parser.cpp"
    break;

  case 122: /* csrwr: CSRWR REGISTER COMMA SYSTEM_REGISTER terminate  */
#line 363 "misc/parser.y"
                                                      {
    section->add_instruction(0b1001, 0b0100, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 2051 "src/parser.cpp"
    break;


#line 2055 "src/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 367 "misc/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "ERROR (token=%d, text=%s) (last symbol='%s')\n", yychar, yytext, yylval.str);
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
