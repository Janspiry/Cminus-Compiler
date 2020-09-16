#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <list>
#include <set>
using namespace std;
int lineNumber,colNumber;

enum TokenType
{
	ENDFILE=0,
	IF,ELSE,INT,RETURN,VOID,WHILE,
	ID,NUMBER,
	ASSIGN,LT,GT,EQ,NEQ,GEQ,LEQ,
	PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,LBRACKET,RBRACKET,LBRACE,RBRACE,COMMA,SEMI,
};

enum StateType
{
	SUNACCEPTED = 0,
	SID,
	SRESERVED,
	SNUMBER,
	SOPERATOR,
	SCOMMENT,
	SENDFILE
};
enum TreeNodeType
{
	FuncK,IntK,IdK,ParamsK,ParamK,CompK,ConstK,CallK,ArgsK,VoidK,Var_DeclK,Arry_DeclK,Arry_ElemK,AssignK/*,WhileK*/,OpK,
	Selection_StmtK,Iteration_StmtK,Return_StmtK
}; 

typedef bool(*ACTION)(char);

struct Edge
{
	int transition;
	ACTION action;

	Edge(int transition, ACTION action)
	{
		this->transition = transition;
		this->action = action;
	}
};

struct Node
{
	StateType type;
	std::list<Edge> edges;
};

struct Token
{
	int line;
	std::string value;
	TokenType type;
	Token(){
		line=1;
		type=INT;
		value.reserve(100);
	}
	Token(const int line,const std::string& value, TokenType type)
	{
		this->line = line;
		this->value = value;
		this->type = type;
	}
};

vector<Node> dfa;
unordered_map<string,TokenType> String2Token;
unordered_map<TokenType,string> Token2String;
unordered_map<TreeNodeType,string> TreeNode2String;

bool IsLetter(char ch);
bool IsDigit(char ch);
bool IsOtherOperator(char ch);
bool IsBlank(char ch);
bool IsOther(char ch);
void DfaBuild();
Token StateTpye2TokenType(int line,string token,StateType state);
Token TokenGet();

void LexInit();
void PraseInit();

Token GlobalToken;
struct TreeTokenNode{ 
	char val[20];
	TokenType type;
	TreeTokenNode(){
		memset(val,0,sizeof(val));
	}
};
struct TreeNode
{
	struct TreeNode *child[4];
	struct TreeNode *brother;
    TreeNodeType TreenodeType;
	TreeTokenNode token;
};

void match(TokenType expected);
void TokenCopy(TreeTokenNode &src,Token dst);
TreeNode *newNode(TreeNodeType typ);

TreeNode * declaration_list();
TreeNode * declaration();
TreeNode * params();
TreeNode * param_list(TreeNode * par);
TreeNode * param(TreeNode * par);
TreeNode * compound_stmt();
TreeNode * local_declaration();
TreeNode * statement_list();
TreeNode * statement();
TreeNode * expression_stmt();
TreeNode * selection_stmt();
TreeNode * iteration_stmt();
TreeNode * return_stmt();
TreeNode * expression();
TreeNode * var();
TreeNode * simple_expression(TreeNode * par);
TreeNode * additive_expression(TreeNode * par);
TreeNode * term(TreeNode * par);
TreeNode * factor(TreeNode * par);
TreeNode * call(TreeNode * par);
TreeNode * args();
 

void ParseSyntax();
void TokenOut();
void PreOrder(TreeNode * root);

void ShowLexError(string token);
void ShowGrammarError();