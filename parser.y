%{
#include <stdarg.h>
#include "parse.h"

// Student: 
// Hadriel Benjo - 302185061 
// Semion phifman- 317743425

// function declarations
int yylex(void);
void yyerror(char *);
extern int yylineno;
extern char* yytext;

// parser|syntax fucntions 
int isSimpleTree(node* head);
node* makeNode(char *value, int count,...);
node* makeNode2(char *value,type t, kind k, int count,...);
node* makeParent(char* value, node* child);
node* makeParent2(char* value,type t, kind k, node* child);
void printTree(node* ast, int indent, int s, int l);

// semantic function
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
void addVars(node *vars); //
void addVar(node *var,type t); //
void compareArgs(node_t *f, node *args);
type getLhsType(node *lhs);
type getExprType(node *expr);

int varCount;
int lineCount;
char* freshVar();
void getVarsSize(node *vars, int *size);
void getVarSize(node *var,type t, int *size);
int getSize(node* n);

// help function for debug of semantic function
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
// three address code (3AC)- functions
%}

%union {
	char *value;
	node *nPtr;
};

%start program

%token <value> INTEGER CHAR CHARPTR CHAR_CONST BOOLEAN BOOLEAN_CONST BINARY_CONST VOID
%token <value> VAR  RETURN IDENTIFIER INTPTR DECIMAL_CONST NULL_ OCTAL_CONST 
%token <value> AND DIV EQUAL GE GREATER LE MINUS NOT NE OR PLUS MULTI ADDRESS_OP HEX_CONST
%token <value> DEREFERENCE STRING_CONST IF WHILE STRING FLOAT_CONST LESS FOR DO

%left OR
%left AND
%left EQUAL
%left NE
%left GREATER GE LE LESS
%left PLUS MINUS
%left MULTI DIV
%right UNARY

%nonassoc ENDIF
%nonassoc ELSE

/* type from the union:  nPtr (as node) or value (as char*) */
%type <nPtr> program func_call list_arg_func list_arg_func list_arg_func_rec list_param_func list_param_func_rec declare_one_var multi_func func 
%type <nPtr>  statement multi_statements stat_rec assignment assign lhs code_block   while doWhile for inits_update exp_for 
%type <nPtr>  var_identifier var_declarations var_declarations_ne declare_vars expression 
%type <nPtr>  id_node const_value  empty n_types  if if_else  integer_literal  var_arr_identifier str_type
%type <value> op_unary 

%%
/* * * * START * * * */
program		:	empty						{ h = NULL;  } 	
		|	multi_func 					{ h = makeNode2("PROGRAM",NONE, DEF,1 , $1); } 
		;
/* * * * FUNCTIONS * * * */
/* functions : type func ( param , ... ) { [statemenst , ... |empty] [return [type] | empty]; } */
multi_func	: 	func 						{ $$ = makeNode2("FUNCTION", NONE, DEF,1, $1);}
		|	multi_func func 				{ $$ = makeNode2("FUNCTION", NONE, DEF,2, $1,$2);} 	
		;	
func		:	VOID IDENTIFIER    '(' list_param_func ')' '{' code_block  '}' 	{ $$ = makeNode2($2 ,NONE, FUNC, 2,makeParent2("ARG",DEF, ARG,$4),$7);} 	
		|	n_types	IDENTIFIER '(' list_param_func ')' '{' code_block '}' 	
		{ $$ = makeNode2($2, $1->t , FUNC, 2 ,makeParent2("ARG",DEF, ARG,$4),$7 );} 
		;
/* function parameter_list : int a, int b, char c*/
list_param_func	:	empty	 	
		|	list_param_func_rec														        
		;
list_param_func_rec:	declare_one_var 				{$$ = makeNode("",1,$1);}
		|	list_param_func_rec ',' declare_one_var  	{$$ = makeNode("",2,$1,$3);}
		;
declare_one_var :	n_types id_node  				{$$ = makeParent2($1->value, $1->t, $1->k,$2);}
		|	str_type id_node				{$$ = makeParent2($1->value, $1->t, $1->k,$2);}
	      	;
/* call function like : goo(a,b,c) */		
func_call	:	IDENTIFIER '(' list_arg_func ')' {$$ = makeNode2("FUNC_CALL",DEF, FC,2, makeParent("NAME",makeNode($1,0)),makeParent("ARG",$3));}
		;
list_arg_func	:	empty
		|	list_arg_func_rec					
		;
