#include "CTree.h"


CTree::CTree()
{
	pc_root = NULL;
	i_expression_queue_length = 0;
}//CTree::CTree(queue<string>)


CTree::~CTree()
{
	if (pc_root != NULL)
	{
		delete pc_root;
	}//if (pc_root != NULL)
}//CTree::~CTree()


void CTree::vBuildTree(queue <string> &pcExpressionQueue, vector<CErrorInfo> &pvErrorVector)
{
	if (pc_root != NULL)
	{
		delete pc_root;
		pc_root = NULL;
	}//if (pc_root != NULL)

	if(!c_variables.empty())
	{
		c_variables.clear();
	}//if(!c_variables.empty)

	if (pcExpressionQueue.empty())
	{
		return;
	}//if (&pcExpressionQueue.empty())

	i_expression_queue_length = pcExpressionQueue.size();
	pc_root = new CTreeNode(pcExpressionQueue.front(), NULL);
	pcExpressionQueue.pop();
	
	//current position in input queue
	int i_position = I_NUM0;
	
	if(!(b_is_digit(pc_root->s_value) || b_is_variable(pc_root->s_value)))
	{
		pc_root->pc_left_child = v_build_subtree(pcExpressionQueue, i_position, pvErrorVector, pc_root);
		pc_root->pc_right_child = v_build_subtree(pcExpressionQueue, i_position, pvErrorVector, pc_root);

	}//if(!(b_is_digit(pc_root->s_value) || b_is_variable(pc_root->s_value)))

	v_check_parse_errors(pcExpressionQueue, pvErrorVector, pcExpressionQueue.size());

}//void CTree::vBuildTree()


/**
 * iErrorIndex:
 * 0 - missing operand
 * 1 - missing operator
 */
CTreeNode* CTree::v_build_subtree(queue<string> &pcExpressionQueue, int iPosition, vector<CErrorInfo> &pvErrorVector,CTreeNode *pcParentNode)
{
	CTreeNode *pc_result_node;

	if (pcExpressionQueue.empty())
	{
		pc_result_node = new CTreeNode("1", pcParentNode);
		CErrorInfo sct_error;
		sct_error.iErrorIndex = I_NUM0;
		sct_error.iPosition = i_expression_queue_length - iPosition;
		sct_error.sValue = pcParentNode->s_value;

		pvErrorVector.push_back(sct_error);
		
	}//if (pcExpressionQueue.empty())
	else
	{
		pc_result_node = new CTreeNode(pcExpressionQueue.front(), pcParentNode);
		pcExpressionQueue.pop();
		iPosition++;
	}//if (pcExpressionQueue.empty()) else
	
	if (b_is_digit(pc_result_node->s_value))
	{
		return pc_result_node;
	} //if (b_is_digit(pcResultNode->s_value))

	else if (b_is_variable(pc_result_node->s_value))
	{
		c_variables.insert(pair<string, int>(pc_result_node->s_value, I_DEFAULT_VARIABLE_VALUE));

		return pc_result_node;
	}//else if (b_is_letter(pcResultNode->s_value))

	else if (b_is_unary_opeartor(pc_result_node -> s_value))
	{
		pc_result_node->pc_left_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
		pc_result_node->pc_right_child = NULL;
	}//else if (b_is_unary_operator(pcResultNode->s_value))

	else if(b_is_binary_operator(pc_result_node -> s_value))
	{
		pc_result_node->pc_left_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
		pc_result_node->pc_right_child = v_build_subtree(pcExpressionQueue, iPosition, pvErrorVector, pc_result_node);
	}//else if(b_is_binary_operator(pc_result_node -> s_value))

	return pc_result_node;

}//void CTree::v_build_subtree(queue<string>, CTreeNode)

bool CTree::b_is_digit(string &sToCheck)
{
	return (sToCheck[0] >= CH_0 && sToCheck[0] <= CH_9);
}//bool CTree::b_is_digit(string sToCheck) 


