%{
#include <cstdio>
#include <iostream>
#include "src/include/core/script/parser.h"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
 
void yyerror(const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char *sval;
}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token END ENDL
%token SECTION_HEADER 
%token RIGHT_CURLY_BRACE 
%token LEFT_CURLY_BRACE
%token RIGHT_CHEVRON
%token LEFT_CHEVRON
%token COLON
%token ADDRESS_IDENTIFIER
//RIGHT_CURLY_BRACE ENDLS section_descriptions ENDLS LEFT_CURLY_BRACE ENDLS
//RIGHT_CHEVRON STRING LEFT_CHEVRON COLON RIGHT_CHEVRON INT LEFT_CHEVRON RIGHT_CURLY_BRACE ENDLS section_identifications ENDLS LEFT_CURLY_BRACE { cout << "new section description name=" << $2 << " and aligment=" << $6 << std::endl; }
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
	ADDRESS_IDENTIFIER STRING  { cout << "new section name" << $2 <<std::endl;  }
	| STRING COLON RIGHT_CURLY_BRACE section_identifications LEFT_CURLY_BRACE { cout << "whiskey" << $1 <<std::endl;  }
	;
section_identifications:
	section_identifications section_identification
	| section_identification
	;
section_identification:
	 STRING { cout << "new section description" << $1 <<std::endl;  }
	;
ENDLS:
	ENDLS ENDL
	| ENDL 
	| ;
%%

int N_Core::N_Parser::Parser::parse() 
{
	// open a file handle to a particular file:
	FILE *myfile = fopen("testfiles/simple_script", "r");
	// make sure it is valid:
	if (!myfile) {
		cout << "Cannot find the file" << endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}