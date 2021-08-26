
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */


/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../main/errors.h"
#include "../main/mtypes.h"

#include "program.h"
#include "prog_parameter.h"
#include "prog_parameter_layout.h"
#include "prog_statevars.h"
#include "prog_instruction.h"

#include "symbol_table.h"
#include "program_parser.h"

#include "../util/u_math.h"
#include "../util/u_memory.h"

enum {
   STATE_MATRIX_NO_MODIFIER,
   STATE_MATRIX_INVERSE,
   STATE_MATRIX_TRANSPOSE,
   STATE_MATRIX_INVTRANS,
};

extern void *yy_scan_string(char *);
extern void yy_delete_buffer(void *);

static struct asm_symbol *declare_variable(struct asm_parser_state *state,
    char *name, enum asm_type t, struct YYLTYPE *locp);

static int add_state_reference(struct gl_program_parameter_list *param_list,
    const gl_state_index16 tokens[STATE_LENGTH]);

static int initialize_symbol_from_state(struct gl_program *prog,
    struct asm_symbol *param_var, const gl_state_index16 tokens[STATE_LENGTH]);

static int initialize_symbol_from_param(struct gl_program *prog,
    struct asm_symbol *param_var, const gl_state_index16 tokens[STATE_LENGTH]);

static int initialize_symbol_from_const(struct gl_program *prog,
    struct asm_symbol *param_var, const struct asm_vector *vec,
    GLboolean allowSwizzle);

static int yyparse(struct asm_parser_state *state);

static char *make_error_string(const char *fmt, ...);

static void yyerror(struct YYLTYPE *locp, struct asm_parser_state *state,
    const char *s);

static int validate_inputs(struct YYLTYPE *locp,
    struct asm_parser_state *state);

static void init_dst_reg(struct prog_dst_register *r);

static void set_dst_reg(struct prog_dst_register *r,
                        gl_register_file file, GLint index);

static void init_src_reg(struct asm_src_register *r);

static void set_src_reg(struct asm_src_register *r,
                        gl_register_file file, GLint index);

static void set_src_reg_swz(struct asm_src_register *r,
                            gl_register_file file, GLint index, GLuint swizzle);

static void asm_instruction_set_operands(struct asm_instruction *inst,
    const struct prog_dst_register *dst, const struct asm_src_register *src0,
    const struct asm_src_register *src1, const struct asm_src_register *src2);

static struct asm_instruction *asm_instruction_ctor(enum prog_opcode op,
    const struct prog_dst_register *dst, const struct asm_src_register *src0,
    const struct asm_src_register *src1, const struct asm_src_register *src2);

static struct asm_instruction *asm_instruction_copy_ctor(
    const struct prog_instruction *base, const struct prog_dst_register *dst,
    const struct asm_src_register *src0, const struct asm_src_register *src1,
    const struct asm_src_register *src2);

#ifndef FALSE
#define FALSE 0
#define TRUE (!FALSE)
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N)					\
   do {									\
      if (N) {							\
	 (Current).first_line = YYRHSLOC(Rhs, 1).first_line;		\
	 (Current).first_column = YYRHSLOC(Rhs, 1).first_column;	\
	 (Current).position = YYRHSLOC(Rhs, 1).position;		\
	 (Current).last_line = YYRHSLOC(Rhs, N).last_line;		\
	 (Current).last_column = YYRHSLOC(Rhs, N).last_column;		\
      } else {								\
	 (Current).first_line = YYRHSLOC(Rhs, 0).last_line;		\
	 (Current).last_line = (Current).first_line;			\
	 (Current).first_column = YYRHSLOC(Rhs, 0).last_column;		\
	 (Current).last_column = (Current).first_column;		\
	 (Current).position = YYRHSLOC(Rhs, 0).position			\
	    + (Current).first_column;					\
      }									\
   } while(0)



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ARBvp_10 = 258,
     ARBfp_10 = 259,
     ADDRESS = 260,
     ALIAS = 261,
     ATTRIB = 262,
     OPTION = 263,
     OUTPUT = 264,
     PARAM = 265,
     TEMP = 266,
     END = 267,
     BIN_OP = 268,
     BINSC_OP = 269,
     SAMPLE_OP = 270,
     SCALAR_OP = 271,
     TRI_OP = 272,
     VECTOR_OP = 273,
     ARL = 274,
     KIL = 275,
     SWZ = 276,
     TXD_OP = 277,
     INTEGER = 278,
     REAL = 279,
     AMBIENT = 280,
     ATTENUATION = 281,
     BACK = 282,
     CLIP = 283,
     COLOR = 284,
     DEPTH = 285,
     DIFFUSE = 286,
     DIRECTION = 287,
     EMISSION = 288,
     ENV = 289,
     EYE = 290,
     FOG = 291,
     FOGCOORD = 292,
     FRAGMENT = 293,
     FRONT = 294,
     HALF = 295,
     INVERSE = 296,
     INVTRANS = 297,
     LIGHT = 298,
     LIGHTMODEL = 299,
     LIGHTPROD = 300,
     LOCAL = 301,
     MATERIAL = 302,
     MAT_PROGRAM = 303,
     MATRIX = 304,
     MATRIXINDEX = 305,
     MODELVIEW = 306,
     MVP = 307,
     NORMAL = 308,
     OBJECT = 309,
     PALETTE = 310,
     PARAMS = 311,
     PLANE = 312,
     POINT_TOK = 313,
     POINTSIZE = 314,
     POSITION = 315,
     PRIMARY = 316,
     PROGRAM = 317,
     PROJECTION = 318,
     RANGE = 319,
     RESULT = 320,
     ROW = 321,
     SCENECOLOR = 322,
     SECONDARY = 323,
     SHININESS = 324,
     SIZE_TOK = 325,
     SPECULAR = 326,
     SPOT = 327,
     STATE = 328,
     TEXCOORD = 329,
     TEXENV = 330,
     TEXGEN = 331,
     TEXGEN_Q = 332,
     TEXGEN_R = 333,
     TEXGEN_S = 334,
     TEXGEN_T = 335,
     TEXTURE = 336,
     TRANSPOSE = 337,
     TEXTURE_UNIT = 338,
     TEX_1D = 339,
     TEX_2D = 340,
     TEX_3D = 341,
     TEX_CUBE = 342,
     TEX_RECT = 343,
     TEX_SHADOW1D = 344,
     TEX_SHADOW2D = 345,
     TEX_SHADOWRECT = 346,
     TEX_ARRAY1D = 347,
     TEX_ARRAY2D = 348,
     TEX_ARRAYSHADOW1D = 349,
     TEX_ARRAYSHADOW2D = 350,
     VERTEX = 351,
     VTXATTRIB = 352,
     IDENTIFIER = 353,
     USED_IDENTIFIER = 354,
     MASK4 = 355,
     MASK3 = 356,
     MASK2 = 357,
     MASK1 = 358,
     SWIZZLE = 359,
     DOT_DOT = 360,
     DOT = 361
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


   struct asm_instruction *inst;
   struct asm_symbol *sym;
   struct asm_symbol temp_sym;
   struct asm_swizzle_mask swiz_mask;
   struct asm_src_register src_reg;
   struct prog_dst_register dst_reg;
   struct prog_instruction temp_inst;
   char *string;
   unsigned result;
   unsigned attrib;
   int integer;
   float real;
   gl_state_index16 state[STATE_LENGTH];
   int negate;
   struct asm_vector vector;
   enum prog_opcode opcode;

   struct {
      unsigned swz;
      unsigned rgba_valid:1;
      unsigned xyzw_valid:1;
      unsigned negate:1;
   } ext_swizzle;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


extern int
_mesa_program_lexer_lex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
                        void *yyscanner);

