#ifndef NODE_H
#define NODE_H

#include "Rule.h"

struct Node
{
	Node(Rule nodeRule, size_t nodeNum) : nodeRule(nodeRule), nodeNum(nodeNum), visited(false), postOrder(0), repeats(false)
	{
		
	}
	bool getRepeat()
	{
		for (size_t i = 0; i < nodeRule.predicateList.size(); ++i)
		{
			if (nodeRule.predicateList.at(i).id == nodeRule.headId)
			{
				repeats = true;
				break;
			}
		}
		return repeats;
	}
	Rule nodeRule;
	size_t nodeNum;
	bool visited;
	size_t postOrder;
	bool repeats;
	void setPostOrder(size_t val) { postOrder = val; }
};

#endif