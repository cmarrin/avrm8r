/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     K_FUNCTION = 1,
     K_NEW = 2,
     K_DELETE = 3,
     K_TYPEOF = 4,
     K_IN = 5,
     K_VAR = 6,
     K_TRUE = 7,
     K_FALSE = 8,
     K_NULL = 9,
     K_DO = 10,
     K_WHILE = 11,
     K_FOR = 12,
     K_IF = 13,
     K_ELSE = 14,
     K_SWITCH = 15,
     K_CASE = 16,
     K_DEFAULT = 17,
     K_BREAK = 18,
     K_CONTINUE = 19,
     K_RETURN = 20,
     K_THIS = 21,
     T_FLOAT = 48,
     T_IDENTIFIER = 49,
     T_STRING = 50,
     T_INTEGER = 51,
     O_RSHIFTEQ = 65,
     O_RSHIFTFILLEQ = 66,
     O_LSHIFTEQ = 67,
     O_ADDEQ = 68,
     O_SUBEQ = 69,
     O_MULEQ = 70,
     O_DIVEQ = 71,
     O_MODEQ = 72,
     O_ANDEQ = 73,
     O_XOREQ = 74,
     O_OREQ = 75,
     O_RSHIFT = 76,
     O_RSHIFTFILL = 77,
     O_LSHIFT = 78,
     O_INC = 79,
     O_DEC = 80,
     O_LAND = 81,
     O_LOR = 82,
     O_LE = 83,
     O_GE = 84,
     O_EQ = 85,
     O_NE = 86,
     O_BLOCK_START = 87,
     O_OBJ_START = 88
   };
#endif
/* Tokens.  */
#define K_FUNCTION 1
#define K_NEW 2
#define K_DELETE 3
#define K_TYPEOF 4
#define K_IN 5
#define K_VAR 6
#define K_TRUE 7
#define K_FALSE 8
#define K_NULL 9
#define K_DO 10
#define K_WHILE 11
#define K_FOR 12
#define K_IF 13
#define K_ELSE 14
#define K_SWITCH 15
#define K_CASE 16
#define K_DEFAULT 17
#define K_BREAK 18
#define K_CONTINUE 19
#define K_RETURN 20
#define K_THIS 21
#define T_FLOAT 48
#define T_IDENTIFIER 49
#define T_STRING 50
#define T_INTEGER 51
#define O_RSHIFTEQ 65
#define O_RSHIFTFILLEQ 66
#define O_LSHIFTEQ 67
#define O_ADDEQ 68
#define O_SUBEQ 69
#define O_MULEQ 70
#define O_DIVEQ 71
#define O_MODEQ 72
#define O_ANDEQ 73
#define O_XOREQ 74
#define O_OREQ 75
#define O_RSHIFT 76
#define O_RSHIFTFILL 77
#define O_LSHIFT 78
#define O_INC 79
#define O_DEC 80
#define O_LAND 81
#define O_LOR 82
#define O_LE 83
#define O_GE 84
#define O_EQ 85
#define O_NE 86
#define O_BLOCK_START 87
#define O_OBJ_START 88




/* Copy the first part of user declarations.  */
#line 16 "ucParse.yy"

#include "config.h"
#include "ucParser.h"
#include "ucParse.hpp"

#ifndef alloca
#define alloca malloc
#endif

#ifdef BISON_DEBUG
#define YYDEBUG 1
#endif

#define YYLEX_PARAM (((UC::Parser*) parser)->getScanner())
#define YYPARSE_PARAM parser

inline void yyerror(const char* s) { /*(UC::ParseHandler* ph)->error(s);*/ }

