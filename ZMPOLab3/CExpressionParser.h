#pragma once
#include <queue>
#include <sstream>
#include <string>
#include "Constants.h"


using namespace std;


class CExpressionParser
{
private:

	bool v_split_expression(string &psExpression, char chDelimiter, vector<string> &pvResultVector);
	bool v_split_expression(string &psExpression, queue<string> &pcResultQueue);
	bool b_is_digit(char chToCheck);
	bool b_is_letter(char chToCheck);

public:
	CExpressionParser();
	~CExpressionParser();
	queue<string> &pcParseExpression(string sExpression, bool &pbIncorrecdCharsOccured);
	
};//END of class CExpressionParser