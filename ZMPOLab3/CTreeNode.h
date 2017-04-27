#pragma once
#include <string>


class CTreeNode
{
public:
	CTreeNode(std::string sValue, CTreeNode *pcParentNode);
	CTreeNode(CTreeNode &pcOther);
	~CTreeNode();
	friend class CTree;

private:
	
	CTreeNode *pc_parent, *pc_left_child, *pc_right_child;
	std::string s_value;

};//class CTreeNode