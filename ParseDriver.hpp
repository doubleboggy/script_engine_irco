#ifndef __PARSERDRIVER_H__
#define __PARSERDRIVER_H__

#include <string>
#include <map>
#include "Parser.hpp"
#include "Scanner.hpp"
#include "ASTnodes.hpp"

class ParseDriver {
public:
	bool playing;
	std::string file;
	std::map<std::string, int> variable_table;
	int result;
	bool trace_scanning;
	bool trace_parsing;
	AST_program *program;

	Scanner *scanner;

	ParseDriver();
	virtual ~ParseDriver();
	void error(const yy::location& l, const std::string& m);
	void run(const std::string& f);
	
	// APIs
	int show_message(AST_cmd_string_message *mes);
	int show_choices(vector<AST_cmd_choice *> &choices);
};

#endif





