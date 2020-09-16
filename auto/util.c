#include "globals.h"
#include "util.h"

void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case ELSE:
    case INT:
    case WHILE:
    case RETURN:
    case VOID:
      fprintf(myout,
         "reserved word: %s\n",tokenString);
      break;
    case LT: fprintf(myout,"<\n"); break;
    case LTEQ: fprintf(myout,"<=\n"); break;
    case GT: fprintf(myout,">\n"); break;
    case GTEQ: fprintf(myout,">=\n"); break;
    case ASSIGN: fprintf(myout,"=\n"); break;
    case DIFF: fprintf(myout,"!=\n"); break;
    case EQUAL: fprintf(myout,"==\n"); break;
    case LPAREN: fprintf(myout,"(\n"); break;
    case RPAREN: fprintf(myout,")\n"); break;
    case LBRACKETS: fprintf(myout,"[\n"); break;
    case RBRACKETS: fprintf(myout,"]\n"); break;
    case LKEY: fprintf(myout,"{\n"); break;
    case RKEY: fprintf(myout,"}\n"); break;
    case SEMI: fprintf(myout,";\n"); break;
    case COMMA: fprintf(myout,",\n"); break;
    case PLUS: fprintf(myout,"+\n"); break;
    case MINUS: fprintf(myout,"-\n"); break;
    case TIMES: fprintf(myout,"*\n"); break;
    case OVER: fprintf(myout,"/\n"); break;
    case ENDFILE: fprintf(myout,"EOF\n"); break;
    case NUM:
      fprintf(myout,
          "number: %s\n",tokenString);
      break;
    case ID:
      fprintf(myout,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      // fprintf(myout,
      //     "ERROR: %s\n",tokenString);
      fprintf(myout,"lex error token: %s\n",tokenString);
      exit(1);
      break;
    default: /* should never happen */
      fprintf(myout,"Unknown token: %d\n",token);
      exit(1);
  }
}

void TokenOut(){
  while(1){
    TokenType token=getToken();
    if(token!=ENDFILE){
      printf("%d:\t",lineno);
      printToken(token,tokenString);
    }else{
      break;
    }
  }
}
TreeNode * newTreeNode(StmtKind kind)
{ 
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(myout,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->brother = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}


TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(myout,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->brother = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;

  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(myout,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

static int indentno = 0;


#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(myout," ");
}

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(myout,"IfK\n");
          break;
        case WhileK:
          fprintf(myout,"WhileK\n");
          break;
        case AssignK:
          fprintf(myout,"AssignK\n");
          break;
        case ReturnK:
          fprintf(myout,"ReturnK\n");
          break;
        case FuncK:
          if(tree->type == 1){
            fprintf(myout,"FuncK: ");
            // INDENT;
            // printSpaces();
            fprintf(myout,"(IntK) ");
            // printSpaces();
            fprintf(myout,"(IdK: %s)\n",tree->attr.name);
          }
            // fprintf(myout,"Function: int %s\n",tree->attr.name);
            
          else{
            fprintf(myout,"FuncK: ");
            // INDENT;
            // printSpaces();
            fprintf(myout,"(VoidK) ");
            // printSpaces();
            fprintf(myout,"(IdK: %s)\n",tree->attr.name);
          }
            // fprintf(myout,"FuncK: void %s\n",tree->attr.name);
          break;
        case CallK:
          // fprintf(myout,"CallK: %s\n",tree->attr.name);
          fprintf(myout,"CallK: ");
          // INDENT;
          // printSpaces();
          fprintf(myout,"(IdK: %s)\n",tree->attr.name);
          break;
        case ParamsK:
          fprintf(myout,"ParamsK\n");
          break;
        case VoidK:
          fprintf(myout,"ParamsK\n");
          INDENT;
          printSpaces();
          fprintf(myout,"VoidK\n");
          UNINDENT;
          break;
        default:
          fprintf(myout,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(myout,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(myout,"ConstK: %d\n",tree->attr.val);
          break;
        case IdK:
          if(tree->is_vector)
            // fprintf(myout,"Vector: %s\n",tree->attr.name);
            {
              fprintf(myout,"IdK: %s\n",tree->attr.name);
              printSpaces();
              fprintf(myout,"IdK: []\n");
            }
          else
            fprintf(myout,"IdK: %s\n",tree->attr.name);
          break;
        case TypeK:
          if(tree->type == 1)
            fprintf(myout,"IntK\n");
          else
            fprintf(myout,"VoidK\n");
          break;
        case VectorK:
          // fprintf(myout,"Vector: %s\n",tree->attr.name);
          fprintf(myout,"IdK: %s\n",tree->attr.name);
          printSpaces();
          fprintf(myout,"IdK: []\n");
          break;
        default:
          fprintf(myout,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(myout,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->brother;
  }
  UNINDENT;
}

