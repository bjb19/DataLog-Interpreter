#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>
#include "Relation.h"

class Database : public std::map<std::string, Relation>
{
public:
	Database() {}
	size_t tupleCount()
	{
		size_t count = 0;
		for (std::map<std::string, Relation>::iterator iter = begin(); iter != end(); ++iter)
		{
			count += (*iter).second.tuples.size();
		}
		return count;
	}
};

#endif 