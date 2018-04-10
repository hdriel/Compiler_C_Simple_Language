/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140715

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
#include <stdarg.h>
#include "parse.h"

/* Student: */
/* Hadriel Benjo - 302185061 */
/* Semion phifman- 317743425*/

/* function declarations*/
int yylex(void);
void yyerror(char *);
extern int yylineno;
extern char* yytext;

/* parser|syntax fucntions */
int isSimpleTree(node* head);
node* makeNode(char *value, int count,...);
node* makeNode2(char *value,type t, kind k, int count,...);
node* makeParent(char* value, node* child);
node* makeParent2(char* value,type t, kind k, node* child);
void printTree(node* ast, int indent, int s, int l);

/* semantic function*/
scope* head;
node* h;
int live_scopes = 0;
int hasMainAllrady = 0;
void pop(node_t ** head);
node_t* push(node_t ** head, char *name, char *value, kind k, type t);
void enterScope(char *nameScope);
void endScope();
node_t* findLocal(scope *s, char *name);
node_t* findGlobal(char* name);
void checkMainFunc(node *func);
char* checkProgram(node *program);
char* checkFunc(node *func);
char* checkBlock(node *block , node_t *last_func, int layout, int *size);
void checkStmnts(node *stmnts , node_t *last_func, int layout, int *size, char **code);
char* checkAss(node *ass, int *size);
char* GenLhs(node *lhs, int *size);
void checkFuncCall(node *fcall, int *size, char** code, char *t, int checkParamStack);
char *GenExpr(node *expr, int *size, char **lv);
void GenParamas(node *args, int *s, int *size, char** code);

void addArgs(node *args, node_t *f);
void addArg(node *arg,type t, node_t *f);
void addVars(node *vars); /**/
void addVar(node *var,type t); /**/
void compareArgs(node_t *f, node *args);
type getLhsType(node *lhs);
type getExprType(node *expr);

int varCount;
int lineCount;
char* freshVar();
void getVarsSize(node *vars, int *size);
void getVarSize(node *var,type t, int *size);
int getSize(node* n);

/* help function for debug of semantic function*/
node_t *copyNode_t(node_t *f);
char* getStrKind(kind k); 
char* getStrType(type t); 
void printNode(node *n); 
void printGlobal();
void printLocal(scope *s, int numScope);
node* createNewNode();
void exitProgram();
void handleIfItMain(node *func);
node* getNestedNodesInOneNode(node *res);
node* addSibling(node *p, node *s);
void reverseSons(node** head_ref);

char* strLineNum(int line);
int noDigits(int n);



int live_gen_scopes = 0;
scope* declareMemoryForVars;
node_t* findGenGlobal(char* name);
void enterGenScope(char* nameScope);
void endGenScope();
void printGenGlobal();
/* three address code (3AC)- functions*/
#line 88 "parser.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	char *value;
	node *nPtr;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 118 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define INTEGER 257