int yylex (YYSTYPE * yylval_param , void* yyscanner)
{
	return ((UC::Scanner*) yyscanner)->getToken(yylval_param);
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 41 "ucParse.yy"
{
	uint16_t	atom;
	int32_t		integer;
	float		number;
}
/* Line 193 of yacc.c.  */
#line 229 "ucParse.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 242 "ucParse.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  85
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   586

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  135
/* YYNRULES -- Number of states.  */
#define YYNSTATES  229

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    61,     2,     2,     2,    64,    67,     2,
      52,    53,    62,    58,    57,    59,    56,    63,    24,    25,
      26,    27,     2,     2,     2,     2,     2,     2,    71,    73,
      65,    72,    66,    70,     2,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,     2,
       2,    54,     2,    55,    68,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    69,    74,    60,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    36,    38,    43,
      47,    52,    55,    58,    62,    65,    68,    70,    74,    76,
      79,    81,    83,    85,    87,    89,    91,    93,    95,    97,
      99,   103,   107,   111,   113,   117,   121,   123,   127,   131,
     135,   137,   141,   145,   149,   153,   157,   159,   163,   167,
     169,   173,   175,   179,   181,   185,   187,   191,   193,   197,
     199,   205,   207,   211,   213,   215,   217,   219,   221,   223,
     225,   227,   229,   231,   233,   235,   237,   241,   245,   247,
     251,   253,   256,   259,   261,   263,   265,   267,   269,   271,
     273,   276,   280,   282,   285,   287,   290,   296,   304,   310,
     314,   320,   322,   323,   325,   328,   333,   337,   343,   351,
     359,   362,   365,   368,   372,   375,   380,   386,   388,   392,
     396,   397,   399,   403,   405,   409
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      76,     0,    -1,    77,    -1,    78,    -1,    77,    78,    -1,
     102,    -1,   115,    -1,    25,    -1,    23,    -1,    11,    -1,
       9,    -1,    10,    -1,    24,    -1,    27,    -1,    26,    -1,
     118,    -1,    52,    97,    53,    -1,    79,    -1,    80,    54,
      97,    55,    -1,    80,    52,    53,    -1,    80,    52,    81,
      53,    -1,    80,   118,    -1,    80,    26,    -1,    80,    56,
      25,    -1,    80,    42,    -1,    80,    43,    -1,    95,    -1,
      81,    57,    95,    -1,    80,    -1,    83,    82,    -1,    58,
      -1,    59,    -1,    60,    -1,    61,    -1,     5,    -1,     6,
      -1,    42,    -1,    43,    -1,     4,    -1,    82,    -1,    84,
      62,    82,    -1,    84,    63,    82,    -1,    84,    64,    82,
      -1,    84,    -1,    85,    58,    84,    -1,    85,    59,    84,
      -1,    85,    -1,    86,    41,    85,    -1,    86,    39,    85,
      -1,    86,    40,    85,    -1,    86,    -1,    87,     7,    86,
      -1,    87,    65,    86,    -1,    87,    66,    86,    -1,    87,
      46,    86,    -1,    87,    47,    86,    -1,    87,    -1,    88,
      48,    87,    -1,    88,    49,    87,    -1,    88,    -1,    89,
      67,    88,    -1,    89,    -1,    90,    68,    89,    -1,    90,
      -1,    91,    69,    90,    -1,    91,    -1,    92,    44,    91,
      -1,    92,    -1,    93,    45,    92,    -1,    93,    -1,    93,
      70,    97,    71,    94,    -1,    94,    -1,    82,    96,    95,
      -1,    72,    -1,    33,    -1,    34,    -1,    35,    -1,    31,
      -1,    32,    -1,    30,    -1,    28,    -1,    29,    -1,    36,
      -1,    37,    -1,    38,    -1,    95,    -1,    97,    57,    95,
      -1,     8,    99,    73,    -1,   100,    -1,    99,    57,   100,
      -1,    25,    -1,    25,   101,    -1,    72,    95,    -1,   103,
      -1,    98,    -1,   105,    -1,   106,    -1,   107,    -1,   113,
      -1,   114,    -1,    50,    74,    -1,    50,   104,    74,    -1,
     102,    -1,   104,   102,    -1,    73,    -1,    97,    73,    -1,
      15,    52,    97,    53,   102,    -1,    15,    52,    97,    53,
     102,    16,   102,    -1,    17,    52,    97,    53,   108,    -1,
      50,   109,    74,    -1,    50,   109,   112,   109,    74,    -1,
     110,    -1,    -1,   111,    -1,   110,   111,    -1,    18,    97,
      71,   102,    -1,    19,    71,   102,    -1,    13,    52,    97,
      53,   102,    -1,    12,   102,    13,    52,    97,    53,    73,
      -1,    14,    52,   105,   105,    97,    53,   102,    -1,    21,
      73,    -1,    20,    73,    -1,    22,    73,    -1,    22,    97,
      73,    -1,     3,   116,    -1,    25,    52,    53,   103,    -1,
      25,    52,   117,    53,   103,    -1,    25,    -1,   117,    57,
      25,    -1,    51,   119,    55,    -1,    -1,   120,    -1,   119,
      57,   120,    -1,    95,    -1,    25,    71,    95,    -1,    26,
      71,    95,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   118,   118,   121,   122,   126,   127,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   157,   158,   162,   163,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   179,
     180,   181,   182,   186,   187,   188,   192,   193,   194,   195,
     199,   200,   201,   202,   203,   204,   208,   209,   210,   214,
     215,   219,   220,   224,   225,   229,   230,   234,   235,   239,
     240,   244,   245,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   264,   265,   269,   273,   274,
     278,   279,   283,   287,   288,   289,   290,   291,   292,   293,
     297,   298,   302,   303,   307,   308,   312,   313,   317,   321,
     322,   326,   327,   331,   332,   336,   340,   344,   345,   346,
     350,   351,   352,   353,   357,   361,   362,   366,   367,   371,
     374,   376,   377,   381,   382,   383
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_FUNCTION", "K_NEW", "K_DELETE",
  "K_TYPEOF", "K_IN", "K_VAR", "K_TRUE", "K_FALSE", "K_NULL", "K_DO",
  "K_WHILE", "K_FOR", "K_IF", "K_ELSE", "K_SWITCH", "K_CASE", "K_DEFAULT",
  "K_BREAK", "K_CONTINUE", "K_RETURN", "K_THIS", "T_FLOAT", "T_IDENTIFIER",
  "T_STRING", "T_INTEGER", "O_RSHIFTEQ", "O_RSHIFTFILLEQ", "O_LSHIFTEQ",
  "O_ADDEQ", "O_SUBEQ", "O_MULEQ", "O_DIVEQ", "O_MODEQ", "O_ANDEQ",
  "O_XOREQ", "O_OREQ", "O_RSHIFT", "O_RSHIFTFILL", "O_LSHIFT", "O_INC",
  "O_DEC", "O_LAND", "O_LOR", "O_LE", "O_GE", "O_EQ", "O_NE",
  "O_BLOCK_START", "O_OBJ_START", "'('", "')'", "'['", "']'", "'.'", "','",
  "'+'", "'-'", "'~'", "'!'", "'*'", "'/'", "'%'", "'<'", "'>'", "'&'",
  "'^'", "'|'", "'?'", "':'", "'='", "';'", "'}'", "$accept", "program",
  "source_elements", "source_element", "primary_expression",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "unary_operator", "multiplicative_expression", "additive_expression",
  "shift_expression", "relational_expression", "equality_expression",
  "and_expression", "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "assignment_operator",
  "expression", "declaration_statement", "variable_declaration_list",
  "variable_declaration", "initializer", "statement", "compound_statement",
  "statement_list", "expression_statement", "selection_statement",
  "switch_statement", "case_block", "case_clauses_opt", "case_clauses",
  "case_clause", "default_clause", "iteration_statement", "jump_statement",
  "function_declaration", "function_definition", "formal_parameter_list",
  "objectLiteral", "propertyNameAndValueList", "property", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    48,    49,    50,    51,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    40,    41,    91,    93,    46,    44,    43,    45,
     126,    33,    42,    47,    37,    60,    62,    38,    94,   124,
      63,    58,    61,    59,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    77,    77,    78,    78,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    81,    81,    82,    82,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    84,
      84,    84,    84,    85,    85,    85,    86,    86,    86,    86,
      87,    87,    87,    87,    87,    87,    88,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      94,    95,    95,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    97,    97,    98,    99,    99,
     100,   100,   101,   102,   102,   102,   102,   102,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   107,   108,
     108,   109,   109,   110,   110,   111,   112,   113,   113,   113,
     114,   114,   114,   114,   115,   116,   116,   117,   117,   118,
     119,   119,   119,   120,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     4,     3,
       4,     2,     2,     3,     2,     2,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       5,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     3,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     1,     2,     1,     2,     5,     7,     5,     3,
       5,     1,     0,     1,     2,     4,     3,     5,     7,     7,
       2,     2,     2,     3,     2,     4,     5,     1,     3,     3,
       0,     1,     3,     1,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    38,    34,    35,     0,    10,    11,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     8,    12,     7,
      14,    13,    36,    37,     0,   130,     0,    30,    31,    32,
      33,   104,     0,     2,     3,    17,    28,    39,     0,    43,
      46,    50,    56,    59,    61,    63,    65,    67,    69,    71,
      85,     0,    94,     5,    93,    95,    96,    97,    98,    99,
       6,    15,     0,   124,    90,     0,    88,     0,     0,     0,
       0,     0,   121,   120,   122,     0,   100,   102,     0,     7,
      14,   133,     0,   131,     0,     1,     4,    22,    24,    25,
       0,     0,     0,    21,    80,    81,    79,    77,    78,    74,
      75,    76,    82,    83,    84,    73,     0,    29,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,    91,     0,    87,     0,     0,     0,     0,
       0,   123,   101,   103,     0,     0,   129,     0,    16,    19,
       0,    26,     0,    23,    72,    40,    41,    42,    39,    44,
      45,    48,    49,    47,    51,    54,    55,    52,    53,    57,
      58,    60,    62,    64,    66,    68,     0,    86,   127,     0,
       0,    92,    89,     0,     0,     0,     0,     0,   134,   135,
     132,    20,     0,    18,     0,   125,     0,     0,     0,   117,
       0,   106,   112,   108,    27,    70,   126,   128,     0,     0,
       0,     0,     0,   111,   113,   118,   119,   107,     0,     0,
     109,   112,   114,     0,     0,     0,   115,   116,   110
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    32,    33,    34,    35,    36,   150,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   106,    51,    52,    65,    66,   133,    53,    54,    78,
      55,    56,    57,   203,   212,   213,   214,   221,    58,    59,
      60,    63,   180,    61,    82,    83
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -153
static const yytype_int16 yypact[] =
{
     131,    39,  -153,  -153,  -153,    49,  -153,  -153,  -153,   354,
      33,    81,    86,    98,   111,   112,    -1,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,   212,   514,   525,  -153,  -153,  -153,
    -153,  -153,   171,   131,  -153,  -153,    28,     1,   525,    97,
     -39,   123,     0,    62,   119,   120,   118,   149,   -27,  -153,
    -153,   -12,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,   142,  -153,   125,     5,  -153,   182,   525,   412,
     525,   525,  -153,  -153,  -153,    50,  -153,  -153,   283,   127,
     128,  -153,    92,  -153,   -36,  -153,  -153,  -153,  -153,  -153,
     456,   525,   177,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,   525,  -153,   525,   525,
     525,   525,   525,   525,   525,   525,   525,   525,   525,   525,
     525,   525,   525,   525,   525,   525,   525,   525,   525,   525,
    -153,   -13,   525,  -153,    49,  -153,   151,    -5,   412,    24,
      55,  -153,  -153,  -153,   525,   525,  -153,   514,  -153,  -153,
      72,  -153,   121,  -153,  -153,  -153,  -153,  -153,  -153,    97,
      97,   -39,   -39,   -39,   123,   123,   123,   123,   123,     0,
       0,    62,   119,   120,   118,   149,   -43,  -153,  -153,   155,
      73,  -153,  -153,   525,   354,   525,   354,   156,  -153,  -153,
    -153,  -153,   525,  -153,   525,  -153,   155,   183,    74,  -153,
      75,   191,   192,  -153,  -153,  -153,  -153,  -153,   136,   354,
     354,   525,    -6,   192,  -153,  -153,  -153,  -153,    -8,   140,
    -153,   192,  -153,   354,   354,   138,  -153,  -153,  -153
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,  -153,   180,  -153,  -153,  -153,   -22,  -153,     8,
      52,    -2,    58,    96,   104,   105,   114,   115,  -153,    37,
     -23,  -153,   -15,  -153,  -153,   107,  -153,    -9,  -152,  -153,
     -63,  -153,  -153,  -153,    22,  -153,    31,  -153,  -153,  -153,
    -153,  -153,  -153,   209,  -153,    99
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      67,    75,    81,     2,     3,     4,   138,   116,     6,     7,
       8,    84,   178,   219,   129,    77,   107,   148,   127,   111,
     112,   129,    17,    18,    19,    20,    21,   195,   194,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     179,    22,    23,   128,   206,   129,   117,   118,   184,   129,
      25,    26,   129,   137,    87,   139,   140,    27,    28,    29,
      30,   130,   134,   223,    62,   119,   120,   151,   220,   143,
      88,    89,    74,   105,    64,   185,   152,   186,   135,    25,
      90,   129,    91,   154,    92,    68,   155,   156,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   177,   129,   187,   181,
     121,   122,   129,   176,   164,   165,   166,   167,   168,   159,
     160,   188,   189,   141,    81,   191,   196,   208,   209,   192,
     197,   129,   129,    69,     1,     2,     3,     4,    70,     5,
       6,     7,     8,     9,    10,    11,    12,   146,    13,   147,
      71,    14,    15,    16,    17,    18,    19,    20,    21,   108,
     109,   110,   113,   114,   115,   161,   162,   163,   198,   204,
     200,    85,   158,    22,    23,   199,   193,   201,   129,   169,
     170,    24,    25,    26,    72,    73,   123,   125,   124,    27,
      28,    29,    30,   126,   131,   136,   218,   132,   144,   145,
     216,   217,   153,   183,    31,    24,   202,   210,   207,   215,
     211,   224,   228,    86,   226,   227,     2,     3,     4,   171,
       5,     6,     7,     8,     9,    10,    11,    12,   172,    13,
     173,   205,    14,    15,    16,    17,    18,    19,    20,    21,
     174,   182,   175,   225,   222,    93,   190,     0,     0,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,    24,    25,    26,     0,     0,     0,     0,     0,
      27,    28,    29,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    76,     2,     3,     4,
       0,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,     0,     0,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,    24,    25,    26,     0,     0,     0,     0,
       0,    27,    28,    29,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,   142,     2,     3,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
       0,    13,     0,     0,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,    24,    25,    26,     0,     0,     0,
       0,     0,    27,    28,    29,    30,     2,     3,     4,     0,
       0,     6,     7,     8,     0,     0,     0,    31,     0,     0,
       0,     0,     0,     0,     0,    17,    18,    19,    20,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       2,     3,     4,    25,    26,     6,     7,     8,     0,     0,
      27,    28,    29,    30,     0,     0,     0,     0,     0,    17,
      18,    19,    20,    21,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,    25,    26,   149,
       0,     0,     0,     0,    27,    28,    29,    30,     2,     3,
       4,     0,     0,     6,     7,     8,     0,     0,     0,     2,
       3,     4,     0,     0,     6,     7,     8,    17,    18,    79,
      80,    21,     0,     0,     0,     0,     0,     0,    17,    18,
      19,    20,    21,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,    25,    26,    22,    23,     0,
       0,     0,    27,    28,    29,    30,    25,    26,     0,     0,
       0,     0,     0,    27,    28,    29,    30
};

static const yytype_int16 yycheck[] =
{
       9,    16,    25,     4,     5,     6,    69,     7,     9,    10,
      11,    26,    25,    19,    57,    24,    38,    53,    45,    58,
      59,    57,    23,    24,    25,    26,    27,   179,    71,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      53,    42,    43,    70,   196,    57,    46,    47,    53,    57,
      51,    52,    57,    68,    26,    70,    71,    58,    59,    60,
      61,    73,    57,    71,    25,    65,    66,    90,    74,    78,
      42,    43,    73,    72,    25,   138,    91,    53,    73,    51,
      52,    57,    54,   106,    56,    52,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   129,    57,    53,   132,
      48,    49,    57,   128,   116,   117,   118,   119,   120,   111,
     112,   144,   145,    73,   147,    53,    53,    53,    53,    57,
      57,    57,    57,    52,     3,     4,     5,     6,    52,     8,
       9,    10,    11,    12,    13,    14,    15,    55,    17,    57,
      52,    20,    21,    22,    23,    24,    25,    26,    27,    62,
      63,    64,    39,    40,    41,   113,   114,   115,   183,   192,
     185,     0,   194,    42,    43,   184,    55,   186,    57,   121,
     122,    50,    51,    52,    73,    73,    67,    69,    68,    58,
      59,    60,    61,    44,    52,    13,   211,    72,    71,    71,
     209,   210,    25,    52,    73,    50,    50,    16,    25,    73,
      18,    71,    74,    33,   223,   224,     4,     5,     6,   123,
       8,     9,    10,    11,    12,    13,    14,    15,   124,    17,
     125,   194,    20,    21,    22,    23,    24,    25,    26,    27,
     126,   134,   127,   221,   213,    36,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,    -1,
      58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    -1,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,     4,     5,
       6,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      -1,    17,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    -1,    -1,    -1,
      -1,    -1,    58,    59,    60,    61,     4,     5,     6,    -1,
      -1,     9,    10,    11,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
       4,     5,     6,    51,    52,     9,    10,    11,    -1,    -1,
      58,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,     4,     5,
       6,    -1,    -1,     9,    10,    11,    -1,    -1,    -1,     4,
       5,     6,    -1,    -1,     9,    10,    11,    23,    24,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    42,    43,    -1,
      -1,    -1,    58,    59,    60,    61,    51,    52,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      13,    14,    15,    17,    20,    21,    22,    23,    24,    25,
      26,    27,    42,    43,    50,    51,    52,    58,    59,    60,
      61,    73,    76,    77,    78,    79,    80,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    97,    98,   102,   103,   105,   106,   107,   113,   114,
     115,   118,    25,   116,    25,    99,   100,   102,    52,    52,
      52,    52,    73,    73,    73,    97,    74,   102,   104,    25,
      26,    95,   119,   120,    97,     0,    78,    26,    42,    43,
      52,    54,    56,   118,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    72,    96,    82,    62,    63,
      64,    58,    59,    39,    40,    41,     7,    46,    47,    65,
      66,    48,    49,    67,    68,    69,    44,    45,    70,    57,
      73,    52,    72,   101,    57,    73,    13,    97,   105,    97,
      97,    73,    74,   102,    71,    71,    55,    57,    53,    53,
      81,    95,    97,    25,    95,    82,    82,    82,    82,    84,
      84,    85,    85,    85,    86,    86,    86,    86,    86,    87,
      87,    88,    89,    90,    91,    92,    97,    95,    25,    53,
     117,    95,   100,    52,    53,   105,    53,    53,    95,    95,
     120,    53,    57,    55,    71,   103,    53,    57,    97,   102,
      97,   102,    50,   108,    95,    94,   103,    25,    53,    53,
      16,    18,   109,   110,   111,    73,   102,   102,    97,    19,
      74,   112,   111,    71,    71,   109,   102,   102,    74
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
      
/* Line 1267 of yacc.c.  */
#line 1734 "ucParse.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 386 "ucParse.yy"

#include <stdio.h>

extern char yytext[];

void yyerror(char* s)
{
	fflush(stdout);
}











#if 0

/*
 * Copyright (C) 2000 Sony Corporation
 * See license.txt for License Agreement
 *
 _______________________________________________________________________
 |
 |
 |   Description:
 |	This file contains the bison rules for the Blendo Parser
 |
 |   Author(s)		: Chris Marrin
 |
 _______________________________________________________________________
 */

%{
#include "ucParseHandler.h"
#include "ucParser.hpp"
#include "ucAtomList.h"

#ifndef alloca
#define alloca malloc
#endif

#ifdef BISON_DEBUG
#define YYDEBUG 1
#endif

#define YYLEX_PARAM (((UC::ParseHandler*) ph)->getScanner())
#define YYPARSE_PARAM ph

inline void yyerror(const char* s) { /*(UC::ParseHandler* ph)->error(s);*/ }

extern int yylex (YYSTYPE * yylval_param , void* yyscanner);

%}

%union {
	UC::RawAtom	atom;
    char*		string;
    int			keyword;
	int32_t		integer;
	float		number;
};

%type <atom> IDENTIFIER
%type <string> STRING
%type <num> NUMBER

%token IDENTIFIER STRING NUMBER INTEGER HEXNUMBER
%token K_FUNCTION
%token K_TRUE K_FALSE K_NULL
%token K_BREAK K_CONTINUE K_ELSE K_FOR K_IF K_NEW K_RETURN K_WHILE K_THIS K_VAR
%token K_DO K_SWITCH K_CASE K_DEFAULT
%token O_EQ O_NE O_LE O_GE O_LSHIFT O_RSHIFT O_RSHIFTFILL O_AND O_OR
%token O_PLUSEQ O_MINUSEQ O_MULTIPLYEQ O_DIVIDEEQ
%token O_MODEQ O_LSHIFTEQ O_IN O_DELETE O_TYPEOF 
%token O_RSHIFTEQ O_RSHIFTFILLEQ O_ANDEQ O_XOREQ O_OREQ O_INCREMENT O_DECREMENT

%left O_INCREMENT O_DECREMENT O_DELETE O_TYPEOF '!' '~' UMINUS
%left '*' '/' '%'
%left '+' '-'
%left O_RSHIFT O_LSHIFT O_RSHIFTFILL
%left O_LE O_GE '<' '>' O_IN
%left O_EQ O_NE
%left '&'
%left '^'
%left '|'
%left O_AND
%left O_OR

/*  we expect if..then..else to produce a shift/reduce conflict
*/

%pure_parser

%%

program:	sourceElements
    ;

sourceElements:	sourceElement
    |			sourceElements sourceElement
    ;

sourceElement:
		statement
    |	functionDeclaration
    ;
	
primaryExpression:
		K_THIS
    |	IDENTIFIER
    |	K_NULL
    |	K_TRUE
    |	K_FALSE
    |	NUMBER
	|	INTEGER
	|	HEXNUMBER
    |	STRING
    |	objectLiteral
    |	'(' expression ')'
    ;

memberExpression:
		primaryExpression
    |	memberExpression '[' expression ']'
    |	memberExpression '.' IDENTIFIER
    ;

newExpression:
		memberExpression
    |	K_NEW newExpression
    ;

callExpression:
		memberExpression arguments
    |	callExpression arguments
    |	callExpression '[' expression ']'
    |	callExpression '.' IDENTIFIER
    ;

arguments:
		'(' ')'
    |	'(' argumentList ')'
    ;

argumentList:
		expression
    |	argumentList ',' expression
    ;

leftHandSideExpression:
		newExpression
    |	callExpression
    ;
	
expression:
		leftHandSideExpression
	|	leftHandSideExpression assignmentOperator expression
	|	'-' expression %prec UMINUS
	|	'!' expression
	|	'~' expression
	|	O_INCREMENT expression
	|	O_DECREMENT expression
	|	O_DELETE expression
	|	O_TYPEOF expression
	|	expression '%' expression
	|	expression '*' expression
	|	expression '/' expression
	|	expression '+' expression
	|	expression '-' expression
	|	expression O_RSHIFT expression
	|	expression O_RSHIFTFILL expression
	|	expression O_LSHIFT expression
	|	expression '<' expression
	|	expression '>' expression
	|	expression O_LE expression
	|	expression O_GE expression
	|	expression O_IN expression
	|	expression O_EQ expression
	|	expression O_NE expression
	|	expression '&' expression
	|	expression '^' expression
	|	expression '|' expression
	|	expression O_AND expression
	|	expression O_OR expression
	|	expression '?' expression ':' expression
	;
	
expressionOpt:
		expression
	|	/* empty */
	;

assignmentOperator:
		O_MULTIPLYEQ
    |	O_DIVIDEEQ
    |	O_MODEQ
    |	O_PLUSEQ
    |	O_MINUSEQ
    |	O_LSHIFTEQ
    |	O_RSHIFTEQ
    |	O_RSHIFTFILLEQ
    |	O_ANDEQ
    |	O_XOREQ
    |	O_OREQ
	|	'='
    ;

statement:
		block
    |	variableStatement
    |	emptyStatement
    |	ifStatement
    |	iterationStatement
    |	continueStatement
    |	breakStatement
    |	returnStatement
    |	expressionStatement
	|	switchStatement
    ;

block:
		'{' '}'
    |	'{' statementList '}'
    ;

statementList:
		statement
    |	statementList statement
    ;
	
statementListOpt:
		statementList
	|	/* empty */
	;

variableStatement:
		K_VAR variableDeclarationList ';'
    ;

variableDeclarationList:
		variableDeclaration
    |	variableDeclarationList ',' variableDeclaration
    ;

variableDeclaration:
    	IDENTIFIER
    |	IDENTIFIER initializer
    ;

initializer:
    	'=' expression
    ;

emptyStatement:
		';'
    ;

expressionStatement:
		expression ';'
    ;

ifStatement:
		K_IF '(' expression ')' statement K_ELSE statement
    |	K_IF '(' expression ')' statement
    ;

iterationStatement:
		K_DO statement K_WHILE '(' expression ')' ';'
	|	K_WHILE '(' expression ')' statement
    |	K_FOR '(' expressionOpt ';' expressionOpt ';' expressionOpt ')' statement
    |	K_FOR '(' K_VAR variableDeclaration ';' expressionOpt ';' expressionOpt ')' statement
    |	K_FOR '(' leftHandSideExpression O_IN expression ')' statement
    |	K_FOR '(' K_VAR variableDeclaration O_IN expression ')' statement
    ;

continueStatement:
		K_CONTINUE ';'
    ;

breakStatement:
		K_BREAK ';'	
    ;

returnStatement:
		K_RETURN expressionOpt ';'
    ;

switchStatement:
		K_SWITCH '(' expression ')' caseBlock
	;

caseBlock:
		'{' caseClausesOpt '}'
	|	'{' caseClausesOpt defaultClause caseClausesOpt '}'
	;

caseClausesOpt:
		caseClauses
	|	/* empty */
	;

caseClauses:
		caseClause
	|	caseClauses caseClause
	;

caseClause:
		K_CASE expression ':' statementListOpt
	;

defaultClause:
		K_DEFAULT ':' statementListOpt
	;

functionDeclaration:
		K_FUNCTION functionDefinition
	;

functionDefinition:
		IDENTIFIER '(' formalParameterListOpt ')' block
    ;

formalParameterListOpt:
		formalParameterList
    |	/* empty */
    ;

formalParameterList:
		IDENTIFIER
    |	formalParameterList ',' IDENTIFIER
    ;

objectLiteral:
		'[' propertyNameAndValueList ']'
    ;

propertyNameAndValueList:
		/* empty */
    |	propertyName ':' expression
    |	propertyNameAndValueList ',' propertyName ':' expression
    ;

propertyName:
		IDENTIFIER
    |	STRING
    |	NUMBER
    ;

%%

#endif
