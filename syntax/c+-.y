%{
#include <cstdio>
#include <iostream>
#include "../include/parse_tree/all_nodes.h"
#include "../include/semantic_analyzer.h"
#include "../include/scope.h"
using namespace std;

int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno; 
void yyerror(char *s);
Node* root;
Symbol_table* st;
Scope_stack* scope;

%}
%locations /* To use yylloc */

/*Safely ignoring the if/else conflict
 (else will always be attached to the innermost if)*/
%expect 1

%code requires{
	#include "../include/parse_tree/all_nodes.h"
}

%union {
	int ival;
	long long lval;
	char cval;
    bool bval;
    Node* tnode;
	Expression* expression;
	Vars_declarations* vars_declaration;
	Parameter_list* params;
	Var* var;
	Arguments* arg;
	Type* type;
    std::string* str;
	char ch;
	t_entry* tentry;
}

%error-verbose
%token <str> INT 
%token <str> XINT 
%token <str> CHARLIE
%token <ival> NUM
%token FOR FOREVER
%token IF ELSE
%token RETURN
%token <ch> CHAR
%token <str> ID
%token <ival> PLUS MINUS MULT DIV 
%token <ival> SMALLER GREATER EQ DIFF AND
%token SEP ENDI
%token ATT
%token CHECK COLON
%token FACT INC DEC OITOMIL

%type <tnode> cmm declarations declaration var_decl
%type <tnode> fun_decl 
%type <tnode> ID_list block command selection_cmd
%type <tnode> cmd_list cmd_list_center iteration_cmd return_cmd 
%type <tnode> var_eq exp_num_var 
%type <tnode> attribution

%type <vars_declaration> var_decls
%type <arg> arguments 
%type <expression> sum_exp mult_exp factor expression ternary_operator
%type <expression> func_call 
%type <ival> relational_op 
%type <ival> mult_op sum_op 
%type <type> type
%type <var> var IDs parameter 
%type <params> param_list 

%start cmm
%%
cmm:
	declarations {
		root = new Node($1);
		cout<< "/------------------------\\"<<endl;
		cout<< "| Creating the root node |" <<endl;
		cout<< "\\------------------------/"<<endl;
	}
	|/* empty program */ {root = new Node();}
;

declarations:
	declaration {
		$$ = new Declarations($1);
	}
	| declarations declaration {
		$1->add_child($2); 
	}
;

declaration:
	fun_decl {
		$$ = ($1);
		//scope->push_same_level($$->get_entry());
		//$$->check_scope(scope);
	}
	| var_decl /* Global vars */{
		$$ = ($1);
		//$$->check_scope(scope);
	}
;
var_decl:
        type ID_list ENDI {
			$$ = new Var_declaration((Type*)$1, (Id_list*)$2);
		}
;
ID_list:
       ID_list SEP IDs { $1->add_child($3); }
       | IDs {
			$$ = new Id_list((Var*)$1); //Id_list() is used only here!
		}
;
IDs:
	ID {
    	$$ = new Var(*$1);
		$$->set_line(yylineno);
	}
    |ID '[' NUM ']' { 
	    $$ = new Array(*$1, $3);
		$$->set_line(yylineno);
	}
;	

type:
	XINT {
		$$ = new Type(LVAL);
	}
	| INT {
		$$ = new Type(IVAL);
	}
    | CHARLIE {
		$$ = new Type(CVAL);
	}
;

fun_decl:
	type ID '(' param_list ')' block {
        $$ = new Function($1, $4, $6);
		((Function*)$$)->set_id(*$2);
		((Function*)$$)->set_line(yylineno);
		cout<<"Function "<< *$2 <<" parsed!"<<endl;
    }
    | type ID '(' error ')' block {
		cout<<"Error while parsing parameters for "<< *$2 <<endl;
        $$ = new Function ($1, (Parameter_list*)new Null_node(), $6);
		((Function*)$$)->set_id(*$2);
		((Function*)$$)->set_line(yylineno);
    }
;

param_list:
	param_list SEP parameter { $1->add_child($3); }
	| parameter {$$ = new Parameter_list($1); }
	| {$$ = new Parameter_list();}
