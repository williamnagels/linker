%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parser.hpp"  // to get the token types that we return

%}
%%
(?i:sections) { return SECTION_HEADER; }
(?i:ENTRY)    { return ENTRY_POINT_IDENTIFIER; }
(?i:ALIGN)    { return ALIGN_KEYWORD; }
"{" 		  { return RIGHT_CURLY_BRACE;}
"}" 		  { return LEFT_CURLY_BRACE;}
"("           { return RIGHT_PARANTHESE;}
")"           { return LEFT_PARANTHESE;}
"<" 		  { return RIGHT_CHEVRON;}
">" 		  { return LEFT_CHEVRON;}
":"		      { return COLON;}
"*"           { return ASTERIX;}

[ \t\n]+             ;
[. =]+               { return ADDRESS_IDENTIFIER;}
[0-9]+\.[0-9]+       { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+               { yylval.ival = atoi(yytext); return INT; }
[_.a-zA-Z0-9]+       { yylval.sval = strdup(yytext); return STRING;}
.                    ;
%%