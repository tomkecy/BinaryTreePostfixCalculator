#include "CTree.h"


CTree::CTree()
{
	pc_root = NULL;
	i_expression_queue_length = 0;
}//END of CTree constructor


CTree::~CTree()
{
	if (pc_root != NULL)
	{
		delete pc_root;
	}
}//END of ~CTree


void CTree::v_delete_existing_tree()
{
	if (pc_root != NULL)
	{
		delete pc_root;
		pc_root = NULL;
	}

	if(!c_variables.empty())
	{
		c_variables.clear();
	}
}//END of v_delete_existing_tree

void CTree::vBuildTree(queue <string> &pcExpressionQueue, vector<CErrorInfo> &pvErrorVector)
{
	v_delete_existing_tree();
	if (pcExpressionQueue.empty())
	{
		return;
	}

	i_expression_queue_length = pcExpressionQueue.size();
	pc_root = new CTreeNode(pcExpressionQueue.front(), NULL);
	pcExpressionQueue.pop();
	
	//current position in input queue
	int i_position = 0;
	
	if(!(b_is_digit(pc_root->s_value) || b_is_variable(pc_root->s_value)))
	{
		pc_root->pc_left_child = v_build_subtree(pcExpressionQueue, i_position, pvErrorVector, pc_root);
		pc_root->pc_right_child = v_build_subtree(pcExpressionQueue, i_position, pvErrorVector, pc_root);

	}

	v_check_parse_errors(pcExpressionQueue, pvErrorVector, pcExpressionQueue.size());

}//END of vBuildTree


/**
 * iErrorIndex:
 * 0 - missing operand
 * 1 - missing operator
 */
CTreeNode* CTree::v_build_subtree(queue<string> &pcExpressionQueue, int iPosition, vector<CErrorInfo> &pvErrorVector,CTreeNode *pcParentNode)
{
	CTreeNode *pc_result_node;

	//Create node
	if (pcExpressionQueue.empty())
	{
		pc_result_node = new CTreeNode("1", pcParentNode);
		CErrorInfo c_error;
		c_error.iErrorIndex = CErrorInfo::I_MISSING_OPERAND;
		c_error.iPosition = i_expression_queue_length - iPosition;
		c_error.sValue = pcParentNode->s_value;

		pvErrorVector.push_back(c_error);
		
	}
	else
	{
		pc_result_node = new CTreeNode(pcExpressionQueue.front(), pcParentNode);
		pcExpressionQueue.pop();
		iPosition++;
	}
	
	//New node contains number - finish building subtree
	if (b_is_number(pc_result_node->s_value))
	{
		return pc_result_node;
	}

	//New node contains variable - map value and finish building subtree
	if (b_is_variable(pc_result_node->s_value))
	{
		c_variables.insert(pair<string, int>(pc_result_node->s_value, I_DEFAULT_VARIABLE_VALUE));

		return pc_result_node;
	}//else if (b_is_letter(pcResultNode->s_value))

	//New node contains unary operator - build only left subtree
	if (b_is_unary_opeartor(pc_result_node -> s_value))
	{
		pc_result_node->pc_left_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
		pc_result_node->pc_right_child = NULL;
	}

	//New node contains binary operator - build both subtrees
	else if(b_is_binary_operator(pc_result_node -> s_value))
	{
		pc_result_node->pc_left_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
		pc_result_node->pc_right_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
	}//else if(b_is_binary_operator(pc_result_node -> s_value))

	return pc_result_node;

}//END OF v_build_subtree(queue<string>, CTreeNode)

bool CTree::b_is_digit(string &sToCheck)
{
	return (sToCheck[0] >= CH_0 && sToCheck[0] <= CH_9);
}//END of b_is_digit

bool CTree::b_is_digit(char cToCheck)
{
	return cToCheck >= CH_0 && cToCheck <= CH_9;
}//END of b_is_digit

bool CTree::b_is_number(string &sToCheck)
{
	bool b_only_numbers_occured = true;
	for(int i=0; i<sToCheck.length() && b_only_numbers_occured; i++)
	{
		if(!b_is_digit(sToCheck[i]))
		{
			b_only_numbers_occured = false;
		}
	}

	return b_only_numbers_occured;
}//END OF b_is_number

bool CTree::b_is_unary_opeartor(string &sToCheck) 
{
	return sToCheck[0] == CH_TYLDA;
}//END of b_is_unary_operator


bool CTree::b_is_binary_operator(string &sToCheck) 
{
	return sToCheck[0] == CH_PLUS || sToCheck[0] == CH_MINUS || sToCheck[0] == CH_MULTIPLY || sToCheck[0] == CH_DIVISION;
}//END of b_is_binary_operator

string CTree::sGetInOrderTraversal()
{
	string s_accumulator = "";

	if (pc_root != NULL)
	{
		v_build_in_order_traversal(pc_root, s_accumulator);
	}
	
	return s_accumulator;
}//END of sInOrder

