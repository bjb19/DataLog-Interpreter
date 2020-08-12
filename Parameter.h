#ifndef PARAMETER_H
#define PARAMETER_H

#include "Token.h"
#include <string>
#include <vector>
#include <iostream>

struct Parameter
{
	Parameter() : isConstant(true) {}
	Parameter(std::string setString) 
	{
		if (setString.at(0) == '\'') 
		{
			paramType = "STRING";
			paramVal = setString;
			isConstant = true;
		}
		else if (setString.at(0) == '(')
		{
			paramType = "EXPRESSION";
			paramVal = setString;
			isConstant = false;
		}
		else
		{
			paramType = "ID";
			paramVal = setString;
			isConstant = false;
		}
	}
	std::string paramType;
	std::string paramVal;
	bool isConstant;

	std::string toString() { return paramVal; }
};



#endif