static int
yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
      struct asm_parser_state *state)
{
   return _mesa_program_lexer_lex(yylval_param, yylloc_param, state->scanner);
}



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
# if YYENABLE_NLS
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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   352

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  137
/* YYNRULES -- Number of rules.  */
#define YYNRULES  270
/* YYNRULES -- Number of states.  */
#define YYNSTATES  453

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   361

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   111,   108,   112,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   107,
       2,   113,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   109,     2,   110,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   114,     2,   115,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    10,    12,    15,    16,    20,    23,
      24,    27,    30,    32,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    52,    54,    59,    64,    69,    76,    83,
      92,   101,   104,   117,   120,   122,   124,   126,   128,   130,
     132,   134,   136,   138,   140,   142,   144,   151,   154,   157,
     161,   164,   167,   175,   178,   180,   182,   184,   186,   191,
     193,   195,   197,   199,   201,   203,   205,   209,   210,   213,
     216,   218,   220,   222,   224,   226,   228,   230,   232,   234,
     235,   237,   239,   241,   243,   244,   246,   248,   250,   252,
     254,   256,   261,   264,   267,   269,   271,   274,   276,   279,
     284,   289,   291,   293,   295,   298,   300,   303,   305,   307,
     311,   318,   319,   321,   324,   329,   331,   335,   337,   339,
     341,   343,   345,   347,   349,   351,   353,   355,   358,   361,
     364,   367,   370,   373,   376,   379,   382,   385,   388,   391,
     395,   397,   399,   401,   407,   409,   411,   413,   416,   418,
     420,   423,   425,   428,   435,   437,   441,   443,   445,   447,
     449,   451,   453,   455,   457,   462,   464,   466,   468,   470,
     472,   474,   477,   479,   481,   487,   489,   492,   494,   496,
     502,   505,   506,   513,   517,   518,   520,   522,   524,   526,
     528,   531,   533,   535,   538,   543,   548,   549,   553,   555,
     557,   559,   562,   564,   566,   568,   570,   576,   578,   582,
     588,   594,   596,   600,   606,   608,   610,   612,   614,   616,
     618,   620,   622,   624,   628,   634,   642,   652,   655,   658,
     660,   662,   663,   664,   668,   669,   673,   677,   679,   684,
     687,   690,   693,   696,   700,   703,   707,   708,   712,   714,
     716,   717,   719,   721,   722,   724,   726,   727,   729,   731,
     732,   736,   737,   741,   742,   746,   748,   750,   752,   757,
     759
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     117,     0,    -1,   118,   119,   121,    12,    -1,     3,    -1,
       4,    -1,   119,   120,    -1,    -1,     8,   252,   107,    -1,
     121,   122,    -1,    -1,   123,   107,    -1,   161,   107,    -1,
     124,    -1,   125,    -1,   126,    -1,   127,    -1,   128,    -1,
     129,    -1,   130,    -1,   131,    -1,   137,    -1,   132,    -1,
     133,    -1,   134,    -1,    19,   142,   108,   138,    -1,    18,
     141,   108,   140,    -1,    16,   141,   108,   138,    -1,    14,
     141,   108,   138,   108,   138,    -1,    13,   141,   108,   140,
     108,   140,    -1,    17,   141,   108,   140,   108,   140,   108,
     140,    -1,    15,   141,   108,   140,   108,   135,   108,   136,
      -1,    20,   140,    -1,    22,   141,   108,   140,   108,   140,
     108,   140,   108,   135,   108,   136,    -1,    83,   246,    -1,
      84,    -1,    85,    -1,    86,    -1,    87,    -1,    88,    -1,
      89,    -1,    90,    -1,    91,    -1,    92,    -1,    93,    -1,
      94,    -1,    95,    -1,    21,   141,   108,   146,   108,   143,
      -1,   232,   139,    -1,   146,   158,    -1,   232,   146,   159,
      -1,   147,   160,    -1,   155,   157,    -1,   144,   108,   144,
     108,   144,   108,   144,    -1,   232,   145,    -1,    23,    -1,
     252,    -1,    99,    -1,   163,    -1,   148,   109,   149,   110,
      -1,   176,    -1,   239,    -1,    99,    -1,    99,    -1,   150,
      -1,   151,    -1,    23,    -1,   155,   156,   152,    -1,    -1,
     111,   153,    -1,   112,   154,    -1,    23,    -1,    23,    -1,
      99,    -1,   103,    -1,   103,    -1,   103,    -1,   103,    -1,
     100,    -1,   104,    -1,    -1,   100,    -1,   101,    -1,   102,
      -1,   103,    -1,    -1,   162,    -1,   168,    -1,   233,    -1,
     235,    -1,   238,    -1,   251,    -1,     7,    98,   113,   163,
      -1,    96,   164,    -1,    38,   167,    -1,    60,    -1,    53,
      -1,    29,   244,    -1,    37,    -1,    74,   245,    -1,    50,
     109,   166,   110,    -1,    97,   109,   165,   110,    -1,    23,
      -1,    23,    -1,    60,    -1,    29,   244,    -1,    37,    -1,
      74,   245,    -1,   169,    -1,   170,    -1,    10,    98,   172,
      -1,    10,    98,   109,   171,   110,   173,    -1,    -1,    23,
      -1,   113,   175,    -1,   113,   114,   174,   115,    -1,   177,
      -1,   174,   108,   177,    -1,   179,    -1,   216,    -1,   226,
      -1,   179,    -1,   216,    -1,   227,    -1,   178,    -1,   217,
      -1,   226,    -1,   179,    -1,    73,   204,    -1,    73,   180,
      -1,    73,   182,    -1,    73,   185,    -1,    73,   187,    -1,
      73,   194,    -1,    73,   189,    -1,    73,   197,    -1,    73,
     199,    -1,    73,   201,    -1,    73,   203,    -1,    73,   215,
      -1,    47,   243,   181,    -1,   191,    -1,    33,    -1,    69,
      -1,    43,   109,   193,   110,   183,    -1,   192,    -1,    60,
      -1,    26,    -1,    72,   184,    -1,    40,    -1,    32,    -1,
      44,   186,    -1,    25,    -1,   243,    67,    -1,    45,   109,
     193,   110,   243,   188,    -1,   191,    -1,    75,   247,   190,
      -1,    29,    -1,    25,    -1,    31,    -1,    71,    -1,    25,
      -1,    31,    -1,    71,    -1,    23,    -1,    76,   245,   195,
     196,    -1,    35,    -1,    54,    -1,    79,    -1,    80,    -1,
      78,    -1,    77,    -1,    36,   198,    -1,    29,    -1,    56,
      -1,    28,   109,   200,   110,    57,    -1,    23,    -1,    58,
     202,    -1,    70,    -1,    26,    -1,   206,    66,   109,   209,
     110,    -1,   206,   205,    -1,    -1,    66,   109,   209,   105,
     209,   110,    -1,    49,   210,   207,    -1,    -1,   208,    -1,
      41,    -1,    82,    -1,    42,    -1,    23,    -1,    51,   211,
      -1,    63,    -1,    52,    -1,    81,   245,    -1,    55,   109,
     213,   110,    -1,    48,   109,   214,   110,    -1,    -1,   109,
     212,   110,    -1,    23,    -1,    23,    -1,    23,    -1,    30,
      64,    -1,   220,    -1,   223,    -1,   218,    -1,   221,    -1,
      62,    34,   109,   219,   110,    -1,   224,    -1,   224,   105,
     224,    -1,    62,    34,   109,   224,   110,    -1,    62,    46,
     109,   222,   110,    -1,   225,    -1,   225,   105,   225,    -1,
      62,    46,   109,   225,   110,    -1,    23,    -1,    23,    -1,
     228,    -1,   230,    -1,   229,    -1,   230,    -1,   231,    -1,
      24,    -1,    23,    -1,   114,   231,   115,    -1,   114,   231,
     108,   231,   115,    -1,   114,   231,   108,   231,   108,   231,
     115,    -1,   114,   231,   108,   231,   108,   231,   108,   231,
     115,    -1,   232,    24,    -1,   232,    23,    -1,   111,    -1,
     112,    -1,    -1,    -1,    11,   234,   237,    -1,    -1,     5,
     236,   237,    -1,   237,   108,    98,    -1,    98,    -1,     9,
      98,   113,   239,    -1,    65,    60,    -1,    65,    37,    -1,
      65,   240,    -1,    65,    59,    -1,    65,    74,   245,    -1,
      65,    30,    -1,    29,   241,   242,    -1,    -1,   109,    23,
     110,    -1,    39,    -1,    27,    -1,    -1,    61,    -1,    68,
      -1,    -1,    39,    -1,    27,    -1,    -1,    61,    -1,    68,
      -1,    -1,   109,   248,   110,    -1,    -1,   109,   249,   110,
      -1,    -1,   109,   250,   110,    -1,    23,    -1,    23,    -1,
      23,    -1,     6,    98,   113,    99,    -1,    98,    -1,    99,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   295,   295,   298,   306,   318,   319,   322,   346,   347,
     350,   365,   368,   373,   380,   381,   382,   383,   384,   385,
     386,   389,   390,   391,   394,   400,   406,   412,   419,   425,
     432,   476,   483,   527,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   542,   543,   544,   547,   559,   569,   578,
     591,   613,   620,   653,   660,   676,   735,   778,   787,   809,
     819,   823,   852,   871,   871,   873,   880,   892,   893,   894,
     897,   911,   925,   945,   956,   968,   970,   971,   972,   973,
     976,   976,   976,   976,   977,   980,   981,   982,   983,   984,
     985,   988,  1007,  1011,  1017,  1021,  1025,  1029,  1033,  1037,
    1042,  1048,  1059,  1061,  1065,  1069,  1073,  1079,  1079,  1081,
    1099,  1125,  1128,  1143,  1149,  1155,  1156,  1163,  1169,  1175,
    1183,  1189,  1195,  1203,  1209,  1215,  1223,  1224,  1227,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1240,
    1249,  1253,  1257,  1263,  1272,  1276,  1280,  1289,  1293,  1299,
    1305,  1312,  1317,  1325,  1335,  1337,  1345,  1351,  1355,  1359,
    1365,  1369,  1373,  1379,  1390,  1399,  1403,  1408,  1412,  1416,
    1420,  1426,  1433,  1437,  1443,  1451,  1462,  1469,  1473,  1479,
    1488,  1498,  1502,  1520,  1529,  1532,  1538,  1542,  1546,  1552,
    1563,  1568,  1573,  1578,  1583,  1588,  1596,  1599,  1604,  1617,
    1625,  1636,  1644,  1644,  1646,  1646,  1648,  1658,  1663,  1670,
    1680,  1689,  1694,  1701,  1711,  1721,  1733,  1733,  1734,  1734,
    1736,  1746,  1754,  1764,  1772,  1780,  1789,  1800,  1804,  1810,
    1811,  1812,  1815,  1815,  1818,  1818,  1821,  1828,  1837,  1851,
    1860,  1869,  1873,  1882,  1891,  1902,  1909,  1919,  1947,  1956,
    1968,  1971,  1980,  1991,  1992,  1993,  1996,  1997,  1998,  2001,
    2002,  2005,  2006,  2009,  2010,  2013,  2024,  2035,  2046,  2072,
    2073
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ARBvp_10", "ARBfp_10", "ADDRESS",
  "ALIAS", "ATTRIB", "OPTION", "OUTPUT", "PARAM", "TEMP", "END", "BIN_OP",
  "BINSC_OP", "SAMPLE_OP", "SCALAR_OP", "TRI_OP", "VECTOR_OP", "ARL",
  "KIL", "SWZ", "TXD_OP", "INTEGER", "REAL", "AMBIENT", "ATTENUATION",
  "BACK", "CLIP", "COLOR", "DEPTH", "DIFFUSE", "DIRECTION", "EMISSION",
  "ENV", "EYE", "FOG", "FOGCOORD", "FRAGMENT", "FRONT", "HALF", "INVERSE",
  "INVTRANS", "LIGHT", "LIGHTMODEL", "LIGHTPROD", "LOCAL", "MATERIAL",
  "MAT_PROGRAM", "MATRIX", "MATRIXINDEX", "MODELVIEW", "MVP", "NORMAL",
  "OBJECT", "PALETTE", "PARAMS", "PLANE", "POINT_TOK", "POINTSIZE",
  "POSITION", "PRIMARY", "PROGRAM", "PROJECTION", "RANGE", "RESULT", "ROW",
  "SCENECOLOR", "SECONDARY", "SHININESS", "SIZE_TOK", "SPECULAR", "SPOT",
  "STATE", "TEXCOORD", "TEXENV", "TEXGEN", "TEXGEN_Q", "TEXGEN_R",
  "TEXGEN_S", "TEXGEN_T", "TEXTURE", "TRANSPOSE", "TEXTURE_UNIT", "TEX_1D",
  "TEX_2D", "TEX_3D", "TEX_CUBE", "TEX_RECT", "TEX_SHADOW1D",
  "TEX_SHADOW2D", "TEX_SHADOWRECT", "TEX_ARRAY1D", "TEX_ARRAY2D",
  "TEX_ARRAYSHADOW1D", "TEX_ARRAYSHADOW2D", "VERTEX", "VTXATTRIB",
  "IDENTIFIER", "USED_IDENTIFIER", "MASK4", "MASK3", "MASK2", "MASK1",
  "SWIZZLE", "DOT_DOT", "DOT", "';'", "','", "'['", "']'", "'+'", "'-'",
  "'='", "'{'", "'}'", "$accept", "program", "language", "optionSequence",
  "option", "statementSequence", "statement", "instruction",
  "ALU_instruction", "TexInstruction", "ARL_instruction",
  "VECTORop_instruction", "SCALARop_instruction", "BINSCop_instruction",
  "BINop_instruction", "TRIop_instruction", "SAMPLE_instruction",
  "KIL_instruction", "TXD_instruction", "texImageUnit", "texTarget",
  "SWZ_instruction", "scalarSrcReg", "scalarUse", "swizzleSrcReg",
  "maskedDstReg", "maskedAddrReg", "extendedSwizzle", "extSwizComp",
  "extSwizSel", "srcReg", "dstReg", "progParamArray", "progParamArrayMem",
  "progParamArrayAbs", "progParamArrayRel", "addrRegRelOffset",
  "addrRegPosOffset", "addrRegNegOffset", "addrReg", "addrComponent",
  "addrWriteMask", "scalarSuffix", "swizzleSuffix", "optionalMask",
  "namingStatement", "ATTRIB_statement", "attribBinding", "vtxAttribItem",
  "vtxAttribNum", "vtxWeightNum", "fragAttribItem", "PARAM_statement",
  "PARAM_singleStmt", "PARAM_multipleStmt", "optArraySize",
  "paramSingleInit", "paramMultipleInit", "paramMultInitList",
  "paramSingleItemDecl", "paramSingleItemUse", "paramMultipleItem",
  "stateMultipleItem", "stateSingleItem", "stateMaterialItem",
  "stateMatProperty", "stateLightItem", "stateLightProperty",
  "stateSpotProperty", "stateLightModelItem", "stateLModProperty",
  "stateLightProdItem", "stateLProdProperty", "stateTexEnvItem",
  "stateTexEnvProperty", "ambDiffSpecPropertyMaterial",
  "ambDiffSpecPropertyLight", "stateLightNumber", "stateTexGenItem",
  "stateTexGenType", "stateTexGenCoord", "stateFogItem",
  "stateFogProperty", "stateClipPlaneItem", "stateClipPlaneNum",
  "statePointItem", "statePointProperty", "stateMatrixRow",
  "stateMatrixRows", "optMatrixRows", "stateMatrixItem",
  "stateOptMatModifier", "stateMatModifier", "stateMatrixRowNum",
  "stateMatrixName", "stateOptModMatNum", "stateModMatNum",
  "statePaletteMatNum", "stateProgramMatNum", "stateDepthItem",
  "programSingleItem", "programMultipleItem", "progEnvParams",
  "progEnvParamNums", "progEnvParam", "progLocalParams",
  "progLocalParamNums", "progLocalParam", "progEnvParamNum",
  "progLocalParamNum", "paramConstDecl", "paramConstUse",
  "paramConstScalarDecl", "paramConstScalarUse", "paramConstVector",
  "signedFloatConstant", "optionalSign", "TEMP_statement", "@1",
  "ADDRESS_statement", "@2", "varNameList", "OUTPUT_statement",
  "resultBinding", "resultColBinding", "optResultFaceType",
  "optResultColorType", "optFaceType", "optColorType",
  "optTexCoordUnitNum", "optTexImageUnitNum", "optLegacyTexUnitNum",
  "texCoordUnitNum", "texImageUnitNum", "legacyTexUnitNum",
  "ALIAS_statement", "string", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,    59,    44,    91,
      93,    43,    45,    61,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   117,   118,   118,   119,   119,   120,   121,   121,
     122,   122,   123,   123,   124,   124,   124,   124,   124,   124,
     124,   125,   125,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   145,   146,   146,   146,   146,
     147,   147,   148,   149,   149,   150,   151,   152,   152,   152,
     153,   154,   155,   156,   157,   158,   159,   159,   159,   159,
     160,   160,   160,   160,   160,   161,   161,   161,   161,   161,
     161,   162,   163,   163,   164,   164,   164,   164,   164,   164,
     164,   165,   166,   167,   167,   167,   167,   168,   168,   169,
     170,   171,   171,   172,   173,   174,   174,   175,   175,   175,
     176,   176,   176,   177,   177,   177,   178,   178,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   180,
     181,   181,   181,   182,   183,   183,   183,   183,   183,   184,
     185,   186,   186,   187,   188,   189,   190,   191,   191,   191,
     192,   192,   192,   193,   194,   195,   195,   196,   196,   196,
     196,   197,   198,   198,   199,   200,   201,   202,   202,   203,
     204,   205,   205,   206,   207,   207,   208,   208,   208,   209,
     210,   210,   210,   210,   210,   210,   211,   211,   212,   213,
     214,   215,   216,   216,   217,   217,   218,   219,   219,   220,
     221,   222,   222,   223,   224,   225,   226,   226,   227,   227,
     228,   229,   229,   230,   230,   230,   230,   231,   231,   232,
     232,   232,   234,   233,   236,   235,   237,   237,   238,   239,
     239,   239,   239,   239,   239,   240,   241,   241,   241,   241,
     242,   242,   242,   243,   243,   243,   244,   244,   244,   245,
     245,   246,   246,   247,   247,   248,   249,   250,   251,   252,
     252
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     1,     2,     0,     3,     2,     0,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     6,     6,     8,
       8,     2,    12,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     2,     2,     3,
       2,     2,     7,     2,     1,     1,     1,     1,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     4,     2,     2,     1,     1,     2,     1,     2,     4,
       4,     1,     1,     1,     2,     1,     2,     1,     1,     3,
       6,     0,     1,     2,     4,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       1,     1,     1,     5,     1,     1,     1,     2,     1,     1,
       2,     1,     2,     6,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     5,     1,     2,     1,     1,     5,
       2,     0,     6,     3,     0,     1,     1,     1,     1,     1,
       2,     1,     1,     2,     4,     4,     0,     3,     1,     1,
       1,     2,     1,     1,     1,     1,     5,     1,     3,     5,
       5,     1,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     7,     9,     2,     2,     1,
       1,     0,     0,     3,     0,     3,     3,     1,     4,     2,
       2,     2,     2,     3,     2,     3,     0,     3,     1,     1,
       0,     1,     1,     0,     1,     1,     0,     1,     1,     0,
       3,     0,     3,     0,     3,     1,     1,     1,     4,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     3,     4,     0,     6,     1,     9,     0,     5,     0,
     269,   270,     0,   234,     0,     0,     0,     0,   232,     2,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       8,     0,    12,    13,    14,    15,    16,    17,    18,    19,
      21,    22,    23,    20,     0,    85,    86,   107,   108,    87,
      88,    89,    90,     7,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    84,    60,     0,     0,     0,     0,     0,
      72,     0,     0,   229,   230,    31,     0,     0,     0,    10,
      11,   237,   235,     0,     0,     0,   111,   231,   109,   233,
     246,   244,   240,   242,   239,   259,   241,   231,    80,    81,
      82,    83,    50,   231,   231,   231,   231,   231,   231,    74,
      51,   222,   221,     0,     0,     0,     0,    56,   231,    79,
       0,    57,    59,   120,   121,   202,   203,   122,   218,   219,
       0,   231,     0,   268,    91,   238,   112,     0,   113,   117,
     118,   119,   216,   217,   220,     0,   249,   248,     0,   250,
       0,   243,     0,     0,     0,     0,    26,     0,    25,    24,
     256,   105,   103,   259,    93,     0,     0,     0,     0,     0,
       0,   253,     0,   253,     0,     0,   263,   259,   128,   129,
     130,   131,   133,   132,   134,   135,   136,   137,     0,   138,
     256,    97,     0,    95,    94,   259,     0,    92,     0,    77,
      76,    78,    49,     0,     0,     0,   236,     0,   228,   227,
       0,   251,   252,   245,   265,     0,   231,   231,    47,     0,
       0,   231,   257,   258,   104,   106,     0,     0,     0,   201,
     172,   173,   171,     0,   151,   255,   254,   150,     0,     0,
       0,     0,   196,   192,     0,   191,   259,   184,   178,   177,
     176,     0,     0,     0,     0,    96,     0,    98,     0,   231,
     223,    65,     0,    63,    64,     0,   231,   231,     0,   110,
     247,   260,    28,    27,    75,    48,   261,     0,     0,   214,
       0,   215,     0,   175,     0,   163,     0,   152,     0,   157,
     158,   141,   142,   159,   139,   140,     0,     0,   190,     0,
     193,   186,   188,   187,   183,   185,   267,     0,   156,   155,
     165,   166,     0,     0,   102,     0,   101,     0,     0,    58,
      73,    67,    46,     0,     0,     0,   231,     0,    33,     0,
     231,   209,   213,     0,     0,   253,   200,     0,   198,     0,
     199,     0,   264,   170,   169,   167,   168,   164,   189,     0,
      99,   100,   231,   224,     0,     0,    66,   231,    54,    53,
      55,   231,     0,     0,     0,   115,   123,   126,   124,   204,
     205,   125,   266,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    30,    29,   174,   160,
     146,   161,   148,   145,   162,     0,   143,   144,     0,   195,
     197,   194,   179,     0,    70,    68,    71,    69,     0,     0,
       0,     0,   127,   181,   231,   114,   262,   149,   147,   153,
     154,   231,   225,   231,     0,     0,     0,     0,   180,   116,
       0,     0,     0,     0,   207,     0,   211,     0,   226,   231,
       0,   206,     0,   210,     0,     0,    52,    32,   208,   212,
       0,     0,   182
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     6,     8,     9,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,   277,
     386,    43,   153,   218,    75,    62,    71,   322,   323,   359,
     119,    63,   120,   262,   263,   264,   356,   405,   407,    72,
     321,   110,   275,   202,   102,    44,    45,   121,   197,   317,
     315,   164,    46,    47,    48,   137,    88,   269,   364,   138,
     122,   365,   366,   123,   178,   294,   179,   396,   418,   180,
     237,   181,   419,   182,   309,   295,   397,   286,   183,   312,
     347,   184,   232,   185,   284,   186,   250,   187,   412,   428,
     188,   304,   305,   349,   247,   298,   339,   341,   337,   189,
     124,   368,   369,   433,   125,   370,   435,   126,   280,   282,
     371,   127,   142,   128,   129,   144,    76,    49,    59,    50,
      54,    82,    51,    64,    96,   149,   213,   238,   224,   151,
     328,   252,   215,   373,   307,    52,    12
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -403
static const yytype_int16 yypact[] =
{
     117,  -403,  -403,    35,  -403,  -403,    18,    -9,  -403,   177,
    -403,  -403,   -52,  -403,   -29,   -27,    14,    34,  -403,  -403,
     -32,   -32,   -32,   -32,   -32,   -32,    11,    47,   -32,   -32,
    -403,    31,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,    43,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,    37,    58,    65,    72,    44,    37,
      86,  -403,   118,    67,  -403,   119,   120,   121,   122,   123,
    -403,   124,    77,  -403,  -403,  -403,   -16,   125,   126,  -403,
    -403,  -403,   127,   137,   -21,   160,   219,   -35,  -403,   127,
     -14,  -403,  -403,  -403,  -403,   134,  -403,    47,  -403,  -403,
    -403,  -403,  -403,    47,    47,    47,    47,    47,    47,  -403,
    -403,  -403,  -403,    -1,    80,    64,     0,   135,    47,    61,
     136,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
     -16,    47,   148,  -403,  -403,  -403,  -403,   138,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,   153,  -403,  -403,   224,     4,
     226,  -403,   142,   143,   -16,   145,  -403,   146,  -403,  -403,
      38,  -403,  -403,   134,  -403,   147,   149,   150,   188,    12,
     151,    78,   152,    92,   100,     8,   154,   134,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,   189,  -403,
      38,  -403,   155,  -403,  -403,   134,   157,  -403,    26,  -403,
    -403,  -403,  -403,    -3,   159,   163,  -403,   161,  -403,  -403,
     158,  -403,  -403,  -403,  -403,   162,    47,    47,  -403,   170,
     174,    47,  -403,  -403,  -403,  -403,   239,   252,   253,  -403,
    -403,  -403,  -403,   254,  -403,  -403,  -403,  -403,   211,   254,
      33,   171,   172,  -403,   173,  -403,   134,     6,  -403,  -403,
    -403,   256,   255,     9,   176,  -403,   260,  -403,   263,    47,
    -403,  -403,   178,  -403,  -403,   184,    47,    47,   175,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,   181,   183,   185,  -403,
     182,  -403,   186,  -403,   187,  -403,   190,  -403,   192,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,   271,   272,  -403,   275,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,   193,  -403,  -403,
    -403,  -403,   144,   276,  -403,   194,  -403,   195,    28,  -403,
    -403,   106,  -403,   199,   -12,   200,   -30,   278,  -403,   116,
      47,  -403,  -403,   257,   102,    92,  -403,   201,  -403,   202,
    -403,   203,  -403,  -403,  -403,  -403,  -403,  -403,  -403,   205,
    -403,  -403,    47,  -403,   286,   287,  -403,    47,  -403,  -403,
    -403,    47,    91,    64,    39,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,   206,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,   285,  -403,  -403,    20,  -403,
    -403,  -403,  -403,    41,  -403,  -403,  -403,  -403,   210,   212,
     213,   214,  -403,   258,   -30,  -403,  -403,  -403,  -403,  -403,
    -403,    47,  -403,    47,   174,   239,   252,   216,  -403,  -403,
     204,   218,   222,   221,   227,   223,   230,   276,  -403,    47,
     116,  -403,   239,  -403,   252,   -49,  -403,  -403,  -403,  -403,
     276,   228,  -403
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -103,
    -101,  -403,   -99,  -403,   -92,   191,  -403,  -403,  -338,  -403,
    -100,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,   139,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,   259,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,   -77,  -403,   -86,  -403,  -403,  -403,  -403,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,   -58,  -403,   105,  -403,  -403,
    -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -403,
     -22,  -403,  -403,  -388,  -403,  -403,  -403,  -403,  -403,  -403,
     261,  -403,  -403,  -403,  -403,  -403,  -403,  -403,  -402,  -374,
     262,  -403,  -403,  -403,   -85,  -115,   -87,  -403,  -403,  -403,
    -403,   288,  -403,   265,  -403,  -403,  -403,  -169,   156,  -153,
    -403,  -403,  -403,  -403,  -403,  -403,    21
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -63
static const yytype_int16 yytable[] =
{
     145,   139,   143,   198,   240,   152,   156,   111,   112,   159,
     225,   358,   155,   146,   157,   158,   154,   113,   154,   408,
     261,   154,   113,   434,   253,   147,     7,   114,   160,   190,
     204,   145,   362,    60,   248,     5,   161,   191,   115,   205,
     448,   230,   257,   363,   310,   289,   114,   301,   302,   445,
     192,   290,   436,   193,   219,    53,   450,   115,   289,   162,
     194,   402,   451,   311,   290,   211,   291,    61,   231,    55,
     449,    56,   212,   163,   195,   116,    73,    74,   249,   118,
     116,    73,    74,   117,   118,   431,    10,    11,   303,    10,
      11,   293,   167,   300,   168,   148,    70,   196,   118,   222,
     169,   446,   292,   234,   293,   235,   223,   170,   171,   172,
      70,   173,    57,   174,   165,    90,    91,   236,   273,   235,
       1,     2,   175,    92,   272,   410,   166,   389,   390,   278,
     154,   236,    58,   391,   259,    81,   352,   411,    79,   176,
     177,   260,   392,   353,   318,    93,    94,   414,   241,   421,
      80,   242,   243,    86,   415,   244,   422,    87,    73,    74,
      95,   199,   393,   245,   200,   201,   398,    98,    99,   100,
     101,    83,   145,   394,   395,   325,   208,   209,    84,   324,
     109,   246,    13,    14,    15,    85,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,    65,    66,    67,    68,    69,   354,   355,    77,
      78,   343,   344,   345,   346,    60,    97,   103,   104,   105,
     106,   107,   108,   130,   131,   132,   133,   403,   387,   145,
     367,   143,   136,   150,   -62,   203,   206,   210,   207,   214,
     216,   217,   229,   220,   221,   254,   226,   276,   227,   228,
     233,   239,   279,   251,   256,   145,   258,   266,   270,   409,
     324,   267,   271,   274,   268,   281,   283,   285,   287,   306,
     296,   297,   299,   314,   308,   313,   316,   320,   319,   326,
     327,   329,   331,   330,   336,   338,   332,   333,   340,   348,
     334,   372,   335,   342,   350,   351,   430,   357,   361,   404,
     406,   399,   400,   401,   388,   402,   416,   417,   423,   438,
     424,   432,   425,   426,   427,   437,   439,   145,   367,   143,
     440,   441,   442,   443,   145,   444,   324,   429,   452,   447,
     420,   413,   265,   134,   288,   360,   255,    89,   140,   141,
     135,     0,   324
};

static const yytype_int16 yycheck[] =
{
      87,    87,    87,   118,   173,    97,   105,    23,    24,   108,
     163,    23,   104,    27,   106,   107,   103,    38,   105,   357,
      23,   108,    38,   425,   177,    39,     8,    62,    29,    29,
     130,   118,    62,    65,    26,     0,    37,    37,    73,   131,
     442,    29,   195,    73,    35,    25,    62,    41,    42,   437,
      50,    31,   426,    53,   154,   107,   105,    73,    25,    60,
      60,   110,   450,    54,    31,    61,    33,    99,    56,    98,
     444,    98,    68,    74,    74,    96,   111,   112,    70,   114,
      96,   111,   112,    99,   114,   423,    98,    99,    82,    98,
      99,    71,    28,   246,    30,   109,    99,    97,   114,    61,
      36,   439,    69,    25,    71,    27,    68,    43,    44,    45,
      99,    47,    98,    49,    34,    29,    30,    39,   217,    27,
       3,     4,    58,    37,   216,    34,    46,    25,    26,   221,
     217,    39,    98,    31,   108,    98,   108,    46,   107,    75,
      76,   115,    40,   115,   259,    59,    60,   108,    48,   108,
     107,    51,    52,   109,   115,    55,   115,   113,   111,   112,
      74,   100,    60,    63,   103,   104,   335,   100,   101,   102,
     103,   113,   259,    71,    72,   267,    23,    24,   113,   266,
     103,    81,     5,     6,     7,   113,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    21,    22,    23,    24,    25,   111,   112,    28,
      29,    77,    78,    79,    80,    65,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,    99,   352,   330,   326,
     326,   326,    23,   109,   109,   109,    98,    23,   110,    23,
     108,   108,    64,   108,   108,    66,   109,    83,   109,   109,
     109,   109,    23,   109,   109,   352,   109,   108,   110,   361,
     357,   108,   110,   103,   113,    23,    23,    23,    67,    23,
     109,   109,   109,    23,    29,   109,    23,   103,   110,   114,
     109,   108,   110,   108,    23,    23,   110,   110,    23,    23,
     110,    23,   110,   110,   110,   110,   421,   108,   108,    23,
      23,   110,   110,   110,    57,   110,   110,    32,   108,   115,
     108,   424,   109,   109,    66,   109,   108,   414,   414,   414,
     108,   110,   105,   110,   421,   105,   423,   414,   110,   440,
     398,   363,   203,    84,   239,   324,   190,    59,    87,    87,
      85,    -1,   439
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,   117,   118,     0,   119,     8,   120,   121,
      98,    99,   252,     5,     6,     7,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   137,   161,   162,   168,   169,   170,   233,
     235,   238,   251,   107,   236,    98,    98,    98,    98,   234,
      65,    99,   141,   147,   239,   141,   141,   141,   141,   141,
      99,   142,   155,   111,   112,   140,   232,   141,   141,   107,
     107,    98,   237,   113,   113,   113,   109,   113,   172,   237,
      29,    30,    37,    59,    60,    74,   240,   108,   100,   101,
     102,   103,   160,   108,   108,   108,   108,   108,   108,   103,
     157,    23,    24,    38,    62,    73,    96,    99,   114,   146,
     148,   163,   176,   179,   216,   220,   223,   227,   229,   230,
     108,   108,   108,    99,   163,   239,    23,   171,   175,   179,
     216,   226,   228,   230,   231,   232,    27,    39,   109,   241,
     109,   245,   140,   138,   232,   140,   138,   140,   140,   138,
      29,    37,    60,    74,   167,    34,    46,    28,    30,    36,
      43,    44,    45,    47,    49,    58,    75,    76,   180,   182,
     185,   187,   189,   194,   197,   199,   201,   203,   206,   215,
      29,    37,    50,    53,    60,    74,    97,   164,   231,   100,
     103,   104,   159,   109,   146,   140,    98,   110,    23,    24,
      23,    61,    68,   242,    23,   248,   108,   108,   139,   146,
     108,   108,    61,    68,   244,   245,   109,   109,   109,    64,
      29,    56,   198,   109,    25,    27,    39,   186,   243,   109,
     243,    48,    51,    52,    55,    63,    81,   210,    26,    70,
     202,   109,   247,   245,    66,   244,   109,   245,   109,   108,
     115,    23,   149,   150,   151,   155,   108,   108,   113,   173,
     110,   110,   140,   138,   103,   158,    83,   135,   140,    23,
     224,    23,   225,    23,   200,    23,   193,    67,   193,    25,
      31,    33,    69,    71,   181,   191,   109,   109,   211,   109,
     245,    41,    42,    82,   207,   208,    23,   250,    29,   190,
      35,    54,   195,   109,    23,   166,    23,   165,   231,   110,
     103,   156,   143,   144,   232,   140,   114,   109,   246,   108,
     108,   110,   110,   110,   110,   110,    23,   214,    23,   212,
      23,   213,   110,    77,    78,    79,    80,   196,    23,   209,
     110,   110,   108,   115,   111,   112,   152,   108,    23,   145,
     252,   108,    62,    73,   174,   177,   178,   179,   217,   218,
     221,   226,    23,   249,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,   136,   140,    57,    25,
      26,    31,    40,    60,    71,    72,   183,   192,   243,   110,
     110,   110,   110,   231,    23,   153,    23,   154,   144,   140,
      34,    46,   204,   206,   108,   115,   110,    32,   184,   188,
     191,   108,   115,   108,   108,   109,   109,    66,   205,   177,
     231,   144,   135,   219,   224,   222,   225,   109,   115,   108,
     108,   110,   105,   110,   105,   209,   144,   136,   224,   225,
     105,   209,   110
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
      yyerror (&yylloc, state, YY_("syntax error: cannot back up")); \
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
# if YYLTYPE_IS_TRIVIAL
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
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, state)
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
		  Type, Value, Location, state); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct asm_parser_state *state)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct asm_parser_state *state;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (state);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct asm_parser_state *state)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct asm_parser_state *state;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, struct asm_parser_state *state)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, state)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    struct asm_parser_state *state;
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, state); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, struct asm_parser_state *state)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, state)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    struct asm_parser_state *state;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (state);

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
int yyparse (struct asm_parser_state *state);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
yyparse (struct asm_parser_state *state)
#else
int
yyparse (state)
    struct asm_parser_state *state;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

    {
	   if (state->prog->Target != GL_VERTEX_PROGRAM_ARB) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid fragment program header");

	   }
	   state->mode = ARB_vertex;
	;}
    break;

  case 4:

    {
	   if (state->prog->Target != GL_FRAGMENT_PROGRAM_ARB) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid vertex program header");
	   }
	   state->mode = ARB_fragment;

	   state->option.TexRect =
	      (state->ctx->Extensions.NV_texture_rectangle != GL_FALSE);
	;}
    break;

  case 7:

    {
	   int valid = 0;

	   if (state->mode == ARB_vertex) {
	      valid = _mesa_ARBvp_parse_option(state, (yyvsp[(2) - (3)].string));
	   } else if (state->mode == ARB_fragment) {
	      valid = _mesa_ARBfp_parse_option(state, (yyvsp[(2) - (3)].string));
	   }


	   free((yyvsp[(2) - (3)].string));

	   if (!valid) {
	      const char *const err_str = (state->mode == ARB_vertex)
		 ? "invalid ARB vertex program option"
		 : "invalid ARB fragment program option";

	      yyerror(& (yylsp[(2) - (3)]), state, err_str);
	      YYERROR;
	   }
	;}
    break;

  case 10:

    {
	   if ((yyvsp[(1) - (2)].inst) != NULL) {
	      if (state->inst_tail == NULL) {
		 state->inst_head = (yyvsp[(1) - (2)].inst);
	      } else {
		 state->inst_tail->next = (yyvsp[(1) - (2)].inst);
	      }

	      state->inst_tail = (yyvsp[(1) - (2)].inst);
	      (yyvsp[(1) - (2)].inst)->next = NULL;

              state->prog->arb.NumInstructions++;
	   }
	;}
    break;

  case 12:

    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
           state->prog->arb.NumAluInstructions++;
	;}
    break;

  case 13:

    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
           state->prog->arb.NumTexInstructions++;
	;}
    break;

  case 24:

    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_ARL, & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 25:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 26:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 27:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 28:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 29:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (8)].temp_inst), & (yyvsp[(2) - (8)].dst_reg), & (yyvsp[(4) - (8)].src_reg), & (yyvsp[(6) - (8)].src_reg), & (yyvsp[(8) - (8)].src_reg));
	;}
    break;

  case 30:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (8)].temp_inst), & (yyvsp[(2) - (8)].dst_reg), & (yyvsp[(4) - (8)].src_reg), NULL, NULL);
	   if ((yyval.inst) != NULL) {
	      const GLbitfield tex_mask = (1U << (yyvsp[(6) - (8)].integer));
	      GLbitfield shadow_tex = 0;
	      GLbitfield target_mask = 0;


	      (yyval.inst)->Base.TexSrcUnit = (yyvsp[(6) - (8)].integer);

	      if ((yyvsp[(8) - (8)].integer) < 0) {
		 shadow_tex = tex_mask;

		 (yyval.inst)->Base.TexSrcTarget = -(yyvsp[(8) - (8)].integer);
		 (yyval.inst)->Base.TexShadow = 1;
	      } else {
		 (yyval.inst)->Base.TexSrcTarget = (yyvsp[(8) - (8)].integer);
	      }

	      target_mask = (1U << (yyval.inst)->Base.TexSrcTarget);

	      /* If this texture unit was previously accessed and that access
	       * had a different texture target, generate an error.
	       *
	       * If this texture unit was previously accessed and that access
	       * had a different shadow mode, generate an error.
	       */
	      if ((state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] != 0)
		  && ((state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] != target_mask)
		      || ((state->prog->ShadowSamplers & tex_mask)
			  != shadow_tex))) {
		 yyerror(& (yylsp[(8) - (8)]), state,
			 "multiple targets used on one texture image unit");
		 YYERROR;
	      }


	      state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] |= target_mask;
	      state->prog->ShadowSamplers |= shadow_tex;
	   }
	;}
    break;

  case 31:

    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_KIL, NULL, & (yyvsp[(2) - (2)].src_reg), NULL, NULL);
	   state->fragment.UsesKill = 1;
	;}
    break;

  case 32:

    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (12)].temp_inst), & (yyvsp[(2) - (12)].dst_reg), & (yyvsp[(4) - (12)].src_reg), & (yyvsp[(6) - (12)].src_reg), & (yyvsp[(8) - (12)].src_reg));
	   if ((yyval.inst) != NULL) {
	      const GLbitfield tex_mask = (1U << (yyvsp[(10) - (12)].integer));
	      GLbitfield shadow_tex = 0;
	      GLbitfield target_mask = 0;


	      (yyval.inst)->Base.TexSrcUnit = (yyvsp[(10) - (12)].integer);

	      if ((yyvsp[(12) - (12)].integer) < 0) {
		 shadow_tex = tex_mask;

		 (yyval.inst)->Base.TexSrcTarget = -(yyvsp[(12) - (12)].integer);
		 (yyval.inst)->Base.TexShadow = 1;
	      } else {
		 (yyval.inst)->Base.TexSrcTarget = (yyvsp[(12) - (12)].integer);
	      }

	      target_mask = (1U << (yyval.inst)->Base.TexSrcTarget);

	      /* If this texture unit was previously accessed and that access
	       * had a different texture target, generate an error.
	       *
	       * If this texture unit was previously accessed and that access
	       * had a different shadow mode, generate an error.
	       */
	      if ((state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] != 0)
		  && ((state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] != target_mask)
		      || ((state->prog->ShadowSamplers & tex_mask)
			  != shadow_tex))) {
		 yyerror(& (yylsp[(12) - (12)]), state,
			 "multiple targets used on one texture image unit");
		 YYERROR;
	      }


	      state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] |= target_mask;
	      state->prog->ShadowSamplers |= shadow_tex;
	   }
	;}
    break;

  case 33:

    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 34:

    { (yyval.integer) = TEXTURE_1D_INDEX; ;}
    break;

  case 35:

    { (yyval.integer) = TEXTURE_2D_INDEX; ;}
    break;

  case 36:

    { (yyval.integer) = TEXTURE_3D_INDEX; ;}
    break;

  case 37:

    { (yyval.integer) = TEXTURE_CUBE_INDEX; ;}
    break;

  case 38:

    { (yyval.integer) = TEXTURE_RECT_INDEX; ;}
    break;

  case 39:

    { (yyval.integer) = -TEXTURE_1D_INDEX; ;}
    break;

  case 40:

    { (yyval.integer) = -TEXTURE_2D_INDEX; ;}
    break;

  case 41:

    { (yyval.integer) = -TEXTURE_RECT_INDEX; ;}
    break;

  case 42:

    { (yyval.integer) = TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 43:

    { (yyval.integer) = TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 44:

    { (yyval.integer) = -TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 45:

    { (yyval.integer) = -TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 46:

    {
	   /* FIXME: Is this correct?  Should the extenedSwizzle be applied
	    * FIXME: to the existing swizzle?
	    */
	   (yyvsp[(4) - (6)].src_reg).Base.Swizzle = (yyvsp[(6) - (6)].swiz_mask).swizzle;
	   (yyvsp[(4) - (6)].src_reg).Base.Negate = (yyvsp[(6) - (6)].swiz_mask).mask;

	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), NULL, NULL);
	;}
    break;

  case 47:

    {
	   (yyval.src_reg) = (yyvsp[(2) - (2)].src_reg);

	   if ((yyvsp[(1) - (2)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }
	;}
    break;

  case 48:

    {
	   (yyval.src_reg) = (yyvsp[(1) - (2)].src_reg);

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(2) - (2)].swiz_mask).swizzle);
	;}
    break;

  case 49:

    {
	   (yyval.src_reg) = (yyvsp[(2) - (3)].src_reg);

	   if ((yyvsp[(1) - (3)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(3) - (3)].swiz_mask).swizzle);
	;}
    break;

  case 50:

    {
	   (yyval.dst_reg) = (yyvsp[(1) - (2)].dst_reg);
	   (yyval.dst_reg).WriteMask = (yyvsp[(2) - (2)].swiz_mask).mask;

	   if ((yyval.dst_reg).File == PROGRAM_OUTPUT) {
	      /* Technically speaking, this should check that it is in
	       * vertex program mode.  However, PositionInvariant can never be
	       * set in fragment program mode, so it is somewhat irrelevant.
	       */
	      if (state->option.PositionInvariant
	       && ((yyval.dst_reg).Index == VARYING_SLOT_POS)) {
		 yyerror(& (yylsp[(1) - (2)]), state, "position-invariant programs cannot "
			 "write position");
		 YYERROR;
	      }

              state->prog->info.outputs_written |= BITFIELD64_BIT((yyval.dst_reg).Index);
	   }
	;}
    break;

  case 51:

    {
	   set_dst_reg(& (yyval.dst_reg), PROGRAM_ADDRESS, 0);
	   (yyval.dst_reg).WriteMask = (yyvsp[(2) - (2)].swiz_mask).mask;
	;}
    break;

  case 52:

    {
	   const unsigned xyzw_valid =
	      ((yyvsp[(1) - (7)].ext_swizzle).xyzw_valid << 0)
	      | ((yyvsp[(3) - (7)].ext_swizzle).xyzw_valid << 1)
	      | ((yyvsp[(5) - (7)].ext_swizzle).xyzw_valid << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).xyzw_valid << 3);
	   const unsigned rgba_valid =
	      ((yyvsp[(1) - (7)].ext_swizzle).rgba_valid << 0)
	      | ((yyvsp[(3) - (7)].ext_swizzle).rgba_valid << 1)
	      | ((yyvsp[(5) - (7)].ext_swizzle).rgba_valid << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).rgba_valid << 3);

	   /* All of the swizzle components have to be valid in either RGBA
	    * or XYZW.  Note that 0 and 1 are valid in both, so both masks
	    * can have some bits set.
	    *
	    * We somewhat deviate from the spec here.  It would be really hard
	    * to figure out which component is the error, and there probably
	    * isn't a lot of benefit.
	    */
	   if ((rgba_valid != 0x0f) && (xyzw_valid != 0x0f)) {
	      yyerror(& (yylsp[(1) - (7)]), state, "cannot combine RGBA and XYZW swizzle "
		      "components");
	      YYERROR;
	   }

	   (yyval.swiz_mask).swizzle = MAKE_SWIZZLE4((yyvsp[(1) - (7)].ext_swizzle).swz, (yyvsp[(3) - (7)].ext_swizzle).swz, (yyvsp[(5) - (7)].ext_swizzle).swz, (yyvsp[(7) - (7)].ext_swizzle).swz);
	   (yyval.swiz_mask).mask = ((yyvsp[(1) - (7)].ext_swizzle).negate) | ((yyvsp[(3) - (7)].ext_swizzle).negate << 1) | ((yyvsp[(5) - (7)].ext_swizzle).negate << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).negate << 3);
	;}
    break;

  case 53:

    {
	   (yyval.ext_swizzle) = (yyvsp[(2) - (2)].ext_swizzle);
	   (yyval.ext_swizzle).negate = ((yyvsp[(1) - (2)].negate)) ? 1 : 0;
	;}
    break;

  case 54:

    {
	   if (((yyvsp[(1) - (1)].integer) != 0) && ((yyvsp[(1) - (1)].integer) != 1)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	   }

	   (yyval.ext_swizzle).swz = ((yyvsp[(1) - (1)].integer) == 0) ? SWIZZLE_ZERO : SWIZZLE_ONE;
           (yyval.ext_swizzle).negate = 0;

	   /* 0 and 1 are valid for both RGBA swizzle names and XYZW
	    * swizzle names.
	    */
	   (yyval.ext_swizzle).xyzw_valid = 1;
	   (yyval.ext_swizzle).rgba_valid = 1;
	;}
    break;

  case 55:

    {
	   char s;

	   if (strlen((yyvsp[(1) - (1)].string)) > 1) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	   }

	   s = (yyvsp[(1) - (1)].string)[0];
	   free((yyvsp[(1) - (1)].string));

           (yyval.ext_swizzle).rgba_valid = 0;
           (yyval.ext_swizzle).xyzw_valid = 0;
           (yyval.ext_swizzle).negate = 0;

	   switch (s) {
	   case 'x':
	      (yyval.ext_swizzle).swz = SWIZZLE_X;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'y':
	      (yyval.ext_swizzle).swz = SWIZZLE_Y;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'z':
	      (yyval.ext_swizzle).swz = SWIZZLE_Z;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'w':
	      (yyval.ext_swizzle).swz = SWIZZLE_W;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;

	   case 'r':
	      (yyval.ext_swizzle).swz = SWIZZLE_X;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'g':
	      (yyval.ext_swizzle).swz = SWIZZLE_Y;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'b':
	      (yyval.ext_swizzle).swz = SWIZZLE_Z;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'a':
	      (yyval.ext_swizzle).swz = SWIZZLE_W;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;

	   default:
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	      break;
	   }
	;}
    break;

  case 56:

    {
	   struct asm_symbol *const s = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(1) - (1)].string));

	   free((yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_param) && (s->type != at_temp)
		      && (s->type != at_attrib)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type == at_param) && s->param_is_array) {
	      yyerror(& (yylsp[(1) - (1)]), state, "non-array access to array PARAM");
	      YYERROR;
	   }

	   init_src_reg(& (yyval.src_reg));
	   switch (s->type) {
	   case at_temp:
	      set_src_reg(& (yyval.src_reg), PROGRAM_TEMPORARY, s->temp_binding);
	      break;
	   case at_param:
              set_src_reg_swz(& (yyval.src_reg), s->param_binding_type,
                              s->param_binding_begin,
                              s->param_binding_swizzle);
	      break;
	   case at_attrib:
	      set_src_reg(& (yyval.src_reg), PROGRAM_INPUT, s->attrib_binding);
              state->prog->info.inputs_read |= BITFIELD64_BIT((yyval.src_reg).Base.Index);

	      if (!validate_inputs(& (yylsp[(1) - (1)]), state)) {
		 YYERROR;
	      }
	      break;

	   default:
	      YYERROR;
	      break;
	   }
	;}
    break;

  case 57:

    {
	   set_src_reg(& (yyval.src_reg), PROGRAM_INPUT, (yyvsp[(1) - (1)].attrib));
           state->prog->info.inputs_read |= BITFIELD64_BIT((yyval.src_reg).Base.Index);

	   if (!validate_inputs(& (yylsp[(1) - (1)]), state)) {
	      YYERROR;
	   }
	;}
    break;

  case 58:

    {
	   if (! (yyvsp[(3) - (4)].src_reg).Base.RelAddr
	       && ((unsigned) (yyvsp[(3) - (4)].src_reg).Base.Index >= (yyvsp[(1) - (4)].sym)->param_binding_length)) {
	      yyerror(& (yylsp[(3) - (4)]), state, "out of bounds array access");
	      YYERROR;
	   }

	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = (yyvsp[(1) - (4)].sym)->param_binding_type;

	   if ((yyvsp[(3) - (4)].src_reg).Base.RelAddr) {
              state->prog->arb.IndirectRegisterFiles |= (1 << (yyval.src_reg).Base.File);
	      (yyvsp[(1) - (4)].sym)->param_accessed_indirectly = 1;

	      (yyval.src_reg).Base.RelAddr = 1;
	      (yyval.src_reg).Base.Index = (yyvsp[(3) - (4)].src_reg).Base.Index;
	      (yyval.src_reg).Symbol = (yyvsp[(1) - (4)].sym);
	   } else {
	      (yyval.src_reg).Base.Index = (yyvsp[(1) - (4)].sym)->param_binding_begin + (yyvsp[(3) - (4)].src_reg).Base.Index;
	   }
	;}
    break;

  case 59:

    {
           gl_register_file file = ((yyvsp[(1) - (1)].temp_sym).name != NULL)
	      ? (yyvsp[(1) - (1)].temp_sym).param_binding_type
	      : PROGRAM_CONSTANT;
           set_src_reg_swz(& (yyval.src_reg), file, (yyvsp[(1) - (1)].temp_sym).param_binding_begin,
                           (yyvsp[(1) - (1)].temp_sym).param_binding_swizzle);
	;}
    break;

  case 60:

    {
	   set_dst_reg(& (yyval.dst_reg), PROGRAM_OUTPUT, (yyvsp[(1) - (1)].result));
	;}
    break;

  case 61:

    {
	   struct asm_symbol *const s = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(1) - (1)].string));

	   free((yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_output) && (s->type != at_temp)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   }

	   switch (s->type) {
	   case at_temp:
	      set_dst_reg(& (yyval.dst_reg), PROGRAM_TEMPORARY, s->temp_binding);
	      break;
	   case at_output:
	      set_dst_reg(& (yyval.dst_reg), PROGRAM_OUTPUT, s->output_binding);
	      break;
	   default:
	      set_dst_reg(& (yyval.dst_reg), s->param_binding_type, s->param_binding_begin);
	      break;
	   }
	;}
    break;

  case 62:

    {
	   struct asm_symbol *const s = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(1) - (1)].string));

	   free((yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_param) || !s->param_is_array) {
	      yyerror(& (yylsp[(1) - (1)]), state, "array access to non-PARAM variable");
	      YYERROR;
	   } else {
	      (yyval.sym) = s;
	   }
	;}
    break;

  case 65:

    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 66:

    {
	   /* FINISHME: Add support for multiple address registers.
	    */
	   /* FINISHME: Add support for 4-component address registers.
	    */
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.RelAddr = 1;
	   (yyval.src_reg).Base.Index = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 67:

    { (yyval.integer) = 0; ;}
    break;

  case 68:

    { (yyval.integer) = (yyvsp[(2) - (2)].integer); ;}
    break;

  case 69:

    { (yyval.integer) = -(yyvsp[(2) - (2)].integer); ;}
    break;

  case 70:

    {
	   if (((yyvsp[(1) - (1)].integer) < 0) || ((yyvsp[(1) - (1)].integer) > (state->limits->MaxAddressOffset - 1))) {
              char s[100];
              snprintf(s, sizeof(s),
                             "relative address offset too large (%d)", (yyvsp[(1) - (1)].integer));
	      yyerror(& (yylsp[(1) - (1)]), state, s);
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 71:

    {
	   if (((yyvsp[(1) - (1)].integer) < 0) || ((yyvsp[(1) - (1)].integer) > state->limits->MaxAddressOffset)) {
              char s[100];
              snprintf(s, sizeof(s),
                             "relative address offset too large (%d)", (yyvsp[(1) - (1)].integer));
	      yyerror(& (yylsp[(1) - (1)]), state, s);
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 72:

    {
	   struct asm_symbol *const s = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(1) - (1)].string));

	   free((yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid array member");
	      YYERROR;
	   } else if (s->type != at_address) {
	      yyerror(& (yylsp[(1) - (1)]), state,
		      "invalid variable for indexed array access");
	      YYERROR;
	   } else {
	      (yyval.sym) = s;
	   }
	;}
    break;

  case 73:

    {
	   if ((yyvsp[(1) - (1)].swiz_mask).mask != WRITEMASK_X) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid address component selector");
	      YYERROR;
	   } else {
	      (yyval.swiz_mask) = (yyvsp[(1) - (1)].swiz_mask);
	   }
	;}
    break;

  case 74:

    {
	   if ((yyvsp[(1) - (1)].swiz_mask).mask != WRITEMASK_X) {
	      yyerror(& (yylsp[(1) - (1)]), state,
		      "address register write mask must be \".x\"");
	      YYERROR;
	   } else {
	      (yyval.swiz_mask) = (yyvsp[(1) - (1)].swiz_mask);
	   }
	;}
    break;

  case 79:

    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 84:

    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 91:

    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (4)].string), at_attrib, & (yylsp[(2) - (4)]));

	   if (s == NULL) {
	      free((yyvsp[(2) - (4)].string));
	      YYERROR;
	   } else {
	      s->attrib_binding = (yyvsp[(4) - (4)].attrib);
	      state->InputsBound |= BITFIELD64_BIT(s->attrib_binding);

	      if (!validate_inputs(& (yylsp[(4) - (4)]), state)) {
		 YYERROR;
	      }
	   }
	;}
    break;

  case 92:

    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 93:

    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 94:

    {
	   (yyval.attrib) = VERT_ATTRIB_POS;
	;}
    break;

  case 95:

    {
	   (yyval.attrib) = VERT_ATTRIB_NORMAL;
	;}
    break;

  case 96:

    {
	   (yyval.attrib) = VERT_ATTRIB_COLOR0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 97:

    {
	   (yyval.attrib) = VERT_ATTRIB_FOG;
	;}
    break;

  case 98:

    {
	   (yyval.attrib) = VERT_ATTRIB_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 99:

    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 100:

    {
	   (yyval.attrib) = VERT_ATTRIB_GENERIC0 + (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 101:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxAttribs) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid vertex attribute reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 103:

    {
	   (yyval.attrib) = VARYING_SLOT_POS;
	;}
    break;

  case 104:

    {
	   (yyval.attrib) = VARYING_SLOT_COL0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 105:

    {
	   (yyval.attrib) = VARYING_SLOT_FOGC;
	;}
    break;

  case 106:

    {
	   (yyval.attrib) = VARYING_SLOT_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 109:

    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (3)].string), at_param, & (yylsp[(2) - (3)]));

	   if (s == NULL) {
	      free((yyvsp[(2) - (3)].string));
	      YYERROR;
	   } else {
	      s->param_binding_type = (yyvsp[(3) - (3)].temp_sym).param_binding_type;
	      s->param_binding_begin = (yyvsp[(3) - (3)].temp_sym).param_binding_begin;
	      s->param_binding_length = (yyvsp[(3) - (3)].temp_sym).param_binding_length;
              s->param_binding_swizzle = (yyvsp[(3) - (3)].temp_sym).param_binding_swizzle;
	      s->param_is_array = 0;
	   }
	;}
    break;

  case 110:

    {
	   if (((yyvsp[(4) - (6)].integer) != 0) && ((unsigned) (yyvsp[(4) - (6)].integer) != (yyvsp[(6) - (6)].temp_sym).param_binding_length)) {
	      free((yyvsp[(2) - (6)].string));
	      yyerror(& (yylsp[(4) - (6)]), state,
		      "parameter array size and number of bindings must match");
	      YYERROR;
	   } else {
	      struct asm_symbol *const s =
		 declare_variable(state, (yyvsp[(2) - (6)].string), (yyvsp[(6) - (6)].temp_sym).type, & (yylsp[(2) - (6)]));

	      if (s == NULL) {
		 free((yyvsp[(2) - (6)].string));
		 YYERROR;
	      } else {
		 s->param_binding_type = (yyvsp[(6) - (6)].temp_sym).param_binding_type;
		 s->param_binding_begin = (yyvsp[(6) - (6)].temp_sym).param_binding_begin;
		 s->param_binding_length = (yyvsp[(6) - (6)].temp_sym).param_binding_length;
                 s->param_binding_swizzle = SWIZZLE_XYZW;
		 s->param_is_array = 1;
	      }
	   }
	;}
    break;

  case 111:

    {
	   (yyval.integer) = 0;
	;}
    break;

  case 112:

    {
	   if (((yyvsp[(1) - (1)].integer) < 1) || ((unsigned) (yyvsp[(1) - (1)].integer) > state->limits->MaxParameters)) {
              char msg[100];
              snprintf(msg, sizeof(msg),
                             "invalid parameter array size (size=%d max=%u)",
                             (yyvsp[(1) - (1)].integer), state->limits->MaxParameters);
	      yyerror(& (yylsp[(1) - (1)]), state, msg);
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 113:

    {
	   (yyval.temp_sym) = (yyvsp[(2) - (2)].temp_sym);
	;}
    break;

  case 114:

    {
	   (yyval.temp_sym) = (yyvsp[(3) - (4)].temp_sym);
	;}
    break;

  case 116:

    {
	   (yyvsp[(1) - (3)].temp_sym).param_binding_length += (yyvsp[(3) - (3)].temp_sym).param_binding_length;
	   (yyval.temp_sym) = (yyvsp[(1) - (3)].temp_sym);
	;}
    break;

  case 117:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 118:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 119:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector), GL_TRUE);
	;}
    break;

  case 120:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 121:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 122:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector), GL_TRUE);
	;}
    break;

  case 123:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 124:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 125:

    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector), GL_FALSE);
	;}
    break;

  case 126:

    { memcpy((yyval.state), (yyvsp[(1) - (1)].state), sizeof((yyval.state))); ;}
    break;

  case 127:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 128:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 129:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 130:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 131:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 132:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 133:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 134:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 135:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 136:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 137:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 138:

    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 139:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_MATERIAL;
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer) + (yyvsp[(2) - (3)].integer);
	   (yyval.state)[2] = 0;
	;}
    break;

  case 140:

    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 141:

    {
	   (yyval.integer) = MAT_ATTRIB_FRONT_EMISSION;
	;}
    break;

  case 142:

    {
	   (yyval.integer) = MAT_ATTRIB_FRONT_SHININESS;
	;}
    break;

  case 143:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHT;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	   (yyval.state)[2] = (yyvsp[(5) - (5)].integer);
	;}
    break;

  case 144:

    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 145:

    {
	   (yyval.integer) = STATE_POSITION;
	;}
    break;

  case 146:

    {
	   if (!state->ctx->Extensions.EXT_point_parameters) {
	      yyerror(& (yylsp[(1) - (1)]), state, "GL_ARB_point_parameters not supported");
	      YYERROR;
	   }

	   (yyval.integer) = STATE_ATTENUATION;
	;}
    break;

  case 147:

    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 148:

    {
	   (yyval.integer) = STATE_HALF_VECTOR;
	;}
    break;

  case 149:

    {
	   (yyval.integer) = STATE_SPOT_DIRECTION;
	;}
    break;

  case 150:

    {
	   (yyval.state)[0] = (yyvsp[(2) - (2)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (2)].state)[1];
	;}
    break;

  case 151:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_AMBIENT;
	;}
    break;

  case 152:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_SCENECOLOR;
	   (yyval.state)[1] = (yyvsp[(1) - (2)].integer);
	;}
    break;

  case 153:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTPROD;
	   (yyval.state)[1] = (yyvsp[(3) - (6)].integer);
	   (yyval.state)[2] = (yyvsp[(6) - (6)].integer) + (yyvsp[(5) - (6)].integer);
	   (yyval.state)[3] = 0;
	;}
    break;

  case 155:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(3) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 156:

    {
	   (yyval.integer) = STATE_TEXENV_COLOR;
	;}
    break;

  case 157:

    {
	   (yyval.integer) = MAT_ATTRIB_FRONT_AMBIENT;
	;}
    break;

  case 158:

    {
	   (yyval.integer) = MAT_ATTRIB_FRONT_DIFFUSE;
	;}
    break;

  case 159:

    {
	   (yyval.integer) = MAT_ATTRIB_FRONT_SPECULAR;
	;}
    break;

  case 160:

    {
           (yyval.integer) = STATE_AMBIENT;
        ;}
    break;

  case 161:

    {
           (yyval.integer) = STATE_DIFFUSE;
        ;}
    break;

  case 162:

    {
           (yyval.integer) = STATE_SPECULAR;
        ;}
    break;

  case 163:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxLights) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid light selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 164:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_TEXGEN;
	   (yyval.state)[1] = (yyvsp[(2) - (4)].integer);
	   (yyval.state)[2] = (yyvsp[(3) - (4)].integer) + (yyvsp[(4) - (4)].integer);
	;}
    break;

  case 165:

    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S;
	;}
    break;

  case 166:

    {
	   (yyval.integer) = STATE_TEXGEN_OBJECT_S;
	;}
    break;

  case 167:

    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 168:

    {
	   (yyval.integer) = STATE_TEXGEN_EYE_T - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 169:

    {
	   (yyval.integer) = STATE_TEXGEN_EYE_R - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 170:

    {
	   (yyval.integer) = STATE_TEXGEN_EYE_Q - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 171:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 172:

    {
	   (yyval.integer) = STATE_FOG_COLOR;
	;}
    break;

  case 173:

    {
	   (yyval.integer) = STATE_FOG_PARAMS;
	;}
    break;

  case 174:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_CLIPPLANE;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	;}
    break;

  case 175:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxClipPlanes) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid clip plane selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 176:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 177:

    {
	   (yyval.integer) = STATE_POINT_SIZE;
	;}
    break;

  case 178:

    {
	   (yyval.integer) = STATE_POINT_ATTENUATION;
	;}
    break;

  case 179:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (5)].state)[0] + (yyvsp[(1) - (5)].state)[2];
	   (yyval.state)[1] = (yyvsp[(1) - (5)].state)[1];
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	;}
    break;

  case 180:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (2)].state)[0] + (yyvsp[(1) - (2)].state)[2];
	   (yyval.state)[1] = (yyvsp[(1) - (2)].state)[1];
	   (yyval.state)[2] = (yyvsp[(2) - (2)].state)[2];
	   (yyval.state)[3] = (yyvsp[(2) - (2)].state)[3];
	;}
    break;

  case 181:

    {
	   (yyval.state)[2] = 0;
	   (yyval.state)[3] = 3;
	;}
    break;

  case 182:

    {
	   /* It seems logical that the matrix row range specifier would have
	    * to specify a range or more than one row (i.e., $5 > $3).
	    * However, the ARB_vertex_program spec says "a program will fail
	    * to load if <a> is greater than <b>."  This means that $3 == $5
	    * is valid.
	    */
	   if ((yyvsp[(3) - (6)].integer) > (yyvsp[(5) - (6)].integer)) {
	      yyerror(& (yylsp[(3) - (6)]), state, "invalid matrix row range");
	      YYERROR;
	   }

	   (yyval.state)[2] = (yyvsp[(3) - (6)].integer);
	   (yyval.state)[3] = (yyvsp[(5) - (6)].integer);
	;}
    break;

  case 183:

    {
	   (yyval.state)[0] = (yyvsp[(2) - (3)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (3)].state)[1];
	   (yyval.state)[2] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 184:

    {
	   (yyval.integer) = STATE_MATRIX_NO_MODIFIER;
	;}
    break;

  case 185:

    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 186:

    {
	   (yyval.integer) = STATE_MATRIX_INVERSE;
	;}
    break;

  case 187:

    {
	   (yyval.integer) = STATE_MATRIX_TRANSPOSE;
	;}
    break;

  case 188:

    {
	   (yyval.integer) = STATE_MATRIX_INVTRANS;
	;}
    break;

  case 189:

    {
	   if ((yyvsp[(1) - (1)].integer) > 3) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid matrix row reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 190:

    {
	   (yyval.state)[0] = STATE_MODELVIEW_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 191:

    {
	   (yyval.state)[0] = STATE_PROJECTION_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 192:

    {
	   (yyval.state)[0] = STATE_MVP_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 193:

    {
	   (yyval.state)[0] = STATE_TEXTURE_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 194:

    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 195:

    {
	   (yyval.state)[0] = STATE_PROGRAM_MATRIX;
	   (yyval.state)[1] = (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 196:

    {
	   (yyval.integer) = 0;
	;}
    break;

  case 197:

    {
	   (yyval.integer) = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 198:

    {
	   /* Since GL_ARB_vertex_blend isn't supported, only modelview matrix
	    * zero is valid.
	    */
	   if ((yyvsp[(1) - (1)].integer) != 0) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid modelview matrix index");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 199:

    {
	   /* Since GL_ARB_matrix_palette isn't supported, just let any value
	    * through here.  The error will be generated later.
	    */
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 200:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxProgramMatrices) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program matrix selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 201:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_DEPTH_RANGE;
	;}
    break;

  case 206:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum_env;
	   (yyval.state)[1] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[1];
           (yyval.state)[3] = 0;
	;}
    break;

  case 207:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 208:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 209:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum_env;
	   (yyval.state)[1] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
           (yyval.state)[3] = 0;
	;}
    break;

  case 210:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum_local;
	   (yyval.state)[1] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[1];
           (yyval.state)[3] = 0;
	;}
    break;

  case 211:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 212:

    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 213:

    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum_local;
	   (yyval.state)[1] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
           (yyval.state)[3] = 0;
	;}
    break;

  case 214:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxEnvParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid environment parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 215:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxLocalParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid local parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 220:

    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3].f = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 221:

    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2].f = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3].f = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 222:

    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0].f = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[1].f = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[2].f = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[3].f = (float) (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 223:

    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0].f = (yyvsp[(2) - (3)].real);
	   (yyval.vector).data[1].f = 0.0f;
	   (yyval.vector).data[2].f = 0.0f;
	   (yyval.vector).data[3].f = 1.0f;
	;}
    break;

  case 224:

    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0].f = (yyvsp[(2) - (5)].real);
	   (yyval.vector).data[1].f = (yyvsp[(4) - (5)].real);
	   (yyval.vector).data[2].f = 0.0f;
	   (yyval.vector).data[3].f = 1.0f;
	;}
    break;

  case 225:

    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0].f = (yyvsp[(2) - (7)].real);
	   (yyval.vector).data[1].f = (yyvsp[(4) - (7)].real);
	   (yyval.vector).data[2].f = (yyvsp[(6) - (7)].real);
	   (yyval.vector).data[3].f = 1.0f;
	;}
    break;

  case 226:

    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0].f = (yyvsp[(2) - (9)].real);
	   (yyval.vector).data[1].f = (yyvsp[(4) - (9)].real);
	   (yyval.vector).data[2].f = (yyvsp[(6) - (9)].real);
	   (yyval.vector).data[3].f = (yyvsp[(8) - (9)].real);
	;}
    break;

  case 227:

    {
	   (yyval.real) = ((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].real) : (yyvsp[(2) - (2)].real);
	;}
    break;

  case 228:

    {
	   (yyval.real) = (float)(((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].integer) : (yyvsp[(2) - (2)].integer));
	;}
    break;

  case 229:

    { (yyval.negate) = FALSE; ;}
    break;

  case 230:

    { (yyval.negate) = TRUE;  ;}
    break;

  case 231:

    { (yyval.negate) = FALSE; ;}
    break;

  case 232:

    { (yyval.integer) = (yyvsp[(1) - (1)].integer); ;}
    break;

  case 234:

    { (yyval.integer) = (yyvsp[(1) - (1)].integer); ;}
    break;

  case 236:

    {
	   if (!declare_variable(state, (yyvsp[(3) - (3)].string), (yyvsp[(0) - (3)].integer), & (yylsp[(3) - (3)]))) {
	      free((yyvsp[(3) - (3)].string));
	      YYERROR;
	   }
	;}
    break;

  case 237:

    {
	   if (!declare_variable(state, (yyvsp[(1) - (1)].string), (yyvsp[(0) - (1)].integer), & (yylsp[(1) - (1)]))) {
	      free((yyvsp[(1) - (1)].string));
	      YYERROR;
	   }
	;}
    break;

  case 238:

    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (4)].string), at_output, & (yylsp[(2) - (4)]));

	   if (s == NULL) {
	      free((yyvsp[(2) - (4)].string));
	      YYERROR;
	   } else {
	      s->output_binding = (yyvsp[(4) - (4)].result);
	   }
	;}
    break;

  case 239:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VARYING_SLOT_POS;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 240:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VARYING_SLOT_FOGC;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 241:

    {
	   (yyval.result) = (yyvsp[(2) - (2)].result);
	;}
    break;

  case 242:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VARYING_SLOT_PSIZ;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 243:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VARYING_SLOT_TEX0 + (yyvsp[(3) - (3)].integer);
	   } else {
	      yyerror(& (yylsp[(2) - (3)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 244:

    {
	   if (state->mode == ARB_fragment) {
	      (yyval.result) = FRAG_RESULT_DEPTH;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 245:

    {
	   (yyval.result) = (yyvsp[(2) - (3)].integer) + (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 246:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VARYING_SLOT_COL0;
	   } else {
	      if (state->option.DrawBuffers)
		 (yyval.integer) = FRAG_RESULT_DATA0;
	      else
		 (yyval.integer) = FRAG_RESULT_COLOR;
	   }
	;}
    break;

  case 247:

    {
	   if (state->mode == ARB_vertex) {
	      yyerror(& (yylsp[(1) - (3)]), state, "invalid program result name");
	      YYERROR;
	   } else {
	      if (!state->option.DrawBuffers) {
		 /* From the ARB_draw_buffers spec (same text exists
		  * for ATI_draw_buffers):
		  *
		  *     If this option is not specified, a fragment
		  *     program that attempts to bind
		  *     "result.color[n]" will fail to load, and only
		  *     "result.color" will be allowed.
		  */
		 yyerror(& (yylsp[(1) - (3)]), state,
			 "result.color[] used without "
			 "`OPTION ARB_draw_buffers' or "
			 "`OPTION ATI_draw_buffers'");
		 YYERROR;
	      } else if ((yyvsp[(2) - (3)].integer) >= state->MaxDrawBuffers) {
		 yyerror(& (yylsp[(1) - (3)]), state,
			 "result.color[] exceeds MAX_DRAW_BUFFERS_ARB");
		 YYERROR;
	      }
	      (yyval.integer) = FRAG_RESULT_DATA0 + (yyvsp[(2) - (3)].integer);
	   }
	;}
    break;

  case 248:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VARYING_SLOT_COL0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 249:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VARYING_SLOT_BFC0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 250:

    {
	   (yyval.integer) = 0;
	;}
    break;

  case 251:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 252:

    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 1;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 253:

    { (yyval.integer) = 0; ;}
    break;

  case 254:

    { (yyval.integer) = 0; ;}
    break;

  case 255:

    { (yyval.integer) = 1; ;}
    break;

  case 256:

    { (yyval.integer) = 0; ;}
    break;

  case 257:

    { (yyval.integer) = 0; ;}
    break;

  case 258:

    { (yyval.integer) = 1; ;}
    break;

  case 259:

    { (yyval.integer) = 0; ;}
    break;

  case 260:

    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 261:

    { (yyval.integer) = 0; ;}
    break;

  case 262:

    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 263:

    { (yyval.integer) = 0; ;}
    break;

  case 264:

    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 265:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureCoordUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture coordinate unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 266:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureImageUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture image unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 267:

    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 268:

    {
	   struct asm_symbol *exist = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(2) - (4)].string));
	   struct asm_symbol *target = (struct asm_symbol *)
              _mesa_symbol_table_find_symbol(state->st, (yyvsp[(4) - (4)].string));

	   free((yyvsp[(4) - (4)].string));

	   if (exist != NULL) {
	      char m[1000];
	      snprintf(m, sizeof(m), "redeclared identifier: %s", (yyvsp[(2) - (4)].string));
	      free((yyvsp[(2) - (4)].string));
	      yyerror(& (yylsp[(2) - (4)]), state, m);
	      YYERROR;
	   } else if (target == NULL) {
	      free((yyvsp[(2) - (4)].string));
	      yyerror(& (yylsp[(4) - (4)]), state,
		      "undefined variable binding in ALIAS statement");
	      YYERROR;
	   } else {
              _mesa_symbol_table_add_symbol(state->st, (yyvsp[(2) - (4)].string), target);
	   }
	;}
    break;



      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
      yyerror (&yylloc, state, YY_("syntax error"));
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
	    yyerror (&yylloc, state, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, state, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc, state);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, state);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, state);
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





void
asm_instruction_set_operands(struct asm_instruction *inst,
			     const struct prog_dst_register *dst,
			     const struct asm_src_register *src0,
			     const struct asm_src_register *src1,
			     const struct asm_src_register *src2)
{
   /* In the core ARB extensions only the KIL instruction doesn't have a
    * destination register.
    */
   if (dst == NULL) {
      init_dst_reg(& inst->Base.DstReg);
   } else {
      inst->Base.DstReg = *dst;
   }

   if (src0 != NULL) {
      inst->Base.SrcReg[0] = src0->Base;
      inst->SrcReg[0] = *src0;
   } else {
      init_src_reg(& inst->SrcReg[0]);
   }

   if (src1 != NULL) {
      inst->Base.SrcReg[1] = src1->Base;
      inst->SrcReg[1] = *src1;
   } else {
      init_src_reg(& inst->SrcReg[1]);
   }

   if (src2 != NULL) {
      inst->Base.SrcReg[2] = src2->Base;
      inst->SrcReg[2] = *src2;
   } else {
      init_src_reg(& inst->SrcReg[2]);
   }
}


struct asm_instruction *
asm_instruction_ctor(enum prog_opcode op,
		     const struct prog_dst_register *dst,
		     const struct asm_src_register *src0,
		     const struct asm_src_register *src1,
		     const struct asm_src_register *src2)
{
   struct asm_instruction *inst = CALLOC_STRUCT(asm_instruction);

   if (inst) {
      _mesa_init_instructions(& inst->Base, 1);
      inst->Base.Opcode = op;

      asm_instruction_set_operands(inst, dst, src0, src1, src2);
   }

   return inst;
}


struct asm_instruction *
asm_instruction_copy_ctor(const struct prog_instruction *base,
			  const struct prog_dst_register *dst,
			  const struct asm_src_register *src0,
			  const struct asm_src_register *src1,
			  const struct asm_src_register *src2)
{
   struct asm_instruction *inst = CALLOC_STRUCT(asm_instruction);

   if (inst) {
      _mesa_init_instructions(& inst->Base, 1);
      inst->Base.Opcode = base->Opcode;
      inst->Base.Saturate = base->Saturate;

      asm_instruction_set_operands(inst, dst, src0, src1, src2);
   }

   return inst;
}


void
init_dst_reg(struct prog_dst_register *r)
{
   memset(r, 0, sizeof(*r));
   r->File = PROGRAM_UNDEFINED;
   r->WriteMask = WRITEMASK_XYZW;
}


/** Like init_dst_reg() but set the File and Index fields. */
void
set_dst_reg(struct prog_dst_register *r, gl_register_file file, GLint index)
{
   const GLint maxIndex = 1 << INST_INDEX_BITS;
   const GLint minIndex = 0;
   assert(index >= minIndex);
   (void) minIndex;
   assert(index <= maxIndex);
   (void) maxIndex;
   assert(file == PROGRAM_TEMPORARY ||
	  file == PROGRAM_ADDRESS ||
	  file == PROGRAM_OUTPUT);
   memset(r, 0, sizeof(*r));
   r->File = file;
   r->Index = index;
   r->WriteMask = WRITEMASK_XYZW;
}


void
init_src_reg(struct asm_src_register *r)
{
   memset(r, 0, sizeof(*r));
   r->Base.File = PROGRAM_UNDEFINED;
   r->Base.Swizzle = SWIZZLE_NOOP;
   r->Symbol = NULL;
}


/** Like init_src_reg() but set the File and Index fields.
 * \return GL_TRUE if a valid src register, GL_FALSE otherwise
 */
void
set_src_reg(struct asm_src_register *r, gl_register_file file, GLint index)
{
   set_src_reg_swz(r, file, index, SWIZZLE_XYZW);
}


void
set_src_reg_swz(struct asm_src_register *r, gl_register_file file, GLint index,
                GLuint swizzle)
{
   const GLint maxIndex = (1 << INST_INDEX_BITS) - 1;
   const GLint minIndex = -(1 << INST_INDEX_BITS);
   assert(file < PROGRAM_FILE_MAX);
   assert(index >= minIndex);
   (void) minIndex;
   assert(index <= maxIndex);
   (void) maxIndex;
   memset(r, 0, sizeof(*r));
   r->Base.File = file;
   r->Base.Index = index;
   r->Base.Swizzle = swizzle;
   r->Symbol = NULL;
}


/**
 * Validate the set of inputs used by a program
 *
 * Validates that legal sets of inputs are used by the program.  In this case
 * "used" included both reading the input or binding the input to a name using
 * the \c ATTRIB command.
 *
 * \return
 * \c TRUE if the combination of inputs used is valid, \c FALSE otherwise.
 */
int
validate_inputs(struct YYLTYPE *locp, struct asm_parser_state *state)
{
   const GLbitfield64 inputs = state->prog->info.inputs_read | state->InputsBound;
   GLbitfield ff_inputs = 0;

   /* Since Mesa internal attribute indices are different from
    * how NV_vertex_program defines attribute aliasing, we have to construct
    * a separate usage mask based on how the aliasing is defined.
    *
    * Note that attribute aliasing is optional if NV_vertex_program is
    * unsupported.
    */
   if (inputs & VERT_BIT_POS)
      ff_inputs |= 1 << 0;
   if (inputs & VERT_BIT_NORMAL)
      ff_inputs |= 1 << 2;
   if (inputs & VERT_BIT_COLOR0)
      ff_inputs |= 1 << 3;
   if (inputs & VERT_BIT_COLOR1)
      ff_inputs |= 1 << 4;
   if (inputs & VERT_BIT_FOG)
      ff_inputs |= 1 << 5;

   ff_inputs |= ((inputs & VERT_BIT_TEX_ALL) >> VERT_ATTRIB_TEX0) << 8;

   if ((ff_inputs & (inputs >> VERT_ATTRIB_GENERIC0)) != 0) {
      yyerror(locp, state, "illegal use of generic attribute and name attribute");
      return 0;
   }

   return 1;
}


struct asm_symbol *
declare_variable(struct asm_parser_state *state, char *name, enum asm_type t,
		 struct YYLTYPE *locp)
{
   struct asm_symbol *s = NULL;
   struct asm_symbol *exist = (struct asm_symbol *)
      _mesa_symbol_table_find_symbol(state->st, name);


   if (exist != NULL) {
      yyerror(locp, state, "redeclared identifier");
   } else {
      s = calloc(1, sizeof(struct asm_symbol));
      s->name = name;
      s->type = t;

      switch (t) {
      case at_temp:
         if (state->prog->arb.NumTemporaries >= state->limits->MaxTemps) {
	    yyerror(locp, state, "too many temporaries declared");
	    free(s);
	    return NULL;
	 }

         s->temp_binding = state->prog->arb.NumTemporaries;
         state->prog->arb.NumTemporaries++;
	 break;

      case at_address:
         if (state->prog->arb.NumAddressRegs >=
             state->limits->MaxAddressRegs) {
	    yyerror(locp, state, "too many address registers declared");
	    free(s);
	    return NULL;
	 }

	 /* FINISHME: Add support for multiple address registers.
	  */
         state->prog->arb.NumAddressRegs++;
	 break;

      default:
	 break;
      }

      _mesa_symbol_table_add_symbol(state->st, s->name, s);
      s->next = state->sym;
      state->sym = s;
   }

   return s;
}


int add_state_reference(struct gl_program_parameter_list *param_list,
			const gl_state_index16 tokens[STATE_LENGTH])
{
   const GLuint size = 4; /* XXX fix */
   char *name;
   GLint index;

   name = _mesa_program_state_string(tokens);
   index = _mesa_add_parameter(param_list, PROGRAM_STATE_VAR, name,
                               size, GL_NONE, NULL, tokens, true);
   param_list->StateFlags |= _mesa_program_state_flags(tokens);

   /* free name string here since we duplicated it in add_parameter() */
   free(name);

   return index;
}


int
initialize_symbol_from_state(struct gl_program *prog,
			     struct asm_symbol *param_var,
			     const gl_state_index16 tokens[STATE_LENGTH])
{
   int idx = -1;
   gl_state_index16 state_tokens[STATE_LENGTH];


   memcpy(state_tokens, tokens, sizeof(state_tokens));

   param_var->type = at_param;
   param_var->param_binding_type = PROGRAM_STATE_VAR;

   /* If we are adding a STATE_MATRIX that has multiple rows, we need to
    * unroll it and call add_state_reference() for each row
    */
   if (state_tokens[0] >= STATE_MODELVIEW_MATRIX &&
       state_tokens[0] <= STATE_PROGRAM_MATRIX_INVTRANS
       && (state_tokens[2] != state_tokens[3])) {
      int row;
      const int first_row = state_tokens[2];
      const int last_row = state_tokens[3];

      for (row = first_row; row <= last_row; row++) {
	 state_tokens[2] = state_tokens[3] = row;

	 idx = add_state_reference(prog->Parameters, state_tokens);
	 if (param_var->param_binding_begin == ~0U) {
	    param_var->param_binding_begin = idx;
            param_var->param_binding_swizzle = SWIZZLE_XYZW;
         }

	 param_var->param_binding_length++;
      }
   }
   else {
      idx = add_state_reference(prog->Parameters, state_tokens);
      if (param_var->param_binding_begin == ~0U) {
	 param_var->param_binding_begin = idx;
         param_var->param_binding_swizzle = SWIZZLE_XYZW;
      }
      param_var->param_binding_length++;
   }

   return idx;
}


int
initialize_symbol_from_param(struct gl_program *prog,
			     struct asm_symbol *param_var,
			     const gl_state_index16 tokens[STATE_LENGTH])
{
   int idx = -1;
   gl_state_index16 state_tokens[STATE_LENGTH];


   memcpy(state_tokens, tokens, sizeof(state_tokens));

   assert(state_tokens[0] == STATE_VERTEX_PROGRAM_ENV ||
          state_tokens[0] == STATE_VERTEX_PROGRAM_LOCAL ||
          state_tokens[0] == STATE_FRAGMENT_PROGRAM_ENV ||
          state_tokens[0] == STATE_FRAGMENT_PROGRAM_LOCAL);

   /*
    * The param type is STATE_VAR.  The program parameter entry will
    * effectively be a pointer into the LOCAL or ENV parameter array.
    */
   param_var->type = at_param;
   param_var->param_binding_type = PROGRAM_STATE_VAR;

   /* If we are adding a STATE_ENV or STATE_LOCAL that has multiple elements,
    * we need to unroll it and call add_state_reference() for each row
    */
   if (state_tokens[1] != state_tokens[2]) {
      int row;
      const int first_row = state_tokens[1];
      const int last_row = state_tokens[2];

      for (row = first_row; row <= last_row; row++) {
	 state_tokens[1] = state_tokens[2] = row;

	 idx = add_state_reference(prog->Parameters, state_tokens);
	 if (param_var->param_binding_begin == ~0U) {
	    param_var->param_binding_begin = idx;
            param_var->param_binding_swizzle = SWIZZLE_XYZW;
         }
	 param_var->param_binding_length++;
      }
   }
   else {
      idx = add_state_reference(prog->Parameters, state_tokens);
      if (param_var->param_binding_begin == ~0U) {
	 param_var->param_binding_begin = idx;
         param_var->param_binding_swizzle = SWIZZLE_XYZW;
      }
      param_var->param_binding_length++;
   }

   return idx;
}


/**
 * Put a float/vector constant/literal into the parameter list.
 * \param param_var  returns info about the parameter/constant's location,
 *                   binding, type, etc.
 * \param vec  the vector/constant to add
 * \param allowSwizzle  if true, try to consolidate constants which only differ
 *                      by a swizzle.  We don't want to do this when building
 *                      arrays of constants that may be indexed indirectly.
 * \return index of the constant in the parameter list.
 */
int
initialize_symbol_from_const(struct gl_program *prog,
			     struct asm_symbol *param_var,
			     const struct asm_vector *vec,
                             GLboolean allowSwizzle)
{
   unsigned swizzle;
   const int idx = _mesa_add_unnamed_constant(prog->Parameters,
                                              vec->data, vec->count,
                                              allowSwizzle ? &swizzle : NULL);

   param_var->type = at_param;
   param_var->param_binding_type = PROGRAM_CONSTANT;

   if (param_var->param_binding_begin == ~0U) {
      param_var->param_binding_begin = idx;
      param_var->param_binding_swizzle = allowSwizzle ? swizzle : SWIZZLE_XYZW;
   }
   param_var->param_binding_length++;

   return idx;
}


char *
make_error_string(const char *fmt, ...)
{
   int length;
   char *str;
   va_list args;


   /* Call vsnprintf once to determine how large the final string is.  Call it
    * again to do the actual formatting.  from the vsnprintf manual page:
    *
    *    Upon successful return, these functions return the number of
    *    characters printed  (not including the trailing '\0' used to end
    *    output to strings).
    */
   va_start(args, fmt);
   length = 1 + vsnprintf(NULL, 0, fmt, args);
   va_end(args);

   str = malloc(length);
   if (str) {
      va_start(args, fmt);
      vsnprintf(str, length, fmt, args);
      va_end(args);
   }

   return str;
}


void
yyerror(YYLTYPE *locp, struct asm_parser_state *state, const char *s)
{
   char *err_str;


   err_str = make_error_string("glProgramStringARB(%s)\n", s);
   if (err_str) {
      _mesa_error(state->ctx, GL_INVALID_OPERATION, "%s", err_str);
      free(err_str);
   }

   err_str = make_error_string("line %u, char %u: error: %s\n",
			       locp->first_line, locp->first_column, s);
   _mesa_set_program_error(state->ctx, locp->position, err_str);

   if (err_str) {
      free(err_str);
   }
}


GLboolean
_mesa_parse_arb_program(struct gl_context *ctx, GLenum target, const GLubyte *str,
			GLsizei len, struct asm_parser_state *state)
{
   struct asm_instruction *inst;
   unsigned i;
   GLubyte *strz;
   GLboolean result = GL_FALSE;
   void *temp;
   struct asm_symbol *sym;

   state->ctx = ctx;
   state->prog->Target = target;
   state->prog->Parameters = _mesa_new_parameter_list();

   /* Make a copy of the program string and force it to be NUL-terminated.
    */
   strz = (GLubyte *) ralloc_size(state->mem_ctx, len + 1);
   if (strz == NULL) {
      if (state->prog->Parameters) {
         _mesa_free_parameter_list(state->prog->Parameters);
         state->prog->Parameters = NULL;
      }
      _mesa_error(ctx, GL_OUT_OF_MEMORY, "glProgramStringARB");
      return GL_FALSE;
   }
   memcpy (strz, str, len);
   strz[len] = '\0';

   state->prog->String = strz;

   state->st = _mesa_symbol_table_ctor();

   state->limits = (target == GL_VERTEX_PROGRAM_ARB)
      ? & ctx->Const.Program[MESA_SHADER_VERTEX]
      : & ctx->Const.Program[MESA_SHADER_FRAGMENT];

   state->MaxTextureImageUnits = ctx->Const.Program[MESA_SHADER_FRAGMENT].MaxTextureImageUnits;
   state->MaxTextureCoordUnits = ctx->Const.MaxTextureCoordUnits;
   state->MaxTextureUnits = ctx->Const.MaxTextureUnits;
   state->MaxClipPlanes = ctx->Const.MaxClipPlanes;
   state->MaxLights = ctx->Const.MaxLights;
   state->MaxProgramMatrices = ctx->Const.MaxProgramMatrices;
   state->MaxDrawBuffers = ctx->Const.MaxDrawBuffers;

   state->state_param_enum_env = (target == GL_VERTEX_PROGRAM_ARB)
      ? STATE_VERTEX_PROGRAM_ENV : STATE_FRAGMENT_PROGRAM_ENV;
   state->state_param_enum_local = (target == GL_VERTEX_PROGRAM_ARB)
      ? STATE_VERTEX_PROGRAM_LOCAL : STATE_FRAGMENT_PROGRAM_LOCAL;

   _mesa_set_program_error(ctx, -1, NULL);

   _mesa_program_lexer_ctor(& state->scanner, state, (const char *) str, len);
   yyparse(state);
   _mesa_program_lexer_dtor(state->scanner);


   if (ctx->Program.ErrorPos != -1) {
      goto error;
   }

   if (! _mesa_layout_parameters(state)) {
      struct YYLTYPE loc;

      loc.first_line = 0;
      loc.first_column = 0;
      loc.position = len;

      yyerror(& loc, state, "invalid PARAM usage");
      goto error;
   }



   /* Add one instruction to store the "END" instruction.
    */
   state->prog->arb.Instructions =
      rzalloc_array(state->mem_ctx, struct prog_instruction,
                    state->prog->arb.NumInstructions + 1);

   if (state->prog->arb.Instructions == NULL) {
      goto error;
   }

   inst = state->inst_head;
   for (i = 0; i < state->prog->arb.NumInstructions; i++) {
      struct asm_instruction *const temp = inst->next;

      state->prog->arb.Instructions[i] = inst->Base;
      inst = temp;
   }

   /* Finally, tag on an OPCODE_END instruction */
   {
      const GLuint numInst = state->prog->arb.NumInstructions;
      _mesa_init_instructions(state->prog->arb.Instructions + numInst, 1);
      state->prog->arb.Instructions[numInst].Opcode = OPCODE_END;
   }
   state->prog->arb.NumInstructions++;

   state->prog->arb.NumParameters = state->prog->Parameters->NumParameters;
   state->prog->arb.NumAttributes =
      util_bitcount64(state->prog->info.inputs_read);

   /*
    * Initialize native counts to logical counts.  The device driver may
    * change them if program is translated into a hardware program.
    */
   state->prog->arb.NumNativeInstructions = state->prog->arb.NumInstructions;
   state->prog->arb.NumNativeTemporaries = state->prog->arb.NumTemporaries;
   state->prog->arb.NumNativeParameters = state->prog->arb.NumParameters;
   state->prog->arb.NumNativeAttributes = state->prog->arb.NumAttributes;
   state->prog->arb.NumNativeAddressRegs = state->prog->arb.NumAddressRegs;

   result = GL_TRUE;

error:
   for (inst = state->inst_head; inst != NULL; inst = temp) {
      temp = inst->next;
      free(inst);
   }

   state->inst_head = NULL;
   state->inst_tail = NULL;

   for (sym = state->sym; sym != NULL; sym = temp) {
      temp = sym->next;

      free((void *) sym->name);
      free(sym);
   }
   state->sym = NULL;

   _mesa_symbol_table_dtor(state->st);
   state->st = NULL;

   if (result != GL_TRUE) {
      if (state->prog->Parameters) {
         _mesa_free_parameter_list(state->prog->Parameters);
         state->prog->Parameters = NULL;
      }
      ralloc_free(state->prog->String);
      state->prog->String = NULL;
   }

   return result;
}

