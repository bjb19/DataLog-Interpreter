#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>
#include <utility>

struct Tuple : public std::vector<std::string> 
{
	Tuple() {}
	Tuple(std::vector<std::string> myVect) : vector(myVect) {}
	Tuple combine(Tuple addTuple)
	{
		Tuple returnVal = addTuple;
		for (size_t i = 0; i < this->size(); ++i)
		{
			returnVal.push_back(this->at(i));
		}
		return returnVal;
	}
	Tuple matchAndAdd(std::vector<std::pair<size_t, size_t>> matches, Tuple otherTuple)
	{
		Tuple returnTuple;
		for (size_t i = 0; i < this->size(); ++i)
		{
			returnTuple.push_back(this->at(i));
		}

		for (size_t i = 0; i < otherTuple.size(); ++i)
		{
			bool pushBack = true;
			for (size_t j = 0; j < matches.size(); ++j)
			{
				if (matches.at(j).second == i) 
				{ 
					pushBack = false;
					break;
				}
			}
			if (pushBack) { returnTuple.push_back(otherTuple.at(i)); }
		}
		
		return returnTuple;
	}
	std::string printTuple(std::vector<std::string> scheme)
	{
		std::string returnStr = "  ";
		for (size_t i = 0; i < scheme.size(); ++i)
		{
			if (i != scheme.size() - 1) { returnStr += (scheme.at(i) + "=" + this->at(i) + ", "); }
			else { returnStr += (scheme.at(i) + "=" + this->at(i)); }
		}
		return returnStr;
	}
};

#endif