%{
#include "globals.h"
#include "util.h"

char tokenString[MAXTOKENLEN+1];
static int yylex(void);
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"while"         {return WHILE;}
"return"        {return RETURN;}
"void"          {return VOID;}
"="             {return ASSIGN;}
"=="            {return EQUAL;}
"!="            {return DIFF;}
"<"             {return LT;}
"<="            {return LTEQ;}
">"             {return GT;}
">="            {return GTEQ;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRACKETS;}
"]"             {return RBRACKETS;}
"{"             {return LKEY;}
"}"             {return RKEY;}
";"             {return SEMI;}
","             {return COMMA;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"             { char c, ant;
                  c = input();
                  do
                  {
                    ant = c;
                    c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                  } while (ant != '*' || c != '/');

                }
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = myin;
    yyout = myout;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  return currentToken;
}
