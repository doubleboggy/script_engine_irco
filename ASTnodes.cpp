#include "ASTnodes.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ParseDriver.hpp"

using namespace std;

//map<string, int> variable_table;

// ///////////////////////////////////
AST_node::~AST_node() {
	// do nothing, I don't care what destructors drived are going to do
}
// ----
int AST_program::accept(Visitor &v)
{
//	cout << "AST_program::accept(Visitor &v) was called!" << endl;
	return v.visit(this);
}
AST_program::AST_program(AST_scene_list *sc_list)
{
//	cout << "AST_program::AST_program() was called with " << sc_list << endl;
	scene_list = sc_list;
}
AST_program::~AST_program()
{
	delete scene_list;
}
// ----
int AST_statement_list::accept(Visitor &v)
{
	if (v.driver->playing == true) {
		return v.visit(this);
	} else {
		return -1;
	}
}
void AST_statement_list::push_back(AST_statement *stmt)
{
	statement_list.push_back(stmt);
}
AST_statement_list::~AST_statement_list()
{
	for (auto it = statement_list.begin(); it != statement_list.end(); it++) {
		delete *it;
	}
	statement_list.clear();
}
// ----
int AST_cmd_print::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_print::AST_cmd_print(AST_expression *exp)
{
	expression = exp;
}
AST_cmd_print::~AST_cmd_print()
{
	delete expression;
}
// ----
int AST_cmd_if::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_if::AST_cmd_if(AST_expression *exp, AST_statement_list *stmt_then)
{
	expression = exp;
	statements_then = stmt_then;
}
AST_cmd_if::~AST_cmd_if()
{
	delete expression;
	delete statements_then;
}
// ----
int AST_cmd_ifelse::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_ifelse::AST_cmd_ifelse(AST_expression *exp, AST_statement_list *stmt_then, AST_statement_list *stmt_else)
{
	expression = exp;
	statements_then = stmt_then;
	statements_else = stmt_else;
}
AST_cmd_ifelse::~AST_cmd_ifelse()
{
	delete expression;
	delete statements_then;
	delete statements_else;
}
// ///////////////////////////////////
int AST_exp_numeric_literal::accept(Visitor &v)
{
	return v.visit(this);
}
AST_exp_numeric_literal::AST_exp_numeric_literal(int i)
{
	value = i;
}
// ----
int AST_exp_variable::accept(Visitor &v)
{
	return v.visit(this);
}
AST_exp_variable::AST_exp_variable(string *nm)
{
	variable_name = *nm;
}
// ----
int AST_exp_assignment::accept(Visitor &v)
{
	return v.visit(this);
}
AST_exp_assignment::AST_exp_assignment(AST_exp_variable *var, AST_expression *exp)
{
	variable = var;
	expression = exp;
}
AST_exp_assignment::~AST_exp_assignment()
{
	delete variable;
	delete expression;
}
// ----
int AST_exp_binary_operator::accept(Visitor &v)
{
	return v.visit(this);
}
AST_exp_binary_operator::AST_exp_binary_operator(AST_expression *exp1, int op, AST_expression *exp2)
{
	expression1 = exp1;
	operator_kind = op;
	expression2 = exp2;
}
AST_exp_binary_operator::~AST_exp_binary_operator()
{
	delete expression1;
	delete expression2;
}
//
//
////

