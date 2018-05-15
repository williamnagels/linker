%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parser.hpp"  // to get the token types that we return

%}
%%
"Sections"   { return SECTION_HEADER; }
"{" 		 { return RIGHT_CURLY_BRACE;}
"}" 		 { return LEFT_CURLY_BRACE;}
"<" 		 { return RIGHT_CHEVRON;}
">" 		 { return LEFT_CHEVRON;}
":"		     { return COLON;}
[ \t\n]           ;
[0-9]+\.[0-9]+ { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+      {
	// we have to copy because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING;
}
.                ;
%%