;

parameter:
	type ID {
		$$ = new Parameter((Type*)$1, *$2);
		$$->set_line(yylineno);
}
	| type ID '[' NUM ']' {
		$$ = new Array_parameter($1, *$2, $4);
		$$->set_line(yylineno);
}
;
block:
	'{' var_decls cmd_list_center '}' {
		$$ = new Block($2, $3);
	}
    | error '}' {}
;

var_decls: /* Used only in block and cmd_list */
	var_decls var_decl { $1->add_child($2); }
	| /* empty */
	{
		$$ = new Vars_declarations();
	}
;			

command:
	attribution ENDI {$$ = ($1);}
	| selection_cmd	{$$ = ($1);}
	| iteration_cmd {$$ = ($1);}
	| return_cmd ENDI {$$ = ($1);}
	| func_call ENDI {$$ = ($1);}
	| ENDI {$$ = new Null_node();}
    | error ENDI {$$ = new Null_node();
#ifdef DEBUG
		cout << "Command error" << endl;
#endif
	}
;  

selection_cmd:
	IF '(' expression ')' cmd_list {$$ = new Selection($3, $5);}
	| IF '(' expression ')' cmd_list ELSE cmd_list {$$ = new Selection($3, $5, $7);}
    | error ELSE cmd_list {$$ = new Null_node();}
;

cmd_list:
	command {$$ = new Cmd_list($1);}
	| '{' var_decls cmd_list_center '}' {$$ = new Block($2, $3);}
    | error '}' {$$=new Null_node();}
;

cmd_list_center:
	cmd_list_center command {$1->add_child($2);}
	| command {
		$$ = new Node($1);
	}
;
iteration_cmd:
	FOREVER cmd_list {$$ = new Iteration($2);}
	| FOR var cmd_list {$$ = new Iteration($2, $3);}
	| FOR '(' var ')' cmd_list {$$ = new Iteration($3, $5);}
	| FOR NUM cmd_list {
		$$ = new Iteration(new Const_num($2), $3);
	}
	| FOR '(' attribution ENDI expression ENDI attribution ')' cmd_list {
		$$ = new Iteration($3, $5, $7, $9);
	}
	| FOR '(' ENDI expression ENDI attribution ')' cmd_list { //for(;exp;++i)
		$$ = new Iteration(new Null_node(), $4, $6, $8);
	}
	| FOR '(' attribution ENDI expression ENDI ')' cmd_list { //for(i=0;exp;)
		$$ = new Iteration($3, $5, new Null_node(), $8);
	}
	| FOR '(' ENDI expression ENDI ')' cmd_list { //for(;exp;)
		$$ = new Iteration(new Null_node(), $4, new Null_node(), $7);
	}
    | FOR error cmd_list {$$ = new Iteration(new Null_node(), $3);}
;

return_cmd:
	RETURN {
		$$ = new Return();
	}
	| RETURN expression	{
		$$ = new Return($2);
	}
;

attribution:
	var_eq expression {
		$$ = new Attribution($1, $2);
	}
	| var_eq CHAR {
		$$ = new Attribution($1, new Const_char($2));
	}
	| var INC {
		$$ = new Attribution($1, INCR);
	}
	| var DEC {
		$$ = new Attribution($1, DECR);
	}
	| var OITOMIL {
		$$ = new Attribution($1, OITOKR);
	}
	| INC var {
		$$ = new Attribution($2, INCL);
	}
	| DEC var {
		$$ = new Attribution($2, DECL);
	}
	| OITOMIL var {
		$$ = new Attribution($2, OITOKL);
	}
;

var_eq:
	var ATT {$$ = new Lvalue_list($1);}
	| var_eq var ATT {$1->add_child($2);}
;

expression:
	sum_exp {
		$$ = ($1);
	}
	| sum_exp relational_op sum_exp	{
		$$ = new Relational_expression($1, $3, $2);//yes, 132.
	}
	| ternary_operator {$$ = ($1);}
    | error ')' {$$ = (Expression*) new Null_node();}
    | error ENDI {$$ = (Expression*) new Null_node();}
;