#define CHAR 258
#define CHARPTR 259
#define CHAR_CONST 260
#define BOOLEAN 261
#define BOOLEAN_CONST 262
#define BINARY_CONST 263
#define VOID 264
#define VAR 265
#define RETURN 266
#define IDENTIFIER 267
#define INTPTR 268
#define DECIMAL_CONST 269
#define NULL_ 270
#define OCTAL_CONST 271
#define AND 272
#define DIV 273
#define EQUAL 274
#define GE 275
#define GREATER 276
#define LE 277
#define MINUS 278
#define NOT 279
#define NE 280
#define OR 281
#define PLUS 282
#define MULTI 283
#define ADDRESS_OP 284
#define HEX_CONST 285
#define DEREFERENCE 286
#define STRING_CONST 287
#define IF 288
#define WHILE 289
#define STRING 290
#define FLOAT_CONST 291
#define LESS 292
#define FOR 293
#define DO 294
#define UNARY 295
#define ENDIF 296
#define ELSE 297
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    7,    7,    8,    8,    4,    4,    5,    5,
    6,    6,    1,    2,    2,    3,    3,   10,   10,   11,
   11,    9,    9,    9,    9,    9,    9,    9,    9,    9,
    9,    9,    9,   30,   31,   16,   17,   18,   20,   20,
   19,   19,   19,   15,   22,   23,   23,   24,   24,   24,
   21,   21,   33,   33,   29,   29,   29,   29,   29,   34,
   27,   27,   27,   27,   32,   32,   32,   32,   25,   25,
   25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
   25,   25,   25,   25,   25,   25,   25,   14,   14,   14,
   13,   12,   12,   35,   35,   35,   35,   26,   28,
};
static const YYINT yylen[] = {                            2,
    1,    1,    1,    2,    8,    8,    1,    1,    1,    3,
    2,    2,    4,    1,    1,    1,    3,    1,    1,    1,
    2,    2,    1,    1,    1,    1,    3,    1,    1,    1,
    3,    2,    1,    5,    7,    5,    7,    9,    1,    0,
    1,    3,    0,    1,    1,    1,    2,    2,    2,    2,
    1,    3,    4,    6,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    1,    1,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    2,    1,    2,    4,
    1,    3,    3,    1,    1,    1,    1,    1,    0,
};
static const YYINT yydefred[] = {                         0,
   56,   55,   58,   57,    0,   59,    0,    0,    3,    1,
    0,    0,    4,    0,    0,    0,   60,    0,    0,    9,
    7,    0,    0,    0,    0,    0,   98,   11,   12,    0,
    0,   10,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   28,   23,   46,   20,   44,    0,   91,    0,    0,
    0,   26,   29,   30,   33,   45,    0,   19,    0,   24,
   25,    0,    0,   61,   62,   65,    0,   68,   63,   66,
   97,   96,   94,   67,   95,    0,   32,    0,   72,    0,
   71,   64,    0,    0,    0,   89,    0,    0,    0,    0,
    0,   21,   22,    0,    5,   47,    0,    0,    0,    0,
    0,    6,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   31,   87,    0,
    0,    0,   14,    0,    0,    0,    0,   41,    0,    0,
   27,   93,    0,    0,    0,    0,    0,   85,   86,    0,
   84,    0,    0,    0,    0,    0,    0,    0,    0,   83,
    0,   13,    0,   90,    0,    0,    0,    0,    0,    0,
   52,    0,   70,    0,    0,   36,   42,    0,    0,    0,
   53,    0,    0,    0,    0,    0,   35,    0,   37,   54,
    0,   38,
};
static const YYINT yydgoto[] = {                          7,
   79,  120,  121,   18,   19,   20,    8,   44,   45,   46,
   47,   48,   49,   50,   51,   52,   53,   54,  129,  168,
   98,   55,   56,   57,   80,   28,   81,   58,   59,   60,
   61,   82,   99,   62,   83,
};
static const YYINT yysindex[] = {                       -80,
    0,    0,    0,    0, -260,    0,    0,  -80,    0,    0,
 -258,  -30,    0,   -8, -177, -177,    0,   -4,   10,    0,
    0, -208, -208,   34,  -55, -177,    0,    0,    0,  -44,
  272,    0,  272,  158,  -36, -173,   55,   60,   61,  272,
  272,    0,    0,    0,    0,    0,  272,    0,   39,   42,
  -20,    0,    0,    0,    0,    0,   47,    0, -160,    0,
    0, -159,  -14,    0,    0,    0,  -35,    0,    0,    0,
    0,    0,    0,    0,    0,  216,    0,  216,    0,  179,
    0,    0,  216,  216,  216,    0,  216,  216, -259, -175,
   -9,    0,    0,  121,    0,    0,  -34,   74,   75,   29,
   75,    0,  216,   -2,  383,  216,  216,  216,  216,  216,
  216,  216,  216,  216,  216,  216,  216,    0,    0,   80,
   79,  192,    0,  310,   19,   32,   33,    0,  -29,   86,
    0,    0,  192,  216, -140, -138,  323,    0,    0,  -67,
    0,  438, -211, -211, -211, -272,  157, -107, -272,    0,
 -211,    0,  216,    0,  272,  272, -259,  216,  216,  345,
    0,   40,    0,  192, -163,    0,    0,   83,  192,   45,
    0,  216,  272, -259,   84,  370,    0,    6,    0,    0,
  272,    0,
};
static const YYINT yyrindex[] = {                       139,
    0,    0,    0,    0,    0,    0,    0,  144,    0,    0,
    0,    0,    0,    0,  105,  105,    0,    0,  106,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   24,    0,   24,    0,   91,    0,    0,    0,    0,    0,
   24,    0,    0,    0,    0,    0,   30,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -41,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  105,    0,    0,    0,    0,  -25,    0,
    0,    0,    0,    0,    0,    0,  -21,   97,   98,    0,
  100,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  119,   44,    0,    0,    0,    0,   91,    0,    0,    0,
    0,    0,    5,    0,    0,    0,    0,    0,    0,   17,
    0,  104,   58,   71,   81,  -28,  300,    4,  -15,    0,
   94,    0,    0,    0,    0,    0,    0,  113,    0,    0,
    0,    0,    0,   48,  257,    0,    0,    0,  124,    0,
    0,    0,    0,   49,    0,    0,    0,    0,    0,    0,
    0,    0,
};
static const YYINT yygindex[] = {                         0,
  -19,    0,    0,  164,    0,  160,    0,   66,  -23,    0,
    0,    0,  -87,    0,   36,    0,    0,    0,   15,    0,
    0,    0,    0,    0,  593,  167,    0,   20,   25,    0,
    0,    0,  129,   27,    0,
};
#define YYTABLESIZE 765
static const YYINT yytable[] = {                         69,
  107,  128,   69,   84,   84,   16,   12,  127,   14,   15,
  116,   43,   82,   43,  157,   82,   90,   69,   43,   10,
   43,   43,   51,   92,   11,   81,   36,   43,   81,  158,
   82,   16,   11,   43,   21,   21,   25,   51,  138,   22,
   22,   23,   23,   81,   74,   92,  181,   74,   92,  157,
   22,   69,   23,   26,   85,  103,  134,   73,   27,  155,
   73,  107,   74,   92,   82,    9,  112,   31,   63,  167,
  115,  116,  156,   13,   30,   73,   91,   81,   33,    1,
    2,    3,   69,    4,   16,  175,  128,   16,   17,   43,
    6,   17,   43,   86,   87,   82,   74,   93,   78,   88,
   89,   78,   94,  123,   95,   96,   97,  100,   81,   73,
  102,   77,   17,  130,   77,  131,   78,  135,  136,  134,
  152,   79,  153,   85,   79,  159,  161,   74,  162,   77,
  172,  165,  166,  173,   80,   43,   43,   80,   99,   79,
   73,  174,  179,    2,   75,   99,    8,   75,   99,  177,
   78,   88,   80,   43,   18,   48,   50,  182,   49,   15,
   76,   43,   75,   77,  106,  107,  108,  109,  110,  111,
  112,   40,  113,   79,  115,  116,    1,    2,    3,   24,
    4,   78,   39,    5,  117,   32,   80,    6,  178,   29,
  101,    0,    0,    0,   77,    0,   75,   76,    0,    0,
    0,    0,    0,    0,   79,  107,  108,  109,  110,  111,
  112,    0,  113,    0,  115,  116,   77,   80,    0,    0,
    0,    0,    0,    0,  117,    0,    0,   75,    0,    0,
   69,   69,   69,   69,   69,   69,   69,  118,   69,   69,
   69,   69,    0,   82,   78,   82,   82,   82,   82,   82,
   69,   82,   82,   82,    0,   76,   81,    0,   81,   81,
   81,   81,   81,   82,   81,   81,   81,    0,    0,  106,
  107,  108,  109,  110,  111,  112,   81,  113,  114,  115,
  116,   78,    0,    0,   74,    0,    0,    0,   73,  117,
  106,  107,  108,  109,  110,  111,  112,   73,  113,  114,
  115,  116,    0,  106,  107,  108,  109,  110,  111,  112,
  117,  113,  114,  115,  116,   34,  106,  107,  108,  109,
  110,  111,  112,  117,  113,  114,  115,  116,    0,   78,
   42,   78,   78,   78,   78,    0,  117,   78,   78,   78,
   76,    0,   77,   76,   77,   77,   77,   77,    0,   78,
   77,   77,   79,    0,   79,   79,   79,   79,   76,    0,
   79,   79,   77,    0,    0,   80,    0,   80,   80,   80,
   80,    0,   79,   80,   80,   75,    0,   75,    0,   34,
   64,   34,   65,   66,   75,   80,    0,   67,    0,   68,
   69,   70,   76,    0,   41,    0,    0,    0,   71,   72,
    0,    0,  154,    0,   73,   74,   75,  132,    0,    0,
    0,    0,    0,    0,    0,  163,    0,   64,    0,   65,
   66,    0,    0,   76,   67,    0,   68,   69,   70,  107,
    0,  109,  110,  111,  112,   71,   72,  171,  115,  116,
    0,   73,   74,   75,    0,    0,    0,    0,  117,    0,
  106,  107,  108,  109,  110,  111,  112,    0,  113,  114,
  115,  116,  180,  106,  107,  108,  109,  110,  111,  112,
  117,  113,  114,  115,  116,   64,    0,   65,   66,    0,
    0,    0,   67,  117,   68,   69,   70,    0,    0,    0,
    0,    0,    0,   71,   72,    0,    0,    0,    0,   73,
   74,   75,    0,    0,    0,    0,  139,    0,    0,    0,
    0,    0,    0,   34,   34,   34,    0,   34,    0,    0,
   34,    0,   34,   34,   34,    0,    0,    0,    1,    2,
    3,    0,    4,    0,    0,    5,    0,   34,   35,    6,
    0,    0,   34,    0,   34,   34,   34,    0,    0,   34,
   34,    0,    0,    0,    0,    0,    0,   36,    0,   37,
   38,   17,    0,    0,   39,   40,    0,    0,    0,    0,
    0,   76,    0,   76,    0,    0,    0,    0,    0,   76,
   76,  106,  107,  108,  109,  110,  111,  112,    0,  113,
  114,  115,  116,    0,  106,  107,  108,  109,  110,  111,
  112,  117,  113,  114,  115,  116,    0,    0,    0,    0,
    0,    0,    0,    0,  117,    0,  106,  107,  108,  109,
  110,  111,  112,    0,  113,  114,  115,  116,    0,    0,
    0,    0,    0,    0,    0,    0,  117,    0,    0,    0,
    0,  106,  107,  108,  109,  110,  111,  112,    0,  113,
  114,  115,  116,    0,  106,  107,  108,  109,  110,  111,
  112,  117,  113,  114,  115,  116,    0,    0,  104,    0,
  105,    0,    0,    0,  117,  119,  122,  124,    0,  125,
  126,    0,    0,    0,    0,    0,  133,    0,    0,    0,
    0,    0,    0,    0,    0,  137,    0,    0,  140,  141,
  142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
  107,    0,  109,  110,  111,  112,    0,  113,    0,  115,
  116,    0,    0,    0,    0,    0,  160,    0,    0,  117,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  164,    0,    0,    0,    0,
  169,  170,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  176,
};
static const YYINT yycheck[] = {                         41,
  273,   89,   44,   40,   40,   40,  267,  267,  267,   40,
  283,   31,   41,   33,   44,   44,   40,   59,   44,    0,
   40,   41,   44,   47,    0,   41,  286,   47,   44,   59,
   59,   40,    8,   59,   15,   16,   41,   59,   41,   15,
   16,   15,   16,   59,   41,   41,   41,   44,   44,   44,
   26,   93,   26,   44,   91,   91,   91,   41,  267,   41,
   44,  273,   59,   59,   93,    0,  278,  123,   33,  157,
  282,  283,   41,    8,   41,   59,   41,   93,  123,  257,
  258,  259,  124,  261,   41,   41,  174,   44,   41,   41,
  268,   44,   44,  267,   40,  124,   93,   59,   41,   40,
   40,   44,   61,   84,  125,   59,  267,  267,  124,   93,
  125,   41,  290,  289,   44,  125,   59,   44,   44,   91,
   41,   41,   44,   91,   44,   40,  267,  124,  267,   59,
   91,  155,  156,  297,   41,  155,  156,   44,    0,   59,
  124,   59,   59,    0,   41,   41,   41,   44,  125,  173,
   93,   61,   59,  173,  125,   59,   59,  181,   59,   41,
   40,  181,   59,   93,  272,  273,  274,  275,  276,  277,
  278,   59,  280,   93,  282,  283,  257,  258,  259,   16,
  261,  124,   59,  264,  292,   26,   93,  268,  174,   23,
   62,   -1,   -1,   -1,  124,   -1,   93,   40,   -1,   -1,
   -1,   -1,   -1,   -1,  124,  273,  274,  275,  276,  277,
  278,   -1,  280,   -1,  282,  283,   59,  124,   -1,   -1,
   -1,   -1,   -1,   -1,  292,   -1,   -1,  124,   -1,   -1,
  272,  273,  274,  275,  276,  277,  278,   59,  280,  281,
  282,  283,   -1,  272,  124,  274,  275,  276,  277,  278,
  292,  280,  281,  282,   -1,   40,  272,   -1,  274,  275,
  276,  277,  278,  292,  280,  281,  282,   -1,   -1,  272,
  273,  274,  275,  276,  277,  278,  292,  280,  281,  282,
  283,  124,   -1,   -1,  281,   -1,   -1,   -1,  272,  292,
  272,  273,  274,  275,  276,  277,  278,  281,  280,  281,
  282,  283,   -1,  272,  273,  274,  275,  276,  277,  278,
  292,  280,  281,  282,  283,   59,  272,  273,  274,  275,
  276,  277,  278,  292,  280,  281,  282,  283,   -1,  272,
   59,  274,  275,  276,  277,   -1,  292,  280,  281,  124,
   41,   -1,  272,   44,  274,  275,  276,  277,   -1,  292,
  280,  281,  272,   -1,  274,  275,  276,  277,   59,   -1,
  280,  281,  292,   -1,   -1,  272,   -1,  274,  275,  276,
  277,   -1,  292,  280,  281,  272,   -1,  274,   -1,  123,
  260,  125,  262,  263,  281,  292,   -1,  267,   -1,  269,
  270,  271,   93,   -1,  123,   -1,   -1,   -1,  278,  279,
   -1,   -1,   93,   -1,  284,  285,  286,  287,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   93,   -1,  260,   -1,  262,
  263,   -1,   -1,  124,  267,   -1,  269,  270,  271,  273,
   -1,  275,  276,  277,  278,  278,  279,   93,  282,  283,
   -1,  284,  285,  286,   -1,   -1,   -1,   -1,  292,   -1,
  272,  273,  274,  275,  276,  277,  278,   -1,  280,  281,
  282,  283,   93,  272,  273,  274,  275,  276,  277,  278,
  292,  280,  281,  282,  283,  260,   -1,  262,  263,   -1,
   -1,   -1,  267,  292,  269,  270,  271,   -1,   -1,   -1,
   -1,   -1,   -1,  278,  279,   -1,   -1,   -1,   -1,  284,
  285,  286,   -1,   -1,   -1,   -1,  124,   -1,   -1,   -1,
   -1,   -1,   -1,  257,  258,  259,   -1,  261,   -1,   -1,
  264,   -1,  266,  267,  268,   -1,   -1,   -1,  257,  258,
  259,   -1,  261,   -1,   -1,  264,   -1,  266,  267,  268,
   -1,   -1,  286,   -1,  288,  289,  290,   -1,   -1,  293,
  294,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  288,
  289,  290,   -1,   -1,  293,  294,   -1,   -1,   -1,   -1,
   -1,  272,   -1,  274,   -1,   -1,   -1,   -1,   -1,  280,
  281,  272,  273,  274,  275,  276,  277,  278,   -1,  280,
  281,  282,  283,   -1,  272,  273,  274,  275,  276,  277,
  278,  292,  280,  281,  282,  283,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  292,   -1,  272,  273,  274,  275,
  276,  277,  278,   -1,  280,  281,  282,  283,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  292,   -1,   -1,   -1,
   -1,  272,  273,  274,  275,  276,  277,  278,   -1,  280,
  281,  282,  283,   -1,  272,  273,  274,  275,  276,  277,
  278,  292,  280,  281,  282,  283,   -1,   -1,   76,   -1,
   78,   -1,   -1,   -1,  292,   83,   84,   85,   -1,   87,
   88,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  103,   -1,   -1,  106,  107,
  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
  273,   -1,  275,  276,  277,  278,   -1,  280,   -1,  282,
  283,   -1,   -1,   -1,   -1,   -1,  134,   -1,   -1,  292,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  153,   -1,   -1,   -1,   -1,
  158,  159,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  172,
};
#define YYFINAL 7
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 297
#define YYUNDFTOKEN 335
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,"'='",0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'","'|'","'}'",0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"INTEGER","CHAR","CHARPTR","CHAR_CONST","BOOLEAN","BOOLEAN_CONST",
"BINARY_CONST","VOID","VAR","RETURN","IDENTIFIER","INTPTR","DECIMAL_CONST",
"NULL_","OCTAL_CONST","AND","DIV","EQUAL","GE","GREATER","LE","MINUS","NOT",
"NE","OR","PLUS","MULTI","ADDRESS_OP","HEX_CONST","DEREFERENCE","STRING_CONST",
"IF","WHILE","STRING","FLOAT_CONST","LESS","FOR","DO","UNARY","ENDIF","ELSE",0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : program",
"program : empty",
"program : multi_func",
"multi_func : func",
"multi_func : multi_func func",
"func : VOID IDENTIFIER '(' list_param_func ')' '{' code_block '}'",
"func : n_types IDENTIFIER '(' list_param_func ')' '{' code_block '}'",
"list_param_func : empty",
"list_param_func : list_param_func_rec",
"list_param_func_rec : declare_one_var",
"list_param_func_rec : list_param_func_rec ',' declare_one_var",
"declare_one_var : n_types id_node",
"declare_one_var : str_type id_node",
"func_call : IDENTIFIER '(' list_arg_func ')'",
"list_arg_func : empty",
"list_arg_func : list_arg_func_rec",
"list_arg_func_rec : expression",
"list_arg_func_rec : list_arg_func_rec ',' expression",
"multi_statements : stat_rec",
"multi_statements : empty",
"stat_rec : statement",
"stat_rec : stat_rec statement",
"statement : assign ';'",
"statement : func_call",
"statement : if",
"statement : if_else",
"statement : while",
"statement : '{' code_block '}'",
"statement : ';'",
"statement : doWhile",
"statement : for",
"statement : RETURN expression ';'",
"statement : RETURN ';'",
"statement : var_declarations",
"if : IF '(' expression ')' statement",
"if_else : IF '(' expression ')' statement ELSE statement",
"while : WHILE '(' expression ')' statement",
"doWhile : DO statement WHILE '(' expression ')' ';'",
"for : FOR '(' inits_update ';' exp_for ';' inits_update ')' statement",
"exp_for : expression",
"exp_for :",
"inits_update : assign",
"inits_update : inits_update ',' assign",
"inits_update :",
"code_block : multi_statements",
"var_declarations : var_declarations_ne",
"var_declarations_ne : func",
"var_declarations_ne : declare_vars ';'",
"declare_vars : n_types var_identifier",
"declare_vars : str_type var_arr_identifier",
"declare_vars : n_types var_arr_identifier",
"var_identifier : IDENTIFIER",
"var_identifier : var_identifier ',' IDENTIFIER",
"var_arr_identifier : IDENTIFIER '[' expression ']'",
"var_arr_identifier : var_arr_identifier ',' IDENTIFIER '[' expression ']'",
"n_types : CHAR",
"n_types : INTEGER",
"n_types : BOOLEAN",
"n_types : CHARPTR",
"n_types : INTPTR",
"str_type : STRING",
"const_value : CHAR_CONST",
"const_value : BOOLEAN_CONST",
"const_value : NULL_",
"const_value : integer_literal",
"integer_literal : BINARY_CONST",
"integer_literal : OCTAL_CONST",
"integer_literal : HEX_CONST",
"integer_literal : DECIMAL_CONST",
"expression : IDENTIFIER",
"expression : IDENTIFIER '[' expression ']'",
"expression : const_value",
"expression : func_call",
"expression : expression AND expression",
"expression : expression OR expression",
"expression : expression EQUAL expression",
"expression : expression NE expression",
"expression : expression GREATER expression",
"expression : expression GE expression",
"expression : expression LE expression",
"expression : expression LESS expression",
"expression : expression PLUS expression",
"expression : expression MINUS expression",
"expression : expression MULTI expression",
"expression : expression DIV expression",
"expression : '(' expression ')'",
"expression : '|' expression '|'",
"expression : op_unary expression",
"lhs : IDENTIFIER",
"lhs : DEREFERENCE IDENTIFIER",
"lhs : IDENTIFIER '[' expression ']'",
"assign : assignment",
"assignment : lhs '=' expression",
"assignment : lhs '=' STRING_CONST",
"op_unary : ADDRESS_OP",
"op_unary : DEREFERENCE",
"op_unary : NOT",
"op_unary : MINUS",
"id_node : IDENTIFIER",
"empty :",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 281 "parser.y"


