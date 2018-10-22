#ifndef __AST_NODES_HPP__
#define __AST_NODES_HPP__

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// ///////////////////////////////////
// the friend class of AST_cmd_choice_list
class ParseDriver;

// ///////////////////////////////////
// main implements for each node will be written in Visitor::visit()
class Visitor;

// ///////////////////////////////////
//
class AST_node;
class AST_program;
class AST_statement;		// no implement
class AST_statement_list;
class AST_command;			// no implement
class AST_cmd_print;
class AST_cmd_if;
class AST_cmd_ifelse;
class AST_expression;		// no implement
class AST_exp_variable;
class AST_exp_assignment;
class AST_exp_numeric_literal;
class AST_exp_binary_operator;
//
class AST_scene_list;
class AST_scene;
class AST_cmd_switch_choice;
class AST_cmd_choice_list;
class AST_cmd_choice;
class AST_parameter_list;
class AST_exp_string_literal;
class AST_cmd_string_message;
class AST_scene_block;
class AST_scene_block_list;
class AST_cmd_end;
//
class AST_cmd_show_variables;

// ///////////////////////////////////
class AST_node
{
protected:
	void delete_all_vector_members(vector<AST_node*> &vec);
public:
//	virtual void sayhello() { cout << "hello!" << endl; }
	// all children has same function 'accept()' to dispatch to certain process for certain type of node
	virtual int accept(Visitor &v) = 0;
	virtual ~AST_node();
};
// ----
// ///////////////////////////////////
class AST_program : public AST_node
{
	friend class Visitor;
	AST_scene_list *scene_list;
public:
	virtual int accept(Visitor &v);
	AST_program(AST_scene_list *scene_list);
	virtual ~AST_program();
};
// ///////////////////////////////////
class AST_statement : public AST_node
{
	// no implement
};
class AST_statement_list : public AST_node
{
	friend class Visitor;
	vector<AST_statement *> statement_list;
public:
	virtual int accept(Visitor &v);
	void push_back(AST_statement *stmt);
	virtual ~AST_statement_list();
};
// ///////////////////////////////////
class AST_command : public AST_statement
{
	// no implement
};
// ----
class AST_cmd_print : public AST_command
{
	friend class Visitor;
	AST_expression *expression;
public:
	virtual int accept(Visitor &v);
	AST_cmd_print(AST_expression *exp);
	virtual ~AST_cmd_print();
};
// ----
class AST_cmd_if : public AST_command
{
	friend class Visitor;
	AST_expression *expression;
	AST_statement_list *statements_then;
public:
	virtual int accept(Visitor &v);
	AST_cmd_if(AST_expression *exp, AST_statement_list *stmt_then);
	virtual ~AST_cmd_if();
};
// ----
class AST_cmd_ifelse : public AST_command
{
	friend class Visitor;
	AST_expression *expression;
	AST_statement_list *statements_then;
	AST_statement_list *statements_else;
public:
	virtual int accept(Visitor &v);
	AST_cmd_ifelse(AST_expression *exp, AST_statement_list *stmt_then, AST_statement_list *stmt_else);
	virtual ~AST_cmd_ifelse();
};

