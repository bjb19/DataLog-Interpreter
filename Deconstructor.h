#ifndef DECONSTRUCTOR_H
#define DECONSTRUCOTR_H

#include <vector>
#include "Token.h"
#include "DatalogProgram.h"

class Deconstrucor 
{
public:
	void setData(std::vector<std::vector<Token>> data) { rawData = data; }
	DatalogProgram getLog() { return log; }
	void sort()
	{
		int i = 0;
		bool firstId = true;
		std::vector<Parameter> params;
		std::vector<Predicate> predicateList;
		std::string headId = "";
		//SCHEMES
		for (size_t j = 0; j < rawData.at(i).size(); ++j)
		{
			if (firstId && rawData.at(i).at(j).type == TokenType::ID)
			{
				headId = rawData.at(i).at(j).value;
				firstId = false;
			}
			else if (!firstId && rawData.at(i).at(j).type == TokenType::ID) { params.push_back(Parameter(rawData.at(i).at(j).value)); }
			else if (rawData.at(i).at(j).type == TokenType::RIGHT_PAREN)
			{
				predicateList.push_back(Predicate(headId, params));
				firstId = true;
				headId = "";
				params.clear();
			}
		}

		++i;
		log.schemes = predicateList;
		predicateList.clear();
		params.clear();
		firstId = true;
		headId = "";

		//FACTS
		for (size_t j = 0; j < rawData.at(i).size(); ++j)
		{
			if (firstId && rawData.at(i).at(j).type == TokenType::ID)
			{
				headId = rawData.at(i).at(j).value;
				firstId = false;
			}
			else if (rawData.at(i).at(j).type == TokenType::STRING) { params.push_back(Parameter(rawData.at(i).at(j).value)); }
			else if (rawData.at(i).at(j).type == TokenType::PERIOD)
			{
				predicateList.push_back(Predicate(headId, params));
				firstId = true;
				headId = "";
				params.clear();
			}
		}

		++i;
		log.facts = predicateList;
		predicateList.clear();
		params.clear();
		firstId = true;
		headId = "";
		bool workingOnHeadPredicate = true;
		bool firstPredicateId = true;
		std::vector<Parameter> idList;
		std::vector<Rule> ruleList;
		std::string predicateId = "";
		bool firstParen = true;
		//RULES
		for (size_t j = 0; j < rawData.at(i).size(); ++j)
		{
			if (firstId && rawData.at(i).at(j).type == TokenType::ID)
			{
				headId = rawData.at(i).at(j).value;
				firstId = false;
			}

			else if (rawData.at(i).at(j).type == TokenType::COLON_DASH)
			{
				workingOnHeadPredicate = false;
				firstPredicateId = true;
			}

			else if (rawData.at(i).at(j).type == TokenType::ID && workingOnHeadPredicate)
			{
				idList.push_back(Parameter(rawData.at(i).at(j).value));
			}

			else if (firstPredicateId && rawData.at(i).at(j).type == TokenType::ID)
			{
				predicateId = rawData.at(i).at(j).value;
				firstPredicateId = false;
			}

			else if (rawData.at(i).at(j).type == TokenType::RIGHT_PAREN && !workingOnHeadPredicate)
			{
				predicateList.push_back(Predicate(predicateId, params));
				predicateId = "";
				firstParen = true;
				firstPredicateId = true;
				params.clear();
			}

			else if (rawData.at(i).at(j).type == TokenType::STRING) { params.push_back(Parameter(rawData.at(i).at(j).value)); }

			else if (rawData.at(i).at(j).type == TokenType::ID) { params.push_back(Parameter(rawData.at(i).at(j).value)); }

			else if (rawData.at(i).at(j).type == TokenType::LEFT_PAREN && firstParen && !workingOnHeadPredicate) { firstParen = false; }

			else if (rawData.at(i).at(j).type == TokenType::LEFT_PAREN && !firstParen && !workingOnHeadPredicate) 
			{ params.push_back(Parameter(recursiveParen(rawData.at(i), j))); }

			else if (rawData.at(i).at(j).type == TokenType::PERIOD)
			{
				ruleList.push_back(Rule(headId, idList, predicateList));
				firstPredicateId = false;
				firstId = true;
				firstParen = true;
				workingOnHeadPredicate = true;
				predicateId = "";
				headId = "";
				predicateList.clear();
				params.clear();
				idList.clear();
			}
		}
		//QUERIES

		++i;
		log.rules = ruleList;
		predicateList.clear();
		params.clear();
		firstId = true;
		headId = "";
		firstParen = true;

		for (size_t j = 0; j < rawData.at(i).size(); ++j)
		{
			if (firstId && rawData.at(i).at(j).type == TokenType::ID)
			{
				headId = rawData.at(i).at(j).value;
				firstId = false;
			}
			else if (rawData.at(i).at(j).type == TokenType::ID) { params.push_back(Parameter(rawData.at(i).at(j).value)); }
			else if (rawData.at(i).at(j).type == TokenType::STRING) { params.push_back(Parameter(rawData.at(i).at(j).value)); }
			else if (rawData.at(i).at(j).type == TokenType::LEFT_PAREN && firstParen) { firstParen = false; }
			else if (rawData.at(i).at(j).type == TokenType::LEFT_PAREN && !firstParen) { params.push_back(Parameter(recursiveParen(rawData.at(i), j))); }
			else if (rawData.at(i).at(j).type == TokenType::Q_MARK)
			{
				predicateList.push_back(Predicate(headId, params));
				firstId = true;
				firstParen = true;
				headId = "";
				params.clear();
			}
		}
		log.queries = predicateList;
	}
private:
	std::vector<std::vector<Token>> rawData;
	DatalogProgram log;
	std::string recursiveParen(std::vector<Token> list, size_t& index) 
	{
		std::string returnStr = "";
		bool firstLoop = true;
		for (index = index; index < list.size(); ++index)
		{
			if (list.at(index).type == TokenType::LEFT_PAREN && !firstLoop)
			{ returnStr += recursiveParen(list, index); }
			else if (list.at(index).type == TokenType::RIGHT_PAREN)
			{
				returnStr += ")";
				return returnStr;
			}
			else { returnStr += (list.at(index).value); }
			firstLoop = false;
		}
		return returnStr;
	}
};

#endif

