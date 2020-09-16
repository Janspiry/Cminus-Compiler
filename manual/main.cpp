#include <iostream>
#include "util.h"
using namespace std;
bool IsLetter(char ch)
{
	return 'A' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z';
}

bool IsDigit(char ch)
{
	return '0' <= ch && ch <= '9';
}

bool IsOtherOperator(char ch)
{
	switch (ch)
	{
	case '+': case '-': case '*':
	case ':':
	case ',':
	case '(': case ')':
	case '[': case ']':
	case '{':case '}':
	case ';':
		return true;
	default:
		return false;
	}
}

bool IsBlank(char ch)
{
	return ch==' '||ch=='\t';
}

bool IsOther(char ch)
{
	return true;
}

void DfaBuild()
{
	dfa.clear();

	Node node0;
	node0.type = StateType::SUNACCEPTED;
	node0.edges.emplace_back(1, IsLetter);
	node0.edges.emplace_back(2, IsDigit);
	node0.edges.emplace_back(3, [](char ch) {return ch == '<' || ch == '>' || ch == '=' || ch == '!'; });
	node0.edges.emplace_back(5, IsOtherOperator);
	node0.edges.emplace_back(6, [](char ch) {return ch == '/'; });
	dfa.push_back(node0);

	Node node1;
	node1.type = StateType::SID;
	node1.edges.emplace_back(1, IsLetter);
	dfa.push_back(node1);

	Node node2;
	node2.type = StateType::SNUMBER;
	node2.edges.emplace_back(2, IsDigit);
	dfa.push_back(node2);

	Node node3;
	node3.type = StateType::SOPERATOR;
	node3.edges.emplace_back(4, [](char ch) {return ch == '='; });
	dfa.push_back(node3);

	Node node4;
	node4.type = StateType::SOPERATOR;
	dfa.push_back(node4);

	Node node5;
	node5.type = StateType::SOPERATOR;
	dfa.push_back(node5);

	Node node6;
	node6.type = StateType::SOPERATOR;
	node6.edges.emplace_back(7, [](char ch) {return ch == '*'; });
	dfa.push_back(node6);

	Node node7;
	node7.type = StateType::SUNACCEPTED;
	node7.edges.emplace_back(8, [](char ch) {return ch == '*'; });
	node7.edges.emplace_back(7, IsOther);

	dfa.push_back(node7);

	Node node8;
	node8.type = StateType::SUNACCEPTED;
	node8.edges.emplace_back(9, [](char ch) {return ch == '/'; });
	node8.edges.emplace_back(7, IsOther);
	dfa.push_back(node8);

	Node node9;
	node9.type = StateType::SCOMMENT;
	dfa.push_back(node9);

}

Token StateTpye2TokenType(int line,string token,StateType state){
	switch (state)
	{
	case StateType::SID:
		if(String2Token[token]>0){
			return Token(lineNumber,token,String2Token[token]);
		}else{
			return Token(lineNumber,token,TokenType::ID);
		}
	case StateType::SNUMBER:
		return Token(lineNumber,token,TokenType::NUMBER);
	case StateType::SENDFILE:
		return Token(lineNumber,token,TokenType::ENDFILE);
	default:
		return Token(lineNumber,token,String2Token[token]);
	}
}

Token TokenGet(){
	int state = 0;
	string token="";
	while (true){
		char ch = fgetc(stdin);
		colNumber++;
		bool flag = false;
		for (auto it : dfa[state].edges){
			if (it.action(ch)){
				flag = true;
				state = it.transition;
				token.push_back(ch);
				break;
			}
		}
		if (!flag){
			if (dfa[state].type != StateType::SUNACCEPTED){
				if (!IsBlank(ch)){
					ungetc(ch,stdin);
				}
				if (dfa[state].type != StateType::SCOMMENT){
					return StateTpye2TokenType(lineNumber,token,dfa[state].type);
				}else{
					token.clear();
					state=0;
				}
			}else if ((ch!='\n'&&ch!='\t'&&ch!=' '&&ch != EOF)){
				ShowLexError(token);
			}
		}
		if (ch == '\n'){
			lineNumber++;
			colNumber = 0;
		}else if (ch == EOF){
			return StateTpye2TokenType(lineNumber,token,StateType::SENDFILE);
		}
	}
}