// Simple node is when the node :
// 1. isn't NULL
// 2. isn't Leaf
// 3. has sons only as leaf
// like: a[b,c,d]      
// to a has only the sons b, c, d as a leaf
// and not like: a[b[b1, b2], c, d[d1]]
// to a has son b ,c, d that the son b has a sons b1, b2, and the son d has the son d1 , etc..
int isSimpleTree(node* head)
{
	int i;
	
	// if this node is NULL return false
	if(head == NULL) 
		return 0;
	// if this node is NULL return false
	if(head->count == 0) 
		return 0;

	// if this son node i  of the node param is not NULL AND there are some sons for the sons return false
	for(i = 0; i < head->count ; i++)
		if(head->sons[i] != NULL && head->sons[i]->count > 0) 
			return 0;

	// return true
	return 1;
}
// the makeNode method get the value of the node, and the number of the sons
// , and the ... (the va_args an array of node that represent the sons that pass to this node) 
// for example-> IF_ELSE : IF '(' expression ')' '{' code_block '}' ELSE '{' code_block '}'
//                  $$   : $1  $2    $3       $4  $5    $6      $7   $8   $9     $10    $11 
// makeNode("COND", 3 , $3, $6, $10);
// the Node "COND" (come from IF-ELSE) , has 3 sons, like the following chart: 
//  
//                ( COND )
//             /     |       \
//            /      |        \
//           /       |         \
//     EXP ($3) CODE_BLOCK($6) CODE_BLOCK($10)
// arguments of the function:  
// va_list = [EXP, CODE_BLOCK, CODE_BLOCK ];
// count   = 3; (len of the va_args list)
// value   = "COND"
node* makeNode(char *value, int count,...)
{
	va_list ap;
  	node *p;
	node **alocSons;
  	int i;
	int AllSonIsNull = 1;

  	// if allocate node feiled - stop the program by message out of memory
	if ((p = (node*)malloc(sizeof(node))) == NULL) yyerror("out of memory");

  	// copy information
  	p->value = value;
  	p->count = count;
	// count is the number to allocate memory of array nodes for the son array in the parent node value
	if(count != 0)
	{
		// if allocate node feiled - stop the program by message out of memory
		if ((alocSons = (node**)malloc(count * sizeof(node*))) == NULL) yyerror("out of memory");
		
		// init the alocated son array in the va_arg , mean ... that pass as argument to function 
		va_start(ap, count);
		for (i = 0; i < count; i++)
		{
			alocSons[i] = va_arg(ap, node*);
			// exist at leat on son
			if(alocSons[i] != NULL)
				AllSonIsNull = 0;
		}
		va_end(ap);


		p->sons = alocSons;
		
		if(AllSonIsNull)
			return NULL;
    	}
	else
	{
		// if the argument count is <= 0, mean that there aren't any sons
		p->sons = NULL;
	}
	return p;
}

//the makeNode2 method get the value of the node, type and kind that in the file parser.h and the number of the sons
// , and the ... (the va_args an array of node that represent the sons that pass to this node) 
// 
node* makeNode2(char *value, type t, kind k, int count,...) 
{
    va_list ap;
    node *p;
    node **s;
    int i;
    int flag = 1;

    /* allocate node */
    if ((p = (node*)malloc(sizeof(node))) == NULL)
        yyerror("out of memory");
	
    /* copy information */
    p->value = value;
    p->t = t; // update the type of the node
    p->k = k; // update the kind of the node
    p->count = count;
    if(count != 0)
    {
	if ((s = (node**)malloc(count * sizeof(node*))) == NULL)
		yyerror("out of memory");
	va_start(ap, count);
	for (i = 0; i < count; i++)
	{
		s[i] = va_arg(ap, node*);
		if(s[i] != NULL)
			flag = 0;
	}
	va_end(ap);
	p->sons = s;
	if(flag)
	    return NULL;
    }
    else
    {
	p->sons = NULL;
    }
    return p;
}

// the makeParent methods , define a parent node to a child node with type and kind properties
// by default the type is DEF, and the kind is NONE
node* makeParent2(char* value, type t, kind k, node* child)
{
	if(child == NULL)
		return NULL;
	if(child->value != "" || child->k != NONE || child->t != DEF)
	{
		return makeNode2(value,t,k,1,child);
	}
	child->value = value;
	child->t = t;
	child->k = k;
	return child;
}

node* makeParent(char* value, node* child) {
	return makeParent2(value, DEF, NONE, child);
}