void CTree::v_build_in_order_traversal(CTreeNode *pcTreeNode, string &sAccumulator)
{

	if (pcTreeNode->pc_left_child != NULL)
	{
		v_build_in_order_traversal(pcTreeNode->pc_left_child, sAccumulator);
	}

	sAccumulator += (pcTreeNode->s_value + " ");

	if (pcTreeNode->pc_right_child != NULL)
	{
		v_build_in_order_traversal(pcTreeNode->pc_right_child, sAccumulator);
	}

}//END of v_in_order_traversal


double CTree::dEvaluate()
{
	if (pc_root == NULL)
	{
		return 0;
	}
	
	return d_evaluate(pc_root);
}//END of dEvaluate


double CTree::d_evaluate(CTreeNode *pcTreeNode)
{
	//Return leaf value
	if (pcTreeNode->pc_left_child == NULL && pcTreeNode->pc_right_child == NULL)
	{
		if(b_is_variable(pcTreeNode->s_value))
		{
			return c_variables[pcTreeNode->s_value];
		}
		return stod(pcTreeNode->s_value);
	}

	//Evaluate binary operation
	if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child != NULL)
	{
		return d_evaluate_binary_operator(pcTreeNode);
	}//else if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child != NULL)

	//Evaluate unary operation
	if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child == NULL)
	{
		return -d_evaluate(pcTreeNode->pc_left_child);
	}

	return 0.0;
}//END of d_evaluate


double CTree::d_evaluate_binary_operator(CTreeNode* pcTreeNode)
{
	double d_result_accumulator = 0.0;
	double d_left_child_result = d_evaluate(pcTreeNode->pc_left_child);
	double d_right_child_result = d_evaluate(pcTreeNode->pc_right_child);
	char ch_operator = pcTreeNode->s_value[0];

	switch (ch_operator)
	{
	case CH_PLUS:
		d_result_accumulator = d_right_child_result + d_left_child_result;
		break;
	case CH_MINUS:
		d_result_accumulator = d_right_child_result - d_left_child_result;
		break;
	case CH_MULTIPLY:
		d_result_accumulator = d_right_child_result * d_left_child_result;
		break;
	case CH_DIVISION:
		d_result_accumulator = d_right_child_result / d_left_child_result;
		break;
	default:
		d_result_accumulator = d_left_child_result + d_right_child_result;
		break;
	}

	return d_result_accumulator;

}//END of d_evaluate_binary_operator

bool CTree::b_is_variable(string &sToCheck) 
{
	bool b_only_letters_occured = true;

	for (int ii = 0; ii < sToCheck.length() && b_only_letters_occured; ii++) 
	{
		char ch_current = sToCheck[ii];
		if (ch_current<CH_A || ch_current>CH_z || (ch_current>CH_Z && ch_current<CH_a)) 
		{
			b_only_letters_occured = false;
		}
	}//END of for loop

	return b_only_letters_occured;
}//END of b_is_variable


string CTree::sGetVariables()
{
	string s_result_accumulator = "";

	map<string, int>::iterator c_map_iterator = c_variables.begin();
	while(c_map_iterator != c_variables.end())
	{
		s_result_accumulator += (c_map_iterator->first + " = " + to_string(c_map_iterator->second) + '\n');
		++c_map_iterator;
	}
		
	return s_result_accumulator;

}//END of sGetVariables()

bool CTree::bSetVariableValue(string sVariable, int iNewValue)
{
	if (bContainsVariable(sVariable))
	{
		map<string, int>::iterator c_map_iterator = c_variables.find(sVariable);
		c_map_iterator->second = iNewValue;
		return true;
	}//if (bContainsVariable(sVariable))
	else
		return false;

}//END of sSetVariableValue

bool CTree::bContainsVariable(string sVariable)
{
	map<string, int>::iterator c_map_iterator = c_variables.find(sVariable);

	if (c_map_iterator != c_variables.end())
	{
		return true;
	}

	return false;
}//END of bContainsVariable(string sVariable)

string CTree::sGetInfixExpression()
{
	string s_result_accumulator = "";

	if(pc_root!= NULL)
	{
		if(pc_root->pc_right_child != NULL)
		{
			s_result_accumulator = s_get_infix_expression(pc_root->pc_right_child);
		}

		s_result_accumulator += pc_root->s_value;

		if (pc_root->pc_left_child != NULL)
		{
			s_result_accumulator += s_get_infix_expression(pc_root->pc_left_child);
		}
	}//END of if(pc_root!= NULL)

	return s_result_accumulator;
}//END of sGetInfixExpression()

