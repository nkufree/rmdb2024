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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"

#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;

#line 86 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"

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

#include "yacc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SHOW = 3,                       /* SHOW  */
  YYSYMBOL_TABLES = 4,                     /* TABLES  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_DROP = 7,                       /* DROP  */
  YYSYMBOL_DESC = 8,                       /* DESC  */
  YYSYMBOL_INSERT = 9,                     /* INSERT  */
  YYSYMBOL_INTO = 10,                      /* INTO  */
  YYSYMBOL_VALUES = 11,                    /* VALUES  */
  YYSYMBOL_DELETE = 12,                    /* DELETE  */
  YYSYMBOL_FROM = 13,                      /* FROM  */
  YYSYMBOL_ASC = 14,                       /* ASC  */
  YYSYMBOL_ORDER = 15,                     /* ORDER  */
  YYSYMBOL_GROUP = 16,                     /* GROUP  */
  YYSYMBOL_BY = 17,                        /* BY  */
  YYSYMBOL_HAVING = 18,                    /* HAVING  */
  YYSYMBOL_WHERE = 19,                     /* WHERE  */
  YYSYMBOL_UPDATE = 20,                    /* UPDATE  */
  YYSYMBOL_SET = 21,                       /* SET  */
  YYSYMBOL_SELECT = 22,                    /* SELECT  */
  YYSYMBOL_MAX = 23,                       /* MAX  */
  YYSYMBOL_MIN = 24,                       /* MIN  */
  YYSYMBOL_SUM = 25,                       /* SUM  */
  YYSYMBOL_COUNT = 26,                     /* COUNT  */
  YYSYMBOL_AS = 27,                        /* AS  */
  YYSYMBOL_INT = 28,                       /* INT  */
  YYSYMBOL_CHAR = 29,                      /* CHAR  */
  YYSYMBOL_FLOAT = 30,                     /* FLOAT  */
  YYSYMBOL_INDEX = 31,                     /* INDEX  */
  YYSYMBOL_AND = 32,                       /* AND  */
  YYSYMBOL_JOIN = 33,                      /* JOIN  */
  YYSYMBOL_EXIT = 34,                      /* EXIT  */
  YYSYMBOL_HELP = 35,                      /* HELP  */
  YYSYMBOL_TXN_BEGIN = 36,                 /* TXN_BEGIN  */
  YYSYMBOL_TXN_COMMIT = 37,                /* TXN_COMMIT  */
  YYSYMBOL_TXN_ABORT = 38,                 /* TXN_ABORT  */
  YYSYMBOL_TXN_ROLLBACK = 39,              /* TXN_ROLLBACK  */
  YYSYMBOL_ORDER_BY = 40,                  /* ORDER_BY  */
  YYSYMBOL_ENABLE_NESTLOOP = 41,           /* ENABLE_NESTLOOP  */
  YYSYMBOL_ENABLE_SORTMERGE = 42,          /* ENABLE_SORTMERGE  */
  YYSYMBOL_ON = 43,                        /* ON  */
  YYSYMBOL_LEQ = 44,                       /* LEQ  */
  YYSYMBOL_NEQ = 45,                       /* NEQ  */
  YYSYMBOL_GEQ = 46,                       /* GEQ  */
  YYSYMBOL_T_EOF = 47,                     /* T_EOF  */
  YYSYMBOL_IDENTIFIER = 48,                /* IDENTIFIER  */
  YYSYMBOL_VALUE_STRING = 49,              /* VALUE_STRING  */
  YYSYMBOL_VALUE_INT = 50,                 /* VALUE_INT  */
  YYSYMBOL_VALUE_FLOAT = 51,               /* VALUE_FLOAT  */
  YYSYMBOL_VALUE_BOOL = 52,                /* VALUE_BOOL  */
  YYSYMBOL_53_ = 53,                       /* ';'  */
  YYSYMBOL_54_ = 54,                       /* '='  */
  YYSYMBOL_55_ = 55,                       /* '('  */
  YYSYMBOL_56_ = 56,                       /* ')'  */
  YYSYMBOL_57_ = 57,                       /* ','  */
  YYSYMBOL_58_ = 58,                       /* '.'  */
  YYSYMBOL_59_ = 59,                       /* '*'  */
  YYSYMBOL_60_ = 60,                       /* '<'  */
  YYSYMBOL_61_ = 61,                       /* '>'  */
  YYSYMBOL_YYACCEPT = 62,                  /* $accept  */
  YYSYMBOL_start = 63,                     /* start  */
  YYSYMBOL_stmt = 64,                      /* stmt  */
  YYSYMBOL_txnStmt = 65,                   /* txnStmt  */
  YYSYMBOL_dbStmt = 66,                    /* dbStmt  */
  YYSYMBOL_setStmt = 67,                   /* setStmt  */
  YYSYMBOL_ddl = 68,                       /* ddl  */
  YYSYMBOL_dml = 69,                       /* dml  */
  YYSYMBOL_oneValue = 70,                  /* oneValue  */
  YYSYMBOL_valuesList = 71,                /* valuesList  */
  YYSYMBOL_fieldList = 72,                 /* fieldList  */
  YYSYMBOL_colNameList = 73,               /* colNameList  */
  YYSYMBOL_field = 74,                     /* field  */
  YYSYMBOL_type = 75,                      /* type  */
  YYSYMBOL_valueList = 76,                 /* valueList  */
  YYSYMBOL_value = 77,                     /* value  */
  YYSYMBOL_condition = 78,                 /* condition  */
  YYSYMBOL_opOnClause = 79,                /* opOnClause  */
  YYSYMBOL_optWhereClause = 80,            /* optWhereClause  */
  YYSYMBOL_whereClause = 81,               /* whereClause  */
  YYSYMBOL_col = 82,                       /* col  */
  YYSYMBOL_colList = 83,                   /* colList  */
  YYSYMBOL_op = 84,                        /* op  */
  YYSYMBOL_expr = 85,                      /* expr  */
  YYSYMBOL_setClauses = 86,                /* setClauses  */
  YYSYMBOL_setClause = 87,                 /* setClause  */
  YYSYMBOL_selector = 88,                  /* selector  */
  YYSYMBOL_tableList = 89,                 /* tableList  */
  YYSYMBOL_opt_order_clause = 90,          /* opt_order_clause  */
  YYSYMBOL_opt_group_clause = 91,          /* opt_group_clause  */
  YYSYMBOL_order_clause = 92,              /* order_clause  */
  YYSYMBOL_opt_asc_desc = 93,              /* opt_asc_desc  */
  YYSYMBOL_opt_aggregate = 94,             /* opt_aggregate  */
  YYSYMBOL_set_knob_type = 95,             /* set_knob_type  */
  YYSYMBOL_tbName = 96,                    /* tbName  */
  YYSYMBOL_colName = 97,                   /* colName  */
  YYSYMBOL_alias = 98                      /* alias  */
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
<<<<<<< HEAD
#define YYFINAL  45
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   129
=======
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   163
>>>>>>> q4

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
<<<<<<< HEAD
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  145
=======
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  177
>>>>>>> q4

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
      55,    56,    59,     2,    57,     2,    58,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    53,
      60,    54,    61,     2,     2,     2,     2,     2,     2,     2,
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
<<<<<<< HEAD
       0,    59,    59,    64,    69,    74,    82,    83,    84,    85,
      86,    90,    94,    98,   102,   109,   113,   120,   127,   131,
     135,   139,   143,   150,   154,   158,   162,   169,   176,   180,
     187,   191,   198,   202,   209,   216,   220,   224,   231,   235,
     242,   246,   250,   254,   261,   269,   271,   278,   279,   286,
     290,   297,   301,   308,   312,   319,   323,   327,   331,   335,
     339,   346,   350,   357,   361,   368,   375,   379,   383,   387,
     391,   398,   402,   406,   413,   414,   415,   419,   420,   423,
     425
