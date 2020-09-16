%{
#define YYPARSER 

#include "globals.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

#define YYSTYPE TreeNode *
int yyerror(char * message);
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);

%}
%start PROGRAM
%token ID NUM
%token SEMI COMMA
%token IF INT ELSE RETURN VOID WHILE
%token ASSIGN EQUAL LT LTEQ GT GTEQ DIFF
%token LPAREN RPAREN LBRACKETS RBRACKETS LKEY RKEY
%left PLUS MINUS
%left TIMES OVER
%token ERROR
%expect 1

%% /* Grammar for C- */
PROGRAM   : stmt_seq
              {savedTree = $1;}
          ;

//BEGIN FUCTION

stmt_seq   :  stmt { $$ =  $1; }
           |  stmt_seq stmt
              {
                  YYSTYPE t = $1;
                  if ( t != NULL )
                  {
                    while( t->brother != NULL )
                       t = t->brother;
                    t->brother = $2;
                    $$ = $1;
                  } else $$ = $2;
              }
           ;

stmt       : var_stmt  { $$ = $1; }
           | func_stmt { $$ = $1; }
           ;

var_stmt   : type_id id SEMI
               {
                 $$ = $1;
                 $$->child[0] = $2;
               }
           | type_id id LBRACKETS num RBRACKETS SEMI
               {
                 $$ = $1;
                 $2->is_vector = 1;
                 $$->child[0] = $2;
                 $2->child[0] = $4;
               }
           ;

type_id   : INT
                {
                    $$ = newExpNode(TypeK);
                    $$->type=Integer;
                    $$->attr.name = copyString(tokenString);
                }

          | VOID
                {
                    $$ = newExpNode(TypeK);
                    $$->type=Void;
                    $$->attr.name = copyString(tokenString);
                }
          ;

func_stmt   : type_id id LPAREN params RPAREN comp_stmt
                {
                  $$ = newTreeNode(FuncK);
                  $$->attr.name = $2->attr.name;
                  $$->child[0] = $4;
                  $$->child[1] = $6;
                  if (!strcmp($1->attr.name,"int"))
                      $$->type = Integer;
                  else
                      $$->type = Void;
                }
            ;
params      : param_list
                {
                  $$ = newTreeNode(ParamsK);
                  $$->child[0] = $1;
                }

            | VOID
                { $$ = newTreeNode(VoidK);
                  $$->child[0] = $1; }

            |
                { $$ = NULL; }
            ;

param_list  : param_list COMMA param
                {
                    YYSTYPE t = $1;
                    if (t != NULL)
                    {
                      while (t->brother != NULL)
                          t = t->brother;
                      t->brother = $3;
                      $$ = $1;
                    } else $$ = $3;
                }
            | param
                { $$ = $1; }
            ;

param       : type_id id
                {
                  $$ = $1;
                  $$->child[0] = $2;
                  $2->is_vector = 0;
                }

            | type_id id LBRACKETS RBRACKETS
                {
                  $$ = $1;
                  $$->child[0] = $2;
                  $2->is_vector = 1;
                }
            ;

comp_stmt   : LKEY local_stmt stmtem_seq RKEY
                 {
                      YYSTYPE t = $2;
                      if (t != NULL)
                      {
                        while (t->brother != NULL)
                             t = t->brother;
                        t->brother = $3;
                        $$ = $2;
                      } else $$ = $3;
                  }
            ;

local_stmt  : local_stmt var_stmt
                  {
                      YYSTYPE t = $1;
                      if (t != NULL)
                      {
                          while (t->brother != NULL)
                              t = t->brother;
                          t->brother = $2;
                          $$ = $1;
                      } else $$ = $2;
                  }
            | empty {$$ = NULL;}
            ;
//END FUNCTION

stmtem_seq   : stmtem_seq stmtem
              {
                  YYSTYPE t = $1;
                  if ( t != NULL )
                  {
                    while( t->brother != NULL )
                       t = t->brother;
                    t->brother = $2;
                    $$ = $1;
                  } else $$ = $2;
              }
           |  empty { $$ = NULL; }
           ;