string CTree::s_get_infix_expression(CTreeNode *pcTreeNode)
{
	string s_result_accumulator = "";

	if (b_is_digit(pcTreeNode->s_value) || b_is_variable(pcTreeNode->s_value))
	{
		return pcTreeNode->s_value;
	}
	else if (b_is_unary_opeartor(pcTreeNode->s_value))
	{
		s_result_accumulator = "(" + pcTreeNode->s_value + s_get_infix_expression(pcTreeNode->pc_left_child) + ")";
		return s_result_accumulator;

	}
	else if (b_is_binary_operator(pcTreeNode->s_value))
	{
		s_result_accumulator = "(" + s_get_infix_expression(pcTreeNode->pc_right_child) + pcTreeNode->s_value
			+ s_get_infix_expression(pcTreeNode->pc_left_child)  + ")";

		return s_result_accumulator;
	}

	return s_result_accumulator;
}//END of s_get_infix_expression(CTreeNode *pcTreeNode)

string CTree::sGetPostfixExpression()
{
	string s_result_accumulator = "";

	if (pc_root != NULL)
	{
		if (pc_root->pc_right_child != NULL)
		{
			s_result_accumulator += s_get_postfix_expression(pc_root->pc_right_child);
		}

		if (pc_root->pc_left_child != NULL)
		{
			s_result_accumulator += s_get_postfix_expression(pc_root->pc_left_child);
		}

		s_result_accumulator += pc_root->s_value;

	}//END OF if (pc_root != NULL)

	return s_result_accumulator;
	
}//END OF sGetPostfixExpression()

string CTree::s_get_postfix_expression(CTreeNode *pcTreeNode)
{
	string s_result_accumulator = "";

	if (b_is_digit(pcTreeNode->s_value) || b_is_variable(pcTreeNode->s_value))
	{
		return pcTreeNode->s_value + " ";
	}
	else if (b_is_unary_opeartor(pcTreeNode->s_value))
	{
		s_result_accumulator = s_get_postfix_expression(pcTreeNode->pc_left_child) + pcTreeNode->s_value + " ";
		return s_result_accumulator;

	}
	else if (b_is_binary_operator(pcTreeNode->s_value))
	{
		s_result_accumulator = s_get_postfix_expression(pcTreeNode->pc_right_child) + 
			s_get_postfix_expression(pcTreeNode->pc_left_child) + pcTreeNode->s_value + " ";
		return s_result_accumulator;
	}
	return s_result_accumulator;
}//END OF s_get_postfix_expression(CTreeNode *pcTreeNode)

void CTree::v_check_parse_errors(queue<string>& pcExpressionQueue, vector<CTree::CErrorInfo>& pvErrorVector, int iPosition)
{
	while (!pcExpressionQueue.empty())
	{
		CErrorInfo c_error;
		c_error.iPosition = iPosition--;
		if (b_is_digit(pcExpressionQueue.front()) || b_is_variable(pcExpressionQueue.front()))
		{
			c_error.iErrorIndex = CErrorInfo::I_MISSING_OPERATOR;
			c_error.sValue = pcExpressionQueue.front();
		}
		else
		{
			c_error.iErrorIndex = CErrorInfo::I_MISSING_OPERAND;
			c_error.sValue = pcExpressionQueue.front();
		}

		pvErrorVector.push_back(c_error);
		pcExpressionQueue.pop();
	}//END OF while loop

}//END OF v_check_parse_errors

bool CTree::bSwapNodes(CTreeNode *pcFirstNode, CTreeNode *pcSecondNode)
{
	if(pcFirstNode==NULL || pcSecondNode== NULL)
	{	
		return false;
	}

	CTreeNode *pc_first_node_parent = pcFirstNode->pc_parent;
	CTreeNode *pc_second_node_parent = pcSecondNode->pc_parent;
	bool is_first_node_left_child = is_left_child(pcFirstNode);
	bool is_second_node_left_child = is_left_child(pcSecondNode);

	if (is_first_node_left_child)
	{
		pc_first_node_parent->pc_left_child = pcSecondNode;
	}
	else
	{
		pc_first_node_parent->pc_right_child = pcSecondNode;
	}
	pcSecondNode->pc_parent = pc_first_node_parent;

	if (is_second_node_left_child)
	{
		pc_second_node_parent->pc_left_child = pcFirstNode;
	}
	else
	{
		pc_second_node_parent->pc_right_child = pcFirstNode;
	}
	pcFirstNode->pc_parent = pc_second_node_parent;

	return true;
	
}//END OF bSwapNodes(CTreeNode * pcFirstNode, CTreeNode * pcSecondNode)

bool CTree::bSwapRootChildren()
{
	return bSwapNodes(pc_root->pc_left_child, pc_root->pc_right_child);
}//END OF bSwapRootChildren()

bool CTree::is_left_child(CTreeNode* pcTreeNode)
{
	if (pcTreeNode->pc_parent->pc_left_child == pcTreeNode)
	{
		return true;
	}
	return false;
}//END OF is_left_child(CTreeNode* pcTreeNode)