=======
       0,    61,    61,    66,    71,    76,    84,    85,    86,    87,
      88,    92,    96,   100,   104,   111,   118,   125,   129,   133,
     137,   141,   148,   152,   156,   160,   167,   174,   178,   185,
     189,   196,   200,   207,   214,   218,   222,   229,   233,   240,
     244,   248,   252,   259,   267,   269,   276,   277,   284,   288,
     296,   300,   305,   309,   313,   318,   323,   328,   333,   338,
     347,   351,   356,   360,   368,   372,   376,   380,   384,   388,
     395,   399,   406,   410,   417,   424,   428,   432,   436,   440,
     447,   451,   455,   460,   465,   469,   476,   477,   478,   482,
     483,   484,   485,   488,   489,   492,   494,   496
>>>>>>> q4
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SHOW", "TABLES",
  "CREATE", "TABLE", "DROP", "DESC", "INSERT", "INTO", "VALUES", "DELETE",
  "FROM", "ASC", "ORDER", "GROUP", "BY", "HAVING", "WHERE", "UPDATE",
  "SET", "SELECT", "MAX", "MIN", "SUM", "COUNT", "AS", "INT", "CHAR",
  "FLOAT", "INDEX", "AND", "JOIN", "EXIT", "HELP", "TXN_BEGIN",
  "TXN_COMMIT", "TXN_ABORT", "TXN_ROLLBACK", "ORDER_BY", "ENABLE_NESTLOOP",
  "ENABLE_SORTMERGE", "ON", "LEQ", "NEQ", "GEQ", "T_EOF", "IDENTIFIER",
  "VALUE_STRING", "VALUE_INT", "VALUE_FLOAT", "VALUE_BOOL", "';'", "'='",
  "'('", "')'", "','", "'.'", "'*'", "'<'", "'>'", "$accept", "start",
  "stmt", "txnStmt", "dbStmt", "setStmt", "ddl", "dml", "oneValue",
  "valuesList", "fieldList", "colNameList", "field", "type", "valueList",
  "value", "condition", "opOnClause", "optWhereClause", "whereClause",
  "col", "colList", "op", "expr", "setClauses", "setClause", "selector",
  "tableList", "opt_order_clause", "opt_group_clause", "order_clause",
  "opt_asc_desc", "opt_aggregate", "set_knob_type", "tbName", "colName",
  "alias", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

<<<<<<< HEAD
#define YYPACT_NINF (-87)
=======
#define YYPACT_NINF (-102)
>>>>>>> q4

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

<<<<<<< HEAD
#define YYTABLE_NINF (-80)
=======
#define YYTABLE_NINF (-96)
>>>>>>> q4

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
<<<<<<< HEAD
      43,    10,    13,    15,   -28,     8,     7,   -28,     6,   -26,
     -87,   -87,   -87,   -87,   -87,   -87,   -87,    16,   -19,   -87,
     -87,   -87,   -87,   -87,   -87,    19,   -28,   -28,   -28,   -28,
     -87,   -87,   -28,   -28,    26,   -87,   -87,    17,     2,   -87,
     -87,    30,    47,    18,   -87,   -87,   -87,   -28,    38,    40,
     -87,    53,    54,    85,    62,    59,    64,   -28,    62,   -87,
      62,    62,    62,    58,    64,   -87,   -87,   -12,   -87,    60,
     -87,   -87,   -14,   -87,   -87,    35,   -87,    56,    46,   -87,
      48,    14,   -87,    61,   -87,    84,    29,    62,   -87,    14,
     -28,    64,   -28,    85,   -87,    62,   -87,    65,   -87,   -87,
     -87,    62,   -87,   -87,   -87,   -87,   -87,    50,   -87,    58,
      64,   -87,   -87,   -87,   -87,   -87,   -87,    49,   -87,   -87,
     -87,    84,   -87,    95,   -87,    69,   -87,   -87,    14,   -87,
     -87,   -87,   -87,   -87,    98,   -87,    66,   -87,    64,   -87,
       3,   -87,   -87,   -87,   -87