bool CTree::b_is_unary_opeartor(string &sToCheck) 
{
	return sToCheck[0] == CH_TYLDA;
}//bool CTree::b_is_unary_opeartor(string &sToCheck)


bool CTree::b_is_binary_operator(string &sToCheck) 
{
	return sToCheck[0] == CH_PLUS || sToCheck[0] == CH_MINUS || sToCheck[0] == CH_MULTIPLY || sToCheck[0] == CH_DIVISION;
}//bool CTree::b_is_binary_operator(string &sToCheck) 

string CTree::sInOrder()
{
	string s_result = "";

	if (pc_root != NULL)
	{
		v_in_order(pc_root, s_result);
	}//if (pc_root != NULL)
	
	return s_result;
}//string CTree::sInOrder()

void CTree::v_in_order(CTreeNode *pcTreeNode, string &sAccumulator)
{

	if (pcTreeNode->pc_left_child != NULL)
	{
		v_in_order(pcTreeNode->pc_left_child, sAccumulator);
	}//if (pcResultNode->pc_left_child != NULL)

	sAccumulator += (pcTreeNode->s_value + " ");

	if (pcTreeNode->pc_right_child != NULL)
	{
		v_in_order(pcTreeNode->pc_right_child, sAccumulator);
	}//	if (pcResultNode->pc_right_child != NULL)

}//void CTree::v_in_order(CTreeNode *pcTreeNode)



double CTree::dEvaluate()
{
	if (pc_root == NULL)
	{
		return 0;
	}//if (pc_root == NULL)
	
	return d_evaluate(pc_root);
}//double CTree::dEvaluate()


double CTree::d_evaluate(CTreeNode *pcTreeNode)
{
	if (pcTreeNode->pc_left_child == NULL && pcTreeNode->pc_right_child == NULL)
	{
		if(b_is_variable(pcTreeNode->s_value))
		{
			return c_variables[pcTreeNode->s_value];
		}//if(b_is_letter(pcTreeNode->s_value))
		else
			return stod(pcTreeNode->s_value);
	}//if (pcTreeNode->pc_left_child == NULL && pcTreeNode->pc_right_child == NULL)

	else if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child != NULL)
	{
		return d_evaluate_binary_operator(pcTreeNode);
	}//else if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child != NULL)

	else if (pcTreeNode->pc_left_child != NULL && pcTreeNode->pc_right_child == NULL)
	{
		return -d_evaluate(pcTreeNode->pc_left_child);
	}//else if (pcTreeNode->pc_left_child != NULL & pcTreeNode->pc_right_child == NULL)

	return 0.0;
}//double CTree::d_evaluate(CTreeNode *pcTreeNode)


double CTree::d_evaluate_binary_operator(CTreeNode* pcTreeNode)
{
	double d_result = 0.0;
	double d_left_child_result = d_evaluate(pcTreeNode->pc_left_child);
	double d_right_child_result = d_evaluate(pcTreeNode->pc_right_child);
	char ch_operator = pcTreeNode->s_value[0];

	switch (ch_operator)
	{
	case CH_PLUS:
		d_result = d_right_child_result + d_left_child_result;
		break;
	case CH_MINUS:
		d_result = d_right_child_result - d_left_child_result;
		break;
	case CH_MULTIPLY:
		d_result = d_right_child_result * d_left_child_result;
		break;
	case CH_DIVISION:
		d_result = d_right_child_result / d_left_child_result;
		break;
	default:
		d_result = d_left_child_result + d_right_child_result;
		break;
	}//	switch (ch_operator)
	return d_result;

}//double CTree::d_evaluate_binary_operator(CTreeNode* pcTreeNode)


bool CTree::b_is_variable(string &sToCheck) 
{
	bool b_only_letters_occured = true;

	for (int ii = 0; ii < sToCheck.length(); ii++) 
	{
		char ch_current = sToCheck[ii];
		if (ch_current<CH_A || ch_current>CH_z || (ch_current>CH_Z && ch_current<CH_a)) 
		{
			b_only_letters_occured = false;
		}
	}//for (int ii = 0; ii < sToCheck.length(); ii++) 
	return b_only_letters_occured;
}//bool CTree::b_is_letter(string sToCheck) 


