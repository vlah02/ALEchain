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

#line 123 "src/parser.cpp"

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
  YYSYMBOL_HALT = 10,                      /* HALT  */
  YYSYMBOL_INTERRUPT = 11,                 /* INTERRUPT  */
  YYSYMBOL_INTERRUPT_RETURN = 12,          /* INTERRUPT_RETURN  */
  YYSYMBOL_CALL = 13,                      /* CALL  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_JUMP = 15,                      /* JUMP  */
  YYSYMBOL_BRANCH_EQUAL = 16,              /* BRANCH_EQUAL  */
  YYSYMBOL_BRANCH_notEQUAL = 17,           /* BRANCH_notEQUAL  */
  YYSYMBOL_BRANCH_GREATER = 18,            /* BRANCH_GREATER  */
  YYSYMBOL_PUSH = 19,                      /* PUSH  */
  YYSYMBOL_POP = 20,                       /* POP  */
  YYSYMBOL_EXCHANGE = 21,                  /* EXCHANGE  */
  YYSYMBOL_ADD = 22,                       /* ADD  */
  YYSYMBOL_SUBTRACT = 23,                  /* SUBTRACT  */
  YYSYMBOL_MULTIPLY = 24,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 25,                    /* DIVIDE  */
  YYSYMBOL_NOT = 26,                       /* NOT  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_OR = 28,                        /* OR  */
  YYSYMBOL_XOR = 29,                       /* XOR  */
  YYSYMBOL_SHIFT_LEFT = 30,                /* SHIFT_LEFT  */
  YYSYMBOL_SHIFT_RIGHT = 31,               /* SHIFT_RIGHT  */
  YYSYMBOL_LOAD = 32,                      /* LOAD  */
  YYSYMBOL_STORE = 33,                     /* STORE  */
  YYSYMBOL_CSRRD = 34,                     /* CSRRD  */
  YYSYMBOL_CSRWR = 35,                     /* CSRWR  */
  YYSYMBOL_NEWLINE = 36,                   /* NEWLINE  */
  YYSYMBOL_COMMENT = 37,                   /* COMMENT  */
  YYSYMBOL_STRING = 38,                    /* STRING  */
  YYSYMBOL_SYMBOL = 39,                    /* SYMBOL  */
  YYSYMBOL_INTEGER = 40,                   /* INTEGER  */
  YYSYMBOL_REGISTER = 41,                  /* REGISTER  */
  YYSYMBOL_SYSTEM_REGISTER = 42,           /* SYSTEM_REGISTER  */
  YYSYMBOL_PLUS = 43,                      /* PLUS  */
  YYSYMBOL_MINUS = 44,                     /* MINUS  */
  YYSYMBOL_STAR = 45,                      /* STAR  */
  YYSYMBOL_SLASH = 46,                     /* SLASH  */
  YYSYMBOL_COLON = 47,                     /* COLON  */
  YYSYMBOL_DOLLAR = 48,                    /* DOLLAR  */
  YYSYMBOL_LBRACKET = 49,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 50,                  /* RBRACKET  */
  YYSYMBOL_COMMA = 51,                     /* COMMA  */
  YYSYMBOL_CATCH_ERROR = 52,               /* CATCH_ERROR  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_line = 54,                      /* line  */
  YYSYMBOL_instruction = 55,               /* instruction  */
  YYSYMBOL_directive = 56,                 /* directive  */
  YYSYMBOL_terminate = 57,                 /* terminate  */
  YYSYMBOL_symblist = 58,                  /* symblist  */
  YYSYMBOL_vallist = 59,                   /* vallist  */
  YYSYMBOL_label = 60,                     /* label  */
  YYSYMBOL_global = 61,                    /* global  */
  YYSYMBOL_section = 62,                   /* section  */
  YYSYMBOL_word = 63,                      /* word  */
  YYSYMBOL_skip = 64,                      /* skip  */
  YYSYMBOL_ascii = 65,                     /* ascii  */
  YYSYMBOL_equ = 66,                       /* equ  */
  YYSYMBOL_end = 67,                       /* end  */
  YYSYMBOL_halt = 68,                      /* halt  */
  YYSYMBOL_int = 69,                       /* int  */
  YYSYMBOL_call = 70,                      /* call  */
  YYSYMBOL_jmp = 71,                       /* jmp  */
  YYSYMBOL_beq = 72,                       /* beq  */
  YYSYMBOL_bne = 73,                       /* bne  */
  YYSYMBOL_bgt = 74,                       /* bgt  */
  YYSYMBOL_xchg = 75,                      /* xchg  */
  YYSYMBOL_add = 76,                       /* add  */
  YYSYMBOL_sub = 77,                       /* sub  */
  YYSYMBOL_mul = 78,                       /* mul  */
  YYSYMBOL_div = 79,                       /* div  */
  YYSYMBOL_not = 80,                       /* not  */
  YYSYMBOL_and = 81,                       /* and  */
  YYSYMBOL_or = 82,                        /* or  */
  YYSYMBOL_xor = 83,                       /* xor  */
  YYSYMBOL_shl = 84,                       /* shl  */
  YYSYMBOL_shr = 85,                       /* shr  */
  YYSYMBOL_push = 86,                      /* push  */
  YYSYMBOL_pop = 87,                       /* pop  */
  YYSYMBOL_st = 88,                        /* st  */
  YYSYMBOL_ld = 89,                        /* ld  */
  YYSYMBOL_ret = 90,                       /* ret  */
  YYSYMBOL_iret = 91,                      /* iret  */
  YYSYMBOL_csrrd = 92,                     /* csrrd  */
  YYSYMBOL_csrwr = 93                      /* csrwr  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  117
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   245

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  250

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


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
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    70,    72,    76,    78,    80,    82,    87,
      91,    97,   101,   113,   119,   123,   127,   132,   136,   140,
     143,   147,   150,   155,   158,   162,   165,   169,   172,   177,
     181,   184,   187,   190,   194,   197,   200,   203,   207,   210,
     214,   218,   222,   225,   228,   230,   232,   234,   239,   242,
     245,   249,   253,   255,   257,   259,   264,   268,   274,   277
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
  "HALT", "INTERRUPT", "INTERRUPT_RETURN", "CALL", "RETURN", "JUMP",
  "BRANCH_EQUAL", "BRANCH_notEQUAL", "BRANCH_GREATER", "PUSH", "POP",
  "EXCHANGE", "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "NOT", "AND", "OR",
  "XOR", "SHIFT_LEFT", "SHIFT_RIGHT", "LOAD", "STORE", "CSRRD", "CSRWR",
  "NEWLINE", "COMMENT", "STRING", "SYMBOL", "INTEGER", "REGISTER",
  "SYSTEM_REGISTER", "PLUS", "MINUS", "STAR", "SLASH", "COLON", "DOLLAR",
  "LBRACKET", "RBRACKET", "COMMA", "CATCH_ERROR", "$accept", "line",
  "instruction", "directive", "terminate", "symblist", "vallist", "label",
  "global", "section", "word", "skip", "ascii", "equ", "end", "halt",
  "int", "call", "jmp", "beq", "bne", "bgt", "xchg", "add", "sub", "mul",
  "div", "not", "and", "or", "xor", "shl", "shr", "push", "pop", "st",
  "ld", "ret", "iret", "csrrd", "csrwr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-36)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       3,   -35,     0,    10,     1,    17,    18,    42,    42,    42,
      42,    20,    42,    23,    41,    51,    52,    53,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       4,    67,    45,   101,   -36,    97,   145,   -36,   -36,   -36,
     107,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,    95,    42,    42,    96,    98,    42,
      42,    42,    99,   -36,   -36,   -36,   -36,    42,    42,   -36,
      42,    42,   100,   102,   103,    42,    42,   105,   106,   121,
     122,   123,    42,   124,   128,   131,   132,   133,   134,   135,
     136,    25,   111,   137,   138,   139,    42,   -36,   -36,   -36,
     -35,   -36,   -36,    10,    10,   -36,   -36,   -36,   140,   -36,
     -36,   -36,   -36,   117,   151,   152,   -36,   -36,   153,   154,
     155,   156,   157,   -36,   160,   161,   162,   163,   164,   165,
     166,   167,   158,   168,     8,     7,   175,   176,   -36,   -36,
     -36,   -36,    42,   170,   172,   173,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,   177,
     184,    27,   178,    42,    42,    42,   185,    42,    42,   -36,
      31,    36,    38,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,    42,    42,   180,   181,
     186,   -36,   -36,   -36,    11,   -36,   -36,    42,    42,    42,
      42,    42,    42,   -36,   -36,   182,   183,    42,    46,    42,
     -36,   -36,   -36,   -36,   -36,   -36,   187,   191,   -36,   189,
     190,   -36,    42,    42,    42,    42,   -36,   -36,   -36,   -36
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      42,     0,     0,     0,     0,     0,     0,    42,    42,    42,
      42,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    41,     0,     0,     3,     5,     7,
       2,    34,    35,    36,    37,    38,    39,    40,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    28,    27,    29,
      30,    31,    32,    33,    43,    42,    42,    45,    47,    42,
      42,    42,     0,    56,    57,    58,    97,    42,    42,    96,
      42,    42,     0,     0,     0,    42,    42,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,     1,     4,     6,
       0,    50,    51,     0,     0,    52,    53,    54,     0,    59,
      60,    61,    62,     0,     0,     0,    80,    81,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    44,
      46,    48,    42,     0,     0,     0,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,     0,
       0,     0,     0,    42,    42,    42,     0,    42,    42,    55,
       0,     0,     0,    69,    70,    71,    72,    73,    75,    76,
      77,    78,    79,    91,    90,    92,    42,    42,     0,     0,
       0,    83,    82,    84,     0,    98,    99,    42,    42,    42,
      42,    42,    42,    89,    88,     0,     0,    42,     0,    42,
      63,    64,    65,    66,    67,    68,     0,     0,    93,     0,
       0,    85,    42,    42,    42,    42,    95,    94,    87,    86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -36,   -36,   108,   201,    -7,   125,   -33,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,
     -36
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    36,    37,    38,    39,    75,    79,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      83,    84,    85,    86,    74,    89,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    76,
      34,    80,    35,   108,   109,   110,   183,   184,   185,    77,
      78,   181,   111,   112,   228,    81,   186,    82,   182,    87,
      88,   229,    90,    91,   152,   153,   208,   209,   121,   122,
     217,   218,   125,   126,   127,   219,   220,   221,   222,    34,
     129,   130,    92,   131,   132,   239,   240,   114,   136,   137,
     160,   161,    93,    94,    95,   143,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   113,   158,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,   115,   116,   117,   120,   123,   118,   124,
     128,   133,   154,   134,   135,   189,   138,   139,   163,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   140,   141,   142,   144,   211,   212,   213,   145,
     215,   216,   146,   147,   148,   149,   150,   151,   155,   156,
     157,   162,   164,   165,   166,   167,   168,   169,   170,   223,
     224,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     230,   231,   232,   233,   234,   235,   187,   188,   206,   180,
     238,   190,   241,   191,   192,   207,   214,   227,   242,   210,
     225,   226,   243,   236,   237,   246,   247,   248,   249,   244,
     245,   119,     0,     0,     0,   159
};

static const yytype_int16 yycheck[] =
{
       7,     8,     9,    10,    39,    12,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    39,
      37,    40,    39,    39,    40,    41,    39,    40,    41,    39,
      40,    43,    48,    49,    43,    38,    49,    39,    50,    39,
      40,    50,    39,    40,    39,    40,    39,    40,    75,    76,
      39,    40,    79,    80,    81,    39,    40,    39,    40,    37,
      87,    88,    41,    90,    91,    39,    40,    42,    95,    96,
     123,   124,    41,    41,    41,   102,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,   116,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    42,    47,     0,    51,    51,    40,    51,
      51,    51,    41,    51,    51,   162,    51,    51,    41,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    51,    51,    51,    51,   183,   184,   185,    51,
     187,   188,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    41,    41,    41,    41,    41,    41,    41,   206,
     207,    41,    41,    41,    41,    41,    41,    41,    41,    51,
     217,   218,   219,   220,   221,   222,    41,    41,    41,    51,
     227,    51,   229,    51,    51,    41,    41,    41,    41,    51,
      50,    50,    41,    51,    51,   242,   243,   244,   245,    50,
      50,    40,    -1,    -1,    -1,   120
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    37,    39,    54,    55,    56,    57,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    39,    58,    39,    39,    40,    59,
      40,    38,    39,    57,    57,    57,    57,    39,    40,    57,
      39,    40,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    39,    40,
      41,    48,    49,    41,    42,    42,    47,     0,    55,    56,
      51,    57,    57,    51,    51,    57,    57,    57,    51,    57,
      57,    57,    57,    51,    51,    51,    57,    57,    51,    51,
      51,    51,    51,    57,    51,    51,    51,    51,    51,    51,
      51,    51,    39,    40,    41,    51,    51,    51,    57,    58,
      59,    59,    51,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    51,
      51,    43,    50,    39,    40,    41,    49,    41,    41,    57,
      51,    51,    51,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    41,    41,    39,    40,
      51,    57,    57,    57,    41,    57,    57,    39,    40,    39,
      40,    39,    40,    57,    57,    50,    50,    41,    43,    50,
      57,    57,    57,    57,    57,    57,    51,    51,    57,    39,
      40,    57,    41,    41,    50,    50,    57,    57,    57,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    54,    54,    54,    54,    54,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    56,    56,    56,    56,    56,    56,
      56,    57,    57,    58,    58,    59,    59,    59,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    74,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    88,    88,    88,    88,    88,    89,    89,
      89,    89,    89,    89,    89,    89,    90,    91,    92,    93
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     3,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     5,     2,     2,     2,     3,
       3,     3,     3,     7,     7,     7,     7,     7,     7,     5,
       5,     5,     5,     5,     3,     5,     5,     5,     5,     5,
       3,     3,     5,     5,     5,     7,     9,     9,     6,     6,
       5,     5,     5,     7,     9,     9,     2,     2,     5,     5
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
  case 43: /* symblist: SYMBOL  */
