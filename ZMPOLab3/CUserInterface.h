#pragma once
#include <iostream>
#include "Constants.h"
#include "CExpressionParser.h"
#include "CTree.h"


using namespace std;

class CUserInterface
{
private:
	bool b_menu_loop_condition;

	CTree c_tree;
	CExpressionParser c_expression_parser;

	void v_show_menu();
	int i_get_user_input();

	string s_get_user_input();
	void v_print_parsing_errors(vector<CTree::CErrorInfo> v_errors);
	void v_new_expression();
	void v_evaluate_expression();
	void v_print_variables();
	void v_set_variable_value();
	void v_print_infix_form();
	void v_print_postfix_form();
	void v_print_in_order();
	void v_swap_test();
public:
	CUserInterface();
	~CUserInterface();

	void vRun();

	
	
};//class CUserInterface