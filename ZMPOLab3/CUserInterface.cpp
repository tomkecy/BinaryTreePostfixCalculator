#include "CUserInterface.h"

CUserInterface::CUserInterface()
{
	b_menu_loop_condition = false;
}//CUserInterface::CUserInterface()


CUserInterface::~CUserInterface()
{
	
}//CUserInterface::~CUserInterface()


void CUserInterface::vRun()
{
	b_menu_loop_condition = true;
	while(b_menu_loop_condition)
	{
		v_show_menu();
	}//while(b_menu_loop_condition)
}//void CUserInterface::vRun()

void CUserInterface::v_show_menu()
{
	int i_user_input;
	cout << endl << "MENU" << endl;
	cout << "1. Enter new expression" << endl;
	cout << "2. Evaluate expression" << endl;
	cout << "3. Print all variables" << endl;
	cout << "4. Set variables values" << endl;
	cout << "5. Print expression in infix form" << endl;
	cout << "6. Print expression in postfix form" << endl;
	cout << "7. Print tree inOrder" << endl;
	cout << "8. Swap test" << endl;
	cout << "0. Exit" << endl;

	i_user_input = i_get_user_input();

	switch(i_user_input)
	{
	case I_NUM1: 
		v_new_expression();
		break;
	case I_NUM2:
		v_evaluate_expression();
		break;
	case I_NUM3:
		v_print_variables();
		break;
	case I_NUM4:
		v_set_variable_value();
		break;
	case I_NUM5:
		v_print_infix_form();
		break;
	case I_NUM6:
		v_print_postfix_form();
		break;
	case I_NUM7:
		v_print_in_order();
		break;
	case I_NUM8:
		v_swap_test();
		break;
	case I_NUM0:
		b_menu_loop_condition = false;
		break;
	default:
		cout << "Wrong input!" << endl;
	}//switch(i_user_input)

}//void CUserInterface::v_show_menu()


int CUserInterface::i_get_user_input()
{
	int i_user_input;
	cin >> i_user_input;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "You have entered wrong input" << endl << "Try again: ";
		cin >> i_user_input;
	}//while (cin.fail())

	return i_user_input;
}//int CUI::i_get_user_input()

string CUserInterface::s_get_user_input()
{
	string s_user_input;
	getline(cin, s_user_input);
	getline(cin, s_user_input);

	while(s_user_input.empty())
	{
		cout << "You have entered wrond input" << endl << "Try again: " << endl;
		getline(cin, s_user_input);
	}//while(s_user_input.empty())
	return s_user_input;
}//string CUserInterface::s_get_user_input()

void CUserInterface::v_print_parsing_errors(vector<CTree::CErrorInfo> v_errors)
{
	if(!v_errors.empty())
	{
		cout << "Errors occured in expression in the following places: " << endl;
		for(int ii=0; ii<v_errors.size(); ii++)
		{
			if (v_errors[ii].iErrorIndex == 0)
			{
				cout << v_errors[ii].iPosition << " - " << "missing operand for " << v_errors[ii].sValue << " operation" << endl;
			}//if (v_errors[ii].iErrorIndex == 0)
			else
			{
				cout << v_errors[ii].iPosition << " - " << "missing operator for " << v_errors[ii].sValue << " operand" << endl;
			}//if (v_errors[ii].iErrorIndex == 0) else
		}//for(int ii=0; ii<v_errors.size(); ii++)

		cout << "Expression improvement were done" << endl << "New expression: " << c_tree.sGetPostfixExpression() << endl;
	}//if(!v_errors.empty())
	else//if(!v_errors.empty()) else
	{
		cout << "Successfully parsed an expression" << endl;
	}
}//void CUserInterface::v_print_parsing_errors(vector<CTree::SctErrorInformation> v_errors)

void CUserInterface::v_new_expression()
{
	cout << "Enter new expression: " << endl;
	string s_expression = s_get_user_input();
	vector<CTree::CErrorInfo> v_errors;
	bool b_incorrect_chars_occured;
	queue<string> c_expression_queue = c_expression_parser.pcParseExpression(s_expression, b_incorrect_chars_occured);

	if(b_incorrect_chars_occured)
	{
		cout << "Incorrect characters in expression were removed" << endl;
	}//if(b_incorrect_chars_occured)

	c_tree.vBuildTree(c_expression_queue, v_errors);

	v_print_parsing_errors(v_errors);

}//void CUserInterface::v_new_expression()

void CUserInterface::v_evaluate_expression()
{
	cout << c_tree.sGetPostfixExpression() << " = " << c_tree.dEvaluate() << endl;
}//void CUserInterface::v_evaluate_expression()

void CUserInterface::v_print_variables()
{
	cout << "Variables in current expression: " << endl;
	cout << c_tree.sGetVariables() << endl;
}//void CUserInterface::v_print_variables()

void CUserInterface::v_set_variable_value()
{
	string s_variable;
	int i_new_variable_value;
	cout << "Enter variable: ";
	cin >> s_variable;
	if (c_tree.bContainsVariable(s_variable))
	{
		cout << "Enter new value: ";
		i_new_variable_value = i_get_user_input();
		c_tree.bSetVariableValue(s_variable, i_new_variable_value);
	}//if(c_tree.bContainsVariable(s_variable))
	else
		cout << "Variable doesn't exist" << endl;

}//void CUserInterface::v_set_variable_value()


void CUserInterface::v_print_infix_form()
{
	cout << "Infix: " << c_tree.sGetInfixExpression() << endl;
}//v_print_infix_form();

void CUserInterface::v_print_postfix_form()
{
	cout << "Postfix: " << c_tree.sGetPostfixExpression() << endl;
}//void CUserInterface::v_print_postfix_form()

void CUserInterface::v_print_in_order()
{
	cout << "InOrder: " << c_tree.sInOrder() << endl;
}//void CUserInterface::v_print_in_order()

void CUserInterface::v_swap_test()
{
	c_tree.bSwapRootChildren();
}//void CUserInterface::v_swap_test()