#line 70 "misc/parser.y"
                 {
    symblist.push_back((yyvsp[0].str));
}
#line 1363 "src/parser.cpp"
    break;

  case 44: /* symblist: SYMBOL COMMA symblist  */
#line 72 "misc/parser.y"
                          {
    symblist.push_back((yyvsp[-2].str));
}
#line 1371 "src/parser.cpp"
    break;

  case 45: /* vallist: SYMBOL  */
#line 76 "misc/parser.y"
                {
    vallist.push_back({(yyvsp[0].str), valtype::SYM});
 }
#line 1379 "src/parser.cpp"
    break;

  case 46: /* vallist: SYMBOL COMMA vallist  */
#line 78 "misc/parser.y"
                          {
    vallist.push_back({(yyvsp[-2].str), valtype::SYM});
  }
#line 1387 "src/parser.cpp"
    break;

  case 47: /* vallist: INTEGER  */
#line 80 "misc/parser.y"
              {
    vallist.push_back({(yyvsp[0].str), valtype::INT});
  }
#line 1395 "src/parser.cpp"
    break;

  case 48: /* vallist: INTEGER COMMA vallist  */
#line 82 "misc/parser.y"
                            {
    vallist.push_back({(yyvsp[-2].str), valtype::INT});
  }
#line 1403 "src/parser.cpp"
    break;

  case 49: /* label: SYMBOL COLON terminate  */
