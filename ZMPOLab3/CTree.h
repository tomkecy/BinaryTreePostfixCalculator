#pragma once
#include "CTreeNode.h"
#include "Constants.h"
#include <queue>
#include <map>


using namespace std;

class CTree
{
private:
	CTreeNode *pc_root;	
	map<string, int> c_variables;
	static const int I_DEFAULT_VARIABLE_VALUE = 0;
	int i_expression_queue_length;

	class CErrorInfo;

	CTreeNode *v_build_subtree(queue<string> &pcExpressionQueue,int iPosition, vector<CErrorInfo> &pvErrorVector, CTreeNode *pcParentNode);

	
	bool b_is_digit(string &sToCheck);
	bool b_is_digit(char cToCheck);
	bool b_is_number(string &sToCheck);
	bool b_is_unary_opeartor(string &sToCheck);
	bool b_is_binary_operator(string &sToCheck);
	void v_build_in_order_traversal(CTreeNode *pcTreeNode, string &sAccumulator);
	bool b_is_variable(string &sToCheck);
	double d_evaluate(CTreeNode *pcTreeNode);
	double d_evaluate_binary_operator(CTreeNode *pcTreeNode);
	string s_get_infix_expression(CTreeNode *pcTreeNode);
	string s_get_postfix_expression(CTreeNode *pcTreeNode);
	bool is_left_child(CTreeNode *pcTreeNode);



public:

	/**
	* iErrorIndex:
	* 0 - missing operand
	* 1 - missing operator
	*/
	class CErrorInfo
	{
	private:

	public:
		int iPosition;
		int iErrorIndex;
		string sValue;
	};

	CTree();
	~CTree();
	void v_delete_existing_tree();
	void v_check_parse_errors(queue<string>& pcExpressionQueue, vector<CTree::CErrorInfo>& pvErrorVector, int i_position);
	void v_assign_node(CTreeNode* pcFirstNode, CTreeNode* pcSecondNode);
	void vBuildTree(queue <string> &cExpressionQueue, vector<CErrorInfo> &pvErrorVector);
	string sGetInOrderTraversal();
	double dEvaluate();
	string sGetVariables();
	string sGetInfixExpression();
	string sGetPostfixExpression();

	bool bSetVariableValue(string sVariable, int iNewValue);
	bool bContainsVariable(string sVariable);

	bool bSwapNodes(CTreeNode *pcFirstNode, CTreeNode *pcSecondNode);
	bool bSwapRootChildren();
	
};//END of class CTree