void LexInit(){
	lineNumber = 1, colNumber = 0;
	String2Token["if"]=IF;
	String2Token["else"]=ELSE;
	String2Token["int"]=INT;
	String2Token["void"]=VOID;
	String2Token["while"]=WHILE;
	String2Token["return"]=RETURN;

	String2Token["="]=ASSIGN;
	String2Token["<"]=LT;
	String2Token[">"]=GT;
	String2Token["=="]=EQ;
	String2Token["!="]=NEQ;
	String2Token[">="]=GEQ;
	String2Token["<="]=LEQ;

	String2Token["+"]=PLUS;
	String2Token["-"]=MINUS;
	String2Token["*"]=TIMES;
	String2Token["/"]=OVER;
	String2Token["("]=LPAREN;
	String2Token[")"]=RPAREN;
	String2Token["["]=LBRACKET;
	String2Token["]"]=RBRACKET;
	String2Token["{"]=LBRACE;
	String2Token["}"]=RBRACE;
	String2Token[";"]=SEMI;
	String2Token[","]=COMMA;


	Token2String[IF]="if";
	Token2String[ELSE]="else";
	Token2String[INT]="int";
	Token2String[VOID]="void";
	Token2String[WHILE]="while";
	Token2String[RETURN]="return";

	Token2String[ASSIGN]="=";
	Token2String[LT]="<";
	Token2String[GT]=">";
	Token2String[EQ]="==";
	Token2String[NEQ]="!=";
	Token2String[GEQ]=">=";
	Token2String[LEQ]="<=";

	Token2String[PLUS]="+";
	Token2String[MINUS]="-";
	Token2String[TIMES]="*";
	Token2String[OVER]="/";
	Token2String[LPAREN]="(";
	Token2String[RPAREN]=")";
	Token2String[LBRACKET]="[";
	Token2String[RBRACKET]="]";
	Token2String[LBRACE]="{";
	Token2String[RBRACE]="}";
	Token2String[SEMI]=";";
	Token2String[COMMA]=",";
}

void PraseInit(){
	lineNumber = 1, colNumber = 0;
	
	TreeNode2String[FuncK]="Funck";
	TreeNode2String[IntK]="IntK";
	TreeNode2String[IdK]="IdK";
	TreeNode2String[ParamsK]="ParamsK";
	TreeNode2String[ParamK]="ParamK";
	TreeNode2String[CompK]="CompK";
	TreeNode2String[ConstK]="ConstK";
	TreeNode2String[CallK]="CallK";

	TreeNode2String[ArgsK]="ArgsK";
	TreeNode2String[VoidK]="VoidK";
	TreeNode2String[Var_DeclK]="Var_DeclK";
	TreeNode2String[Arry_DeclK]="Arry_DeclK";
	TreeNode2String[Arry_ElemK]="Arry_ElemK";
	TreeNode2String[AssignK]="AssignK";
	TreeNode2String[OpK]="OpK";
	TreeNode2String[Selection_StmtK]="Selection_StmtK";
	TreeNode2String[Iteration_StmtK]="Iteration_StmtK";
	TreeNode2String[Return_StmtK]="Return_StmtK";
}

void ShowGrammarError(){
	cout<<"unexpected token:"<<GlobalToken.value
		<<" in line "<<lineNumber
		<<" , col "<<colNumber<<endl;
	exit(1);
}

void ShowLexError(string token){
	cout << "lex error:" << token
		<<" in line "<<lineNumber
		<<" , col "<<colNumber<<endl;
	exit(1);
}
void match(TokenType expected)
{
	if(GlobalToken.type==expected){
		GlobalToken=TokenGet();
	}else{
		ShowGrammarError();
	}
}
void TokenCopy(TreeTokenNode &src,Token dst){
	src.type=dst.type;
	// cout<<"dst.value="<<dst.value<<endl;
	strcpy(src.val,dst.value.c_str());
}
TreeNode *newNode(TreeNodeType typ)
{ 
	TreeNode *ret = (TreeNode *) malloc(sizeof(TreeNode));
	for (int i=0;i<4;i++) {
		ret->child[i] = NULL;
	}
    ret->brother = NULL;
    ret->TreenodeType = typ;
	return ret;
}

//declaration_list->declaration{declaration}
TreeNode * declaration_list()
{
	TreeNode * root = declaration();
	TreeNode * now =root;
	while(GlobalToken.type==INT||GlobalToken.type==VOID){
		TreeNode *nxt = declaration();
		if (nxt!=NULL){
			now->brother=nxt;
			now=nxt;
		}
	}
	match(TokenType::ENDFILE);
	return root;
}