#line 87 "misc/parser.y"
                              {
    SymTab::add_definition((yyvsp[-2].str), section->getName(), section->getSize());
}
#line 1411 "src/parser.cpp"
    break;

  case 50: /* global: dotGLOBAL symblist terminate  */
#line 91 "misc/parser.y"
                                     {
    for (auto& symb : symblist)
        SymTab::globals.insert(symb);
    symblist.clear();
}
#line 1421 "src/parser.cpp"
    break;

  case 51: /* section: dotSECTION SYMBOL terminate  */
#line 97 "misc/parser.y"
                                     {
    section = Section::extract((yyvsp[-1].str));
}
#line 1429 "src/parser.cpp"
    break;

  case 52: /* word: dotWORD vallist terminate  */
#line 101 "misc/parser.y"
                                {
    for (auto it = vallist.rbegin(); it != vallist.rend(); ++it) {
        if (it->second == valtype::INT) {
            section->insertInt(toInt(it->first));
        } else if (it->second == valtype::SYM) {
            section->insertInt(-1);
            SymTab::add_occurrence(it->first, section->getName(), section->getSize());
        }
    }
    vallist.clear();
}
#line 1445 "src/parser.cpp"
    break;

  case 53: /* skip: dotSKIP INTEGER terminate  */
#line 113 "misc/parser.y"
                                {
    for (int i = 0; i < toInt((yyvsp[-1].str)); i++) {
        section->insertByte(0);
    }
}
#line 1455 "src/parser.cpp"
    break;

  case 54: /* ascii: dotASCII STRING terminate  */
