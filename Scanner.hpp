#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include <iostream>
#include <fstream>
using namespace std;

#undef yyFlexLexer
#include <FlexLexer.h>

#include "Parser.hpp"

// Forward declarations (Scanner <-> ParseDriver）
class Scanner;
#include "ParseDriver.hpp"

// replace the declaration of yylex() by #undef & #(re)define
// following definition will be appear at the end of "Scanner.cpp" that will be created by  flex
// the original YY_DECL is undefined, and redefined by following declaration that has a new parameter 'ParserDriver& driver' at the 3rd position. this makes it possible to handle my original APIs(drowing pictures, playing sounds...) in ParserDriver in parsing phase.
#undef YY_DECL
#define YY_DECL \
yy::Parser::token_type \
	Scanner::scan(yy::Parser::semantic_type* yylval, \
				  yy::Parser::location_type* yylloc, \
				  ParseDriver& driver)

class Scanner : public yyFlexLexer {
public:
	Scanner(ifstream &ifs) : yyFlexLexer(&ifs) { }
	yy::Parser::token_type scan(yy::Parser::semantic_type* value,
								yy::Parser::location_type* location,
								ParseDriver& driver);
};

#endif