TreeNode * declaration()
{
	TreeNode * root = NULL;
	TreeNode * fir = NULL,* sec = NULL,* thi = NULL,* fou = NULL;
	if (GlobalToken.type==INT){
		fir=newNode(IntK);
		match(TokenType::INT);
	}else{//VOID
		fir=newNode(VoidK);
		match(TokenType::VOID);
	}
	if(GlobalToken.type==ID){
		sec = newNode(IdK);
		TokenCopy(sec->token,GlobalToken);
		match(TokenType::ID);
		switch (GlobalToken.type)
		{
		case LPAREN:
			root = newNode(FuncK);
			root->child[0] = fir;   
			root->child[1] = sec;	

			match(TokenType::LPAREN);
			root->child[2] = params();
			match(TokenType::RPAREN);
			
			root->child[3] = compound_stmt();
			break;
		case LBRACKET:
			root = newNode(Var_DeclK);
			fou = newNode(Arry_DeclK);
			root->child[0] = fir;   
			root->child[1] = fou;

			match(TokenType::LBRACKET);
			thi = newNode(ConstK);
			TokenCopy(thi->token,GlobalToken);
			match(TokenType::NUMBER);
			fou->child[0]=sec;
			fou->child[1]=thi;
			match(TokenType::RBRACKET);
			match(TokenType::SEMI);
			break;
		case SEMI:
			root = newNode(Var_DeclK);
			root->child[0] = fir;
			root->child[1] = sec;
			match(SEMI);
			break;
		default:
			ShowGrammarError();
			break;
		}
	}else{
		ShowGrammarError();
	}
	return root;
}


//params_list | void
TreeNode * params(){
	TreeNode * root = newNode(ParamsK);
	TreeNode * now  = NULL;
	if (GlobalToken.type==VOID){
		now = newNode(VoidK);
		match(VOID);
		root->child[0] = now;
	}else{
		root->child[0] = param_list(now);
	}
	return root;
}

//params_list->param{,param}
TreeNode * param_list(TreeNode * par)
{
	TreeNode * root = param(par);
	TreeNode * now = root; 
	par = NULL;
	while (GlobalToken.type==COMMA){
		TreeNode * nxt = NULL;
		match(COMMA);
		nxt = param(par);
		if (nxt!=NULL){
			now->brother = nxt;
			now = nxt;
		}
	}
	return root;
}

TreeNode * param(TreeNode * par)
{
	TreeNode * root = newNode(ParamK);
	TreeNode * now = NULL,* nxt = NULL;
	if(par==NULL){
		if(GlobalToken.type==VOID){
			now = newNode(VoidK);
			match(VOID);
		}else if(GlobalToken.type==INT){
			now = newNode(IntK);
			match(INT);
		}
	}else{
		now = par;
	}
	if(now!=NULL){
		root->child[0] = now;
		if (GlobalToken.type==ID){
			nxt = newNode(IdK);
			TokenCopy(nxt->token,GlobalToken);
			root->child[1] = nxt;
			match(ID);
		}
		if (GlobalToken.type==LBRACKET&&(root->child[1]!=NULL)){//num[]
			match(LBRACKET);
			root->child[2] = newNode(IdK);
			strcpy(root->child[2]->token.val,"[]");
			match(RBRACKET);
		}else{  
			return root; 
		}		
	}else {
		ShowGrammarError();
	}
	return root;
}
 
TreeNode * compound_stmt()
{  
	TreeNode * t = newNode(CompK);
	match(LBRACE);
	t->child[0] = local_declaration();
	t->child[1] = statement_list(); 
	match(RBRACE);
	return t;
}
 
TreeNode * local_declaration()
{  
	TreeNode * root = NULL,* now = NULL,* nxt = NULL;
	while(GlobalToken.type==INT || GlobalToken.type==VOID){ 
		nxt = newNode(Var_DeclK);
		if(GlobalToken.type==INT){
			nxt->child[0] = newNode(IntK);
			match(INT);
		}else{
			nxt->child[0] = newNode(VoidK);
			match(VOID);
		}
		if(GlobalToken.type==ID){ 
			nxt->child[1] = newNode(IdK);
			TokenCopy(nxt->child[1]->token,GlobalToken);
			match(ID);
			if(GlobalToken.type==LBRACKET){ 
				nxt->child[3] = newNode(Var_DeclK);
				match(LBRACKET);
				match(RBRACKET);
				match(SEMI);
			}else{
				match(SEMI);
			}
		} 
		if(root==NULL){
			root = now = nxt;
		}else{
			now->brother = nxt;
			now = nxt;
		}
	}
	return root;
}

