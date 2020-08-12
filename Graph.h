#ifndef GRAPH_H
#define GRAPH_H
   
#include "Node.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include "SCC.h"

struct Graph
{
	Graph(std::vector<Rule> rules) 
	{
		for (size_t i = 0; i < rules.size(); ++i) { nodes.push_back( Node(rules.at(i), i) ); }
	}

	std::string nodesToStr()
	{
		std::string returnStr = "";
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			returnStr += ("R" + std::to_string(i) + " " + nodes.at(i).nodeRule.toString() + "\n");
		}
		return returnStr;
	}

	std::string graphToStr(std::vector<std::set<size_t>> printer)
	{
		std::string returnStr = "";
		for (size_t i = 0; i < printer.size(); ++i)
		{
			returnStr += ("R" + std::to_string(i) + ":");
			for (std::set<size_t>::iterator it = printer.at(i).begin(); it != printer.at(i).end(); ++it)
			{
				returnStr += ("R" + std::to_string(*it) + ",");
			}
			if (returnStr.at(returnStr.size() - 1) == ',') { returnStr = returnStr.substr(0, returnStr.size() - 1); }
			returnStr += "\n";
		}
		return returnStr;
	}

	
	std::vector<std::set<size_t>> dependentGraph() 
	{
		std::vector<std::set<size_t>> returnGraph;
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			std::set<size_t> newSet;
			returnGraph.push_back(newSet);
			for (size_t j = 0; j < nodes.at(i).nodeRule.predicateList.size(); ++j)
			{
				for (size_t k = 0; k < nodes.size(); ++k)
				{
					if (nodes.at(k).nodeRule.headId == nodes.at(i).nodeRule.predicateList.at(j).id) { returnGraph.at(i).insert(k); }
				}
			}
		}
		return returnGraph;
	}
	
	std::vector<std::set<size_t>> reverse(std::vector<std::set<size_t>> ogGraph)
	{
		std::vector<std::set<size_t>> returnGraph;
		for (size_t i = 0; i < ogGraph.size(); ++i)
		{
			std::set<size_t> newSet;
			returnGraph.push_back(newSet);
		}

		for (size_t i = 0; i < ogGraph.size(); ++i)
		{
			for (std::set<size_t>::iterator iter = ogGraph.at(i).begin(); iter != ogGraph.at(i).end(); ++iter)
			{ returnGraph.at(*iter).insert(i); }
		}
		return returnGraph;
	}

	std::vector<SCC> getSCC(std::vector<std::set<size_t>> graph)
	{
		reset();
		std::vector<SCC> returnVal;
		for (size_t i = 0; i < graph.size(); ++i)
		{
			if (nodes.at(i).visited == false)
			{
				returnVal.push_back(SCC());
				size_t sccIndex = returnVal.size() - 1;
				nodes.at(i).visited = true;
				returnVal.at(sccIndex).push_back(nodes.at(i));
				for (std::set<size_t>::iterator iter = graph.at(i).begin(); iter != graph.at(i).end(); ++iter)
				{
					if (nodes.at(*iter).visited == false) { SCCRecursive(graph, *iter, returnVal); }
				}
			}
		}
		return returnVal;
	}

	void SCCRecursive(std::vector<std::set<size_t>> graph, size_t index, std::vector<SCC>& returnVal)
	{
		size_t sccIndex = returnVal.size() - 1;
		nodes.at(index).visited = true;
		returnVal.at(sccIndex).push_back(nodes.at(index));
		for (std::set<size_t>::iterator iter = graph.at(index).begin(); iter != graph.at(index).end(); ++iter)
		{
			if (nodes.at(*iter).visited == false) { SCCRecursive(graph, *iter, returnVal); }
		}
	}

	void dfs(std::vector<std::set<size_t>> graph)
	{
		reset();
		size_t counter = 0;
		for (size_t i = 0; i < graph.size(); ++i)
		{
			nodes.at(i).visited = true;
			for (std::set<size_t>::iterator iter = graph.at(i).begin(); iter != graph.at(i).end(); ++iter)
			{
				if (nodes.at(*iter).visited == false) { dfsRecursive(graph, *iter, counter); }
			}
			if (nodes.at(i).postOrder == 0)
			{
				++counter;
				nodes.at(i).setPostOrder(counter);
			}
		}
	}

	void dfsRecursive(std::vector<std::set<size_t>> graph, size_t index, size_t& counter)
	{
		nodes.at(index).visited = true;
		for (std::set<size_t>::iterator iter = graph.at(index).begin(); iter != graph.at(index).end(); ++iter)
		{
			if (nodes.at(*iter).visited == false) { dfsRecursive(graph, *iter, counter); }
		}

		if (nodes.at(index).postOrder == 0)
		{
			++counter;
			nodes.at(index).setPostOrder(counter);
		}
	}

	void reset()
	{
		for (size_t i = 0; i < nodes.size(); ++i) { nodes.at(i).visited = false; }
		return;
	}

	std::vector<size_t> getPostOrder()
	{
		std::vector<size_t> returnVect;
		for (size_t i = 0; i < nodes.size(); ++i) { returnVect.push_back(nodes.at(i).postOrder); }
		return returnVect;
	}

	void sortNodes()
	{
		std::vector<Node> newOrder;
		for (size_t i = nodes.size(); i > 0; --i)
		{
			for (size_t j = 0; j < nodes.size(); ++j)
			{
				if (nodes.at(j).postOrder == i)
				{ 
					newOrder.push_back(nodes.at(j));
					break;
				}
			}
		}
		nodes = newOrder;
	}

	void sortSCC(std::vector<SCC>& list)
	{
		for (size_t i = 0; i < list.size(); ++i)
		{
			bool change = true;
			while (change)
			{
				change = false;
				for (size_t j = 0; j < list.at(i).size(); ++j)
				{
					if (j != 0)
					{
						if (list.at(i).at(j - 1).nodeNum > list.at(i).at(j).nodeNum)
						{
							Node temp = list.at(i).at(j);
							list.at(i).at(j) = list.at(i).at(j - 1);
							list.at(i).at(j - 1) = temp;
							change = true;
						}
					}
				}
			}
		}
		return;
	}

	std::vector<Node> nodes;
};

#endif
