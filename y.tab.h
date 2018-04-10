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
extern YYSTYPE yylval;
