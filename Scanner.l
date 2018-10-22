%{
#include <cstdlib>
#include <errno.h>
#include <limits.h>
#include <string>
#include "Scanner.hpp"

// a patch for flex'bug (ver2.5.31-2.5.33)
#undef yywrap
#define yywrap() 1

// yyterminate() returns 0 originally.
#define yyterminate() return token::END
#define YY_USER_ACTION yylloc->columns(yyleng);

std::string str_buffer;
%}

%option c++
%option yyclass="Scanner"
%option noyywrap nounput batch debug

%start _CCMT _CPPCMT _STR_MSG _STR_LITERAL

%%
%{
	yylloc->step();
	typedef yy::Parser::token token;
	typedef yy::Parser::token_type token_type;
%}
<INITIAL>[ \t]+		{ yylloc->step(); }
<INITIAL>[\n]		{ yylloc->lines(yyleng); yylloc->step(); }
<INITIAL>[0-9]+		{ yylval->ival = atoi(yytext); return token::NUMBER; }
<INITIAL>"true"		{ yylval->ival = 1; return token::NUMBER; }
<INITIAL>"false"	{ yylval->ival = 0; return token::NUMBER; }
<INITIAL>"scene"	{ return token::SCENE; }
<INITIAL>"=="		{ return token::EQUAL; }
<INITIAL>"!="		{ return token::NEQUAL; }
<INITIAL>"&&"		{ return token::LAND; }
<INITIAL>"||"		{ return token::LOR; }
<INITIAL>"print"	{ return token::PRINT; }
<INITIAL>"if"		{ return token::IF; }
<INITIAL>"else"		{ return token::ELSE; }
<INITIAL>"switch"	{ return token::SWITCH; }
<INITIAL>"choice"	{ return token::CHOICE; }
<INITIAL>"pre"		{ return token::PRE; }
<INITIAL>"post"		{ return token::POST; }
<INITIAL>"main"		{ return token::MAIN; }
<INITIAL>"end"		{ return token::END_GAME; }
<INITIAL>"@show_variables"	{ return token::SHOW_VARIABLES; }
<INITIAL>"//"		{ BEGIN _CPPCMT; }
<INITIAL>"/*"		{ BEGIN _CCMT; }
<INITIAL>"::"		{ str_buffer.clear(); BEGIN _STR_MSG; }
<INITIAL>"\""		{ str_buffer.clear(); BEGIN _STR_LITERAL; }
<INITIAL>$[a-zA-Z0-9][a-zA-Z0-9]*	{ yylval->sval = new std::string(yytext); return token::VARIABLE_NAME; }
<INITIAL>[a-zA-Z][a-zA-Z0-9]*		{ yylval->sval = new std::string(yytext); return token::IDENTIFIER; }
<INITIAL>[-+={}()!,]	{ return yy::Parser::token_type (yytext[0]); }

<_CPPCMT>\n	{ yylloc->lines(yyleng); BEGIN INITIAL; }
<_CPPCMT>.	{ }
<_CCMT>"*/"	{ BEGIN INITIAL; }
<_CCMT>[\n]	{ yylloc->lines(yyleng); }
<_CCMT>.	{ }
<_STR_MSG>\n { yylloc->lines(yyleng); BEGIN INITIAL; yylval->sval = new std::string(str_buffer); return token::STRING_MESSAGE; }
<_STR_MSG>.	{ str_buffer += yytext[0]; }
<_STR_LITERAL>"\""	{ yylval->sval = new std::string(str_buffer); BEGIN INITIAL; return token::STRING_LITERAL; }
<_STR_LITERAL>.		{ str_buffer += yytext[0]; }

<INITIAL>.			{ driver.error(*yylloc, "invalid charactor");
			  printf(" -->'%c(0x%x)'\n", yytext[0], yytext[0]);
}
%%