int AST_scene_list::accept(Visitor &v)
{
	return v.visit(this);
}
void AST_scene_list::push_back(AST_scene *scene)
{
	scene_list.push_back(scene);
}
AST_scene_list::~AST_scene_list()
{
	for (auto it  = scene_list.begin(); it != scene_list.end(); it++) {
		delete *it;
	}
	scene_list.clear();
}
// ----
int AST_scene::accept(Visitor &v)
{
	return v.visit(this);
}
AST_scene::AST_scene(AST_scene_block_list *scene_blk_list)
{
	scene_block_list = scene_blk_list;
}
AST_scene::~AST_scene()
{
	delete scene_block_list;
}
// ----
int AST_cmd_switch_choice::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_switch_choice::AST_cmd_switch_choice(AST_cmd_choice_list *ch_list)
{
//	cout << "****AST_cmd_switch_choice::AST_cmd_switch_choice() was called!" << endl;
//	printf("choice_list = %p\n", choice_list);
	choice_list = ch_list;
}
AST_cmd_switch_choice::~AST_cmd_switch_choice()
{
	delete choice_list;
}
// ----
int AST_cmd_choice_list::accept(Visitor &v)
{
	return v.visit(this);
}
void AST_cmd_choice_list::push_back(AST_cmd_choice *choice)
{
	choice_list.push_back(choice);
}
AST_cmd_choice_list::~AST_cmd_choice_list()
{
	for (auto it = choice_list.begin(); it != choice_list.end(); it++) {
		delete *it;
	}
	choice_list.clear();
}
// ----
int AST_cmd_choice::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_choice::AST_cmd_choice(AST_parameter_list *pl, AST_statement_list *stmt_list)
{
	parameter_list = pl;
	statement_list = stmt_list;
}
AST_cmd_choice::~AST_cmd_choice()
{
	delete parameter_list;
	delete statement_list;
}
// ----
int AST_parameter_list::accept(Visitor &v)
{
	return v.visit(this);
}
AST_parameter_list::AST_parameter_list(string *str_ltrl, AST_expression *exp)
{
	string_choice = *str_ltrl;
	expression = exp;
}
AST_parameter_list::~AST_parameter_list()
{
	delete expression;
}
// ----
int AST_exp_string_literal::accept(Visitor &v)
{
	return v.visit(this);
}
AST_exp_string_literal::AST_exp_string_literal(string *str)
{
	val_string = *str;
}
// ---
int AST_cmd_string_message::accept(Visitor &v)
{
	return v.visit(this);
}
AST_cmd_string_message::AST_cmd_string_message(string *str_msg)
{
	string_message = *str_msg;
}
// ---
int AST_scene_block::accept(Visitor &v)
{
	return v.visit(this);
}
int AST_scene_block::get_type()
{
	return type;
}
AST_scene_block::AST_scene_block(int ty, AST_statement_list *stmt_list)
{
	type = ty;
	statement_list = stmt_list;
}
AST_scene_block::~AST_scene_block()
{
	delete statement_list;
}
// ---
int AST_scene_block_list::accept(Visitor &v)
{
	return v.visit(this);
}
void AST_scene_block_list::push_back(AST_scene_block *block)
{
	scene_blocks.push_back(block);
}
AST_scene_block_list::~AST_scene_block_list()
{
	for (auto it = scene_blocks.begin(); it != scene_blocks.end(); it++) {
		delete (*it);
	}
	scene_blocks.clear();
}
// ---
int AST_cmd_end::accept(Visitor &v)
{
	return v.visit(this);
}
// ---
int AST_cmd_show_variables::accept(Visitor &v)
{
	return v.visit(this);
}

