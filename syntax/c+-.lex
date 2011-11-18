%{
#include <iostream>
#define SAVE_TOKEN yylval.str = new std::string(yytext, yyleng)
#define SAVE_CHAR_TOKEN yylval.ch = yytext[1]
#define SAVE_INT_TOKEN yylval.ival = atoi(yytext)

#include "c+-.tab.h"
using namespace std;
int line_num = 1;
%}
%option yylineno
%x COMMENTS
%%
[ \t]          ;
\n				{ ++line_num; }
-?[ \t]*[0-9]+  { SAVE_INT_TOKEN; return NUM; }


\/\*              { BEGIN(COMMENTS); }
<COMMENTS>\*\/    { BEGIN(INITIAL);  }
<COMMENTS>\n	  { ++line_num; }
<COMMENTS>.       ;

int              { return INT; }
extremeint       { return XINT; }
charlie          { return CHARLIE; }
for              { return FOR; }
forever          { return FOREVER; }
if               { return IF; }
else             { return ELSE; }
return           { return RETURN; }
(\'.\')		{ SAVE_CHAR_TOKEN; return CHAR; }
[a-z_A-Z][a-z_A-Z0-9]*    { SAVE_TOKEN; return ID;}

\+  { return PLUS; }
-   { return MINUS; }
\*  { return MULT; }
\/  { return DIV; } 
\<  { return SMALLER; }
>   { return GREATER;}
==  { return EQ;}
!=  { return DIFF;}
,   { return SEP;}
\(  { return '(';}
\)  { return ')';}
\[  { return '[';}
\]   { return ']';}
\{   { return '{';}
\}   { return '}';}
=   { return ATT; }
&&  { return AND; }
\!  { return FACT; }
\+\+ { return INC; }
\-\- { return DEC; }
\+\- { return OITOMIL; }
\:  { return COLON; }
\?  { return CHECK; }
;  { return ENDI;}

.  { cout << "Lexical error in line " << yylineno
		<< " with token "<< yytext << '.' << endl; }
%%
