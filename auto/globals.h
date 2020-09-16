#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER

#include "yacc.tab.h"

#define ENDFILE 0

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8

typedef int TokenType;

extern FILE* myin; 
extern FILE* myout; 

extern int lineno; 



typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,WhileK,AssignK,ReturnK,FuncK,CallK,ParamsK,VoidK} StmtKind;
typedef enum {OpK,ConstK,IdK,TypeK,VectorK} ExpKind;

/* Type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 6

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * brother;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     int is_vector;
     ExpType type; /* for type checking of exps */
   } TreeNode;
extern int Error;
#endif