//////////////////////////////////////
// visitor implemetation
// ----
Visitor::Visitor(ParseDriver *d)
{
	driver = d;
}
// ----
int Visitor::visit(AST_program *program)
{
//	cout << "AST_program: scene_list->accept()" << endl;
	return program->scene_list->accept(*this);
}
// ----
int Visitor::visit(AST_statement_list *statement_list) {
	for (auto it = statement_list->statement_list.begin(); it != statement_list->statement_list.end(); it++) {
		(*it)->accept(*this);
	}
	return 0;
}
// ----
int Visitor::visit(AST_cmd_print *cmd_print)
{
	int value = cmd_print->expression->accept(*this);
	printf("%d\n", value);
	return 0;
}
// ----
int Visitor::visit(AST_cmd_if *cmd_if)
{
	int value = cmd_if->expression->accept(*this);
	if (value != 0) {
		return cmd_if->statements_then->accept(*this);
	}
	return 0;
}
// ----
int Visitor::visit(AST_cmd_ifelse *cmd_ifelse)
{
	int value = cmd_ifelse->expression->accept(*this);
	if (value != 0) {
		return cmd_ifelse->statements_then->accept(*this);
	} else {
		return cmd_ifelse->statements_else->accept(*this);
	}
}
// ----
int Visitor::visit(AST_exp_variable *exp_variable)
{
	string name = exp_variable->variable_name;

	// search the name designated by exp_variable
//	auto it = variable_table.find(name);
	auto it = driver->variable_table.find(name);
//	if (it != variable_table.end()) {
	if (it != driver->variable_table.end()) {
		// returns the value of variable when found
		return it->second;
	}
	// returns 0 when the variable was not defined
	return 0;
}
// ----
int Visitor::visit(AST_exp_assignment *exp_assignment)
{
	string name = exp_assignment->variable->variable_name;
	int value = exp_assignment->expression->accept(*this);

	// save the value into variable_table
//	variable_table.insert(make_pair(name, value));
	driver->variable_table.insert(make_pair(name, value));
	// then returns the same value as a result of this *expression* 'assignment'
	return value;
}
// ----
int Visitor::visit(AST_exp_numeric_literal *exp_numeric_literal)
{
	// simply returns an integer value that stored in the numeric_literal instance
	return exp_numeric_literal->value;
}
// ----
int Visitor::visit(AST_exp_binary_operator *exp_binary_operaotor)
{
	// this function covers following all binary operators
	//  +, -, ==, !=
	const char *op_string[] = { "none", "+", "-", "==", "!=" };
	int ans = 0;
	int op_kind = AST_exp_binary_operator::none;
	int value1=0, value2=0;
	
	value1 = exp_binary_operaotor->expression1->accept(*this);
	value2 = exp_binary_operaotor->expression2->accept(*this);
	op_kind = exp_binary_operaotor->operator_kind;
	if (op_kind == AST_exp_binary_operator::add) {
		ans = value1 + value2;
	} else if (op_kind == AST_exp_binary_operator::subtract) {
		ans = value1 - value2;
	} else if (op_kind == AST_exp_binary_operator::equal) {
		ans = (value1 == value2) ? 1 : 0;
	} else if (op_kind == AST_exp_binary_operator::not_equal) {
		ans = (value1 != value2) ? 1 : 0;
	} else if (op_kind == AST_exp_binary_operator::logical_and) {
		ans = (value1 && value2) ? 1 : 0;
	} else if (op_kind == AST_exp_binary_operator::logical_or) {
		ans = (value1 || value2) ? 1 : 0;
	}

	return ans;
}
// ----
int Visitor::visit(AST_scene_list *slist)
{
	for (auto it = slist->scene_list.begin(); it != slist->scene_list.end(); it++) {
		if ((*it)->accept(*this) == -1) {
			return -1;
		}
	}

	return 0;
}
// ----
int Visitor::visit(AST_scene *scene)
{
	return scene->scene_block_list->accept(*this);
}
// ----
int Visitor::visit(AST_cmd_switch_choice *cmd_switch_choice)
{
	vector<AST_cmd_choice *> valid_choices;
	vector<AST_cmd_choice *> &clist = cmd_switch_choice->choice_list->choice_list;
	for (auto it = clist.begin(); it != clist.end(); it++) {
		AST_expression *condition = (*it)->parameter_list->expression;
		if (condition == 0) {
			// the choice has no condition
			valid_choices.push_back(*it);
		} else {
			// the choice has a condition
			if (condition->accept(*this) != 0) {
				valid_choices.push_back(*it);
			} else {
				/* doesn't make it validated (do nothing) */
			}
		}
	}
	int choice_number = driver->show_choices(valid_choices);
	AST_cmd_choice *theChoice = valid_choices.at(choice_number);
	return theChoice->accept(*this);
	/*
	int choice_number = driver->show_choices(cmd_switch_choice->choice_list);
	cout << "choosed : " << choice_number << endl;

	AST_cmd_choice *choice = cmd_switch_choice->choice_list->choice_list.at(choice_number);
//	AST_cmd_choice *choice = clist.at(choice_number);
	return choice->accept(*this);
	*/
}
// ----
int Visitor::visit(AST_cmd_choice_list *cmd_choice_list)
{
	return 0;	// could erase this member
}
// ----
int Visitor::visit(AST_cmd_choice *cmd_choice)
{
	return cmd_choice->statement_list->accept(*this);
}
// ----
int Visitor::visit(AST_parameter_list *parameter_list)
{
	return 0;	// could erase this member
}
// ----
int Visitor::visit(AST_exp_string_literal *exp_string_literal)
{
	return 0;	// could erase this member
}
int Visitor::visit(AST_cmd_string_message *string_message)
{
	return driver->show_message(string_message);
}
// ----
int Visitor::visit(AST_scene_block *scene_block)
{
	return scene_block->statement_list->accept(*this);
	//	   (AST_statement_list *     )->accept(*this);
}
// ----
int Visitor::visit_scene_block(int ty, AST_scene_block_list *blk_list)
{
	vector<AST_scene_block *>  &scene_blocks = blk_list->scene_blocks;
	for (auto it = scene_blocks.begin(); it != scene_blocks.end(); it++) {
		// type of it : AST_scene_block *
		if ((*it)->get_type() == ty) {
			return (*it)->accept(*this);
		}
	}
	// no scene_block was matched with 'ty'
	return -1;
}
// ----
int Visitor::visit(AST_scene_block_list * scene_block_list)
{
	cout << "assuming PRE block" << endl;
	if (visit_scene_block(AST_scene_block::pre, scene_block_list) == -1) {
		return -1;
	}
	cout << "assuming MAIN block" << endl;
	while (driver->playing) {
		if (visit_scene_block(AST_scene_block::main, scene_block_list) == -1) {
			return -1;
		}
	}
	cout << "assuming POST block" << endl;
	if (visit_scene_block(AST_scene_block::post, scene_block_list) == -1) {
		return -1;
	}
	return 0;
}
// ----
int Visitor::visit(AST_cmd_end *cmd_end) {
	driver->playing = false;
	return 0;
}
// ----
int Visitor::visit(AST_cmd_show_variables *cmd_show_variables) {
	for (auto it = driver->variable_table.begin(); it != driver->variable_table.end(); it++) {
		printf("%s = %d, ", it->first.c_str(), it->second);
	}
	printf("\n");
	return 0;
}