#line 119 "misc/parser.y"
                                 {

}
#line 1463 "src/parser.cpp"
    break;

  case 55: /* equ: dotEQU SYMBOL COMMA COMMA terminate  */
#line 123 "misc/parser.y"
                                         {

}
#line 1471 "src/parser.cpp"
    break;

  case 56: /* end: dotEND terminate  */
#line 127 "misc/parser.y"
                      {
    YYABORT;
}
#line 1479 "src/parser.cpp"
    break;

  case 57: /* halt: HALT terminate  */
#line 132 "misc/parser.y"
                     {
    section->add_instruction();
}
#line 1487 "src/parser.cpp"
    break;

  case 58: /* int: INTERRUPT terminate  */
#line 136 "misc/parser.y"
                         {
    section->add_instruction(0b0001);
}
#line 1495 "src/parser.cpp"
    break;

  case 59: /* call: CALL SYMBOL terminate  */
#line 140 "misc/parser.y"
                            {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0010, 0b0001, 15);
}
#line 1504 "src/parser.cpp"
    break;

  case 60: /* call: CALL INTEGER terminate  */
#line 143 "misc/parser.y"
                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0010, 0b0001, 15);
}
#line 1513 "src/parser.cpp"
    break;

  case 61: /* jmp: JUMP SYMBOL terminate  */
