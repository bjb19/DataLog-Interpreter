#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include <vector>
#include "Predicate.h"
#include "Rule.h"

struct DatalogProgram
{
	std::vector<Predicate> schemes;
	std::vector<Predicate> facts;
	std::vector<Predicate> queries;
	std::vector<Rule> rules;
	std::vector<std::string> domain;

	void setDomain()
	{
		for (size_t i = 0; i < facts.size(); ++i)
		{
			std::vector<std::string> add;
			add = facts.at(i).getDomain();
			for (size_t j = 0; j < add.size(); ++j)
			{
				if (std::find(domain.begin(), domain.end(), add.at(j)) == domain.end())
				{
					domain.push_back(add.at(j));
				}
			}
			std::sort(domain.begin(), domain.end());
		}
	}

	std::string toString()
	{
		std::string returnStr = "";
		returnStr += ("Schemes(" + std::to_string(schemes.size()) + "):\n");
		for (size_t i = 0; i < schemes.size(); ++i)
		{
			returnStr += ("  " + schemes.at(i).toString() + "\n");
		}

		returnStr += ("Facts(" + std::to_string(facts.size()) + "):\n");
		for (size_t i = 0; i < facts.size(); ++i)
		{
			returnStr += ("  " + facts.at(i).toString() + ".\n");
		}

		returnStr += ("Rules(" + std::to_string(rules.size()) + "):\n");
		for (size_t i = 0; i < rules.size(); ++i)
		{
			returnStr += ("  " + rules.at(i).toString() + ".\n");
		}
		returnStr += ("Queries(" + std::to_string(queries.size()) + "):\n");
		for (size_t i = 0; i < queries.size(); ++i)
		{
			returnStr += ("  " + queries.at(i).toString() + "?\n");
		}

		setDomain();

		returnStr += ("Domain(" + std::to_string(domain.size()) + "):\n");
		for (size_t i = 0; i < domain.size(); ++i)
		{
			if (i != domain.size() - 1) { returnStr += ("  " + domain.at(i) + "\n"); }
			else { returnStr += ("  " + domain.at(i)); }
		}
		return returnStr;
	}
};

#endif