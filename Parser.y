%skeleton "lalr1.cc"
%require "2.1a"
%defines
%define parser_class_name { Parser }
%code requires {
	#include "ASTnodes.hpp"
	class ParseDriver;
}
%debug
%error-verbose
%parse-param { ParseDriver& driver }	// the 1st param of Parser's constructor
%lex-param	 { PaersDriver& driver }	// the 3rd param of yylex()
%locations
%initial-action
{
	// refer "location.hh", "position.hh"
	@$.begin.filename = @$.end.filename = &driver.file;
};

%union {
	int			 ival;
	std::string *sval;
	AST_program *program;
	AST_statement *statement;
	AST_statement_list *statement_list;
	AST_command *command;
	AST_expression *expression;
	//
	AST_scene_list *scene_list;
	AST_scene *scene;
	AST_cmd_choice_list *choice_list;
	AST_cmd_choice *choice;
	AST_parameter_list *param_list;
	AST_scene_block *scene_block;
	AST_scene_block_list *scene_block_list;
	AST_cmd_show_variables *show_variables;
};

// -- move to later part
%type <program> program "program"
%type <statement_list> statement_list "statement_list"
%type <statement> statement "statement"
%type <command> command "command"
%type <expression> expression "expression"
// --
%type <scene_list> scene_list "scene_list"
%type <scene> scene "scene"
%type <choice_list> choice_list "choice list"
%type <choice> choice "choice"
%type <param_list> param_list "param_list"
%type <scene_block> scene_block "scene block"
%type <scene_block_list> scene_block_list "scene block list"

%{

#include "ParseDriver.hpp"			// not to forget to include it here
// new yylex() function for Parser
yy::Parser::token_type
yylex(yy::Parser::semantic_type* yylval,
yy::Parser::location_type* yylloc,
ParseDriver& driver) {
	return driver.scanner->scan(yylval, yylloc, driver);
}

// bison's spec: it's our duty to implement error routine
void yy::Parser::error(const yy::Parser::location_type& l, const std::string& m) {
	driver.error(l, m);
}
typedef AST_exp_binary_operator::op_type op_type;
%}

%token			END 0			"end of file"
%token <sval>	VARIABLE_NAME	"variable name"
%token <ival>	NUMBER			"number"
%token IF ELSE PRINT '{' '}' SWITCH CHOICE SCENE PRE MAIN POST END_GAME
%token SYS_SHOW_VARIABLES SYS_BRIGHTNESS SYS_LOAD_BACKGROUND_IMAGE SYS_LOAD_MUSIC
%token SYS_PLAY_MUSIC SYS_WAIT SYS_FADE_IN_BRIGHTNESS SYS_FADE_OUT_MUSIC SYS_FADE_OUT_BRIGHTNESS
%token SYS_PLAY_SOUND SYS_MOVE_SCENE
%token <sval>	STRING_MESSAGE	"string message"
%token <sval>	STRING_LITERAL	"string literal"
%token <sval>	IDENTIFIER		"identifier"
%right '='
%left  LAND LOR
%left  EQUAL NEQUAL
%left  '+'
%left  '-'

%printer	{ debug_stream() << *$$; } "variable name"
%destructor	{ delete $$; } "variable name" "string message" "string literal"
%printer	{ debug_stream() << $$; } "number"

%%

program			: scene_list { $$ = new AST_program($1); driver.program = $$; }
;
scene_list		: scene_list scene { $$->push_back($2); }
				| scene { $$ = new AST_scene_list(); $$->push_back($1); }
;
scene			: SCENE IDENTIFIER '{' scene_block_list '}' { $$ = new AST_scene($4); }
;
scene_block_list: scene_block_list scene_block { $$->push_back($2); }
				| scene_block { $$ = new AST_scene_block_list(); $$->push_back($1); }
;
scene_block		: PRE  '{' statement_list '}' { $$ = new AST_scene_block(AST_scene_block::pre, $3);  }
				| POST '{' statement_list '}' { $$ = new AST_scene_block(AST_scene_block::post, $3); }
				| MAIN '{' statement_list '}' { $$ = new AST_scene_block(AST_scene_block::main, $3); }