#line 147 "misc/parser.y"
                           {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1522 "src/parser.cpp"
    break;

  case 62: /* jmp: JUMP INTEGER terminate  */
#line 150 "misc/parser.y"
                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1000, 15);
}
#line 1531 "src/parser.cpp"
    break;

  case 63: /* beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 155 "misc/parser.y"
                                                                 {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1001, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1540 "src/parser.cpp"
    break;

  case 64: /* beq: BRANCH_EQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 158 "misc/parser.y"
                                                                 {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1001, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1549 "src/parser.cpp"
    break;

  case 65: /* bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 162 "misc/parser.y"
                                                                    {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1010, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1558 "src/parser.cpp"
    break;

  case 66: /* bne: BRANCH_notEQUAL REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 165 "misc/parser.y"
                                                                    {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1010, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1567 "src/parser.cpp"
    break;

  case 67: /* bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA SYMBOL terminate  */
#line 169 "misc/parser.y"
                                                                   {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b0011, 0b1011, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1576 "src/parser.cpp"
    break;

  case 68: /* bgt: BRANCH_GREATER REGISTER COMMA REGISTER COMMA INTEGER terminate  */
#line 172 "misc/parser.y"
                                                                   {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b0011, 0b1011, 15, regs[(yyvsp[-5].str)], regs[(yyvsp[-3].str)]);
}
#line 1585 "src/parser.cpp"
    break;

  case 69: /* xchg: EXCHANGE REGISTER COMMA REGISTER terminate  */
#line 177 "misc/parser.y"
                                                 {
    section->add_instruction(0b0100, 0b0000, 0, regs[(yyvsp[-3].str)], regs[(yyvsp[-1].str)]);
}
#line 1593 "src/parser.cpp"
    break;

  case 70: /* add: ADD REGISTER COMMA REGISTER terminate  */