ternary_operator:
	exp_num_var CHECK exp_num_var COLON exp_num_var {
		$$ = new Ternary_operator($1, $3, $5);
	}
    | error COLON exp_num_var {$$ = (Expression*) new Null_node();}
;

exp_num_var:
	NUM {
		$$ = new Const_num($1);
	}
    |var {$$ = $1;}
    |'(' expression ')'{$$ = $2;}
;

var:
	ID
	{
		//$$ = (Var*)st->get_node(*$1);
		$$ = new Var(*$1);
		$$->set_line(yylineno);
	}
    |ID '[' var ']' 
	{
		//$$ = (Var*)st->get_node(*$1);
		$$ = new Array(*$1);
		$$->set_line(yylineno);
		try{
			$$->set_indexer($3);
		}
		catch(const char* problem){
			cout << problem << endl;
		}
	}
    |ID '[' NUM ']'
	{
		//$$ = (Var*)st->get_node(*$1);
		$$ = new Array(*$1);
		$$->set_line(yylineno);
		try{
			$$->set_indexer($3);
		}
		catch(const char* problem){
			cout << problem << endl;
		}

	}
;

relational_op:
	GREATER {
		$$ = $1;
	}
    |SMALLER {
		$$ = $1;
	}
    |EQ {
		$$ = $1;
	}
    |DIFF {
		$$ = $1;
	}
    |AND {
		$$ = $1;
	}
;

sum_exp:
	mult_exp {
		$$ = $1;
	}
	| sum_exp sum_op mult_exp {
		if($2 == 1)
			$$ = new Sum($1, $3);
		else if ($2 == 2)
			$$ = new Subtraction($1, $3);
		else{
#ifdef DEBUG
			cout << "Error evaluating sum expression (c+-.y : sum_exp)" << endl;
#endif
		}
	}
;
sum_op:
	PLUS {$$ = 1;}
    |MINUS {$$ = 2};
;
mult_exp:
	mult_exp mult_op factor {
		if($2==1)
			$$ = new Multiplication($1, $3);
		else if($2==2)
			$$ = new Division($1, $3);
		else
#ifdef DEBUG
			cout << "Error evaluating multiplication expression" << endl;
#endif
	}
	| factor {$$ = $1;}
;
mult_op:
	MULT{$$ = 1;}
	|DIV{$$ = 2;}
;
// note: FACT stands for FACTORIAL
factor:
	'(' expression ')' {$$ = new Factor($2);}
	| var {$$ = new Factor($1);}
	| func_call {$$ = new Factor($1);}
	| NUM {
#ifdef DEBUG
	cout << "NUM FACTOR! (*y:factor) " << $1 << endl;
#endif
		Const_num* cn = new Const_num($1);
		$$ = new Factor(cn);
	}
	| '(' expression ')' FACT {
		$$ = new Factorial($2);
	}
	| var FACT{
		$$ = new Factorial($1);
	}
	| func_call FACT{
		$$ = new Factorial($1);
	}
	| NUM FACT {
		$$ = new Factorial(new Const_num($1));
	}
;

func_call:
	ID '(' arguments ')'{ 
		$$ = (Expression*)st->get_node(*$1);
		if($$->get_type() != NOTYPE)
			$$ = (Expression*) new Function_call($3, (*$1));
		else
			$$ = (Expression*) new Null_node();
	}
;

arguments:
	expression {$$ = new Arguments($1);}
	| arguments SEP expression {$1->add_child($3);}
	| {$$ = new Arguments();}
;
%%


main(int argc, char** argv) {
	
	// open a file handle to a particular file:
	FILE *myfile = fopen(argc>1 ? argv[1] : "", "r");
	// make sure it's valid:
	if (!myfile) {
		cout << "I can't open file!" << endl;
		return -1;
	}
	st = new Symbol_table();
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;

	do {
		yyparse();
	} while (!feof(yyin));
	
	delete scope;
	Semantic_analyzer* sa = new Semantic_analyzer(root, st);
	sa->analyze();
}

void yyerror(char *s) {
	cout << "Parse error in line " << yylineno << "! Message: " << s << endl;
	// might as well halt now:
	// exit(-1);
}
