#define MAXTOKENLEN 40
extern char tokenString[MAXTOKENLEN+1];

TokenType getToken(void);

void printToken( TokenType, const char* );

void TokenOut();

TreeNode * newTreeNode(StmtKind);

TreeNode * newExpNode(ExpKind);

char * copyString( char * );

TreeNode * parse(void);
void printTree( TreeNode * );