=======
      74,     4,    16,    22,     2,    29,    46,     2,   -21,    -7,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,    62,    12,  -102,
    -102,  -102,  -102,  -102,  -102,     2,     2,     2,     2,  -102,
    -102,     2,     2,    50,  -102,  -102,    34,  -102,  -102,  -102,
    -102,    14,  -102,    73,    36,    91,    51,    47,    80,  -102,
    -102,    64,    65,  -102,    67,   112,   109,    82,    77,    83,
       9,     2,     7,    82,    83,    82,    82,    82,    78,     9,
    -102,  -102,    -8,  -102,    84,  -102,  -102,  -102,   105,    30,
    -102,    87,    76,    88,   110,  -102,   -20,  -102,    69,   -14,
    -102,    19,    18,  -102,    79,  -102,   107,    81,    82,  -102,
      18,    83,     2,     9,     2,   109,   113,    82,   118,    83,
    -102,    82,  -102,    92,  -102,  -102,  -102,    82,  -102,  -102,
    -102,  -102,  -102,    28,  -102,    78,     9,  -102,  -102,  -102,
    -102,  -102,  -102,    66,  -102,  -102,  -102,  -102,   107,  -102,
     131,    83,    93,    83,  -102,  -102,    98,  -102,  -102,    18,
    -102,  -102,  -102,  -102,  -102,   133,   135,  -102,   125,  -102,
      99,  -102,     9,   137,  -102,    83,  -102,    15,  -102,     9,
    -102,  -102,  -102,  -102,   -13,     9,   107
>>>>>>> q4
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     3,    11,    12,    13,    14,     5,     0,     0,     9,
<<<<<<< HEAD
       6,    10,     7,     8,    15,     0,     0,     0,     0,     0,
      79,    20,     0,     0,     0,    77,    78,     0,    80,    66,
      53,    67,     0,     0,    52,     1,     2,     0,     0,     0,
      19,     0,     0,    47,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,    24,    80,    47,    63,     0,
      17,    54,    45,    68,    51,     0,    30,     0,     0,    32,
       0,     0,    28,    23,    49,    48,     0,     0,    25,     0,
       0,     0,     0,    47,    18,     0,    35,     0,    37,    34,
      21,     0,    22,    42,    40,    41,    43,     0,    38,     0,
       0,    59,    58,    60,    55,    56,    57,     0,    64,    65,
      70,    46,    69,    72,    31,     0,    33,    27,     0,    29,
      50,    61,    62,    44,     0,    26,     0,    39,     0,    36,
      76,    71,    75,    74,    73
=======
       6,    10,     7,     8,    15,     0,     0,     0,     0,    95,
      19,     0,     0,     0,    93,    94,     0,    89,    90,    91,
      92,    96,    75,    60,    76,     0,     0,     0,    51,     1,
       2,     0,     0,    18,     0,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    96,    46,    72,     0,    16,    97,    61,    62,    44,
      77,     0,     0,     0,    50,    53,     0,    29,     0,     0,
      31,     0,     0,    27,    22,    48,    47,     0,     0,    24,
       0,     0,     0,     0,     0,    46,    57,     0,    56,     0,
      17,     0,    34,     0,    36,    33,    20,     0,    21,    41,
      39,    40,    42,     0,    37,     0,     0,    68,    67,    69,
      64,    65,    66,     0,    73,    74,    63,    79,    45,    78,
      81,     0,     0,     0,    52,    30,     0,    32,    26,     0,
      28,    49,    70,    71,    43,     0,    84,    59,    54,    58,
       0,    38,     0,     0,    25,     0,    35,    88,    80,     0,
      55,    87,    86,    85,    82,     0,    83
>>>>>>> q4
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
<<<<<<< HEAD
     -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,     9,   -87,
     -87,    55,    21,   -87,   -87,   -86,    11,   -87,   -60,    28,
      -9,   -87,   -87,   -87,   -87,    33,   -87,   -87,   -87,   -87,
     -87,   -87,    -3,   -52
=======
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,    31,  -102,
    -102,    90,    48,  -102,  -102,   -93,    32,  -102,   -57,  -101,
      -9,    -6,  -102,  -102,  -102,    63,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,    -1,   -53,   -63
>>>>>>> q4
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
<<<<<<< HEAD
       0,    17,    18,    19,    20,    21,    22,    23,    82,    83,
      75,    78,    76,    99,   107,   108,    84,    93,    65,    85,
      86,    41,   117,   133,    67,    68,    42,    72,   135,   141,
     144,    37,    43,    44
=======
       0,    17,    18,    19,    20,    21,    22,    23,    93,    94,
      86,    89,    87,   115,   123,   124,    95,   105,    70,    96,
      97,    44,   133,   154,    72,    73,    45,    79,   156,   164,
     168,   173,    46,    36,    47,    48,    77
>>>>>>> q4
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
<<<<<<< HEAD
      40,    31,    69,   119,    34,    64,    74,    88,    77,    79,
      79,   142,    90,    30,    24,    38,    45,   143,    32,    26,
      33,    28,    91,    48,    49,    50,    51,    46,    39,    52,
      53,   131,    47,   123,    25,    69,    92,    27,    87,    29,
      35,    36,   137,    77,    59,    54,     1,    71,     2,   126,
       3,     4,     5,   -79,    73,     6,   103,   104,   105,   106,
      57,     7,     8,     9,    55,    63,   111,   112,   113,    58,
      10,    11,    12,    13,    14,    15,   114,    96,    97,    98,
      56,   115,   116,    16,    94,    95,    60,   120,    61,   122,
      38,   103,   104,   105,   106,   100,   101,   102,   101,   127,
     128,    62,    64,    66,    70,    38,    81,    89,   132,   110,
     134,   109,   136,   125,   138,   139,   124,    80,   129,   121,
     118,   130,     0,     0,     0,     0,     0,     0,     0,   140
=======
      43,    85,   138,    30,    74,   175,    33,   135,    24,    83,
      84,    69,    88,    90,    90,    99,    37,    38,    39,    40,
      34,    35,    25,   171,    51,    52,    53,    54,    27,   172,
      55,    56,    37,    38,    39,    40,   110,   111,   136,    31,
     152,    41,   116,   117,    60,    74,   144,    26,   140,    98,
      29,    78,    42,    28,   142,    41,   161,    41,    88,    32,
      80,    82,    49,   102,   147,    50,    81,   119,   120,   121,
     122,    57,   -95,   103,   176,   118,   117,     1,   157,     2,
     159,     3,     4,     5,   148,   149,     6,   104,    58,    37,
      38,    39,    40,    60,     7,     8,     9,   112,   113,   114,
      59,   137,   170,   139,    61,    63,    62,    64,    10,    11,
      12,    13,    14,    15,    41,   119,   120,   121,   122,    65,
      66,    16,    67,    68,   153,   127,   128,   129,    69,    75,
      71,    76,   101,    92,   107,   130,   125,   109,   100,   126,
     141,   131,   132,   106,   108,   143,   155,   146,   160,   158,
     162,   163,   165,   167,   169,   166,   150,    91,   151,   145,
      43,   134,     0,   174
