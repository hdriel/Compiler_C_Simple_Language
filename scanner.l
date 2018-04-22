%{
	#include "parse.h"
	#include "y.tab.h"

	void yyerror(char *);
	extern int yylineno;
	extern char* yytext;
	

%}

%%

boolean					return BOOLEAN;
char					return CHAR;
int					return INTEGER;
string					return STRING ;
intptr|intp|int\*				return INTPTR ;
charptr|charp|char\*				return CHARPTR;
if 					return IF;
else 					return ELSE;
while 					return WHILE;
var 					return VAR ;
return 					return RETURN ; 
null 					return NULL_;
void 					return VOID;
do 					return DO;
for 					return FOR;
"/*"([^*]|\*+[^*/])*\*+"/"		; /* for the comment like this comment */
"//"[^\n]*|"\/\*".*"\*\/"		; /* for the comment: '// comment... ' */
\/%[^%\/]*%\/ 				; /* for the comment: '/% comment... %/' */
&& 					{yylval.value = strdup(yytext); return AND;}
\/ 					{yylval.value = strdup(yytext); return DIV;}
= 					{yylval.value = strdup(yytext); return '=';}
== 					{yylval.value = strdup(yytext); return EQUAL;}
\>= 					{yylval.value = strdup(yytext); return GE;}
\> 					{yylval.value = strdup(yytext); return GREATER;}
\<= 					{yylval.value = strdup(yytext); return LE;}
\< 					{yylval.value = strdup(yytext); return LESS;}
\- 					{yylval.value = strdup(yytext); return MINUS;}
! 					{yylval.value = strdup(yytext); return NOT;}
!= 					{yylval.value = strdup(yytext); return NE;}
\|\|					{yylval.value = strdup(yytext); return OR;}
\+ 					{yylval.value = strdup(yytext); return PLUS;}
\* 					{yylval.value = strdup(yytext); return MULTI;}
& 					{yylval.value = strdup(yytext); return ADDRESS_OP;}
\^ 					{yylval.value = strdup(yytext); return DEREFERENCE;}
true|false 				{yylval.value = strdup(yytext); return BOOLEAN_CONST;}
[0-9]*\.[0-9]+ 				{yylval.value = strdup(yytext); return FLOAT_CONST;}
([1-9][0-9]*)|0 			{yylval.value = strdup(yytext); return DECIMAL_CONST;}
0[x|X][0-9A-F]+ 			{yylval.value = strdup(yytext); return HEX_CONST;}
0[0-7]* 				{yylval.value = strdup(yytext); return OCTAL_CONST;}
[0-1]+b 				{yylval.value = strdup(yytext); return BINARY_CONST;}
'[^']' 					{yylval.value = strdup(yytext); return CHAR_CONST;}
\"[^"]*\" 				{yylval.value = strdup(yytext); return STRING_CONST;}
[_a-zA-Z][a-zA-Z0-9_]* 			{yylval.value = strdup(yytext); return IDENTIFIER;}
; 					return ';';
: 					return ':';
, 					return ',';
\|					return '|';
\{					return '{';
\}					return '}';
\(					return '(';
\)					return ')';
\[					return '[';
\]					return ']';
[ \t\r\f]+ 				;
\n					++yylineno;
. 					yyerror("invalid character");
%%

int yywrap(void) {
	return 1;
}