#line 181 "misc/parser.y"
                                           {
    section->add_instruction(0b0101, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1601 "src/parser.cpp"
    break;

  case 71: /* sub: SUBTRACT REGISTER COMMA REGISTER terminate  */
#line 184 "misc/parser.y"
                                                {
    section->add_instruction(0b0101, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1609 "src/parser.cpp"
    break;

  case 72: /* mul: MULTIPLY REGISTER COMMA REGISTER terminate  */
#line 187 "misc/parser.y"
                                                {
    section->add_instruction(0b0101, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1617 "src/parser.cpp"
    break;

  case 73: /* div: DIVIDE REGISTER COMMA REGISTER terminate  */
#line 190 "misc/parser.y"
                                              {
    section->add_instruction(0b0101, 0b0011, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1625 "src/parser.cpp"
    break;

  case 74: /* not: NOT REGISTER terminate  */
#line 194 "misc/parser.y"
                            {
    section->add_instruction(0b0110, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1633 "src/parser.cpp"
    break;

  case 75: /* and: AND REGISTER COMMA REGISTER terminate  */
#line 197 "misc/parser.y"
                                           {
    section->add_instruction(0b0110, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1641 "src/parser.cpp"
    break;

  case 76: /* or: OR REGISTER COMMA REGISTER terminate  */
#line 200 "misc/parser.y"
                                         {
    section->add_instruction(0b0110, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1649 "src/parser.cpp"
    break;

  case 77: /* xor: XOR REGISTER COMMA REGISTER terminate  */
#line 203 "misc/parser.y"
                                           {
    section->add_instruction(0b0110, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1657 "src/parser.cpp"
    break;

  case 78: /* shl: SHIFT_LEFT REGISTER COMMA REGISTER terminate  */
#line 207 "misc/parser.y"
                                                  {
    section->add_instruction(0b0111, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1665 "src/parser.cpp"
    break;

  case 79: /* shr: SHIFT_RIGHT REGISTER COMMA REGISTER terminate  */
#line 210 "misc/parser.y"
                                                   {
    section->add_instruction(0b0111, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1673 "src/parser.cpp"
    break;

  case 80: /* push: PUSH REGISTER terminate  */
#line 214 "misc/parser.y"
                              {
    section->add_instruction(0b1000, 0b0001, 14, 0, regs[(yyvsp[-1].str)], -4);
}
#line 1681 "src/parser.cpp"
    break;

  case 81: /* pop: POP REGISTER terminate  */
#line 218 "misc/parser.y"
                            {
    section->add_instruction(0b1001, 0b0011, regs[(yyvsp[-1].str)], 14, 0, 4);
}
#line 1689 "src/parser.cpp"
    break;

  case 82: /* st: STORE REGISTER COMMA INTEGER terminate  */
#line 222 "misc/parser.y"
                                           {
    section->add_literal(toInt((yyvsp[-1].str)));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[(yyvsp[-3].str)]);
}
#line 1698 "src/parser.cpp"
    break;

  case 83: /* st: STORE REGISTER COMMA SYMBOL terminate  */
#line 225 "misc/parser.y"
                                          {
    section->add_literal((yyvsp[-1].str));
    section->add_instruction(0b1000, 0b0010, 15, 0, regs[(yyvsp[-3].str)]);
}
#line 1707 "src/parser.cpp"
    break;

  case 84: /* st: STORE REGISTER COMMA REGISTER terminate  */
#line 228 "misc/parser.y"
                                            {
    section->add_instruction(0b1001, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1715 "src/parser.cpp"
    break;

  case 85: /* st: STORE REGISTER COMMA LBRACKET REGISTER RBRACKET terminate  */
#line 230 "misc/parser.y"
                                                              {
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-2].str)], 0, regs[(yyvsp[-5].str)]);
}
#line 1723 "src/parser.cpp"
    break;

  case 86: /* st: STORE REGISTER COMMA LBRACKET REGISTER PLUS INTEGER RBRACKET terminate  */
#line 232 "misc/parser.y"
                                                                           {
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-4].str)], 0, regs[(yyvsp[-7].str)], toInt((yyvsp[-2].str)));
}
#line 1731 "src/parser.cpp"
    break;

  case 87: /* st: STORE REGISTER COMMA LBRACKET REGISTER PLUS SYMBOL RBRACKET terminate  */
#line 234 "misc/parser.y"
                                                                          {
    section->add_literal((yyvsp[-2].str));
    section->add_instruction(0b1000, 0b0000, regs[(yyvsp[-4].str)], 0, regs[(yyvsp[-7].str)]);
}
#line 1740 "src/parser.cpp"
    break;

  case 88: /* ld: LOAD DOLLAR INTEGER COMMA REGISTER terminate  */
#line 239 "misc/parser.y"
                                                 {
    section->add_literal(toInt((yyvsp[-3].str)));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 15);
}
#line 1749 "src/parser.cpp"
    break;

  case 89: /* ld: LOAD DOLLAR SYMBOL COMMA REGISTER terminate  */
#line 242 "misc/parser.y"
                                                {
    section->add_literal((yyvsp[-3].str));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 15);
}
#line 1758 "src/parser.cpp"
    break;

  case 90: /* ld: LOAD INTEGER COMMA REGISTER terminate  */
#line 245 "misc/parser.y"
                                          {
    section->add_literal(toInt((yyvsp[-3].str)));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1768 "src/parser.cpp"
    break;

  case 91: /* ld: LOAD SYMBOL COMMA REGISTER terminate  */
#line 249 "misc/parser.y"
                                         {
    section->add_literal((yyvsp[-3].str));
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], 0b1111);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-1].str)]);
}
#line 1778 "src/parser.cpp"
    break;

  case 92: /* ld: LOAD REGISTER COMMA REGISTER terminate  */
#line 253 "misc/parser.y"
                                           {
    section->add_instruction(0b1001, 0b0001, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1786 "src/parser.cpp"
    break;

  case 93: /* ld: LOAD LBRACKET REGISTER RBRACKET COMMA REGISTER terminate  */
#line 255 "misc/parser.y"
                                                             {
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-4].str)]);
}
#line 1794 "src/parser.cpp"
    break;

  case 94: /* ld: LOAD LBRACKET REGISTER PLUS INTEGER RBRACKET COMMA REGISTER terminate  */
#line 257 "misc/parser.y"
                                                                          {
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-6].str)], toInt((yyvsp[-4].str)));
}
#line 1802 "src/parser.cpp"
    break;

  case 95: /* ld: LOAD LBRACKET REGISTER PLUS SYMBOL RBRACKET COMMA REGISTER terminate  */
