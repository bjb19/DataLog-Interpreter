#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>

#include "Parameter.h"
struct Rule
{
	Rule() {};
	std::string headId;
	std::vector<Parameter> idList;
	std::vector<Predicate> predicateList;
	Rule(std::string headId, std::vector<Parameter> idList, std::vector<Predicate> predicateList) :
		headId(headId), idList(idList), predicateList(predicateList) {}
	std::string toString()
	{
		std::string returnStr = headId + "(";
		for (size_t i = 0; i < idList.size(); ++i)
		{
			returnStr += idList.at(i).toString();
			returnStr += ",";
		}
		returnStr = returnStr.substr(0, returnStr.size() - 1);
		returnStr += ") :- ";
		for (size_t i = 0; i < predicateList.size(); ++i)
		{
			returnStr += predicateList.at(i).toString();
			returnStr += ",";
		}
		returnStr = returnStr.substr(0, returnStr.size() - 1);
		returnStr += ".";
		return returnStr;
	}
	std::vector<std::string> getRuleScheme()
	{
		std::vector<std::string> returnVal;
		for (size_t i = 0; i < idList.size(); ++i) { returnVal.push_back(idList.at(i).toString()); }
		return returnVal;
	}
};
#endif