// ///////////////////////////////////
class AST_expression : public AST_statement
{
	// no implement
};
// ----
class AST_exp_numeric_literal : public AST_expression
{
	friend class Visitor;
	int value;
public:
	virtual int accept(Visitor &v);
	AST_exp_numeric_literal(int i);
};
// ----
class AST_exp_variable : public AST_expression
{
	friend class Visitor;
	string variable_name;
public:
	virtual int accept(Visitor &v);
	AST_exp_variable(string *nm);
};
// ----
class AST_exp_assignment : public AST_expression
{
	friend class Visitor;
	AST_exp_variable *variable;
	AST_expression *expression;
public:
	virtual int accept(Visitor &v);
	AST_exp_assignment(AST_exp_variable *var, AST_expression *exp);
	~AST_exp_assignment();
};
// ----
class AST_exp_binary_operator : public AST_expression
{
	friend class Visitor;
	AST_expression *expression1;
	int operator_kind;
	AST_expression *expression2;
public:
	enum op_type { none=0, add, subtract, equal, not_equal, logical_and, logical_or };
	virtual int accept(Visitor &v);
	AST_exp_binary_operator(AST_expression *exp1, int op, AST_expression *exp2);
	virtual ~AST_exp_binary_operator();
};
//
//
class AST_scene_list : public AST_node
{
	friend class Visitor;
	vector<AST_scene *> scene_list;
public:
	virtual int accept(Visitor &v);
	void push_back(AST_scene *scene);
	virtual ~AST_scene_list();
};
// ----
class AST_scene : public AST_node
{
	friend class Visitor;
	AST_scene_block_list *scene_block_list;
public:
	virtual int accept(Visitor &v);
	AST_scene(AST_scene_block_list *scene_block_list);
	virtual ~AST_scene();
};
// ----
class AST_cmd_switch_choice : public AST_command
{
	friend class Visitor;
	AST_cmd_choice_list *choice_list;
public:
	virtual int accept(Visitor &v);
	AST_cmd_switch_choice(AST_cmd_choice_list *choice_list);
	virtual ~AST_cmd_switch_choice();
	
};
// ----
class AST_cmd_choice_list : public AST_command
{
	friend class Visitor;
	friend class ParseDriver;	// to handle choices
	vector<AST_cmd_choice *> choice_list;
public:
	virtual int accept(Visitor &v);
	void push_back(AST_cmd_choice *choice);
	virtual ~AST_cmd_choice_list();
};
// ----
class AST_cmd_choice : public AST_command
{
	friend class Visitor;
	friend class ParseDriver;	// to handle choices
	AST_parameter_list *parameter_list;
	AST_statement_list *statement_list;
public:
	virtual int accept(Visitor &v);
	AST_cmd_choice(AST_parameter_list *pl, AST_statement_list *stmt_list);
	virtual ~AST_cmd_choice();
};
// ----
class AST_parameter_list : public AST_node
{
	friend class Visitor;
	friend class ParseDriver;	// to handle choices
	string string_choice;
	AST_expression *expression = 0;
public:
	virtual int accept(Visitor &v);
	AST_parameter_list(string *str_ltrl, AST_expression *exp = 0/*default*/);
	virtual ~AST_parameter_list();
};
// ----
class AST_exp_string_literal : public AST_expression
{
	friend class Visitor;
	string val_string;
public:
	virtual int accept(Visitor &v);
	AST_exp_string_literal(string *str);
};
// ----
class AST_cmd_string_message : public AST_command
{
	friend class Visitor;
	friend class ParseDriver;	// to handle string message
	string string_message;
public:
	virtual int accept(Visitor &v);
	AST_cmd_string_message(string *str_msg);
	
};
// ----
class AST_scene_block : public AST_node
{
	friend class Visitor;
	int type = none;
	AST_statement_list *statement_list;
public:
	enum { none=0, pre, post, main };
	virtual int accept(Visitor &v);
	int get_type();
	AST_scene_block(int ty, AST_statement_list *stmt_list);
	virtual ~AST_scene_block();
};
// ----
class AST_scene_block_list : public AST_node
{
	friend class Visitor;
	vector<AST_scene_block *> scene_blocks;
public:
	virtual int accept(Visitor &v);
	void push_back(AST_scene_block *block);
	virtual ~AST_scene_block_list();
};
// ----
class AST_cmd_end : public AST_command
{
	friend class Visitor;
public:
	virtual int accept(Visitor &v);
};
// ----
class AST_cmd_show_variables : public AST_command
{
	friend class Visitor;
public:
	virtual int accept(Visitor &v);
};

// ///////////////////////////////////
// ///////////////////////////////////
class Visitor
{
	int visit_scene_block(int ty, AST_scene_block_list *blk_list);
public:
	Visitor(ParseDriver *d);
	ParseDriver *driver;
public:
	int visit(AST_program *program);
	int visit(AST_statement_list *statement_list);
	int visit(AST_cmd_print *cmd_print);
	int visit(AST_cmd_if *cmd_if);
	int visit(AST_cmd_ifelse *cmd_ifelse);
	int visit(AST_exp_variable *exp_variable);
	int visit(AST_exp_assignment *exp_assignment);
	int visit(AST_exp_numeric_literal *exp_numeric_literal);
	int visit(AST_exp_binary_operator *exp_binary_operaotor);
	//
	int visit(AST_scene_list *scene_list);
	int visit(AST_scene *scene);
	int visit(AST_cmd_switch_choice *cmd_switch_choice);
	int visit(AST_cmd_choice_list *cmd_choice_list);
	int visit(AST_cmd_choice  *cmd_choice);
	int visit(AST_parameter_list *parameter_list);
	int visit(AST_exp_string_literal *exp_string_literal);
	int visit(AST_cmd_string_message *AST_cmd_string_message);
	int visit(AST_scene_block *scene_block);
	int visit(AST_scene_block_list * scene_block_list);
	int visit(AST_cmd_end *end);
	//
	int visit(AST_cmd_show_variables *show_variables);
};

#endif