#line 259 "misc/parser.y"
                                                                         {
    SymTab::add_occurrence((yyvsp[-4].str), section->getName(), section->getSize(), false);
    section->add_instruction(0b1001, 0b0010, regs[(yyvsp[-1].str)], regs[(yyvsp[-6].str)]);
}
#line 1811 "src/parser.cpp"
    break;

  case 96: /* ret: RETURN terminate  */
#line 264 "misc/parser.y"
                      {
    section->add_instruction(0b1001, 0b0011, 15, 14, 0, 4);
}
#line 1819 "src/parser.cpp"
    break;

  case 97: /* iret: INTERRUPT_RETURN terminate  */
#line 268 "misc/parser.y"
                                 {
    section->add_instruction(0b1001, 0b0100, 0b0010);
    section->add_instruction(0b1001, 0b0011, 0b0000, 0b1110, 0, 4);
    section->add_instruction(0b1001, 0b0011, 0b1111, 0b1110, 0, 4);
}
#line 1829 "src/parser.cpp"
    break;

  case 98: /* csrrd: CSRRD SYSTEM_REGISTER COMMA REGISTER terminate  */
#line 274 "misc/parser.y"
                                                      {
    section->add_instruction(0b1001, 0b0000, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1837 "src/parser.cpp"
    break;

  case 99: /* csrwr: CSRWR SYSTEM_REGISTER COMMA REGISTER terminate  */
#line 277 "misc/parser.y"
                                                      {
    section->add_instruction(0b1001, 0b0100, regs[(yyvsp[-1].str)], regs[(yyvsp[-3].str)]);
}
#line 1845 "src/parser.cpp"
    break;


#line 1849 "src/parser.cpp"

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

#line 281 "misc/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "ERROR\n");
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