>>>>>>> q4
};

static const yytype_int16 yycheck[] =
{
<<<<<<< HEAD
       9,     4,    54,    89,     7,    17,    58,    67,    60,    61,
      62,     8,    26,    41,     4,    41,     0,    14,    10,     6,
      13,     6,    36,    26,    27,    28,    29,    46,    54,    32,
      33,   117,    13,    93,    24,    87,    50,    24,    50,    24,
      34,    35,   128,    95,    47,    19,     3,    56,     5,   101,
       7,     8,     9,    51,    57,    12,    42,    43,    44,    45,
      13,    18,    19,    20,    47,    11,    37,    38,    39,    51,
      27,    28,    29,    30,    31,    32,    47,    21,    22,    23,
      50,    52,    53,    40,    49,    50,    48,    90,    48,    92,
      41,    42,    43,    44,    45,    49,    50,    49,    50,    49,
      50,    48,    17,    41,    45,    41,    48,    47,   117,    25,
      15,    50,    43,    48,    16,    49,    95,    62,   109,    91,
      87,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   138
=======
       9,    64,   103,     4,    57,    18,     7,   100,     4,    62,
      63,    19,    65,    66,    67,    72,    23,    24,    25,    26,
      41,    42,     6,     8,    25,    26,    27,    28,     6,    14,
      31,    32,    23,    24,    25,    26,    56,    57,   101,    10,
     133,    48,    56,    57,    57,    98,   109,    31,   105,    57,
      48,    60,    59,    31,   107,    48,   149,    48,   111,    13,
      61,    62,     0,    33,   117,    53,    59,    49,    50,    51,
      52,    21,    58,    43,   175,    56,    57,     3,   141,     5,
     143,     7,     8,     9,    56,    57,    12,    57,    54,    23,
      24,    25,    26,    57,    20,    21,    22,    28,    29,    30,
      27,   102,   165,   104,    13,    58,    55,    27,    34,    35,
      36,    37,    38,    39,    48,    49,    50,    51,    52,    55,
      55,    47,    55,    11,   133,    44,    45,    46,    19,    52,
      48,    48,    27,    55,    58,    54,    57,    27,    54,    32,
      27,    60,    61,    56,    56,    27,    15,    55,    50,    56,
      17,    16,    27,   162,    17,    56,   125,    67,   126,   111,
     169,    98,    -1,   169
>>>>>>> q4
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
<<<<<<< HEAD
       0,     3,     5,     7,     8,     9,    12,    18,    19,    20,
      27,    28,    29,    30,    31,    32,    40,    56,    57,    58,
      59,    60,    61,    62,     4,    24,     6,    24,     6,    24,
      41,    87,    10,    13,    87,    34,    35,    86,    41,    54,
      75,    76,    81,    87,    88,     0,    46,    13,    87,    87,
      87,    87,    87,    87,    19,    47,    50,    13,    51,    87,
      48,    48,    48,    11,    17,    73,    41,    79,    80,    88,
      45,    75,    82,    87,    88,    65,    67,    88,    66,    88,
      66,    48,    63,    64,    71,    74,    75,    50,    73,    47,
      26,    36,    50,    72,    49,    50,    21,    22,    23,    68,
      49,    50,    49,    42,    43,    44,    45,    69,    70,    50,
      25,    37,    38,    39,    47,    52,    53,    77,    80,    70,
      87,    74,    87,    73,    67,    48,    88,    49,    50,    63,
      71,    70,    75,    78,    15,    83,    43,    70,    16,    49,
      75,    84,     8,    14,    85
=======
       0,     3,     5,     7,     8,     9,    12,    20,    21,    22,
      34,    35,    36,    37,    38,    39,    47,    63,    64,    65,
      66,    67,    68,    69,     4,     6,    31,     6,    31,    48,
      96,    10,    13,    96,    41,    42,    95,    23,    24,    25,
      26,    48,    59,    82,    83,    88,    94,    96,    97,     0,
      53,    96,    96,    96,    96,    96,    96,    21,    54,    27,
      57,    13,    55,    58,    27,    55,    55,    55,    11,    19,
      80,    48,    86,    87,    97,    52,    48,    98,    82,    89,
      96,    59,    96,    97,    97,    98,    72,    74,    97,    73,
      97,    73,    55,    70,    71,    78,    81,    82,    57,    80,
      54,    27,    33,    43,    57,    79,    56,    58,    56,    27,
      56,    57,    28,    29,    30,    75,    56,    57,    56,    49,
      50,    51,    52,    76,    77,    57,    32,    44,    45,    46,
      54,    60,    61,    84,    87,    77,    98,    96,    81,    96,
      80,    27,    97,    27,    98,    74,    55,    97,    56,    57,
      70,    78,    77,    82,    85,    15,    90,    98,    56,    98,
      50,    77,    17,    16,    91,    27,    56,    82,    92,    17,
      98,     8,    14,    93,    83,    18,    81
>>>>>>> q4
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
<<<<<<< HEAD
       0,    55,    56,    56,    56,    56,    57,    57,    57,    57,
      57,    58,    58,    58,    58,    59,    59,    60,    61,    61,
      61,    61,    61,    62,    62,    62,    62,    63,    64,    64,
      65,    65,    66,    66,    67,    68,    68,    68,    69,    69,
      70,    70,    70,    70,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    79,    80,    81,    81,    82,    82,
      82,    83,    83,    84,    85,    85,    85,    86,    86,    87,
      88
=======
       0,    62,    63,    63,    63,    63,    64,    64,    64,    64,
      64,    65,    65,    65,    65,    66,    67,    68,    68,    68,
      68,    68,    69,    69,    69,    69,    70,    71,    71,    72,
      72,    73,    73,    74,    75,    75,    75,    76,    76,    77,
      77,    77,    77,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      83,    83,    83,    83,    84,    84,    84,    84,    84,    84,
      85,    85,    86,    86,    87,    88,    88,    89,    89,    89,
      90,    90,    91,    91,    91,    92,    93,    93,    93,    94,
      94,    94,    94,    95,    95,    96,    97,    98
>>>>>>> q4
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
<<<<<<< HEAD
       1,     1,     1,     1,     1,     2,     4,     4,     6,     3,
       2,     6,     6,     5,     4,     5,     7,     3,     1,     3,
       1,     3,     1,     3,     2,     1,     4,     1,     1,     3,
       1,     1,     1,     1,     3,     0,     2,     0,     2,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     1,     1,     3,
       3,     3,     0,     2,     1,     1,     0,     1,     1,     1,
       1
=======
       1,     1,     1,     1,     1,     2,     4,     6,     3,     2,
       6,     6,     5,     4,     5,     8,     3,     1,     3,     1,
       3,     1,     3,     2,     1,     4,     1,     1,     3,     1,
       1,     1,     1,     3,     0,     2,     0,     2,     1,     3,
       3,     1,     5,     3,     6,     8,     4,     4,     6,     6,
       1,     3,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     1,     1,     1,     3,     3,
       3,     0,     3,     5,     0,     2,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1
>>>>>>> q4
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
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: stmt ';'  */
#line 62 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        parse_tree = (yyvsp[-1].sv_node);
        YYACCEPT;
    }