TreeNode * statement_list()
{  
	TreeNode * root = statement();   
	TreeNode * now = root;
	set<int> s{IF,LBRACKET,ID,WHILE,RETURN,SEMI,LPAREN,NUMBER};
	while (s.find(GlobalToken.type)!=s.end()){  
		TreeNode * nxt;
		nxt = statement();
		if(nxt!=NULL){
			if(root==NULL){ 
				root = now = nxt;
			}else{  
				now->brother = nxt;
				now = nxt;
			}
		}
	}
	return root;
}

TreeNode * statement()
{  
	switch(GlobalToken.type){
		case IF:return selection_stmt();
		case WHILE:return iteration_stmt();
		case RETURN:return return_stmt();
		case LBRACE:return compound_stmt();
		case ID: case SEMI: case LPAREN: case NUMBER: return expression_stmt();
		default:ShowGrammarError();
	}
}
 
TreeNode * selection_stmt()
{  
	TreeNode * root = newNode(Selection_StmtK);
	match(IF);
	match(LPAREN);
	root->child[0] = expression();
	match(RPAREN);
	root->child[1] = statement();
	if(GlobalToken.type==ELSE){ 
		match(ELSE);
		root->child[2] = statement();
	}
	return root;
}
 
TreeNode * iteration_stmt()
{  
	TreeNode * root = newNode(Iteration_StmtK);
	match(WHILE);
	match(LPAREN);
	root->child[0] = expression();
	match(RPAREN);
	root->child[1] = statement();
	return root;
}
 
TreeNode * return_stmt()
{  
	TreeNode * root = newNode(Return_StmtK);
	match(RETURN);
	if (GlobalToken.type==SEMI){  
		match(SEMI);
		return root;
	}else{ 
		root->child[0] = expression();
	}
	match(SEMI);
	return root;
}
 
TreeNode * expression_stmt(){  
	TreeNode * root = NULL;
	if(GlobalToken.type==SEMI){ 
		match(SEMI);
		return root;
	}else{
		root = expression();
		match(SEMI);
	}
	return root;
}
 
TreeNode * expression()
{  
	TreeNode * t = var();
	if(t==NULL){ 
		t = simple_expression(t); 
	}else{    
		TreeNode * now = NULL;
		if(GlobalToken.type==ASSIGN){ 
			now = newNode(AssignK);
			match(ASSIGN);
			now->child[0] = t;
			now->child[1] = expression();
			return now;
		}else{ 
			t = simple_expression(t);
		}
	}	   
	return t;
}
 
TreeNode * var()
{  
	TreeNode * root = NULL,* fir = NULL,* sec = NULL;
	if(GlobalToken.type==ID){
		fir = newNode(IdK);
		TokenCopy(fir->token,GlobalToken);
		match(ID);
		if(GlobalToken.type==LBRACKET) { 
			match(LBRACKET);
			sec = expression();
			match(RBRACKET);
			root = newNode(Arry_ElemK);
			root->child[0] = fir;
			root->child[1] = sec;
		}else{
			root = fir;
		}
	}
	return root;
}
 
TreeNode * simple_expression(TreeNode * par)
{   
	TreeNode * root = additive_expression(par);
	par = NULL;
	set<int> s{EQ,GT,GEQ,LT,LEQ,NEQ};
	if(s.find(GlobalToken.type)!=s.end()){  
		TreeNode * now = newNode(OpK);
		TokenCopy(now->token,GlobalToken);
		now->child[0] = root;
		root = now;
		match(GlobalToken.type);
		root->child[1] = additive_expression(par);		 
		return root;
	}
	return root;
}
 
TreeNode * additive_expression(TreeNode * par)
{  
	TreeNode * root = term(par);
	par = NULL;
	while((GlobalToken.type==PLUS)||(GlobalToken.type==MINUS))
	{ 
		TreeNode * now = newNode(OpK);
		TokenCopy(now->token,GlobalToken);
		now->child[0] = root; 
		match(GlobalToken.type);
		now->child[1] = term(par);
		root = now;		
	}
	return root;
}
 
TreeNode * term(TreeNode * par)
{  
	TreeNode * root = factor(par);
	par = NULL;
	while((GlobalToken.type==TIMES)||(GlobalToken.type==OVER)){ 
		TreeNode * now = newNode(OpK);
		TokenCopy(now->token,GlobalToken);
		now->child[0] = root; 
		root = now;
		match(GlobalToken.type);
		now->child[1] = factor(par);
	}
	return root;
}
 
