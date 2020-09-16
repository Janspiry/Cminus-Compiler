#include "globals.h"

#include "util.h"

int Error=0;
int Parse = 0;
int lineno = 0;
FILE * myin;
FILE * myout;
int main( int argc, char * argv[] )
{ 
  TreeNode * syntaxTree;
  char testfile[100]; 

  if (argc <2||argc>3)
  {
      fprintf(stderr,"Usage: %s FileName [Token 0|Syntax-Tree 1]\n",argv[0]);
      exit(1);
  }
  
  
  if(argc ==3&&atoi(argv[2])){
      Parse = 1;
  }
  strcpy(testfile,argv[1]);

  myin = fopen(testfile,"r");
  if (myin==NULL)
  {
    fprintf(stderr,"File %s Not Found\n",testfile);
    exit(1);
  }

  myout = stdout; 
  if(!Parse){
    TokenOut(); 
  }else{
    syntaxTree = parse();
    fprintf(myout,"\nSyntax Tree:\n");
    printTree(syntaxTree);
  }
  fclose(myin);
  return 0;
}