string CTree::sGetVariables()
{
	string s_result = "";

	for (map<string,int>::iterator c_map_iterator = c_variables.begin(); c_map_iterator != c_variables.end(); ++c_map_iterator)
	{
		s_result += (c_map_iterator->first + " = " + to_string(c_map_iterator->second) + '\n');
	}//for (map<string,int>::iterator c_map_iterator = c_variables.begin(); c_map_iterator != c_variables.end(); ++c_map_iterator)
		
	return s_result;

}//string CTree::sGetVariables()


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

}//bool CTree::bSetVariableValue(string sVariable)


bool CTree::bContainsVariable(string sVariable)
{
	map<string, int>::iterator c_map_iterator = c_variables.find(sVariable);

	if (c_map_iterator != c_variables.end())
	{
		return true;
	}//if (c_map_iterator != c_variables.end())

	return false;
}//bool CTree::bContainsVariable(string sVariable)



string CTree::sGetInfixExpression()
{
	string s_infix_expression = "";

	if(pc_root!= NULL)
	{
		if(pc_root->pc_right_child != NULL)
		{
			s_infix_expression = s_get_infix_expression(pc_root->pc_right_child);
		}//if(pc_root->pc_right_child != NULL)

		s_infix_expression += pc_root->s_value;

		if (pc_root->pc_left_child != NULL)
		{
			s_infix_expression += s_get_infix_expression(pc_root->pc_left_child);
		}//if (pc_root->pc_left_child != NULL)
	}//if(pc_root!= NULL)

	return s_infix_expression;
}//string CTree::sGetInfixExpression()

string CTree::s_get_infix_expression(CTreeNode * pcTreeNode)
{
	string s_result = "";

	if (b_is_digit(pcTreeNode->s_value) || b_is_variable(pcTreeNode->s_value))
	{
		return pcTreeNode->s_value;
	} //if (b_is_digit(pcResultNode->s_value))

	else if (b_is_unary_opeartor(pcTreeNode->s_value))
	{
		s_result = "(" + pcTreeNode->s_value + s_get_infix_expression(pcTreeNode->pc_left_child) + ")";
		return s_result;

	}//else if (b_is_unary_operator(pcResultNode->s_value))

	else if (b_is_binary_operator(pcTreeNode->s_value))
	{
		s_result = "(" + s_get_infix_expression(pcTreeNode->pc_right_child) + pcTreeNode->s_value
			+ s_get_infix_expression(pcTreeNode->pc_left_child)  + ")";

		return s_result;
	}//else if(b_is_binary_operator(pc_result_node -> s_value))

	return s_result;
}//string CTree::s_get_infix_expression(CTreeNode * pcTreeNode)

string CTree::sGetPostfixExpression()
{
	string s_postfix_expression = "";

	if (pc_root != NULL)
	{
		
		if (pc_root->pc_right_child != NULL)
		{
			s_postfix_expression += s_get_postfix_expression(pc_root->pc_right_child);
		}//if (pc_root->pc_right_child != NULL)

		if (pc_root->pc_left_child != NULL)
		{
			s_postfix_expression += s_get_postfix_expression(pc_root->pc_left_child);
		}//if (pc_root->pc_left_child != NULL)

		s_postfix_expression += pc_root->s_value;

	}//if (pc_root != NULL)

	return s_postfix_expression;
	
}//string CTree::sGetPostfixExpression()

