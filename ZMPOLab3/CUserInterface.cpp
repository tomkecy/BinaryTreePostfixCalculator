#include "CUserInterface.h"

CUserInterface::CUserInterface()
{
	b_menu_loop_condition = false;
}//END OF CUserInterface()


CUserInterface::~CUserInterface()
{
	
}//END OF ~CUserInterface()


void CUserInterface::vRun()
{
	b_menu_loop_condition = true;
	while(b_menu_loop_condition)
	{
		v_show_menu();
	}
}//END OF vRun()

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
	case I_ENTER_NEW_EXPRESSION: 
		v_new_expression();
		break;
	case I_EVALUATE_EXPRESSION:
		v_evaluate_expression();
		break;
	case I_PRINT_VARIABLES:
		v_print_variables();
		break;
	case I_SET_VARIABLES:
		v_set_variable_value();
		break;
	case I_PRINT_INFIX:
		v_print_infix_form();
		break;
	case I_PRINT_POSTFIX:
		v_print_postfix_form();
		break;
	case I_PRINT_IN_ORDER:
		v_print_in_order();
		break;
	case I_SWAP_TEST:
		v_swap_test();
		break;
	case I_EXIT:
		b_menu_loop_condition = false;
		break;
	default:
		cout << "Wrong input!" << endl;
	}//END OF switch(i_user_input)

}//END OF v_show_menu()


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
	}

	return i_user_input;
}//END OF i_get_user_input()

string CUserInterface::s_get_user_input()
{
	string s_user_input;
	getline(cin, s_user_input);
	getline(cin, s_user_input);

	while(s_user_input.empty())
	{
		cout << "You have entered wrond input" << endl << "Try again: " << endl;
		getline(cin, s_user_input);
	}
	return s_user_input;
}//END OF s_get_user_input()

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
			}
			else
			{
				cout << v_errors[ii].iPosition << " - " << "missing operator for " << v_errors[ii].sValue << " operand" << endl;
			}
		}//END OF for loop

		cout << "Expression improvement were done" << endl << "New expression: " << c_tree.sGetPostfixExpression() << endl;
	}//END OF if(!v_errors.empty())
	else
	{
		cout << "Successfully parsed an expression" << endl;
	}
}//END OF v_print_parsing_errors

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
	}

	c_tree.vBuildTree(c_expression_queue, v_errors);

	v_print_parsing_errors(v_errors);

}//END OF v_new_expression()

void CUserInterface::v_evaluate_expression()
{
	cout << c_tree.sGetPostfixExpression() << " = " << c_tree.dEvaluate() << endl;
}//END OF v_evaluate_expression()

void CUserInterface::v_print_variables()
{
	cout << "Variables in current expression: " << endl;
	cout << c_tree.sGetVariables() << endl;
}//END OF v_print_variables()

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
	}
	else
	{
		cout << "Variable doesn't exist" << endl;
	}
}//END OF v_set_variable_value()


void CUserInterface::v_print_infix_form()
{
	cout << "Infix: " << c_tree.sGetInfixExpression() << endl;
}//END OF v_print_infix_form();

void CUserInterface::v_print_postfix_form()
{
	cout << "Postfix: " << c_tree.sGetPostfixExpression() << endl;
}//END OF v_print_postfix_form()

void CUserInterface::v_print_in_order()
{
	cout << "InOrder: " << c_tree.sGetInOrderTraversal() << endl;
}//END OF v_print_in_order()

void CUserInterface::v_swap_test()
{
	c_tree.bSwapRootChildren();
}//END OF v_swap_test()
