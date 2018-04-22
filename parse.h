#ifndef PARSE_H_HEADER_
#define PARSE_H_HEADER_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef enum type{
	CHAR,		// 'a', 'b' , …
	INTEGER,  	// 1, 2, 500, -12
	BOOLEAN, 	// true, false
	CHARPTR, 	// charptr cp1, c, p2
	INTPTR, 	// intptr    ip1, I, p2
	ARRAY, 		// string a[5] 
	DEF		// define for id and statements
} type;			// the name of this structure - type

typedef enum kind{
	FUNC,		// void func (){}
	VAR,		// var a , int i=5, char c = 'a'
	CONST,		// lhs = STRING_CONST;    5 , true, "STRING CONST"
	EXPR,		// lhs = expression ;  
	ARG, 		// int a, int c, int b
	BLOCK, 		// { … }
	RETURN, 	// return 0; return true; return;
	DEC,		// declares var and func in block	
	DV, 		// declare var inside func
	DF,		// declare func inside func
	STMNT,		// statement
	I,		// operators : + - / * <= < > >=  || ()	
	B,		// Boolean operator: || &&
	ANY,		// operators : != == 	
	UN,		// unary operator
	ASS,		// assument  '='
	FC,		// function call
	IF,		// if statment
	IE,		// if else statement
	W,		// while statment
	DW, 		// Do while statement
	FOR_S, 		// for statement
	NONE		// empty kind
} kind;			// the name of this structure - kind

typedef struct node {
	char *value;	// string value of the node
	int count;	// amount of sons
	kind k;		// FUNC/ VAR/ CONST/ EXPR…
	type t;		//  CHAR / INTEGER/ BOOLEAN/ CHARPTR/ INTPTR/ ARRAY/ DEF
	struct node** sons; // many sons
} node;			// the name of this structure - node

typedef struct entry {
	char *name;	// name of the node that entered
	char *value;	// the value of the node that entered (for me is the name of the list)
	kind k;		// the kind of the list
	type t;		// the type of the list
	int argCount;	// the amount of arguments in the list
	type *args;	// list - arguments of types 
	int useReturn;
	struct entry *next; // the scope of this entry block
}	node_t, *list; 	// the name of this structure - list

typedef struct scope
{
	char *nameScope; // the name of the scope
	list l;		// scope of block (function, or any block)
	struct scope *next; // next scope, is a one direction list
} scope;			// the name of this structure - scope

#endif