string CTree::s_get_postfix_expression(CTreeNode *pcTreeNode)
{
	string s_postfix_expression = "";

	if (b_is_digit(pcTreeNode->s_value) || b_is_variable(pcTreeNode->s_value))
	{
		return pcTreeNode->s_value + " ";
	} //if (b_is_digit(pcResultNode->s_value))

	else if (b_is_unary_opeartor(pcTreeNode->s_value))
	{
		s_postfix_expression = s_get_postfix_expression(pcTreeNode->pc_left_child) + pcTreeNode->s_value + " ";
		return s_postfix_expression;

	}//else if (b_is_unary_operator(pcResultNode->s_value))

	else if (b_is_binary_operator(pcTreeNode->s_value))
	{
		s_postfix_expression = s_get_postfix_expression(pcTreeNode->pc_right_child) + 
			s_get_postfix_expression(pcTreeNode->pc_left_child) + pcTreeNode->s_value + " ";

		return s_postfix_expression;
	}//else if(b_is_binary_operator(pc_result_node -> s_value))

	return s_postfix_expression;
}//string CTree::s_get_postfix_expression(CTreeNode *pcTreeNode)

void CTree::v_check_parse_errors(queue<string>& pcExpressionQueue, vector<CTree::CErrorInfo>& pvErrorVector, int iPosition)
{
	while (!pcExpressionQueue.empty())
	{
		CErrorInfo sct_error;
		sct_error.iPosition = iPosition--;
		if ((b_is_digit(pcExpressionQueue.front()) || b_is_variable(pcExpressionQueue.front())))
		{
			sct_error.iErrorIndex = 1;
			sct_error.sValue = pcExpressionQueue.front();
		}//if(b_is_variable(pcExpressionQueue.front()))
		else
		{
			sct_error.iErrorIndex = 0;
			sct_error.sValue = pcExpressionQueue.front();
		}//if(b_is_variable(pcExpressionQueue.front()))else 

		pvErrorVector.push_back(sct_error);
		pcExpressionQueue.pop();
	}//while(!pcExpressionQueue.empty())
}//void CTree::v_check_parse_errors(queue<string>& pcExpressionQueue, vector<CTree::SctErrorInformation>& pvErrorVector, int iPosition)


void CTree::v_assign_node(CTreeNode* pcFirstNode, CTreeNode* pcSecondNode)
{
	

}//void CTree::v_assign_node(CTreeNode* pcFirstNode, CTreeNode* pcSecondNode)

bool CTree::bSwapNodes(CTreeNode *pcFirstNode, CTreeNode *pcSecondNode)
{
	if(pcFirstNode!=NULL && pcSecondNode!= NULL)
	{
		CTreeNode *pc_first_node_parent = pcFirstNode->pc_parent;
		CTreeNode *pc_second_node_parent = pcSecondNode->pc_parent;
		bool is_first_node_left_child = is_left_child(pcFirstNode);
		bool is_second_node_left_child = is_left_child(pcSecondNode);
		
		if(is_first_node_left_child)
		{
			pc_first_node_parent->pc_left_child = pcSecondNode;
		}//if(is_first_node_left_child)
		else
		{
			pc_first_node_parent->pc_right_child = pcSecondNode;
		}//if(is_first_node_left_child) else

		pcSecondNode->pc_parent = pc_first_node_parent;
		
		if(is_second_node_left_child)
		{
			pc_second_node_parent->pc_left_child = pcFirstNode;
		}//	if(is_second_node_left_child)
		else
		{
			pc_second_node_parent->pc_right_child = pcFirstNode;
		}//	if(is_second_node_left_child) else

		pcFirstNode->pc_parent = pc_second_node_parent;

		return true;

	}//if(pcFirstNode!=NULL && pcSecondNode!= NULL)

	return false;
}//void CTree::bSwapNodes(CTreeNode * pcFirstNode, CTreeNode * pcSecondNode)


bool CTree::bSwapRootChildren()
{
	return bSwapNodes(pc_root->pc_left_child, pc_root->pc_right_child);
}//bool CTree::bSwapRootChildren()

bool CTree::is_left_child(CTreeNode* pcTreeNode)
{
	if (pcTreeNode->pc_parent->pc_left_child == pcTreeNode)
	{
		return true;
	}//if (pcTreeNode->pc_left_child == pcTreeNode)
	else if (pcTreeNode->pc_parent->pc_right_child == pcTreeNode)
	{
		return false;
	}//else if (pcTreeNode->pc_right_child == pcTreeNode)

	return false;
}//bool CTree::is_left_child(CTreeNode* pcTreeNode)



