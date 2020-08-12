#ifndef REALTION_H
#define REALTION_H

#include <set>
#include "Tuple.h"
#include <string>
#include "Scheme.h"
#include <utility>

struct Relation
{
	Relation() {}
	Relation(std::string name, Scheme scheme) : name(name), scheme(scheme) {}
	std::string name;
	std::set<Tuple> tuples;
	Scheme scheme;
	void add(Tuple addVal) { tuples.insert(addVal); }
	std::string toString()
	{
		std::string returnStr = "";
		for (std::set<Tuple>::iterator iter = tuples.begin(); iter != tuples.end(); ++iter)
		{
			for (size_t i = 0; i < scheme.size(); ++i)
			{
				if (i == 0) { returnStr += "  "; }
				returnStr += (scheme.at(i) + "=" + (*iter).at(i) + ", ");
				if (i == scheme.size() - 1) 
				{
					returnStr = returnStr.substr(0, returnStr.length() - 2);
					returnStr += "\n";
				}
			}
		}
		return returnStr;
	}
	Relation select(size_t index, std::string value) 
	{
		Relation newRealtion = Relation(name, scheme);
		std::set<Tuple> newTuples;
		for (std::set<Tuple>::iterator iter = tuples.begin(); iter != tuples.end(); ++iter)
		{
			if ((*iter).at(index) == value) { newRealtion.add(*iter); }
		}
		return newRealtion;
	}
	Relation select(size_t index1, size_t index2)
	{
		Relation newRealtion = Relation(name, scheme);
		std::set<Tuple> newTuples;
		for (std::set<Tuple>::iterator iter = tuples.begin(); iter != tuples.end(); ++iter)
		{
			if ((*iter).at(index1) == (*iter).at(index2)) { newRealtion.add(*iter); }
		}
		return newRealtion;
	}
	Relation project(std::vector<size_t> indexes)
	{
		std::vector<std::string> attributes;
		for (size_t i = 0; i < indexes.size(); ++i) { attributes.push_back(scheme.at(i)); }
		Relation newRealtion = Relation(name, attributes);
		
		for (std::set<Tuple>::iterator iter = tuples.begin(); iter != tuples.end(); ++iter)
		{
			Tuple newTuple;
			for (size_t i = 0; i < indexes.size(); ++i) { newTuple.push_back((*iter).at(indexes.at(i))); }
			newRealtion.add(newTuple);
		}
		return newRealtion;
	}
	Relation rename(std::vector<std::string> newScheme)
	{
		Relation newRelation = Relation(name, scheme);
		newRelation.tuples = this->tuples;
		newRelation.scheme = newScheme;
		return newRelation;
	}
	Relation join(Relation joinRelation)
	{
		bool hasCommonAttribute = false;
		std::vector<std::string> newScheme = scheme;
		std::vector<std::pair<size_t, size_t>> matchingAttributes;

		for (size_t i = 0; i < joinRelation.scheme.size(); ++i)
		{
			bool hasThisAttribute = false;
			for (size_t j = 0; j < scheme.size(); ++j)
			{
				if (joinRelation.scheme.at(i) == scheme.at(j)) 
				{
					std::pair<size_t, size_t> addPair(j, i);
					matchingAttributes.push_back(addPair);
					hasCommonAttribute = true;
					hasThisAttribute = true;
				}
			}
			if (!hasThisAttribute) { newScheme.push_back(joinRelation.scheme.at(i)); }
		}

		if (!hasCommonAttribute) { return cartesianProduct(joinRelation, newScheme); }

		Relation returnRelation = Relation("UPDATE NAME(1)", newScheme);
		for (std::set<Tuple>::iterator iter1 = tuples.begin(); iter1 != tuples.end(); ++iter1)
		{
			for (std::set<Tuple>::iterator iter2 = joinRelation.tuples.begin(); iter2 != joinRelation.tuples.end(); ++iter2)
			{
				bool canAdd = true;
				for (size_t i = 0; i < matchingAttributes.size(); ++i)
				{
					if ((*iter1).at(matchingAttributes.at(i).first) != (*iter2).at(matchingAttributes.at(i).second))
					{
						canAdd = false;
						break;
					}
				}

				if (canAdd)
				{
					Tuple addTuple = *iter1;
					addTuple = addTuple.matchAndAdd(matchingAttributes, *iter2);
					returnRelation.add(addTuple);
				}
			}
		}
		return returnRelation;
	}
	Relation cartesianProduct(Relation joinRelation, std::vector<std::string> newScheme)
	{
		//Not sure what to do about name
		Relation returnVal = Relation("UPDATE NAME(2)", newScheme);
		for (std::set<Tuple>::iterator iter1 = tuples.begin(); iter1 != tuples.end(); ++iter1)
		{
			for (std::set<Tuple>::iterator iter2 = joinRelation.tuples.begin(); iter2 != joinRelation.tuples.end(); ++iter2)
			{
				Tuple addTuple = *iter2;
				addTuple = addTuple.combine(*iter1);
				returnVal.add(addTuple);
			}
		}
		return returnVal;
	}
	void unite(Relation unionRelation)
	{
		if (this->scheme != unionRelation.scheme) 
		{ 
			std::cout << "Relations not union compatible" << std::endl;
			return;
		}

		for (std::set<Tuple>::iterator iter = unionRelation.tuples.begin(); iter != unionRelation.tuples.end(); ++iter)
		{
			if (tuples.find(*iter) == tuples.end())
			{
				Tuple addTuple = *iter;
				std::cout << addTuple.printTuple(scheme) << std::endl;
				add(addTuple);
			}
		}
		return;
	}
};
#endif