<<<<<<< HEAD
#line 1658 "yacc.tab.cpp"
=======
#line 1690 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 3: /* start: HELP  */
#line 67 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
<<<<<<< HEAD
#line 1667 "yacc.tab.cpp"
=======
#line 1699 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 4: /* start: EXIT  */
#line 72 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
<<<<<<< HEAD
#line 1676 "yacc.tab.cpp"
=======
#line 1708 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 5: /* start: T_EOF  */
#line 77 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
<<<<<<< HEAD
#line 1685 "yacc.tab.cpp"
=======
#line 1717 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 11: /* txnStmt: TXN_BEGIN  */
#line 93 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnBegin>();
    }
<<<<<<< HEAD
#line 1693 "yacc.tab.cpp"
=======
#line 1725 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 12: /* txnStmt: TXN_COMMIT  */
#line 97 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnCommit>();
    }
<<<<<<< HEAD
#line 1701 "yacc.tab.cpp"
=======
#line 1733 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 13: /* txnStmt: TXN_ABORT  */
#line 101 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnAbort>();
    }
<<<<<<< HEAD
#line 1709 "yacc.tab.cpp"
=======
#line 1741 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 14: /* txnStmt: TXN_ROLLBACK  */
#line 105 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnRollback>();
    }
<<<<<<< HEAD
#line 1717 "yacc.tab.cpp"
=======
#line 1749 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4
    break;

  case 15: /* dbStmt: SHOW TABLES  */
#line 112 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowTables>();
    }
<<<<<<< HEAD
#line 1725 "yacc.tab.cpp"
    break;

  case 16: /* dbStmt: SHOW INDEX FROM tbName  */
#line 114 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
    }
#line 1733 "yacc.tab.cpp"
    break;

  case 17: /* setStmt: SET set_knob_type '=' VALUE_BOOL  */
#line 121 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<SetStmt>((yyvsp[-2].sv_setKnobType), (yyvsp[0].sv_bool));
    }
#line 1741 "yacc.tab.cpp"
    break;

  case 18: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 128 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateTable>((yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
    }
#line 1749 "yacc.tab.cpp"
    break;

  case 19: /* ddl: DROP TABLE tbName  */
#line 132 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
    }
#line 1757 "yacc.tab.cpp"
    break;

  case 20: /* ddl: DESC tbName  */
#line 136 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
    }
#line 1765 "yacc.tab.cpp"
    break;

  case 21: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 140 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1773 "yacc.tab.cpp"
    break;

  case 22: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 144 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1781 "yacc.tab.cpp"
    break;

  case 23: /* dml: INSERT INTO tbName VALUES valuesList  */
#line 151 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-2].sv_str), (yyvsp[0].sv_vals_list));
    }
#line 1789 "yacc.tab.cpp"
    break;

  case 24: /* dml: DELETE FROM tbName optWhereClause  */
#line 155 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str), (yyvsp[0].sv_conds));
    }
#line 1797 "yacc.tab.cpp"
    break;

  case 25: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 159 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<UpdateStmt>((yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses), (yyvsp[0].sv_conds));
    }
#line 1805 "yacc.tab.cpp"
    break;

  case 26: /* dml: SELECT selector FROM tableList opOnClause optWhereClause opt_order_clause  */
#line 163 "yacc.y"
=======
#line 1757 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 16: /* setStmt: SET set_knob_type '=' VALUE_BOOL  */
#line 119 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<SetStmt>((yyvsp[-2].sv_setKnobType), (yyvsp[0].sv_bool));
    }
#line 1765 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 17: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 126 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateTable>((yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
    }
#line 1773 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 18: /* ddl: DROP TABLE tbName  */
#line 130 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
    }
#line 1781 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 19: /* ddl: DESC tbName  */
#line 134 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
    }
#line 1789 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 20: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 138 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1797 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 21: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 142 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1805 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 22: /* dml: INSERT INTO tbName VALUES valuesList  */
#line 149 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-2].sv_str), (yyvsp[0].sv_vals_list));
    }
#line 1813 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 23: /* dml: DELETE FROM tbName optWhereClause  */
#line 153 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str), (yyvsp[0].sv_conds));
    }
#line 1821 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 24: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 157 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<UpdateStmt>((yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses), (yyvsp[0].sv_conds));
    }
#line 1829 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 25: /* dml: SELECT selector FROM tableList opOnClause optWhereClause opt_order_clause opt_group_clause  */
#line 161 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
>>>>>>> q4
    {
        (yyval.sv_node) = std::make_shared<SelectStmt>((yyvsp[-6].sv_cols), (yyvsp[-4].sv_strs), (yyvsp[-3].sv_conds), (yyvsp[-2].sv_conds), (yyvsp[-1].sv_orderby), (yyvsp[0].sv_groupby));
    }