// print the tree AST , in the preorder tree print 
// like : int main(){ if(c == 0){ a1 = 1; } a3 = 3; }
// ( MAIN
//     (COND
//         ( == C 0)
//         ( BLOCK
//            (= a1 1)
//         )
//     )
//     (= a3 3)
// )
void printTree(node* ast, int indent, int printInNewLine, int l)
{
	int lenSpacesInTab = 4; // the len of the indent spaces from start the line
	int i = 0;    		// for the for loop on the sons */
	int sonsOnlyAsLeafs = 0;// for print in the sons in the same line of the parent , the flag var*/
	if(ast == NULL) 	// if this is a leaf , go back to the parent for continue
		return; 

	// if the ast node is t node with any sons...
	// ...print new line of node in the tree 
	if(ast->count > 0 && ast->value != "") 
		printf("\n%*s( ",indent,""); 	// print (int) inded spaces from start of the line , and print (
	
	// if the ast node as not sons AND print in the new line var boolean is false ,then the print in the same line 
	if(!printInNewLine && ast->count == 0) // the printing in the same line
		printf("\n%*s",indent,"");     // print (int) inded spaces from start of the line

	// print the value of the node
	if(ast->value != "") 
	{
		printf("%s ",ast->value);
	}
	else 
	{
		// if the node is empty , return tab back
		indent--;
	}
	// we move on all the sons ,after we print the parent (this is preorder print)
	for(i = 0; i < ast->count; i++)
	{
		// if there is son i
		if(ast->sons[i] != NULL)
		{
			// check if it is leaf , or certain value [ARGS | VAR] , print in the same line (VAR a b c)
			if(isSimpleTree(ast) || ast->value == "" || 
			  ast->value == "INTEGER" || ast->value == "CHAR" || 
			  ast->value == "BOOLEAN" || ast->value == "CHARPTR" || 
			  ast->value == "ARRAY" || ast->value == "INTPTR") 
			{
				printInNewLine = 1;
			}
			else 
			{
 				// print in new lines ,like L2,L3
				// L0: (ARG a b c ) 
				// L1: (COND 
				// L2: 	  ( > a b ) 
				printInNewLine = 0;
			}
			
			// check if it 
			if(l)
			{
				// if we stop to print all the variables in the lines like: 
				// (VAR a b c d e f g )
				// then stop the 
				if(strlen(ast->value) == 0)
				{
					l = 0;
				}
				printTree(ast->sons[i],indent + lenSpacesInTab,printInNewLine, l);
			}
			else
			{
				// if the len of the value is not empty , then set the 
				if(strlen(ast->value) != 0)
				{
					l = 1;
					printTree(ast->sons[i],indent + lenSpacesInTab,printInNewLine, l);
				}
				else
				{
					printTree(ast->sons[i],indent + 1 ,printInNewLine, l);
				}
			}
			
			// check if there aren't any sons to this son i 
			if(ast->sons[i]->count > 0)
			{ 
				sonsOnlyAsLeafs = 1;
			}
		}
	}
	
	// check if we are in the new line , that the end of the parent subroot tree , then close the node , like L3
	// L1: (COND 
	// L2:     ( > a b ) 
	// L3: )
	if(sonsOnlyAsLeafs == 1 && ast->value != "" && ast->value != "INTEGER" && ast->value != "CHAR" && 
		ast->value != "BOOLEAN" && ast->value != "CHARPTR" && ast->value != "ARRAY" && ast->value != "INTPTR")
	{
		printf("\n%*s%s",indent,"",")");
	}
	
	// if we are in the end of follow cases: 
	// case 1: we now at a tree that consist of...
	// ...parent and sons without sonss' sons , Like:  "( >= a b " , or,  "( VAR a b " 
	// case 2: we now at the line like : "( >= a b " 
	// then print the right prenteses
	else if ((ast->count > 0 && ast->value != "") || ast->value == "INTEGER" || ast->value == "CHAR" || 
		ast->value == "BOOLEAN" || ast->value == "CHARPTR" || ast->value == "ARRAY" || ast->value == "INTPTR")
	{
		printf(")");
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// get out from the list , the first param 
void pop(node_t ** head) {
	node_t * next_node = NULL;

	if (*head == NULL) 
		return;

	next_node = (*head)->next;
	if((*head)->args != NULL)
		free((*head)->args);
	free(*head);
	*head = next_node;
}

// insert into the list a new node_t in the first place 
node_t* push(node_t ** head, char *name, char *value, kind k, type t) {
	node_t * new_node;
	new_node = malloc(sizeof(node_t));

	new_node->name = strdup(name);
	new_node->value = strdup(value);
	new_node->k = k;
	new_node->t = t;
	// for type of the function, to know the uses of return statment
	if(k == FUNC &&t != NONE) 	new_node->useReturn = 0;	
	else if(k == FUNC &&t == NONE)	new_node->useReturn = -1;
	new_node->argCount = 0;
	new_node->args = NULL;
	new_node->next = *head;
	*head = new_node;

	return new_node;
}

// open a new scope in the scope list 
void enterScope(char* nameScope){
	scope* new_scope;
	new_scope = malloc(sizeof(scope));
	new_scope->l = NULL;
	new_scope->nameScope = nameScope;
	new_scope-> next = head;
	head = new_scope;
	live_scopes++;
	enterGenScope(nameScope);
}
// close the last scope in the scope list 
void endScope() {
	scope * next_scope = NULL;

	if (head == NULL) {
		return;
	}

	next_scope = head->next;
	while((head->l) != NULL) {
		pop(&(head->l));
	}
	free(head);
	head = next_scope;
	live_scopes--;
	endGenScope();
}

// find the variable <name> in the current scope s
node_t* findLocal(scope *s, char *name) {
	node_t *n = s->l;
	
	while(n != NULL) {
		if(!strcmp(name,n->name))
			return n;
		n = n->next;
	}
	
	return NULL;
}
// find the variable <name> in all the scope until here
node_t* findGlobal(char* name) {
	scope *s = head;
	node_t *n = NULL;
	
	while(s) {
		if((n = findLocal(s,name)) != NULL)
			return n;
		s = s->next;
	}
	
	return NULL;
}


//-----------------------------------------------------------------------------------------------------------------
/* functions for args and vars */
// add all the vars of the function to the current scope
void addVars(node *vars) {
	int i;
	if(vars->k == ARG || vars->k == DF) {
		for(i = 0; i < vars->count; i++) {
			if(vars->sons[i] != NULL ){
				addVar(vars->sons[i],vars->t);
			}
		}	
		return;
	}
	for(i = 0; i < vars->count; i++) {
		if(vars->sons[i] != NULL ){
			addVars(vars->sons[i]);
		}
	}
}
// add the var of the function to the current scope
void addVar(node *var,type t) {
	char message_error[256];
	int i;
	if(var->count == 0) 
	{
		if(findLocal(head,var->value) != NULL)
		{
			//copy the format string to the message_error and print it in the exit_program method
			sprintf(message_error, "765_ERROR:The variable \"%s\" has been defined more than once.\n", var->value);
			exitProgram(message_error);
		}
		else	
		{
			push(&(head->l), var->value, "", VAR, t);	
		//node_t* push(node_t ** head, char *name, char *value, kind k, type t) {
		}
		return;
	}
	// if this var is node: ID [ exp ], Just add the ID , And ignore from all his sons
	if(var->count == 3 && strcmp(var->sons[0]->value, "[") == 0  &&  strcmp(var->sons[2]->value, "]") == 0){
		if(getExprType(var->sons[1]) == INTEGER)
		{	
			if(var->sons[1]->k == CONST){				
				push(&(head->l), var->value, "", VAR, t);		
			}
			else{
				sprintf(message_error, "ERROR:The expression inside the [] operator In declaration of var string \"%s\" is must be CONST int\n", var->value);
				exitProgram(message_error);
			}
		}
		else{
			sprintf(message_error, "ERROR:The expression inside the [] operator of var string \"%s\" need to be INT\n", var->value);
			exitProgram(message_error);
		}
		if(t != ARRAY)
		{ 
			sprintf(message_error,  "ERROR:The variable \"%s\" can not use [] because it is not a string array.\n", var->value);
			exitProgram(message_error);
		}		
		return;
	}
	for(i = 0; i < var->count; i++) 
	{
		if(var->sons[i] != NULL){	
			addVar(var->sons[i], t);
		}
	}
}

// add all the args to the list in scope of the func 
void addArgs(node *args, node_t *f) {
	int i;
	if(args == NULL) {printf("ERROR: void addArgs >>> if(args == NULL)\n");exit(0);}
	if(args->k == ARG) {
		for(i = 0; i < args->count; i++) {
			if(args->sons[i] != NULL )
				addArg(args->sons[i],args->t, f);
		}	
		return;
	}
	for(i = 0; i < args->count; i++) 
		if(args->sons[i] != NULL )
			addArgs(args->sons[i], f);
}
// add add arg to the arg list in scope of the func 
void addArg(node *arg,type t, node_t *f) {
	int i;
	char message_error[256];
	//printNode(arg);
	if(arg->count == 0)  // if it the ID node with no son
	{
		if(findLocal(head,arg->value) != NULL)
		{
			sprintf(message_error, "ERROR:The argument \"%s\" has been defined more than once at the function \"%s\".\n", arg->value,f->name);
			exitProgram(message_error);
		}
		else	
		{
			push(&(head->l), arg->value, arg->value, ARG, t);
			
		}
		f->argCount = f->argCount + 1;
		f->args = (type *)realloc(f->args, (f->argCount)*sizeof(type));
		f->args[(f->argCount) - 1] = t;
		return;
	}
	
	for(i = 0; i < arg->count; i++) {
		if(arg->sons[i] != NULL)
			addArg(arg->sons[i], t, f);
	}
}


// -------------------------------------------------------------------------------------------------------------------

node* createNewNode(){
	node * newNodeAllocated;
	if ((newNodeAllocated = (node*)malloc(sizeof(node))) == NULL) yyerror("out of memory");
	newNodeAllocated->sons = NULL;
	newNodeAllocated->count = 0;
	newNodeAllocated->value = "";
	return newNodeAllocated;
}
// get nested node, and return all the nested nodes in one array node of sons , like the example
node* getNestedNodesInOneNode(node *program){
	/*
	my program tree see like that: 
			PROGRAM
			FUNCTION
			FUNC1
	or like that:
			PROGRAM
			FUNCTION
		FUNCTION 	FUNC1
	    FUNCTION    FUNC2	
	     FUNC3
	return : FUNC3, FUNC2, FUNC1 
	*/	
	char message_error[256];
	int i, g;
	node * result = createNewNode();
	
	// empty node
	if(program == NULL || program->count == 0) 
		return program; 	
	
	// pre node of the binary list node
	if(program->count == 1)
		program = program->sons[0];	// down to the first FUNCTION node
	
	// if it binary node , that expected to get 
	if(program->count == 2)
	{	
		// for each 2 node, add the right son , and move to the left son
		while(program->count == 2)
		{	
			result = addSibling(result, program->sons[1]); // add the right son 
			handleIfItMain(program->sons[1]); // check if this node is main function	
			program = program->sons[0]; // go to the left son - FUNCTION
		}
		// the last son that we have , is -> node: FUNCTION > node: foo , so go to son and add it
		result = addSibling(result, program->sons[0]);
		handleIfItMain(program->sons[0]);
		reverseSons(&result); // we got in reverse list, so we reverse it to get in the right order
	}
	else{ // if we got only one node that maybe is only the main function 
		handleIfItMain(program->sons[0]);
		result = addSibling(result, program->sons[0]);
	}
	if(hasMainAllrady == 0) // if we have not main at all the program , error it
	{
		sprintf(message_error, "ERROR:The program must contain one function \"main\".\n");
		exitProgram(message_error);
	}
	return result;
}

// * * * CHECKING METHODS - PROGRAM , FUNC, STATEMENT, WHILE, ... * * * 

// check the program node
char* checkProgram(node *program) {
	int i = 0;
	char *code = "";
	lineCount = 0;
	varCount = 0;
	enterScope("Program");
	if(program != NULL)
	{
		program = getNestedNodesInOneNode(program);
		for(i = 0;program != NULL && i < program->count ; i++){
			asprintf(&code,"%s\n%s",code,checkFunc(program->sons[i]));
		}
	}
	endScope();
	return code;	
}
// check the vlidation of the func node 
char* checkFunc(node *func) {
	char message_error[256];
	node_t *f;
	int i;
	int cScope = 0;
	int layoutOutFunc = 0;
	char nameScope[256];
	
	char *code = "";
	int size = 0;
	int countVars = varCount;
	int countLines = lineCount;

	if(func->k != FUNC) 
	{
		if(func->sons[0]->k != FUNC)
		{
			printNode(func);
			printf("848: Isn't func node, exit from checkFunc()\n");
			return code;
		}
		else
		{
			func = func->sons[0];
		}
	}

	//varCount = 0;  // for this var in the functions, start from zero
	lineCount = lineCount + 2; // because the first line is for the command 'BeginFunc' // lineCount = 2;

	if(findLocal(head,func->value))
	{
		sprintf(message_error, "ERROR:The function \"%s\" has been defined more than once.\n", func->value);
		exitProgram(message_error);
	}

	f = push(&(head->l), func->value, func->value, func->k, func->t);
	for(i = 0; i < func->count; i++) 
	{
		if(func->sons[i] != NULL )
		{
			if(func->sons[i]->k == ARG)
			{
				int j;
				cScope ++;
				sprintf(nameScope, "arg's %s ", func->value);
				enterScope(nameScope);
				func->sons[i] = getNestedNodesInOneNode(func->sons[i]);
				for(j = 0; func->sons[i] != NULL && j < func->sons[i]->count; j++)
						addArgs(func->sons[i]->sons[j], f);
			}
			else if(func->sons[i]->k == BLOCK) 
			{	
				cScope++;
				sprintf(nameScope, "block's %s ", func->value);
				enterScope(nameScope);
				code = checkBlock(func->sons[i], f, layoutOutFunc ,&size);
			}		
		}
	}
	while(cScope){
		endScope();
		cScope--;
	}
	if(f->useReturn == 0) {
		sprintf(message_error, "ERROR:The return of function \"%s\" is missed.\n", f->name);
		exitProgram(message_error);
	}
	

	//varCount = curVar + varCount;
	//lineCount = curLine + lineCount;

	asprintf(&code,"%s:\n\tBeginFunc %d;\n\t%sEndFunc;\n\t",func->value,size,code);
	lineCount = lineCount + 1; // 
	//int reg = varCount - countVars + 1;
	//char* summeryFunc = "";
	//asprintf(&summeryFunc,"%s\t\t\t%d - registers\n\t",summeryFunc,reg);
	//printf("%s",summeryFunc);
	//asprintf(&code,"%s%s",code, summeryFunc);
	return(code);
}

// check the block validation, each block has son kind of V, and statments
char* checkBlock(node *block , node_t *last_func, int layout, int *size) {
	int i, j;
	node *temp;
	char *code = "";
	for(i = 0; i < block->count; i++) 
	{	
		if(block->sons[i] != NULL && block->sons[i]->k == STMNT){ 		
			temp = getNestedNodesInOneNode(block->sons[i]);
			checkStmnts(temp, last_func, layout,size, &code);
		}
	}
	return code;
}

// check the statment validation, each statement has son kind of ASSument, Func_call, IF, IfElse, While, BLOCK
// for anything son else do the checkStmnts to them
void checkStmnts(node *stmnts , node_t *last_func, int layout, int *size, char **code) {
	char message_error[256];
	int i, j, k, hasStatment = 0;
	node* node_for_lable;
	node* temp;
	for(i = 0; i < stmnts->count; i++) 
	{
		if(stmnts->sons[i] != NULL) 
		{
			if(stmnts->sons[i]->k != DEC) hasStatment = 1;
			
			if(stmnts->sons[i]->k == DEC) 
			{	
				temp = getNestedNodesInOneNode(stmnts->sons[i]);
				for(j = 0; temp != NULL && j < temp->count; j++)
				{
					if(	temp->sons[j] != NULL && temp->sons[j]->k == DF)			  
					{ 
						asprintf(code,"%s\n%s",*code, checkFunc(temp->sons[j]->sons[0]));
					}
					else if(temp->sons[j] != NULL &&  temp->sons[j]->k == ARG||temp->sons[j]->k == DV)
					{ 
						addVars(temp->sons[j]);
						getVarsSize(temp->sons[j], size);
					}
				}
				if(hasStatment){
					sprintf(message_error, "ERROR:declarations allow only in the first position in the block of function \"%s\".\n", last_func->name);
					exitProgram(message_error);
				}
			}
			
			else if(stmnts->sons[i]->k == ASS){
				asprintf(code,"%s%s",*code,checkAss(stmnts->sons[i]->sons[0], size));
			}

			else if(stmnts->sons[i]->k == FC) {
				checkFuncCall(stmnts->sons[i], size, code, NULL, 1);
			}
			else if(stmnts->sons[i]->k == IF) {
				node *cond , *if_block, *else_block;
				temp = stmnts->sons[i]->sons[0];				
				cond 		= temp->sons[0];
				if_block 	= temp->sons[1];

				if(getExprType(cond->sons[0]) != BOOLEAN) {
					sprintf(message_error, "ERROR:A non boolean expression as a condition of IF statement is illegal.\n");
					exitProgram(message_error);
				}
				char *v;
				node* ifz_exp   = cond->sons[0];
				node* ifz_stmt  = if_block;
				char *a = GenExpr(ifz_exp,size,&v);
				asprintf(code,"%s%sifz %s goto L%%d;\n\t",*code,a, v);
				lineCount++;
				
				if(ifz_stmt->sons[0] != NULL)
					checkStmnts(ifz_stmt,last_func, layout, size, code);
				if((*code)[strlen(*code) - 2] != ':')
					asprintf(code,"%s%s", *code, strLineNum(lineCount));
				asprintf(code,*code, lineCount);
			}
			else if(stmnts->sons[i]->k == IE) 
			{
				node *cond , *if_block, *else_block;
				temp = stmnts->sons[i]->sons[0];				
				cond 		= temp->sons[0];
				if_block 	= temp->sons[1];
				else_block 	= temp->sons[2];

				if(getExprType(cond->sons[0]) != BOOLEAN) 
				{
					sprintf(message_error, "ERROR:A non boolean expression as a condition of IF-ELSE statement is illegal.\n");
					exitProgram(message_error);
				}

				char *v;
				node* ifz_exp       = cond->sons[0];
				node* ifz_stmt_if   = if_block;
				node* ifz_stmt_else = else_block;
				char *a = GenExpr(ifz_exp,size,&v);
				int el;
				asprintf(code,"%s%sifz %s goto L%%d;\n\t",*code,a, v);
				lineCount++;

				checkStmnts(if_block, last_func, layout + 1, size, code);
				lineCount++;
				asprintf(code,"%sgoto L%%d;\n\t%s", *code, strLineNum(lineCount));
				
				el = lineCount;
				checkStmnts(else_block, last_func, layout + 1, size, code);
				if((*code)[strlen(*code) - 2] != ':') 
					asprintf(code,"%s%s", *code, strLineNum(lineCount));

				asprintf(code,*code, el,lineCount);
			}
			else if(stmnts->sons[i]->k == W) {
				if(getExprType(stmnts->sons[i]->sons[0]->sons[0]) != BOOLEAN) {
					sprintf(message_error,  "ERROR:A non boolean expression as a condition of WHILE statement is illegal.\n");
					exitProgram(message_error);
				}
				int l = lineCount;
				if((*code)[strlen(*code) - 2] != ':')
					asprintf(code,"%s%s", *code, strLineNum(l));
				
				char *v;
				char *a = GenExpr(stmnts->sons[i]->sons[0]->sons[0],size,&v);
				asprintf(code,"%s%sifz %s goto L%%d;\n\t",*code, a, v);
				lineCount++;

				if(stmnts->sons[i]->sons[0]->sons[1] != NULL){
					checkStmnts(stmnts->sons[i]->sons[0],last_func, layout + 1, size, code);
				}

				asprintf(code,"%sgoto L%d;\n\t%s",*code, l, strLineNum(++lineCount));
				asprintf(code,*code, lineCount);

			}
			else if(stmnts->sons[i]->k == DW) {
				node* do_while_node = stmnts->sons[i]->sons[0]; 
				int l = lineCount;
				if((*code)[strlen(*code) - 2] != ':')
				{
					asprintf(code,"%s%s", *code, strLineNum(l));
				}

				if(strcmp(do_while_node->sons[0]->value, "DO_WHILE-BLOCK") == 0)
				{
					if(do_while_node->sons[0]->sons[0] != NULL){
						checkStmnts(do_while_node->sons[0],last_func, layout + 1, size, code);
					}
				}
				if(strcmp(do_while_node->sons[1]->value, "DO_WHILE-COND") == 0) 
				{
					if(getExprType(stmnts->sons[i]->sons[0]->sons[j]->sons[0]) != BOOLEAN) {
						sprintf(message_error,  "ERROR:A non boolean expression as a condition of DO WHILE statement is illegal.\n");
						exitProgram(message_error);
					}
					char *v = "";
					char *a = GenExpr(do_while_node->sons[1],size,&v);
					asprintf(code,"%s%sif %s goto L%d;\n\t",*code, a, v, l);
					lineCount++;
				}
				asprintf(code,*code, lineCount);
			}
			else if(stmnts->sons[i]->k == FOR_S) {
				node* for_node = stmnts->sons[i]->sons[0]; 
				//printf("L1750: FOR >  "); printNode(for_node);
				int l = -1;
				node *for_cond, *for_init, *for_update,*for_block;
				
				for_init = for_node->sons[0];	
				//printf("L1751: FOR init >  "); printNode(for_init);			
				for_cond = for_node->sons[1];
				//printf("L1752: FOR cond >  "); printNode(for_cond);			
				for_update = for_node->sons[2];
				//printf("L1753: FOR update >  "); printNode(for_update);			
				for_block = for_node->sons[3];
				//printf("L1754: FOR block >  "); printNode(for_block);
	
				if(for_init != NULL) 
				{
					int k;
					temp = getNestedNodesInOneNode(for_init);
					//printf("L1755: FOR init arr >  "); printNode(temp);			
					for(k = 0;temp != NULL && k < temp->count; k++){
						//printf("L1098: "); printNode(temp->sons[k]);
						asprintf(code,"%s%s",*code,checkAss(temp->sons[k], size));
					}
					
				}
				if(for_cond->sons[0] != NULL)
				{		
					if(getExprType(for_cond->sons[0]) != BOOLEAN) {
						sprintf(message_error,  "ERROR:A non boolean expression as a condition of FOR statement is illegal.\n");
						exitProgram(message_error);
					}

					char *v;
					asprintf(code,"%s%s",*code, strLineNum(lineCount));
					l = lineCount;
					char *a = GenExpr(for_cond->sons[0],size,&v);
					asprintf(code,"%s%sifz %s goto L%%d;\n\t",*code, a, v);
					lineCount++;
				}
				if(for_block->sons[0] != NULL)
				{	
					//printf("L1757: FOR block s >  "); printNode(for_block->sons[0]);			
					checkStmnts(for_block,last_func, layout + 1, size, code);
				}
				if(for_update->sons[0] != NULL)
				{	
					int k;	
					temp = getNestedNodesInOneNode(for_update);
					//printf("L1755: FOR update arr >  "); printNode(temp);			
					for(k = 0;temp != NULL && k < temp->count; k++){
						asprintf(code,"%s%s",*code,checkAss(temp->sons[k], size));
					}
					asprintf(code,"%sgoto L%d;\n\t%s",*code, l, strLineNum(++lineCount));
					asprintf(code,*code, lineCount);
				}
				asprintf(code,*code, lineCount);
				//{$$ = makeNode("FOR",4,makeParent("INITS",$3), makeParent("COND",$5), makeParent("UPDATES",$7),$9);}			
			}
			else if(stmnts->sons[i]->k == BLOCK) {
				enterScope("block stmt");
				asprintf(code,"%s%s",*code, checkBlock(stmnts->sons[i], last_func, layout + 1, size));
				endScope();
			}
			else if(last_func != NULL &&stmnts->sons[i]->k == RETURN) {
				type ex = getExprType(stmnts->sons[i]->sons[0]);
			
				if(ex != last_func->t) {
					sprintf(message_error, "ERROR:The return type of function \"%s\" is different from the actual returned value type.\n", last_func->name);
					exitProgram(message_error);
				}
				if(last_func->useReturn == -1){
					sprintf(message_error, "ERROR:The func \"%s\" is illigal in the function type void.\n", last_func->name);
					exitProgram(message_error);
				}
				else if(layout == 0) last_func->useReturn++;
				
				char *lv;
				char* e = GenExpr(stmnts->sons[i]->sons[0],size, &lv);
				asprintf(code,"%s%sReturn %s;\n\t",*code,e,lv);
			}
			else {
				checkStmnts(stmnts->sons[i], last_func, layout, size, code);
			}
			
		}
	}
}
// check the funcCall validation, check validation of number argument, type atgument, and defined function 
void checkFuncCall(node *fcall, int *size, char** code, char *t, int checkParamStack){
	/*
	the func call node look like that: 
			FUNC_CALL
		NAME			ARG
	   NODE(ID VALUE)		""
				    ""	     EXP1
				 ""	EXP2
				EXP3
	*/		
	char message_error[256];
	
	int s = 0;	
	char *p = "";
	if(t == NULL) {
		t = freshVar();
	}

	if(fcall->k == FC && fcall->count != 2) fcall = fcall->sons[0];
	node* func_name =  fcall->sons[0]; //  NAME
	node* func_arg  =  fcall->sons[1]; //  ARG

		
	node_t *f = findGlobal(func_name->sons[0]->value); // value of id node of name node
	if(f == NULL)
	{
		sprintf(message_error,  "ERROR:The function \"%s\" is not defined.\n",func_name->sons[0]->value);
		exitProgram(message_error);
	}
	func_arg = getNestedNodesInOneNode(func_arg);
	node_t* cmpf = copyNode_t(f);	
	compareArgs(cmpf,func_arg);
	if(checkParamStack){
		GenParamas(func_arg, &s, size, code);
		if (s != 0) asprintf(&p,"PopParams %d;\n\t",s);
		lineCount++;
		*size += 4;
		//             co t        val   v p
		asprintf(code,"%s%s = LCall %s;\n\t%s",*code,t, func_name->sons[0]->value ,p);
		
	}
}

node_t *copyNode_t(node_t *f){
	node_t * new_node;
	new_node = malloc(sizeof(node_t));
	new_node->name = strdup(f->name);
	new_node->value = strdup(f->value);
	new_node->k = f->k;
	new_node->t = f->t;
	new_node->useReturn = f->useReturn;
	new_node->argCount = f->argCount;
	new_node->args = f->args;
}
// check the Args comparetion validation, each compare arg, check the type of the args
void compareArgs(node_t *f, node *args) {
	char message_error[256];
	int  i;
	if(args == NULL) return;
	for(i = args->count - 1; i >= 0; i--) {
		if(args->sons[i] != NULL) {
			if(args->sons[i]->k == NONE)
				compareArgs(f, args->sons[i]);
			else 
			{
				type ex = getExprType(args->sons[i]);
				if(f->argCount == 0) {
					sprintf(message_error,  "2.ERROR:There is an illegal function call of \"%s\" with different amount of arguments.\n",f->name);
					exitProgram(message_error);
				}
				else if(ex != f->args[f->argCount - 1]) {
					sprintf(message_error,  "ERROR:There is an illegal function call of \"%s\" with different arguments types.\n",f->name);
					exitProgram(message_error);
				}
				else {
					f->argCount = f->argCount - 1;
				}
			}
		}
	}
}
// check the assument validation, each assument check if the left side type of the id (lhs) as same type as the right side 
char* checkAss(node *ass, int* size) {
	char message_error[256];
	node_t *lh = NULL;

	char *lv;
	char *e = "";
	char *v;
	if(ass->k == ASS) ass = ass->sons[0];
	//printf("L1281: "); printNode(ass);
	//printf("L1282: "); printNode(ass->sons[0]);
	if(strcmp(ass->sons[0]->value, "^") == 0)
	{
		lh = findGlobal(ass->sons[0]->sons[0]->value);
	}
	else	
	{
		lh = findGlobal(ass->sons[0]->value);
	}	
	if(lh == NULL) 
	{
		sprintf(message_error, "0.ERROR:The variable \"%s\" has been used without getting defined.\n", ass->sons[0]->value);
		exitProgram(message_error);
	}
	type lhs = getLhsType(ass->sons[0]);

	if(ass->k == EXPR) 		// lhs '=' expression ';'	{$$ = makeNode2("=",DEF,EXPR,2,$1,$3);}
	{
		if(ass->sons[1]->k == FC) checkFuncCall(ass->sons[1]->sons[0], size, &e, lv, 0);
		type expr = getExprType(ass->sons[1]);
		if(lhs != expr) 
		{
			if((lhs == CHARPTR || lhs == INTPTR) && expr == DEF) return e; 
			sprintf(message_error, "ERROR:There is an illegal assignment of different types.\n");
			exitProgram(message_error);
		}
	}
	else 				// lhs '=' STRING_CONST ';'	{$$ = makeNode2("=",DEF,CONST,2,$1,makeNode($3,0));}
	{
		if(lhs != ARRAY && lhs != CHARPTR) {
			sprintf(message_error, "ERROR:There is an illegal assignment of different types.\n");
			exitProgram(message_error);
		}
	}
// gen 3ac
	if(ass->k == EXPR && (ass->sons[1]->k == CONST) || ass->k != EXPR) 
	{
		v = freshVar();	
		(*size) += getSize(ass->sons[1]);
		lineCount ++;
		asprintf(&e,"%s = %s;\n\t%s%s;\n\t",v,ass->sons[1]->value, GenLhs(ass->sons[0], size), v);
		lineCount ++;
		return e;
		
	}
	else 
	{
		if(ass->sons[1]->k == FC) 
		{
			lv = freshVar();
			//printf("L1353: "); printNode(ass->sons[1]);
			checkFuncCall(ass->sons[1]->sons[0], size, &e, lv, 1);
		}
		else  
			e = GenExpr(ass->sons[1],size, &lv);
		asprintf(&e,"%s%s%s;\n\t", e, GenLhs(ass->sons[0], size), lv);
		lineCount++;
		return e;
	}
}

// get the type of the lhs (id) var , first search him in the local scope, if not found, search him in the prev scope, until the global scope
type getLhsType(node *lhs) {
	char message_error[256];
	node_t *lh = NULL;
	if(lhs->count == 0)  // IDENTIFIER {$$ = makeNode($1,0);}
	{
		lh = findGlobal(lhs->value);
		if(lh == NULL) 
		{
			sprintf(message_error, "1.ERROR:The variable \"%s\" has been used without getting defined.\n", lhs->value);
			exitProgram(message_error);
		}
		return lh->t;
	}
	else if(lhs->count == 1)  // DEREFERENCE IDENTIFIER {$$ = makeNode("^",1,makeNode($2,0));}
	{
		lh = findGlobal(lhs->sons[0]->value);
		
		if(lh == NULL) {
			sprintf(message_error, "2.ERROR:The variable \"%s\" has been used without getting defined.\n", lhs->sons[0]->value);
			exitProgram(message_error);
		}
		if(lh->t != CHARPTR && lh->t != INTPTR) {
			sprintf(message_error, "ERROR:Can not apply ^ to \"%s\" because the variable \"%s\" is not a pointer.\n", lhs->sons[0]->value, lhs->sons[0]->value);
			exitProgram(message_error);
		}
		else if(lh->t == CHARPTR)  	return CHAR;
		else        			return INTEGER;
	}
	else  // IDENTIFIER '[' expression ']'  {$$ = makeNode($1,3,makeNode("[",0),$3,makeNode("]",0));}
	{ 
		lh = findGlobal(lhs->value);
		
		if(lh == NULL) {
			sprintf(message_error,  "3.ERROR:The variable \"%s\" has been used without getting defined.\n", lhs->value);
			exitProgram(message_error);
		}
		if(lh->t != ARRAY) {
			sprintf(message_error,  "ERROR:The variable \"%s\" can not use [] because it is not a string array.\n", lhs->value);
			exitProgram(message_error);
		}
		if(getExprType(lhs->sons[1]) != INTEGER) {
			sprintf(message_error,  "ERROR:Using an non Integer expression inside [] is illegal.\n");
			exitProgram(message_error);
		}
		return CHAR; // s[5] for example is char
	}
}
// get the type of the expression , here we have the checking validation on the operation ,like + - * / ! and more.. then return the type of the expression
type getExprType(node *expr) {
	char message_error[256];
	node_t *ex = NULL;
	int size = 0;
	char* e;
	if(expr->k == FC) // work well 
	{ 
		while(expr->count == 1) expr = expr->sons[0];
		checkFuncCall(expr, &size, &e, NULL, 1);
		expr = expr->sons[0]; // go to NAME node son of FUNC_CALL
		if(expr != NULL && strcmp(expr->value,"NAME")==0)
		{
			expr = expr->sons[0];
			ex = findGlobal(expr->value);	
		}		
		if(ex == NULL) {
			sprintf(message_error, "4.ERROR:The variable \"%s\" has been used without getting defined.\n", expr->value);
			exitProgram(message_error);
		}
		else{	
			return ex->t;
		}
	}
	else if(expr->k == VAR)   //IDENTIFIER || IDENTIFIER'['expression']'
	{
		if(expr->count == 3)  	//  IDENTIFIER'['expression']'{$$=makeNode2($1,ARRAY,VAR,3,makeNode("[",0),$3,makeNode("]",0));}
		{
		   // IDENTIFIER '[' expression ']'   {$$ = makeNode2($1   , ARRAY, VAR  ,    3    , makeNode("[",0),  $3    ,  makeNode("]",0));}
		   //	   $1    $2      $3     $4                   VALUE , KIND , TYPE , NO.SONS ,   SON[0]       , SON[1] ,        SON[2] 
			
 		    ex = findGlobal(expr->value); // get the variable from the local and global variables until here

		    if(ex == NULL) {
			sprintf(message_error, "5.ERROR:The variable \"%s\" has been used without getting defined.\n", expr->value);
			exitProgram(message_error);
		    }
		    if(ex->t != ARRAY) {
			sprintf(message_error, "ERROR:The variable \"%s\" can not use [] because it is not a string array.\n", expr->value);
			exitProgram(message_error);
		    }
		    if(getExprType(expr->sons[1]) != INTEGER) {
			sprintf(message_error, "ERROR:Using an non Integer expression inside [] is illegal.\n");
			exitProgram(message_error);
		    }
			
		    return CHAR;
		}
		else 	 		// IDENTIFIER {$$ = makeNode2($1,DEF,VAR,0);}
		{
		    ex = findGlobal(expr->value);
	
		    if(ex == NULL) {
			sprintf(message_error,"ERROR:The variable \"%s\" has been used without getting defined.\n", expr->value);
			exitProgram(message_error);
		    }
		    return ex->t;
		}
	}
	else if(expr->k == CONST) // lhs '=' STRING_CONST ';' {$$ = makeNode2("=",DEF,CONST,2,$1,makeNode($3,0));} || 'const_value' 
	{
		return expr->t;
	}
	else if(expr->k == UN)  // op_unary expression	%prec UNARY {$$ = makeNode2($1,DEF,UN,1,$2);}
	{
		if(!strcmp("^",expr->value)) 		// op_unary(DEREFERENCE) expression	%prec UNARY
		{
		    type ext = getExprType(expr->sons[0]);
			
		    if(ext != CHARPTR && ext != INTPTR) {
			sprintf(message_error,"ERROR:Can not apply ^ to a non pointer expression.\n");
			exitProgram(message_error);
		    }
		    else if(ext == CHARPTR) 	return CHAR;
		    else			return INTEGER;
		}
		else if(!strcmp("&",expr->value))  	// op_unary(ADDRESS_OP) expression	%prec UNARY
		{
		    type ext = getExprType(expr->sons[0]);
			
		    if(ext != CHAR && ext != INTEGER) 
		    {
			sprintf(message_error,"ERROR:Can not apply & to a non char nor integer expression.\n");
			exitProgram(message_error);
		    }
		    else if(ext == CHAR)	return CHARPTR;
		    else			return INTPTR;
		}
		else if(!strcmp("!",expr->value)) 	// op_unary(NOT) expression	%prec UNARY
		{
		    type ext = getExprType(expr->sons[0]);
			
		    if(ext != BOOLEAN) {
			sprintf(message_error, "ERROR:Can not apply ! to a non boolean expression.\n");
			exitProgram(message_error);
		    }
		    return BOOLEAN;
		}
		else 					// op_unary(MINUS) expression	%prec UNARY
		{
		    type ext = getExprType(expr->sons[0]);
			
		    if(ext != INTEGER) {
			sprintf(message_error,  "ERROR:Can not apply Minus(\'-\') to a non integer expression.\n");
			exitProgram(message_error);
		    }
		    return INTEGER;
		}
	}
	else if(expr->count == 1)  			// '|' expression '|'   {$$ = makeParent2("",INTEGER,I ,$2);}		
	{
		type ext = getExprType(expr->sons[0]);
			
		if(ext != INTEGER && ext != ARRAY) {
		   sprintf(message_error,  "ERROR:Can not apply Absolute(\'| |\') to a non integer nor string expression.\n");
		   exitProgram(message_error);
		}
		
		return INTEGER;
	}
	else // each expression with 2 sons , like all the operators: + - * / > >= == < <= != && ||
	{
		type ext1 = getExprType(expr->sons[0]);
		type ext2 = getExprType(expr->sons[1]);
		
		if(expr->k == I) // all the operations: + - * / > >= < <= 
		{
		   if((!strcmp("-",expr->value)) || (!strcmp("+",expr->value))) // operators: '+' or '-'
     		   { 
			if(((ext1 == INTPTR || ext1 == CHARPTR) && ext2 == INTEGER) || ((ext2 == INTPTR || ext2 == CHARPTR) && ext1 == INTEGER))
			{
				if(ext2 == INTEGER) 
					return ext1;
				return ext2;
			}
		   }
		   if(ext1 != INTEGER || ext2 != INTEGER) // incomparable expressions
		   {
		   	sprintf(message_error,  "ERROR:Can not apply the operator \'%s\' on expressions that are not integers.\n", expr->value);
			exitProgram(message_error);
		   }
		}
		else if(expr->k == B) // operators: '&&' or '||'
		{
		   if(ext1 != BOOLEAN || ext2 != BOOLEAN)  // incomparable expressions
		   {
		   	sprintf(message_error, "ERROR:Can not apply the operator \'%s\' on expressions that are not booleans.\n", expr->value);
		   	exitProgram(message_error);
		   }
		}
		else // operators of ANY: '==' or '!='
		{
		   if(ext1 != ext2 || ext1 == ARRAY || ext1 == DEF || ext2 == ARRAY || ext2 == DEF )  // incomparable expressions
		   {
	sprintf(message_error, "ERROR:Can not apply the operator \'%s\' on expressions that are not pairs of integers, booleans, chars, or pointers.\n", expr->value);
		      exitProgram(message_error);
		   }
		}
		// if we got until here, so return the convert type between 2 expressions
		return expr->t;
	}
}



// print all the var in the local scope
void printLocal(scope *s, int numScope){
	node_t *n = s->l;
	printf("* scope: %s > ", s->nameScope); // %d , numScope
	while(n != NULL) {
		if(n->next != NULL)
			printf("%s(k=%s|t=%s), ", n->name, getStrKind(n->k), getStrType(n->t));
		else	
			printf("%s(k=%s|t=%s) ", n->name, getStrKind(n->k), getStrType(n->t));
		n = n->next;
	}
	printf(".\n");
}
void printGlobal() {
	int i = 0;
	scope *s = head;

	while(s) {		
		printLocal(s,live_scopes - i);
		s = s->next;
		i++;
	}
	printf("*------------------------------------------------------\n");
}

void printNode(node *n){
	int i = 0;
	int minCharForNoTab = 9;
	if(n == NULL) { printf("NULL pointer node!\n"); return;}
	printf("Node Details: ");
	if(n->value != NULL) 	{printf("\tValue: %s ", n->value);  		if(strlen(n->value) 		< minCharForNoTab) printf("\t");}
	if(n->k )	 	{printf("\tKind: %s",getStrKind(n->k));		if(strlen(getStrKind(n->k)) 	<= minCharForNoTab) printf("\t");}
	if(n->t ) 		{printf("\ttype: %s ",getStrType(n->t));	if(strlen(getStrType(n->t)) 	<= minCharForNoTab) printf("\t");}
				 printf("\tsons(%d): ", n->count);
	for(i = 0; i < n->count; i++ )
	{
		if(i < n->count - 1)
		{
			if(n->sons[i] != NULL)	printf("%s, ", n->sons[i]->value);
			else 			printf("NULL, ");
		}
		else
		{
			if(n->sons[i] != NULL)	printf("%s. ", n->sons[i]->value);
			else 			printf("NULL. ");
		}
		
	}
	printf("\n");
}
char* getStrType(type t){
	switch(t){
		case CHAR: 	return "char";
		case INTEGER: 	return "int";
		case BOOLEAN: 	return "bool";
		case CHARPTR: 	return "char*";
		case INTPTR: 	return "int*";
		case ARRAY:	return "arr";
		case DEF: 	return "DEF";
		default: 	return "void";
	}
}
char* getStrKind(kind k){
	switch(k){
		case FUNC: 	return "func";
		case VAR: 	return "var";
		case CONST: 	return "const";
		case EXPR: 	return "exp";
		case ARG: 	return "arg";
		case BLOCK: 	return "block";
		case RETURN: 	return "return";
		case DEC: 	return "declarations";
		case DV: 	return "var_dec";
		case DF: 	return "func_dec";
		case STMNT: 	return "stmt";
		case I: 	return "I(+-*/><=)";
		case B: 	return "B(&&||)";
		case ANY: 	return "ANY(!= ==)"; 
		case UN: 	return "UNary(^&!-)";
		case ASS: 	return "assument";
		case FC: 	return "func_call";
		case IF: 	return "IF";
		case IE:	return "If_Else";
		case W: 	return "While";
		case NONE: 	return "NONE";
		default: 	return "UNKNOWN_KIND!";
	}
}
void exitProgram(char *message){
	fprintf(stderr, "*******************************************************\n");
	fprintf(stderr, "*\tInvalid Semantic errors:\n");
	
	fprintf(stderr, "*-----------------------------------------------------\n*\n");
	fprintf(stderr, "* print scopes before stack:\n");
	printGlobal();
	
	fprintf(stderr, "*\n*%s*******************************************************\n", message);
	
	exit(0);
}

// get node , and if it main function node - check the validation of the main function 
void handleIfItMain(node *func){
	char message_error[256];
	if(func != NULL && strcmp(func->value,"main")==0)
	{
		if(hasMainAllrady == 0)	
		{
			checkMainFunc(func);
			hasMainAllrady = hasMainAllrady + 1;
		}
		else 
		{
			sprintf(message_error, "ERROR:The function \"main\" has been defined more than once.\n");
			exitProgram(message_error);
		}
	}
}
// check the validation of the main function 
void checkMainFunc(node *func){
	char message_error[256];
	int i;	
	if(func->k != FUNC) 
	{
		printf("844_Isn't func node, exit from checkFunc()\n");
		return;
	}
	for(i = 0; i < func->count; i++)
	{
		if(func->sons[i] != NULL )	
		{
			if(func->sons[i]->k == ARG) 
			{
				sprintf(message_error, "ERROR:The Main function didn't should get params.\n");
				exitProgram(message_error);
			}		
		}
	}
	if(func->t != INTEGER && func->t != NONE)
	{	
		sprintf(message_error, "ERROR:The declaration type of function \"Main\" is should be int or void.\n");
		exitProgram(message_error);
	}
}

// add right sibling to the result node
node* addSibling(node *p, node *s){
	node **alocSons;
	int i, count;
	count = p->count;
	if ((alocSons = (node**)malloc((count+1) * sizeof(node*))) == NULL) yyerror("out of memory");
	for (i = 0; i < count && p->sons != NULL; i++)
		alocSons[i] = p->sons[i];
	alocSons[count] = s;
	p->sons = alocSons;
	p->count = count+1;
	return p;	
}
/* Function to reverse the linked list */
void reverseSons(node** head_ref)
{
	node* temp   = NULL;
	int i, j;
	if(head_ref == NULL || *head_ref == NULL) return;
	j = (*head_ref)->count;
	for (i = 0; i < j/2 ;i++)
	{	
		temp = (*head_ref)->sons[i];
		(*head_ref)->sons[i] = (*head_ref)->sons[j - i - 1];
		(*head_ref)->sons[j - i - 1] = temp;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------
char* strLineNum(int line){
	int amount_Digits = noDigits(line);
	char* res = "";
	int i;
	for(i = 0; i <= amount_Digits + 1; i++)
		asprintf(&res,"\b%s",res);
	asprintf(&res,"\b%sL%d: ",res, line);	
	return res;
}
int noDigits(int n){
	int count = 0;
	while(n > 0){
		count++;
		n = n / 10;
	}
	return count;
}
char* freshVar() {
	char *a;
	asprintf(&a,"t%d",varCount++);
	//printf("4(%s) + ", a);
	return a;
}
void getVarsSize(node *vars, int *size) {
	int i;
	if(vars->k == ARG) {
		for(i = 0; i < vars->count; i++) {
			if(vars->sons[i] != NULL ){
				getVarSize(vars->sons[i],vars->t, size);
			}
		}	
		return;
	}
	for(i = 0; i < vars->count; i++) {
		if(vars->sons[i] != NULL ){
			getVarsSize(vars->sons[i], size);
		}
	}
}

void getVarSize(node *var,type t, int *size) {
	int i;
	node* n = createNewNode();
	n->t = t;
	n->value = "vars";
	if(var->count == 0) {
		int sizeVar = getSize(n);	
		//*size += sizeVar; // didn't should add declaration, only assument of non argument func !! 
		//printf("L2021 var (%d) ", sizeVar);printNode(var);
		return;
	}
	else if(var->count == 3){
		int sizeArr = atoi(var->sons[1]->value) * 1; // 1 for string array of char - size char is 1
		//*size += sizeArr; // didn't should add declaration, only assument of non argument func !! 
		//printf("L2022 arr (%d) ",sizeArr);printNode(var);
		return;
	}
	
	for(i = 0; i < var->count; i++) {
		if(var->sons[i] != NULL)
			getVarSize(var->sons[i], t, size);
	}
}

int getSize(node *n) {
	int memory = 0;
	switch(n->t) {
		case CHAR:	memory = 1; break;
		case INTEGER:	memory = 4; break;
		case BOOLEAN:	memory = 1; break;
		case CHARPTR:	memory = 4; break;
		case INTPTR:	memory = 4; break;
		default:	memory = 0; break;
	}
	//printf("%d(%s|%s) + ",memory , n->value, getStrType(n->t));
	return memory;
}

char* GenLhs(node *lhs, int *size) {
	char *code = "";
	if(lhs == NULL) return code;

	if(strcmp(lhs->value, "^") == 0){
		lhs = lhs->sons[0];
	}
	
	if(lhs->count == 0) {
		asprintf(&code,"%s = ", lhs->value);
		
		node_t* n;
		if((n = findGlobal(lhs->value)) != NULL && n->k != ARG && findGenGlobal(lhs->value) == NULL) { 
			lhs->t = n->t;
			lhs->k = n->k;
			push(&(declareMemoryForVars->l), lhs->value, lhs->value, n->k, n->t);
			*size += getSize(lhs);
		}
		
		return code;
	}
	else if(lhs->count == 1) {
		*size += 4;
		/*
		if(lhs->t == INTPTR)
			*size += 4;
		else
			*size += 1;
		*/
		lineCount++;
		char *v = freshVar();
		asprintf(&code, "%s = ^%s;\n\t%s = ", v, lhs->value, v);
		return code;
	}
	else { // arr[n]
		char *lv;
		char *e = GenExpr(lhs->sons[1],size, &lv);

		char *v = freshVar();
		asprintf(&code, "%s = %s + %s;\n\t", v, lhs->value , lv);
		asprintf(&code, "%s*(%s) = ",code, v);
		return code;
	}
}

char *GenExpr(node *expr, int *size, char **lv) {
	if(expr->k == FC) 
	{ 	
		char *e = "";
		checkFuncCall(expr->sons[0], size, &e, NULL, 1);
		return e;
	}
	else if(expr->k == VAR) 
	{
		if(expr->count == 3) // arr[n] 
		{
			char *e, *v;	
			e = GenExpr(expr->sons[1], size, &v);
			char *t1 = freshVar();
			char *t2 = freshVar();
			asprintf(&e, "%s%s = %s + %s;\n\t",e, t1, expr->value , v);
			asprintf(&e, "%s%s = *%s;\n\t",e, t2, t1);
			asprintf(lv, "%s", t2);
			return e;
		}
		else 
		{
			*lv = strdup(expr->value);
			return "";
		}
	}
	else if(expr->k == CONST) 
	{
		char *e;
		*lv = freshVar();
		(*size) += getSize(expr);
		asprintf(&e,"%s = %s;\n\t",*lv,expr->value);
		lineCount++;
		return e;
	}
	else if(expr->k == UN) 
	{
		if(!strcmp("^",expr->value))
		{
			char *e, *v;
			e = GenExpr(expr->sons[0], size, &v);
			asprintf(lv,"^%s", v);
			
			return e;
		}
		else if(!strcmp("&",expr->value)) 	
		{
			char *e, *v;
			e = GenExpr(expr->sons[0], size, &v);
			asprintf(lv,"&(%s)", v);
			
			return e;
		}
		else if(!strcmp("!",expr->value)) 
		{
			char *e, *v;
			e = GenExpr(expr->sons[0], size, &v);
			*lv = freshVar();	
			(*size) += 4;
			asprintf(&e,"%s;\n\tifz %s goto L%d;\n\t%s = 0;\n\tgoto L%d;\n\t%s%s = 1;\n\t%s",e,v,lineCount + 3,*lv,lineCount + 4,strLineNum(lineCount + 3),*lv,strLineNum(lineCount + 4));
			lineCount += 4;
			
			return e;
		}
		else {
			char *e, *v;
			e = GenExpr(expr->sons[0], size, &v);
			asprintf(lv,"-%s", v);
			
			return e;
		}
	}
	else if(expr->count == 1)
	{
		char *e, *v;
		e = GenExpr(expr->sons[0], size, &v);
		asprintf(lv,"|%s|", v);
		
		return e;
	}
	else 
	{
		if(expr->k == I) 
		{
			if((!strcmp("-",expr->value)) || (!strcmp("+",expr->value)))
			{
				if(expr->t != INTEGER) 
				{
					node *i, *p;
					int s;
					char *e,*e1,*e2,*v1,*v2,*v3;
					
					if(expr->sons[0]->t == INTEGER)
					{
						i = expr->sons[0];
						p = expr->sons[1];
					}
					else 	
					{
						i = expr->sons[1];
						p = expr->sons[0];
					}
					if(p->t == INTPTR) 	s = 4;
					else 		   	s = 1;
					
					e1 = GenExpr(i, size, &v1);
					e2 = GenExpr(p, size, &v2);
					v3 = freshVar();
					(*size) += 4;
					*lv = freshVar();
					(*size) += 4;
					
					asprintf(&e,"%s%s%s = %s * %d;\n\t%s = %s + %s;\n\t",e1,e2,v3,v1,s,*lv,v2,v3);
					lineCount += 2;
					
					return e;
				}
			}
			
			char *e,*e1,*e2,*v1,*v2;
			int s;
			
			if(expr->t == CHAR)	s = 1;
			else			s = 4;
			
			e1 = GenExpr(expr->sons[0], size, &v1);
			e2 = GenExpr(expr->sons[1], size, &v2);
			*lv = freshVar();
			(*size) += s;
			asprintf(&e,"%s%s%s = %s %s %s;\n\t",e1,e2,*lv,v1,expr->value,v2);
			lineCount++;
			
			return e;
		}
		else if(expr->k == B) 
		{
			if(!strcmp("||",expr->value))
			{
				char *e1,*v1,*format, *e2, *v2, *e;
				e1 = GenExpr(expr->sons[0], size, &v1);
				*lv = freshVar();
				(*size) += 4;
				asprintf(&format,"%sifz %s goto L%d;\n\t%s = 1;\n\tgoto L%%d;\n\t%s",e1, v1, lineCount + 3, *lv,strLineNum(lineCount+3));
				lineCount += 3;
				e2 = GenExpr(expr->sons[1], size, &v2);
				asprintf(&format,"%s%sifz %s goto L%d;\n\t%s = 1;\n\tgoto L%d;\n\t%s%s = 0;\n\t%s",
 					  format,e2,v2,lineCount + 3,*lv,lineCount + 4,strLineNum(lineCount + 3),*lv,strLineNum(lineCount + 4));
				asprintf(&e,format,lineCount + 4);
				lineCount += 4;
				
				return e;
			}
			else 
			{
				char *e1,*v1,*format, *e2, *v2, *e;
				e1 = GenExpr(expr->sons[0], size, &v1);
				*lv = freshVar();
				(*size) += 4;
				asprintf(&format,"%sifz %s goto L%%d;\n\t",e1, v1);
				lineCount += 1;
				e2 = GenExpr(expr->sons[1], size, &v2);
				asprintf(&format,"%s%sifz %s goto L%d;\n\t%s = 1;\n\tgoto L%d;\n\t%s%s = 0;\n\t%s",format,e2,v2,lineCount + 3,*lv,lineCount + 4,strLineNum(lineCount + 3),*lv,strLineNum(lineCount + 4));
				asprintf(&e,format,lineCount + 3);
				lineCount += 4;
				
				return e;
			}
		}
		else 
		{
			char *e,*e1,*e2,*v1,*v2;
			e1 = GenExpr(expr->sons[0], size, &v1);
			e2 = GenExpr(expr->sons[1], size, &v2);
			*lv = freshVar();
			(*size) += 4;
			asprintf(&e,"%s%s%s = %s %s %s;\n\t",e1,e2,*lv,v1,expr->value,v2);
			lineCount++;
			
			return e;
		}
	}
}

void GenParamas(node *args, int *s, int *size, char** code) {
	int  i;
	char message_error[256];
	int sizevar;
	char *lv;
	char *e;

	if(args != NULL) 
	{
		//printf("args: "); printNode(args);
		for(i = args->count - 1; i >= 0; i--) 	
		{
			if(args->sons[i] != NULL) 
			{
				if(args->sons[i]->k == NONE)  GenParamas(args->sons[i], s, size, code);
				else if(args->sons[i]->k == I || args->sons[i]->k == B || args->sons[i]->k == ANY)
				{
					node* n = createNewNode();
					n->t = getExprType(args->sons[i]);
					n->value = args->sons[i]->value;
					//printf("L2046"); printNode(n);printNode(args->sons[i]);
					e = GenExpr(args->sons[i],size, &lv);
					asprintf(code,"%s%sPushparam %s;\n\t",*code, e, lv);
					lineCount++;					
					sizevar = getSize(n);
					(*s) += sizevar;
					//printf("size - %d (%s): ", sizevar, getStrType(getExprType(args->sons[i]))); printNode(args->sons[i]);
				}
				else 
				{
					node* n = createNewNode();
					n->t = getExprType(args->sons[i]);
					n->value = args->sons[i]->value;
					//printf("L2059"); printNode(n);printNode(args->sons[i]);
					e = GenExpr(args->sons[i],size, &lv);
					asprintf(code,"%s%sPushparam %s;\n\t",*code, e, lv);
					lineCount++;
					sizevar = getSize(n);
					(*s) += sizevar;
					//printf("size - %d (%s): ", sizevar, getStrType(getExprType(args->sons[i]))); printNode(args->sons[i]);
				}
			}
		}
	}
	//printf("size of vars in func is: %d\n", *s);
}



void printGenGlobal() {
	int i = 0;
	scope *s = declareMemoryForVars;

	while(s) {		
		printLocal(s,live_scopes - i);
		s = s->next;
		i++;
	}
	printf("*------------------------------------------------------\n");
}
// open a new scope in the scope list 
void enterGenScope(char* nameScope){
	scope* new_scope;
	new_scope = malloc(sizeof(scope));
	new_scope->l = NULL;
	new_scope->nameScope = nameScope;
	new_scope-> next = declareMemoryForVars;
	declareMemoryForVars = new_scope;
	live_gen_scopes++;
}
// close the last scope in the scope list 
void endGenScope() {
	scope * next_scope = NULL;

	if (declareMemoryForVars == NULL) {
		return;
	}

	next_scope = declareMemoryForVars->next;
	while((declareMemoryForVars->l) != NULL) {
		pop(&(declareMemoryForVars->l));
	}
	free(declareMemoryForVars);
	declareMemoryForVars = next_scope;
	live_gen_scopes--;
}
node_t* findGenGlobal(char* name) {
	scope *s = declareMemoryForVars;
	node_t *n = NULL;
	
	while(s) {
		if((n = findLocal(s,name)) != NULL){	
			return n;
		}
		s = s->next;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------

void yyerror(char *s) {
	fprintf(stderr, "%s at line:%d (%s)\n", s, yylineno, yytext);
	exit(0);
}

int main(void) {
	char* code;
	h = NULL;
	head = NULL;
	yyparse();
	printTree(h, 0, 0, 1);	
	printf("\n-------------------------------------------------------\n\t\t    Accepted AST!\n-------------------------------------------------------\n");
	code = checkProgram(h);
	printf("\n-------------------------------------------------------\n\t\tNo semantics warnings!\n-------------------------------------------------------\n");
	printf("\n-------------------------------------------------------\n\t\t\t3AC!\n\n%s\n-------------------------------------------------------\n\n", code);
	printf("totals:\n\t%d - Lines\n\t%d - Temp_Registers.\n",--lineCount, varCount);
	return 0;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------



#line 2477 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 121 "parser.y"
	{ h = NULL;  }
break;
case 2:
#line 122 "parser.y"
	{ h = makeNode2("PROGRAM",NONE, DEF,1 , yystack.l_mark[0].nPtr); }
break;
case 3:
#line 126 "parser.y"
	{ yyval.nPtr = makeNode2("FUNCTION", NONE, DEF,1, yystack.l_mark[0].nPtr);}
break;
case 4:
#line 127 "parser.y"
	{ yyval.nPtr = makeNode2("FUNCTION", NONE, DEF,2, yystack.l_mark[-1].nPtr,yystack.l_mark[0].nPtr);}
break;
case 5:
#line 129 "parser.y"
	{ yyval.nPtr = makeNode2(yystack.l_mark[-6].value ,NONE, FUNC, 2,makeParent2("ARG",DEF, ARG,yystack.l_mark[-4].nPtr),yystack.l_mark[-1].nPtr);}
break;
case 6:
#line 131 "parser.y"
	{ yyval.nPtr = makeNode2(yystack.l_mark[-6].value, yystack.l_mark[-7].nPtr->t , FUNC, 2 ,makeParent2("ARG",DEF, ARG,yystack.l_mark[-4].nPtr),yystack.l_mark[-1].nPtr );}
break;
case 9:
#line 137 "parser.y"
	{yyval.nPtr = makeNode("",1,yystack.l_mark[0].nPtr);}
break;
case 10:
#line 138 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 11:
#line 140 "parser.y"
	{yyval.nPtr = makeParent2(yystack.l_mark[-1].nPtr->value, yystack.l_mark[-1].nPtr->t, yystack.l_mark[-1].nPtr->k,yystack.l_mark[0].nPtr);}
break;
case 12:
#line 141 "parser.y"
	{yyval.nPtr = makeParent2(yystack.l_mark[-1].nPtr->value, yystack.l_mark[-1].nPtr->t, yystack.l_mark[-1].nPtr->k,yystack.l_mark[0].nPtr);}
break;
case 13:
#line 144 "parser.y"
	{yyval.nPtr = makeNode2("FUNC_CALL",DEF, FC,2, makeParent("NAME",makeNode(yystack.l_mark[-3].value,0)),makeParent("ARG",yystack.l_mark[-1].nPtr));}
break;
case 16:
#line 149 "parser.y"
	{yyval.nPtr = makeNode("",1,yystack.l_mark[0].nPtr);}
break;
case 17:
#line 150 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 20:
#line 158 "parser.y"
	{yyval.nPtr = makeNode("",1,yystack.l_mark[0].nPtr);}
break;
case 21:
#line 159 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-1].nPtr,yystack.l_mark[0].nPtr);}
break;
case 22:
#line 161 "parser.y"
	{yyval.nPtr = yystack.l_mark[-1].nPtr;}
break;
case 23:
#line 162 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,FC,yystack.l_mark[0].nPtr);}
break;
case 24:
#line 163 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,IF,yystack.l_mark[0].nPtr);}
break;
case 25:
#line 164 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,IE,yystack.l_mark[0].nPtr);}
break;
case 26:
#line 165 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,W,yystack.l_mark[0].nPtr);}
break;
case 27:
#line 166 "parser.y"
	{yyval.nPtr = yystack.l_mark[-1].nPtr;}
break;
case 28:
#line 167 "parser.y"
	{yyval.nPtr = NULL;}
break;
case 29:
#line 168 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,DW,yystack.l_mark[0].nPtr);}
break;
case 30:
#line 169 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,FOR_S,yystack.l_mark[0].nPtr);}
break;
case 31:
#line 170 "parser.y"
	{yyval.nPtr = makeParent2("RETURN",DEF, RETURN,yystack.l_mark[-1].nPtr);}
break;
case 32:
#line 171 "parser.y"
	{yyval.nPtr = makeParent2("EMPTY_RETURN",DEF, RETURN,NULL);}
break;
case 33:
#line 172 "parser.y"
	{yyval.nPtr = makeParent2("DECLARATIONS",DEF, DEC,yystack.l_mark[0].nPtr);}
break;
case 34:
#line 175 "parser.y"
	{yyval.nPtr = makeNode("IF",2,makeParent("COND",yystack.l_mark[-2].nPtr),makeParent("IF-BLOCK",yystack.l_mark[0].nPtr));}
break;
case 35:
#line 177 "parser.y"
	{yyval.nPtr = makeNode("IF-ELSE",3,makeParent("COND",yystack.l_mark[-4].nPtr),makeParent("IF-BLOCK",yystack.l_mark[-2].nPtr),makeParent("ELSE-BLOCK",yystack.l_mark[0].nPtr));}
break;
case 36:
#line 180 "parser.y"
	{yyval.nPtr = makeNode("WHILE",2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 37:
#line 182 "parser.y"
	{yyval.nPtr = makeNode("DO_WHILE",2,makeParent("DO_WHILE-BLOCK",yystack.l_mark[-5].nPtr),makeParent("DO_WHILE-COND",yystack.l_mark[-2].nPtr));}
break;
case 38:
#line 185 "parser.y"
	{yyval.nPtr = makeNode("FOR",4,makeParent("INITS",yystack.l_mark[-6].nPtr), makeParent("COND",yystack.l_mark[-4].nPtr), makeParent("UPDATES",yystack.l_mark[-2].nPtr),yystack.l_mark[0].nPtr);}
break;
case 39:
#line 187 "parser.y"
	{yyval.nPtr = yystack.l_mark[0].nPtr;}
break;
case 40:
#line 188 "parser.y"
	{yyval.nPtr = NULL;}
break;
case 41:
#line 190 "parser.y"
	{yyval.nPtr = makeNode("",1,yystack.l_mark[0].nPtr);}
break;
case 42:
#line 191 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 43:
#line 192 "parser.y"
	{yyval.nPtr = NULL;}
break;
case 44:
#line 196 "parser.y"
	{yyval.nPtr = makeNode2("BLOCK",DEF,BLOCK,2,makeParent2("STATMENTS",DEF,STMNT,yystack.l_mark[0].nPtr), makeNode2("",INTEGER,ARG,0));}
break;
case 46:
#line 202 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,DF,makeParent2("",DEF, DF ,yystack.l_mark[0].nPtr));}
break;
case 47:
#line 203 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,DV,makeParent2("",DEF, DV ,yystack.l_mark[-1].nPtr));}
break;
case 48:
#line 207 "parser.y"
	{yyval.nPtr = makeParent2(yystack.l_mark[-1].nPtr->value, yystack.l_mark[-1].nPtr->t, yystack.l_mark[-1].nPtr->k,yystack.l_mark[0].nPtr);}
break;
case 49:
#line 208 "parser.y"
	{yyval.nPtr = makeParent2(yystack.l_mark[-1].nPtr->value, yystack.l_mark[-1].nPtr->t, yystack.l_mark[-1].nPtr->k,yystack.l_mark[0].nPtr);}
break;
case 50:
#line 209 "parser.y"
	{yyval.nPtr = makeParent2(yystack.l_mark[-1].nPtr->value, yystack.l_mark[-1].nPtr->t, yystack.l_mark[-1].nPtr->k,yystack.l_mark[0].nPtr);}
break;
case 51:
#line 211 "parser.y"
	{yyval.nPtr = makeNode(yystack.l_mark[0].value,0);}
break;
case 52:
#line 212 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-2].nPtr,makeNode(yystack.l_mark[0].value,0));}
break;
case 53:
#line 214 "parser.y"
	{yyval.nPtr = makeNode(yystack.l_mark[-3].value,3,makeNode("[",0),yystack.l_mark[-1].nPtr,makeNode("]",0));}
break;
case 54:
#line 215 "parser.y"
	{yyval.nPtr = makeNode("",2,yystack.l_mark[-5].nPtr,makeNode(yystack.l_mark[-3].value,3,makeNode("[",0),yystack.l_mark[-1].nPtr,makeNode("]",0)));}
break;
case 55:
#line 219 "parser.y"
	{yyval.nPtr = makeNode2("CHAR"	, CHAR	, ARG,0);}
break;
case 56:
#line 220 "parser.y"
	{yyval.nPtr = makeNode2("INTEGER",INTEGER,ARG,0);}
break;
case 57:
#line 221 "parser.y"
	{yyval.nPtr = makeNode2("BOOLEAN",BOOLEAN,ARG,0);}
break;
case 58:
#line 222 "parser.y"
	{yyval.nPtr = makeNode2("CHARPTR",CHARPTR,ARG,0);}
break;
case 59:
#line 223 "parser.y"
	{yyval.nPtr = makeNode2("INTPTR", INTPTR ,ARG,0);}
break;
case 60:
#line 225 "parser.y"
	{yyval.nPtr = makeNode2("STRING", ARRAY	, ARG,0);}
break;
case 61:
#line 228 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,CHAR,CONST,0);}
break;
case 62:
#line 229 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,BOOLEAN,CONST,0);}
break;
case 63:
#line 230 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,DEF,CONST,0);}
break;
case 65:
#line 234 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,INTEGER,CONST,0);}
break;
case 66:
#line 235 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,INTEGER,CONST,0);}
break;
case 67:
#line 236 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,INTEGER,CONST,0);}
break;
case 68:
#line 237 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,INTEGER,CONST,0);}
break;
case 69:
#line 241 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[0].value,DEF,VAR,0);}
break;
case 70:
#line 242 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[-3].value  ,ARRAY  ,VAR,3,makeNode("[",0),yystack.l_mark[-1].nPtr,makeNode("]",0));}
break;
case 71:
#line 243 "parser.y"
	{yyval.nPtr = yystack.l_mark[0].nPtr;}
break;
case 72:
#line 244 "parser.y"
	{yyval.nPtr = makeParent2("FUNC_CALL",DEF,FC,yystack.l_mark[0].nPtr);}
break;
case 73:
#line 245 "parser.y"
	{yyval.nPtr = makeNode2("&&",BOOLEAN,B	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 74:
#line 246 "parser.y"
	{yyval.nPtr = makeNode2("||",BOOLEAN,B	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 75:
#line 247 "parser.y"
	{yyval.nPtr = makeNode2("==",BOOLEAN,ANY,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 76:
#line 248 "parser.y"
	{yyval.nPtr = makeNode2("!=",BOOLEAN,ANY,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 77:
#line 249 "parser.y"
	{yyval.nPtr = makeNode2(">" ,BOOLEAN,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 78:
#line 250 "parser.y"
	{yyval.nPtr = makeNode2(">=",BOOLEAN,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 79:
#line 251 "parser.y"
	{yyval.nPtr = makeNode2("<=",BOOLEAN,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 80:
#line 252 "parser.y"
	{yyval.nPtr = makeNode2("<" ,BOOLEAN,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 81:
#line 253 "parser.y"
	{yyval.nPtr = makeNode2("+" ,INTEGER,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 82:
#line 254 "parser.y"
	{yyval.nPtr = makeNode2("-" ,INTEGER,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 83:
#line 255 "parser.y"
	{yyval.nPtr = makeNode2("*" ,INTEGER,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 84:
#line 256 "parser.y"
	{yyval.nPtr = makeNode2("/" ,INTEGER,I	,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 85:
#line 257 "parser.y"
	{yyval.nPtr = yystack.l_mark[-1].nPtr;}
break;
case 86:
#line 258 "parser.y"
	{yyval.nPtr = makeParent2("ABS",INTEGER,I ,yystack.l_mark[-1].nPtr);}
break;
case 87:
#line 259 "parser.y"
	{yyval.nPtr = makeNode2(yystack.l_mark[-1].value  ,DEF    ,UN	,1,yystack.l_mark[0].nPtr);}
break;
case 88:
#line 261 "parser.y"
	{yyval.nPtr = makeNode(yystack.l_mark[0].value,0);}
break;
case 89:
#line 262 "parser.y"
	{yyval.nPtr = makeNode("^",1,makeNode(yystack.l_mark[0].value,0));}
break;
case 90:
#line 263 "parser.y"
	{yyval.nPtr = makeNode(yystack.l_mark[-3].value,3,makeNode("[",0),yystack.l_mark[-1].nPtr,makeNode("]",0));}
break;
case 91:
#line 265 "parser.y"
	{yyval.nPtr = makeParent2("",DEF,ASS,yystack.l_mark[0].nPtr);}
break;
case 92:
#line 267 "parser.y"
	{yyval.nPtr = makeNode2("=",DEF,EXPR,2,yystack.l_mark[-2].nPtr,yystack.l_mark[0].nPtr);}
break;
case 93:
#line 268 "parser.y"
	{yyval.nPtr = makeNode2("=",DEF,CONST,2,yystack.l_mark[-2].nPtr,makeNode(yystack.l_mark[0].value,0));}
break;
case 98:
#line 275 "parser.y"
	{yyval.nPtr = makeNode(yystack.l_mark[0].value,0);}
break;
case 99:
#line 277 "parser.y"
	{yyval.nPtr = NULL;}
break;
#line 3027 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
