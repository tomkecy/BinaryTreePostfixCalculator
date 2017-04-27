#include "CExpressionParser.h"
#include <iostream>


CExpressionParser::CExpressionParser()
{

}//CExpressionParser::CExpressionParser()

CExpressionParser::~CExpressionParser()
{


}//CExpressionParser::~CExpressionParser()


queue<string>& CExpressionParser::pcParseExpression(string sExpression, bool &pbIncorrectCharsOccured)
{
	queue<string> *pc_result_queue = new queue<string>;
	pbIncorrectCharsOccured = v_split_expression(sExpression, *pc_result_queue);

	return *pc_result_queue;
}//queue<string>* CExpressionParser::pcParseExpression(string psExpression)


bool CExpressionParser::v_split_expression(string &psExpression, char chDelimiter, vector<string> &pvResultVector) 
{
	bool b_incorrect_chars_occured = false;

	for(int ii = 0; ii< psExpression.length(); ii++)
	{
		if(psExpression[ii] == CH_PLUS)
		{
			psExpression.replace(ii, 1, " + ");
			ii++;
		}
		else if (psExpression[ii] == CH_MINUS)
		{
			psExpression.replace(ii, 1, " - ");
			ii++;
		}
		else if (psExpression[ii] == CH_MULTIPLY)
		{
			psExpression.replace(ii, 1, " * ");
			ii++;
		}
		else if (psExpression[ii] == CH_DIVISION)
		{
			psExpression.replace(ii, 1, " / ");
			ii++;
		}
		else if (psExpression[ii] == CH_TYLDA)
		{
			psExpression.replace(ii, 1, " ~ ");
			ii++;
		}
		else if (!(b_is_letter(psExpression[ii]) || b_is_digit(psExpression[ii]) || psExpression[ii] == CH_SPACE))
		{
			psExpression.replace(ii, 1, " ");
			b_incorrect_chars_occured = true;
		}

	}//END of for loop


	stringstream c_string_stream;
	c_string_stream.str(psExpression);

	string s_parsed_operand;

	while (getline(c_string_stream, s_parsed_operand, chDelimiter)) 
	{

		if (!s_parsed_operand.empty())
		{
			pvResultVector.push_back(s_parsed_operand);
		}
		
	}//END of while loop

	return b_incorrect_chars_occured;
}//END of v_split_expression

bool CExpressionParser::v_split_expression(string &psExpression, queue<string> &pcResultQueue)
{
	vector<string> v_helper_vector;
	bool b_incorrect_chars_occured = v_split_expression(psExpression, ' ', v_helper_vector);

	while (!v_helper_vector.empty())
	{
		pcResultQueue.push(v_helper_vector.back());
		v_helper_vector.pop_back();
	}

	return b_incorrect_chars_occured;
}//END of v_split_expression

bool CExpressionParser::b_is_digit(char chToCheck)
{
	return (chToCheck >= CH_0 && chToCheck <= CH_9);
}//END of b_is_digit

bool CExpressionParser::b_is_letter(char chToCheck)
{
	return ((chToCheck>=CH_A && chToCheck<=CH_Z) || (chToCheck >= CH_a && chToCheck <= CH_z));

}//END of b_is_letter