list_arg_func_rec:	expression					{$$ = makeNode("",1,$1);}
	        |	list_arg_func_rec ',' expression		{$$ = makeNode("",2,$1,$3);}
	        ;	
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
/* * * * STATEMENTS * * * */
/* statements : any code in the block- [assignment|func_call|if|doWhile|for|while|block|expression;|function]+ */
multi_statements:	stat_rec 
		|	empty			
	        ;
stat_rec	:	statement					{$$ = makeNode("",1,$1);}
	       	|	stat_rec statement     				{$$ = makeNode("",2,$1,$2);}
		;
statement	:	assign ';'					{$$ = $1;}
	        |	func_call					{$$ = makeParent2("",DEF,FC,$1);}
		|	if						{$$ = makeParent2("",DEF,IF,$1);} 
		|	if_else						{$$ = makeParent2("",DEF,IE,$1);}  				
	        |	while						{$$ = makeParent2("",DEF,W,$1);}
	        |	'{' code_block '}'  				{$$ = $2;}	
		|	';' 						{$$ = NULL;} // ignore from empty command, like just ';' 
		|	doWhile						{$$ = makeParent2("",DEF,DW,$1);}
		|	for						{$$ = makeParent2("",DEF,FOR_S,$1);}
		|	RETURN expression ';'				{$$ = makeParent2("RETURN",DEF, RETURN,$2);}
		|	RETURN ';'					{$$ = makeParent2("EMPTY_RETURN",DEF, RETURN,NULL);}
		|	var_declarations				{$$ = makeParent2("DECLARATIONS",DEF, DEC,$1);}
	        ;
/* IF ELSE */
if		:	IF '(' expression ')' statement	%prec ENDIF 		   {$$ = makeNode("IF",2,makeParent("COND",$3),makeParent("IF-BLOCK",$5));}
		;		
if_else		:	IF '(' expression ')' statement ELSE statement %prec ENDIF {$$ = makeNode("IF-ELSE",3,makeParent("COND",$3),makeParent("IF-BLOCK",$5),makeParent("ELSE-BLOCK",$7));}
		;
/* LOOPS - while / do / for */
while		:	WHILE '(' expression ')' statement		{$$ = makeNode("WHILE",2,$3,$5);}
		;
doWhile		:	DO statement WHILE '(' expression ')'';' 	{$$ = makeNode("DO_WHILE",2,makeParent("DO_WHILE-BLOCK",$2),makeParent("DO_WHILE-COND",$5));}
		;
for		:	FOR '(' inits_update ';' exp_for ';' inits_update ')' statement	
			{$$ = makeNode("FOR",4,makeParent("INITS",$3), makeParent("COND",$5), makeParent("UPDATES",$7),$9);}
		;
exp_for		:	expression					{$$ = $1;}
		|	/* empty cond */ 				{$$ = NULL;}						
		;
inits_update	:	assign 						{$$ = makeNode("",1,$1);}
		|	inits_update ',' assign				{$$ = makeNode("",2,$1,$3);}
		|	/* empty inits update */			{$$ = NULL;}	
		;

/* BLOCKS */	  	
code_block	:	/*var_declarations*/ multi_statements {$$ = makeNode2("BLOCK",DEF,BLOCK,2,makeParent2("STATMENTS",DEF,STMNT,$1), makeNode2("",INTEGER,ARG,0));}	
		;
/* declarations */
// like: int a; int b; char c, d, e = 'a';
var_declarations:	var_declarations_ne	
		;
var_declarations_ne:	func 						{$$ = makeParent2("",DEF,DF,makeParent2("",DEF, DF ,$1));}
		|	declare_vars ';'  				{$$ = makeParent2("",DEF,DV,makeParent2("",DEF, DV ,$1));} 
		;
// like: int a, b, c=2;
// like: string a[], b[], c[];
declare_vars	:	n_types var_identifier				{$$ = makeParent2($1->value, $1->t, $1->k,$2);}
		|	str_type var_arr_identifier			{$$ = makeParent2($1->value, $1->t, $1->k,$2);}
		|	n_types var_arr_identifier			{$$ = makeParent2($1->value, $1->t, $1->k,$2);}
		;
var_identifier	:	IDENTIFIER					{$$ = makeNode($1,0);}
		|	var_identifier ',' IDENTIFIER			{$$ = makeNode("",2,$1,makeNode($3,0));}
		;
var_arr_identifier:	IDENTIFIER'['expression']'			{$$ = makeNode($1,3,makeNode("[",0),$3,makeNode("]",0));}
		|	var_arr_identifier ',' IDENTIFIER'['expression']'{$$ = makeNode("",2,$1,makeNode($3,3,makeNode("[",0),$5,makeNode("]",0)));}
		;
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
/* TYPE VARIABLES */
n_types		:	CHAR						{$$ = makeNode2("CHAR"	, CHAR	, ARG,0);}
		|	INTEGER						{$$ = makeNode2("INTEGER",INTEGER,ARG,0);}
		|	BOOLEAN						{$$ = makeNode2("BOOLEAN",BOOLEAN,ARG,0);}
		|	CHARPTR						{$$ = makeNode2("CHARPTR",CHARPTR,ARG,0);}
		|	INTPTR						{$$ = makeNode2("INTPTR", INTPTR ,ARG,0);}				
		;
