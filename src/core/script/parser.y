%{
#include <cstdio>
#include <iostream>
#include "src/include/core/script/parser.h"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse(void* parser);
extern "C" FILE *yyin;
void yyerror(void* parser, const char *s);
%}
%parse-param {void* parser}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char* sval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <sval> WILDCARD_STRING
%token END ENDL
%token SECTION_HEADER 
%token RIGHT_CURLY_BRACE 
%token LEFT_CURLY_BRACE
%token RIGHT_CHEVRON
%token LEFT_CHEVRON
%token COLON
%token ADDRESS_IDENTIFIER

%%
link_script:
	sections { cout << "done with a linkerscript!" << endl; }
	;
sections:
	SECTION_HEADER RIGHT_CURLY_BRACE section_descriptions LEFT_CURLY_BRACE
	;
section_descriptions:
	section_descriptions section_description 
	| section_description
	;
section_description:
	ADDRESS_IDENTIFIER STRING  { ((N_Core::N_Parser::Parser*)parser)->set_base_address($2);cout << "new base address: " << $2 <<std::endl;  }
	| STRING COLON RIGHT_CURLY_BRACE section_identifications LEFT_CURLY_BRACE { ((N_Core::N_Parser::Parser*)parser)->set_segment_name($1);cout << "segment name: " << $1 <<std::endl;  }
	;
section_identifications:
	section_identifications section_identification
	| section_identification
	;
section_identification:
	WILDCARD_STRING { ((N_Core::N_Parser::Parser*)parser)->add_filter($1); cout << "to filter wildcard: " << $1 <<std::endl;  }
	;
ENDLS:
	ENDLS ENDL
	| ENDL 
	| ;
%%

void N_Core::N_Parser::Parser::parse(std::string const& path) 
{
	FILE *myfile = fopen(path.c_str(), "r");
	if (!myfile) 
  	{
    	throw std::invalid_argument("Cannot find the file");
	}
	
	yyin = myfile; // set flex to read from it instead of defaulting to STDIN:
	
	do 
  	{
		yyparse(this);
	} 
  	while (!feof(yyin));

}
void yyerror(void* parser, char const*s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}