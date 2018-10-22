#include <iostream>
#include <fstream>
using namespace std;

#include "ParseDriver.hpp"
#include "Parser.hpp"
#include "ASTnodes.hpp"

ParseDriver::ParseDriver()
	: trace_scanning(false), trace_parsing(false) {
		program = 0;
		playing = true;
}

ParseDriver::~ParseDriver() {
}

void ParseDriver::error(const yy::location& l,
						const std::string& m) {
	std::cerr << l << ": " << m << std::endl;
}

void ParseDriver::run(const std::string &f) {
	
	if (program != 0) {
		delete program;
		program = 0;
	}
	
	file = f;

	ifstream ifs;
	ifs.open(f.c_str(), ios::in);
	scanner = new Scanner(ifs);
	scanner->set_debug(trace_scanning);

	yy::Parser *parser = new yy::Parser(*this);
	parser->set_debug_level(trace_parsing);
	parser->parse();
	
//	cout << "parse was finished!" << endl;
	if (program != 0) {
//		cout << "let's visit program!" << endl;
		Visitor v(this);
		program->accept(v);
	} else {
		cout << "program was null!" << endl;
	}
	
	delete scanner;
	delete parser;
}


// ////////////////////////////////////
// APIs
int ParseDriver::show_message(AST_cmd_string_message *mes)
{
	cout << mes->string_message << endl;
	return 0;
}
int ParseDriver::show_choices(vector<AST_cmd_choice *> &choices)
{
	int choice_num;
	bool loop = true;
	while (loop) {
		AST_parameter_list *prams;
		int i = 1;
		// show choices
		for (auto it = choices.begin(); it != choices.end(); it++) {
			AST_cmd_choice *cur_choice = *it;
			cout << i << "." << cur_choice->parameter_list->string_choice << endl;
			i++;
		}
		// input user's choice
		cout << ">" << flush;
		cin >> choice_num;

		// if valid exit loop
		if (choice_num > 0 && choice_num < i) {
			loop = false;
		}
	}
	return choice_num - 1;
}