str_type	:	STRING						{$$ = makeNode2("STRING", ARRAY	, ARG,0);}
		;	
/* * * * CONSTS * * * */
const_value	:	CHAR_CONST					{$$ = makeNode2($1,CHAR,CONST,0);}
		|	BOOLEAN_CONST                          	 	{$$ = makeNode2($1,BOOLEAN,CONST,0);}
		|	NULL_           				{$$ = makeNode2($1,DEF,CONST,0);}
		|	integer_literal
		;

integer_literal	:	BINARY_CONST	                        	{$$ = makeNode2($1,INTEGER,CONST,0);}
		|	OCTAL_CONST                             	{$$ = makeNode2($1,INTEGER,CONST,0);}
		|	HEX_CONST                               	{$$ = makeNode2($1,INTEGER,CONST,0);}
		|	DECIMAL_CONST                           	{$$ = makeNode2($1,INTEGER,CONST,0);}
		;

/* * * * EXPRESSIONS * * * */
expression	:	IDENTIFIER					{$$ = makeNode2($1,DEF,VAR,0);}
		|	IDENTIFIER '[' expression ']'			{$$ = makeNode2($1  ,ARRAY  ,VAR,3,makeNode("[",0),$3,makeNode("]",0));}
		|	const_value					{$$ = $1;}
		|	func_call					{$$ = makeParent2("FUNC_CALL",DEF,FC,$1);}
		|	expression	AND 	expression          	{$$ = makeNode2("&&",BOOLEAN,B	,2,$1,$3);}
		|	expression	OR 	expression           	{$$ = makeNode2("||",BOOLEAN,B	,2,$1,$3);}
		|	expression	EQUAL 	expression        	{$$ = makeNode2("==",BOOLEAN,ANY,2,$1,$3);}
		|	expression	NE 	expression           	{$$ = makeNode2("!=",BOOLEAN,ANY,2,$1,$3);}
		|	expression	GREATER expression      	{$$ = makeNode2(">" ,BOOLEAN,I	,2,$1,$3);}
		|	expression	GE 	expression           	{$$ = makeNode2(">=",BOOLEAN,I	,2,$1,$3);}
		|	expression	LE 	expression		{$$ = makeNode2("<=",BOOLEAN,I	,2,$1,$3);}
		|	expression	LESS 	expression		{$$ = makeNode2("<" ,BOOLEAN,I	,2,$1,$3);}
		|	expression	PLUS 	expression		{$$ = makeNode2("+" ,INTEGER,I	,2,$1,$3);}
		|	expression	MINUS 	expression		{$$ = makeNode2("-" ,INTEGER,I	,2,$1,$3);}
		|	expression	MULTI 	expression		{$$ = makeNode2("*" ,INTEGER,I	,2,$1,$3);}
		|	expression	DIV 	expression		{$$ = makeNode2("/" ,INTEGER,I	,2,$1,$3);}
		|	'(' expression ')'				{$$ = $2;}
		|	'|' expression '|'				{$$ = makeParent2("ABS",INTEGER,I ,$2);}
		|	op_unary expression	%prec UNARY		{$$ = makeNode2($1  ,DEF    ,UN	,1,$2);}
		;					
lhs		:	IDENTIFIER					{$$ = makeNode($1,0);}
		|	DEREFERENCE IDENTIFIER				{$$ = makeNode("^",1,makeNode($2,0));}			
		|	IDENTIFIER '[' expression ']'  			{$$ = makeNode($1,3,makeNode("[",0),$3,makeNode("]",0));}
		;
assign		:	assignment					{$$ = makeParent2("",DEF,ASS,$1);}
		;
assignment	:	lhs '=' expression 				{$$ = makeNode2("=",DEF,EXPR,2,$1,$3);}
		|	lhs '=' STRING_CONST 				{$$ = makeNode2("=",DEF,CONST,2,$1,makeNode($3,0));}
		;
op_unary	:	ADDRESS_OP 									
		|	DEREFERENCE 
		|	NOT
		|	MINUS
		;
id_node		:	IDENTIFIER					{$$ = makeNode($1,0);}
		;
empty		:	/* empty */					{$$ = NULL;}
		;

%%


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