TreeNode * factor(TreeNode * par)
{  
	TreeNode * root = NULL;
	if(par!=NULL){
		if(GlobalToken.type==LPAREN && par->TreenodeType!=Arry_ElemK){ 
			root = call(par);
		}else{ 
			root = par; 
		}
	}else{ 
		switch(GlobalToken.type){
		case LPAREN:
			match(LPAREN);
			root = expression();
			match(RPAREN);
			break;
		case ID:
			par = var();
			if(LPAREN==GlobalToken.type && par->TreenodeType!=Arry_ElemK) { 
				root = call(par);
			}else{
				root=par;
			}
			break;
		case NUMBER:
			root = newNode(ConstK);
			TokenCopy(root->token,GlobalToken);
			match(NUMBER);
			break;
		default:
			ShowGrammarError();
		} 
	}
	return root;
}
 
TreeNode * call(TreeNode * par)
{  
	TreeNode * root = newNode(CallK);
	if(par!=NULL)
		root->child[0] = par;
	match(LPAREN);
	if(GlobalToken.type==RPAREN)  {  
		match(RPAREN);
		return root;
	}else if(par!=NULL){ 
		root->child[1] = args();
		match(RPAREN);
	}
	return root;
}
 
TreeNode * args()
{  
	TreeNode * root = newNode(ArgsK);
	TreeNode * tmp = NULL,* now = NULL;
	if(GlobalToken.type!=RPAREN){
		tmp = expression();
		now = tmp;
		while(GlobalToken.type==COMMA){   
			TreeNode * nxt;
			match(COMMA);
			nxt = expression();
			if(nxt!=NULL){
				if(tmp==NULL){ 
					tmp = now = nxt;
				}else{  
					now->brother = nxt;
					now = nxt;
				}
			}
		}
	}	
	if(tmp!=NULL){
		root->child[0] = tmp;
	}
	return root;
}
 
int blankCnt=0;
void PreOrder(TreeNode* t)
{
	string blank="  ";
	for(int i=0;i<blankCnt;i++){
		blank+=" ";
	}
	if(t!=NULL)
	{	
		if(t->TreenodeType==OpK){
			cout<<blank<<"Op: "<<Token2String[t->token.type]<<endl;
		}else{
			cout<<blank<<TreeNode2String[t->TreenodeType];
			if(t->TreenodeType==IdK||t->TreenodeType==ConstK){
				cout<<": "<<t->token.val;
			}
			cout<<endl;
		}
	}
	for(int i=0;i<4;i++){
	    if(t->child[i]!=NULL){
			blankCnt+=2;
	        PreOrder(t->child[i]);
			blankCnt-=2;
	    }
	}
	if(t->brother!=NULL){	
		PreOrder(t->brother);
	}
}
 
void ParseSyntax()
{
    cout<<"Syntax Tree:"<<endl;
	GlobalToken = TokenGet();
	TreeNode * root = declaration_list();
	PreOrder(root);
}
void TokenOut(){
	while(1){
		Token tok=TokenGet();
		TokenType &type=tok.type;
		string token=tok.value;
		if(type==TokenType::ENDFILE){
			break;
		}
		cout<<tok.line<<":\t";
		switch (type){
			case TokenType::ID:
				cout << "ID, name = " << token << endl;
				break;
			case TokenType::IF: case TokenType::ELSE:case TokenType::INT: 
			case TokenType::RETURN: case TokenType::VOID: case TokenType::WHILE:
				cout << "reserved word: " << token << endl;
				break;
			case TokenType::NUMBER:
				cout << "number: " << token << endl;
				break;
			default:
				cout << token << endl;
				break;
		}
	}	
}
int Parse=0;
int main( int argc, char * argv[] )
{ 
	char testfile[100]; 
	if (argc <2||argc>3)
	{
		fprintf(stderr,"Usage: %s FileName [Token 0|Syntax-Tree 1]\n",argv[0]);
		exit(1);
	}
	if(argc ==3&&atoi(argv[2])==1){
		Parse = 1;
	}
	strcpy(testfile,argv[1]);

	freopen(testfile,"r",stdin);
	if (stdin==NULL)
	{
		fprintf(stderr,"File %s Not Found\n",testfile);
		exit(1);
	}
	DfaBuild();
	LexInit();
	if(!Parse){
		TokenOut(); 
	}else{
		fprintf(stdout,"\nSyntax Tree:\n");
		PraseInit();
		ParseSyntax();
	}
	return 0;
}