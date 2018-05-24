%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parser.hpp"  // to get the token types that we return

%}
%%
(?i:sections)   { return SECTION_HEADER; }
"{" 		 { return RIGHT_CURLY_BRACE;}
"}" 		 { return LEFT_CURLY_BRACE;}
"<" 		 { return RIGHT_CHEVRON;}
">" 		 { return LEFT_CHEVRON;}
":"		     { return COLON;}


[ \t\n]+             ;
[\. =]+              { return ADDRESS_IDENTIFIER;}
[0-9]+\.[0-9]+       { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+               { yylval.ival = atoi(yytext); return INT; }
"*"?"("[.a-zA-Z0-9]+")""*"? { yylval.sval = strdup(yytext); return WILDCARD_STRING;}
[.a-zA-Z0-9]+        { yylval.sval = strdup(yytext); return STRING;}
.                    ;
%%