<<<<<<< HEAD
#line 1813 "yacc.tab.cpp"
    break;

  case 27: /* oneValue: '(' valueList ')'  */
#line 170 "yacc.y"
    {
        (yyval.sv_vals) = (yyvsp[-1].sv_vals);
    }
#line 1821 "yacc.tab.cpp"
    break;

  case 28: /* valuesList: oneValue  */
#line 177 "yacc.y"
    {
        (yyval.sv_vals_list) = std::vector<std::vector<std::shared_ptr<Value>>>{(yyvsp[0].sv_vals)};
    }
#line 1829 "yacc.tab.cpp"
    break;

  case 29: /* valuesList: valuesList ',' oneValue  */
#line 181 "yacc.y"
    {
        (yyval.sv_vals_list).push_back((yyvsp[0].sv_vals));
    }
#line 1837 "yacc.tab.cpp"
    break;

  case 30: /* fieldList: field  */
#line 188 "yacc.y"
    {
        (yyval.sv_fields) = std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
    }
#line 1845 "yacc.tab.cpp"
    break;

  case 31: /* fieldList: fieldList ',' field  */
#line 192 "yacc.y"
    {
        (yyval.sv_fields).push_back((yyvsp[0].sv_field));
    }
#line 1853 "yacc.tab.cpp"
    break;

  case 32: /* colNameList: colName  */
#line 199 "yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1861 "yacc.tab.cpp"
    break;

  case 33: /* colNameList: colNameList ',' colName  */
#line 203 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1869 "yacc.tab.cpp"
    break;

  case 34: /* field: colName type  */
#line 210 "yacc.y"
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str), (yyvsp[0].sv_type_len));
    }
#line 1877 "yacc.tab.cpp"
    break;

  case 35: /* type: INT  */
#line 217 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
#line 1885 "yacc.tab.cpp"
    break;

  case 36: /* type: CHAR '(' VALUE_INT ')'  */
#line 221 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1893 "yacc.tab.cpp"
    break;

  case 37: /* type: FLOAT  */
#line 225 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
#line 1901 "yacc.tab.cpp"
    break;

  case 38: /* valueList: value  */
#line 232 "yacc.y"
    {
        (yyval.sv_vals) = std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
    }
#line 1909 "yacc.tab.cpp"
    break;

  case 39: /* valueList: valueList ',' value  */
#line 236 "yacc.y"
    {
        (yyval.sv_vals).push_back((yyvsp[0].sv_val));
    }
#line 1917 "yacc.tab.cpp"
    break;

  case 40: /* value: VALUE_INT  */
#line 243 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
    }
#line 1925 "yacc.tab.cpp"
    break;

  case 41: /* value: VALUE_FLOAT  */
#line 247 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
    }
#line 1933 "yacc.tab.cpp"
    break;

  case 42: /* value: VALUE_STRING  */
#line 251 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
    }
#line 1941 "yacc.tab.cpp"
    break;

  case 43: /* value: VALUE_BOOL  */
#line 255 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<BoolLit>((yyvsp[0].sv_bool));
    }
#line 1949 "yacc.tab.cpp"
    break;

  case 44: /* condition: col op expr  */
#line 262 "yacc.y"
    {
        (yyval.sv_cond) = std::make_shared<BinaryExpr>((yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
    }
#line 1957 "yacc.tab.cpp"
    break;

  case 45: /* opOnClause: %empty  */
#line 269 "yacc.y"
                      { /* ignore*/ }
#line 1963 "yacc.tab.cpp"
    break;

  case 46: /* opOnClause: ON whereClause  */
#line 272 "yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 1971 "yacc.tab.cpp"
    break;

  case 47: /* optWhereClause: %empty  */
#line 278 "yacc.y"
                      { /* ignore*/ }
#line 1977 "yacc.tab.cpp"
    break;

  case 48: /* optWhereClause: WHERE whereClause  */
#line 280 "yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 1985 "yacc.tab.cpp"
    break;

  case 49: /* whereClause: condition  */
#line 287 "yacc.y"
    {
        (yyval.sv_conds) = std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
    }
#line 1993 "yacc.tab.cpp"
    break;

  case 50: /* whereClause: whereClause AND condition  */
#line 291 "yacc.y"
    {
        (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
    }
#line 2001 "yacc.tab.cpp"
    break;

  case 51: /* col: tbName '.' colName  */
#line 298 "yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2009 "yacc.tab.cpp"
    break;

  case 52: /* col: colName  */
#line 302 "yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
    }
#line 2017 "yacc.tab.cpp"
    break;

  case 53: /* colList: col  */
#line 309 "yacc.y"
    {
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
    }
#line 2025 "yacc.tab.cpp"
    break;

  case 54: /* colList: colList ',' col  */
#line 313 "yacc.y"
    {
        (yyval.sv_cols).push_back((yyvsp[0].sv_col));
    }
#line 2033 "yacc.tab.cpp"
    break;

  case 55: /* op: '='  */
#line 320 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_EQ;
    }
#line 2041 "yacc.tab.cpp"
    break;

  case 56: /* op: '<'  */
#line 324 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LT;
    }
#line 2049 "yacc.tab.cpp"
    break;

  case 57: /* op: '>'  */
#line 328 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GT;
    }
#line 2057 "yacc.tab.cpp"
    break;

  case 58: /* op: NEQ  */
#line 332 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_NE;
    }
#line 2065 "yacc.tab.cpp"
    break;

  case 59: /* op: LEQ  */
#line 336 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LE;
    }
#line 2073 "yacc.tab.cpp"
    break;

  case 60: /* op: GEQ  */
#line 340 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GE;
    }
#line 2081 "yacc.tab.cpp"
    break;

  case 61: /* expr: value  */
#line 347 "yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
    }
#line 2089 "yacc.tab.cpp"
    break;

  case 62: /* expr: col  */
#line 351 "yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
    }
#line 2097 "yacc.tab.cpp"
    break;

  case 63: /* setClauses: setClause  */