stmtem     : exp_stmt    { $$ = $1; }
           | comp_stmt   { $$ = $1; }
           | if_stmt     { $$ = $1; }
           | while_stmt  { $$ = $1; }
           | return_stmt { $$ = $1; }
           ;

exp_stmt    : exp SEMI
               {$$ = $1;}
            | SEMI
               {$$ = NULL;}
            ;

if_stmt     : IF LPAREN exp RPAREN stmtem
            {
                     $$ = newTreeNode(IfK);
                     $$->child[0] = $3;
                     $$->child[1] = $5;
            }
            | IF LPAREN exp RPAREN stmtem ELSE stmtem
            {
                      $$ = newTreeNode(IfK);
                      $$->child[0] = $3;
                      $$->child[1] = $5;
                      $$->child[2] = $7;
            }
           ;

while_stmt : WHILE LPAREN exp RPAREN stmtem
            {
                  $$ = newTreeNode(WhileK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
            }
          ;

return_stmt : RETURN SEMI
                  { $$ = newTreeNode(ReturnK); }
            | RETURN exp SEMI
                  {
                    $$ = newTreeNode(ReturnK);
                    $$->child[0] = $2;
                  }
            ;

exp      : var ASSIGN exp
          { $$ = newTreeNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
              | simples_exp
                 { $$ = $1; }
              ;

var        : id
               {
                 $$ = $1;
                 $1->is_vector = 0;
               }
           | id LBRACKETS exp RBRACKETS
               {
                 $$ = newExpNode(VectorK);
                 $$->attr.name = $1->attr.name;
                 $$->is_vector = 1;
                 $$->child[0] = $3;
               }
           ;

simples_exp : soma_exp relacional soma_exp
            {
                $$ = newExpNode(OpK);
                $$->attr.op = $2->attr.op;
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
           | soma_exp { $$ = $1; }
           ;

relacional  : LTEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = LTEQ;
          }
          | LT  {
                $$ = newExpNode(OpK);
                $$->attr.op = LT;
          }
          | GT  {
                $$ = newExpNode(OpK);
                $$->attr.op = GT;
          }
          | GTEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = GTEQ;
                   }
          | EQUAL {
                $$ = newExpNode(OpK);
                $$->attr.op = EQUAL;
          }
          | DIFF {
                $$ = newExpNode(OpK);
                $$->attr.op = DIFF;
            }
            ;

soma_exp   : soma_exp PLUS term
                {
                    $$ = newExpNode(OpK);
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    $$->attr.op = PLUS;
                }
           |  soma_exp MINUS term
                {
                    $$ = newExpNode(OpK);
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    $$->attr.op = MINUS;
                 }
           | term { $$ = $1; }
           ;

term       : term TIMES factor
                 {
                   $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = TIMES;
                 }
            | term OVER factor
                 {
                   $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = OVER;
                 }
            | factor { $$ = $1; }
            ;

factor      : LPAREN exp RPAREN
                 { $$ = $2; }
            | var
                  {$$ = $1;}
            | call
                  {$$ = $1;}
            | num
                  {$$ = $1;}
            ;

//*************BEGIN CALL FUCTION ***************
call        : id LPAREN args RPAREN
              {
                $$ = newTreeNode(CallK);
                $$->attr.name = $1->attr.name;
                $$->child[0] = $3;
              }
            ;

args        : arg_list
                { $$ = $1; }
            | empty
            ;

arg_list  : arg_list COMMA exp
              {
                  YYSTYPE t = $1;
                  if (t != NULL)
                  {
                      while (t->brother != NULL)
                        t = t->brother;
                      t->brother = $3;
                      $$ = $1;
                  } else $$ = $3;
               }
          | exp
               { $$ = $1; }
          ;

//*************END CALL FUCTION ***************

id         : ID
               {
                  $$ = newExpNode(IdK);
                  $$->attr.name = copyString(tokenString);
               }
           ;

num        : NUM
               {
                 $$ = newExpNode(ConstK);
                 $$->attr.val = atoi(copyString(tokenString));
               }
           ;

empty     : { $$ = NULL; }
          ;
%%

int yyerror(char * message)
{ fprintf(myout,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(myout,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}
