#include "CTreeNode.h"



CTreeNode::CTreeNode(std::string sValue, CTreeNode *pcParentNode)
{
	s_value = sValue;
	pc_parent = pcParentNode;
	pc_right_child = NULL;
	pc_left_child = NULL;
}//END of CTreeNode constructor

CTreeNode::CTreeNode(CTreeNode &pcOther)
{
	s_value = pcOther.s_value;
	pc_parent = new CTreeNode(*pcOther.pc_parent);
	pc_left_child = new CTreeNode(*pc_left_child);
	pc_right_child = new CTreeNode(*pc_right_child);
	
}//END of CTreeNode constructor

CTreeNode::~CTreeNode()
{
	if (pc_left_child != NULL)
	{
		delete pc_left_child;
		
	}

	if (pc_right_child != NULL)
	{
		delete pc_right_child;
	}

}//END of ~CTreeNode