#line 358 "yacc.y"
    {
        (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{(yyvsp[0].sv_set_clause)};
    }
#line 2105 "yacc.tab.cpp"
    break;

  case 64: /* setClauses: setClauses ',' setClause  */
#line 362 "yacc.y"
    {
        (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
    }
#line 2113 "yacc.tab.cpp"
    break;

  case 65: /* setClause: colName '=' value  */
#line 369 "yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<SetClause>((yyvsp[-2].sv_str), (yyvsp[0].sv_val));
    }
#line 2121 "yacc.tab.cpp"
    break;

  case 66: /* selector: '*'  */
#line 376 "yacc.y"
    {
        (yyval.sv_cols) = {};
    }
#line 2129 "yacc.tab.cpp"
    break;

  case 68: /* tableList: tbName  */
#line 384 "yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 2137 "yacc.tab.cpp"
    break;

  case 69: /* tableList: tableList ',' tbName  */
#line 388 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2145 "yacc.tab.cpp"
    break;

  case 70: /* tableList: tableList JOIN tbName  */
#line 392 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2153 "yacc.tab.cpp"
    break;

  case 71: /* opt_order_clause: ORDER BY order_clause  */
#line 399 "yacc.y"
    { 
        (yyval.sv_orderby) = (yyvsp[0].sv_orderby); 
    }
#line 2161 "yacc.tab.cpp"
    break;

  case 72: /* opt_order_clause: %empty  */
#line 402 "yacc.y"
                      { /* ignore*/ }
#line 2167 "yacc.tab.cpp"
    break;

  case 73: /* order_clause: col opt_asc_desc  */
#line 407 "yacc.y"
    { 
        (yyval.sv_orderby) = std::make_shared<OrderBy>((yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
    }
#line 2175 "yacc.tab.cpp"
    break;

  case 74: /* opt_asc_desc: ASC  */
#line 413 "yacc.y"
                 { (yyval.sv_orderby_dir) = OrderBy_ASC;     }
#line 2181 "yacc.tab.cpp"
    break;

  case 75: /* opt_asc_desc: DESC  */
#line 414 "yacc.y"
                 { (yyval.sv_orderby_dir) = OrderBy_DESC;    }
#line 2187 "yacc.tab.cpp"
    break;

  case 76: /* opt_asc_desc: %empty  */
#line 415 "yacc.y"
            { (yyval.sv_orderby_dir) = OrderBy_DEFAULT; }
#line 2193 "yacc.tab.cpp"
    break;

  case 77: /* set_knob_type: ENABLE_NESTLOOP  */
#line 419 "yacc.y"
                    { (yyval.sv_setKnobType) = EnableNestLoop; }
#line 2199 "yacc.tab.cpp"
    break;

  case 78: /* set_knob_type: ENABLE_SORTMERGE  */
#line 420 "yacc.y"
                         { (yyval.sv_setKnobType) = EnableSortMerge; }
#line 2205 "yacc.tab.cpp"
    break;


#line 2209 "yacc.tab.cpp"
=======
#line 1837 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 26: /* oneValue: '(' valueList ')'  */
#line 168 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_vals) = (yyvsp[-1].sv_vals);
    }
#line 1845 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 27: /* valuesList: oneValue  */
#line 175 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_vals_list) = std::vector<std::vector<std::shared_ptr<Value>>>{(yyvsp[0].sv_vals)};
    }
#line 1853 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 28: /* valuesList: valuesList ',' oneValue  */
#line 179 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_vals_list).push_back((yyvsp[0].sv_vals));
    }
#line 1861 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 29: /* fieldList: field  */
#line 186 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_fields) = std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
    }
#line 1869 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 30: /* fieldList: fieldList ',' field  */
#line 190 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_fields).push_back((yyvsp[0].sv_field));
    }
#line 1877 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 31: /* colNameList: colName  */
#line 197 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1885 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 32: /* colNameList: colNameList ',' colName  */
#line 201 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1893 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 33: /* field: colName type  */
#line 208 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str), (yyvsp[0].sv_type_len));
    }
#line 1901 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 34: /* type: INT  */
#line 215 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
#line 1909 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 35: /* type: CHAR '(' VALUE_INT ')'  */
#line 219 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1917 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 36: /* type: FLOAT  */
#line 223 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
#line 1925 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 37: /* valueList: value  */
#line 230 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_vals) = std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
    }
#line 1933 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 38: /* valueList: valueList ',' value  */
#line 234 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_vals).push_back((yyvsp[0].sv_val));
    }
#line 1941 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 39: /* value: VALUE_INT  */
#line 241 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
    }
#line 1949 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 40: /* value: VALUE_FLOAT  */
#line 245 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
    }
#line 1957 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 41: /* value: VALUE_STRING  */
#line 249 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
    }
#line 1965 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 42: /* value: VALUE_BOOL  */
#line 253 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<BoolLit>((yyvsp[0].sv_bool));
    }
#line 1973 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 43: /* condition: col op expr  */
#line 260 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_cond) = std::make_shared<BinaryExpr>((yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
    }
#line 1981 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 44: /* opOnClause: %empty  */
#line 267 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                      { /* ignore*/ }
#line 1987 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 45: /* opOnClause: ON whereClause  */
#line 270 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 1995 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 46: /* optWhereClause: %empty  */
#line 276 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2001 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 47: /* optWhereClause: WHERE whereClause  */
#line 278 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 2009 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 48: /* whereClause: condition  */
#line 285 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_conds) = std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
    }
#line 2017 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 49: /* whereClause: whereClause AND condition  */
#line 289 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
    }
#line 2025 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 50: /* col: tbName '.' colName  */
#line 297 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2033 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 51: /* col: colName  */
#line 301 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
    }
#line 2041 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 52: /* col: tbName '.' colName AS alias  */
#line 306 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-4].sv_str), (yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2049 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 53: /* col: colName AS alias  */
#line 310 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2057 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 54: /* col: opt_aggregate '(' tbName '.' colName ')'  */
#line 314 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-3].sv_str), (yyvsp[-1].sv_str));
        (yyval.sv_col)->aggr_type = (yyvsp[-5].sv_aggr_type);
    }
