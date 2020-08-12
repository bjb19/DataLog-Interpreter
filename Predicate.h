#ifndef PREDICATE_H
#define PREDICATE_H

#include "Parameter.h"
#include <string>
struct Predicate
{
	Predicate() {};
	std::string id;
	std::vector<Parameter> params;
	Predicate(std::string id, std::vector<Parameter> params) : id(id), params(params) {}

	std::string toString()
	{
		std::string returnStr = id;
		returnStr += "(";
		for (size_t i = 0; i < params.size(); ++i)
		{
			returnStr += params.at(i).toString();
			if (i != params.size() - 1) { returnStr += ","; }
		}
		returnStr += ")";
		return returnStr;
	}

	std::vector<std::string> paramToString()
	{
		std::vector<std::string> returnVect;
		for (size_t i = 0; i < params.size(); ++i)
		{
			returnVect.push_back(params.at(i).toString());
		}
		return returnVect;
	}

	std::vector<std::string> getDomain()
	{
		std::vector<std::string> returnVect;
		for (size_t i = 0; i < params.size(); ++i)
		{
			if (params.at(i).paramType == "STRING")
			{
				returnVect.push_back(params.at(i).paramVal);
			}
		}
		return returnVect;
	}
	
};
#endif 