;
statement_list	: statement_list statement { $$->push_back($2); }
				| statement { $$ = new AST_statement_list(); $$->push_back($1); }
				| /* empty */ { $$ = new AST_statement_list(); }
;
statement		: command { $$ = $1; }
				| expression { $$ = $1; }
;
command			: PRINT expression { $$ = new AST_cmd_print($2); }
				| IF expression '{' statement_list '}' { $$ = new AST_cmd_if($2, $4); }
				| IF expression '{' statement_list '}' ELSE '{' statement_list '}' { $$ = new AST_cmd_ifelse($2, $4, $8); }
				| STRING_MESSAGE { $$ = new AST_cmd_string_message($1); }
				| SWITCH choice_list { $$ = new AST_cmd_switch_choice($2); }
				| END_GAME { $$ = new AST_cmd_end(); }
				| SYS_SHOW_VARIABLES { $$ = new AST_cmd_show_variables(); }
				| SYS_BRIGHTNESS '(' expression ')' { $$ = new AST_cmd_brightness($3); }
				| SYS_LOAD_BACKGROUND_IMAGE '(' STRING_LITERAL ')' { $$ = new AST_cmd_load_background_image($3); }
				| SYS_LOAD_MUSIC '(' STRING_LITERAL ')'  { $$ = new AST_cmd_load_music($3); }
				| SYS_PLAY_MUSIC '(' STRING_LITERAL ')'  { $$ = new AST_cmd_play_music(); }
				| SYS_WAIT '(' expression ')' { $$ = new AST_cmd_wait($3); }
				| SYS_FADE_IN_BRIGHTNESS '(' experession ',' experession ')' { $$ = new AST_cmd_fade_in_brightness($3, $5); }
				| SYS_FADE_OUT_MUSIC '(' experession ',' experession ')'  { $$ = new AST_cmd_fade_out_music($3, $5); }
				| SYS_FADE_OUT_BRIGHTNESS '(' experession ',' experession ')' { $$ = new AST_cmd_fade_out_brightness($3, $5); }
				| SYS_PLAY_SOUND '(' STRING_LITERAL ')'  { $$ = new AST_cmd_play_sound($3); }
				| SYS_MOVE_SCENE '(' IDENTIFIER ')' { $$ = new AST_cmd_move_scene($3); }
;
choice_list		: choice_list choice { $$->push_back($2); }
				| choice { $$ = new AST_cmd_choice_list(); $$->push_back($1);}
;
choice			: CHOICE '(' param_list ')' '{' statement_list '}' { $$ = new AST_cmd_choice($3, $6); }
;
param_list		: STRING_LITERAL { $$ = new AST_parameter_list($1); }
				| STRING_LITERAL ',' expression { $$ = new AST_parameter_list($1, $3); }
;
expression		: NUMBER { $$ = new AST_exp_numeric_literal($1); }
				| VARIABLE_NAME { $$ = new AST_exp_variable($1); }
				| VARIABLE_NAME '=' expression { AST_exp_variable *var = new AST_exp_variable($1);$$ = new AST_exp_assignment(var, $3); }
				| expression '+'	expression { $$ = new AST_exp_binary_operator($1, op_type::add, $3); }
				| expression '-'	expression { $$ = new AST_exp_binary_operator($1, op_type::subtract, $3); }
				| expression EQUAL	expression { $$ = new AST_exp_binary_operator($1, op_type::equal, $3); }
				| expression NEQUAL	expression { $$ = new AST_exp_binary_operator($1, op_type::not_equal, $3); }
				| expression LAND	expression { $$ = new AST_exp_binary_operator($1, op_type::logical_and, $3); }
				| expression LOR	expression { $$ = new AST_exp_binary_operator($1, op_type::logical_or,  $3); }
				| '(' expression ')' { $$ = $2; }
;
%%





