#line 2066 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 55: /* col: opt_aggregate '(' tbName '.' colName ')' AS alias  */
#line 319 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-5].sv_str), (yyvsp[-3].sv_str), (yyvsp[0].sv_str));
        (yyval.sv_col)->aggr_type = (yyvsp[-7].sv_aggr_type);
    }
#line 2075 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 56: /* col: opt_aggregate '(' colName ')'  */
#line 324 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-1].sv_str));
        (yyval.sv_col)->aggr_type = (yyvsp[-3].sv_aggr_type);
    }
#line 2084 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 57: /* col: opt_aggregate '(' '*' ')'  */
#line 329 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", "*");
        (yyval.sv_col)->aggr_type = (yyvsp[-3].sv_aggr_type);
    }
#line 2093 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 58: /* col: opt_aggregate '(' colName ')' AS alias  */
#line 334 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[-3].sv_str), (yyvsp[0].sv_str));
        (yyval.sv_col)->aggr_type = (yyvsp[-5].sv_aggr_type);
    }
#line 2102 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 59: /* col: opt_aggregate '(' '*' ')' AS alias  */
#line 339 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", "*", (yyvsp[0].sv_str));
        (yyval.sv_col)->aggr_type = (yyvsp[-5].sv_aggr_type);
    }
#line 2111 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 60: /* colList: col  */
#line 348 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
    }
#line 2119 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 61: /* colList: col AS alias  */
#line 352 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyvsp[-2].sv_col)->alias = (yyvsp[0].sv_str);
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[-2].sv_col)};
    }
#line 2128 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 62: /* colList: colList ',' col  */
#line 357 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_cols).push_back((yyvsp[0].sv_col));
    }
#line 2136 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 63: /* colList: colList ',' col AS alias  */
#line 361 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyvsp[-2].sv_col)->alias = (yyvsp[0].sv_str);
        (yyval.sv_cols).push_back((yyvsp[-2].sv_col));
    }
#line 2145 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 64: /* op: '='  */
#line 369 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_EQ;
    }
#line 2153 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 65: /* op: '<'  */
#line 373 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LT;
    }
#line 2161 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 66: /* op: '>'  */
#line 377 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GT;
    }
#line 2169 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 67: /* op: NEQ  */
#line 381 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_NE;
    }
#line 2177 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 68: /* op: LEQ  */
#line 385 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LE;
    }
#line 2185 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 69: /* op: GEQ  */
#line 389 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GE;
    }
#line 2193 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 70: /* expr: value  */
#line 396 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
    }
#line 2201 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 71: /* expr: col  */
#line 400 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
    }
#line 2209 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 72: /* setClauses: setClause  */
#line 407 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{(yyvsp[0].sv_set_clause)};
    }
#line 2217 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 73: /* setClauses: setClauses ',' setClause  */
#line 411 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
    }
#line 2225 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 74: /* setClause: colName '=' value  */
#line 418 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<SetClause>((yyvsp[-2].sv_str), (yyvsp[0].sv_val));
    }
#line 2233 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 75: /* selector: '*'  */
#line 425 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_cols) = {};
    }
#line 2241 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 77: /* tableList: tbName  */
#line 433 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 2249 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 78: /* tableList: tableList ',' tbName  */
#line 437 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2257 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 79: /* tableList: tableList JOIN tbName  */
#line 441 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2265 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 80: /* opt_order_clause: ORDER BY order_clause  */
#line 448 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    { 
        (yyval.sv_orderby) = (yyvsp[0].sv_orderby); 
    }
#line 2273 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 81: /* opt_order_clause: %empty  */
#line 451 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2279 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 82: /* opt_group_clause: GROUP BY colList  */
#line 456 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    { 
        auto group_by = std::make_shared<GroupBy>((yyvsp[0].sv_cols));
        (yyval.sv_groupby) = group_by;
    }
#line 2288 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 83: /* opt_group_clause: GROUP BY colList HAVING whereClause  */
#line 461 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    {
        auto group_by = std::make_shared<GroupBy>((yyvsp[-2].sv_cols), (yyvsp[0].sv_conds));
        (yyval.sv_groupby) = group_by;
    }
#line 2297 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 84: /* opt_group_clause: %empty  */
#line 465 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2303 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 85: /* order_clause: col opt_asc_desc  */
#line 470 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
    { 
        (yyval.sv_orderby) = std::make_shared<OrderBy>((yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
    }
#line 2311 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 86: /* opt_asc_desc: ASC  */
#line 476 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                 { (yyval.sv_orderby_dir) = OrderBy_ASC;     }
#line 2317 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 87: /* opt_asc_desc: DESC  */
#line 477 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                 { (yyval.sv_orderby_dir) = OrderBy_DESC;    }
#line 2323 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 88: /* opt_asc_desc: %empty  */
#line 478 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
            { (yyval.sv_orderby_dir) = OrderBy_DEFAULT; }
#line 2329 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 89: /* opt_aggregate: MAX  */
#line 482 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
        { (yyval.sv_aggr_type) = AGGR_TYPE_MAX; }
#line 2335 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 90: /* opt_aggregate: MIN  */
#line 483 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
          { (yyval.sv_aggr_type) = AGGR_TYPE_MIN; }
#line 2341 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 91: /* opt_aggregate: SUM  */
#line 484 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
          { (yyval.sv_aggr_type) = AGGR_TYPE_SUM; }
#line 2347 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 92: /* opt_aggregate: COUNT  */
#line 485 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
            { (yyval.sv_aggr_type) = AGGR_TYPE_COUNT; }
#line 2353 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 93: /* set_knob_type: ENABLE_NESTLOOP  */
#line 488 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                    { (yyval.sv_setKnobType) = EnableNestLoop; }
#line 2359 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;

  case 94: /* set_knob_type: ENABLE_SORTMERGE  */
#line 489 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
                         { (yyval.sv_setKnobType) = EnableSortMerge; }
#line 2365 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
    break;


#line 2369 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.tab.cpp"
>>>>>>> q4

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

<<<<<<< HEAD
#line 426 "yacc.y"
=======
#line 497 "/home/dengweiwei/Documents/DB2024/db2024-nku/src/parser/yacc.y"
>>>>>>> q4

