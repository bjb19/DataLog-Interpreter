#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Scheme.h"
#include "Graph.h"


class Interpreter 
{
public:
	Interpreter(DatalogProgram data) : data(data) {}
	DatalogProgram data;

	Database makeDatabase()
	{
		Database database;
		for (size_t i = 0; i < data.schemes.size(); ++i)
		{
			Scheme setScheme = data.schemes.at(i).paramToString();
			Relation setRelation(data.schemes.at(i).id, setScheme);
			database[data.schemes.at(i).id] = setRelation;
		}

		for (size_t i = 0; i < data.facts.size(); ++i)
		{
			Tuple setTuple = data.facts.at(i).paramToString();
			database[data.facts.at(i).id].add(setTuple);
		}
		

		//Organizes data into SCC components


		Graph myGraph(data.rules);
		std::vector<std::set<size_t>> dependent = myGraph.dependentGraph();
		std::cout << "Dependency Graph\n";
		std::cout << myGraph.graphToStr(dependent) << "\n";
		std::vector<std::set<size_t>> reverse = myGraph.reverse(dependent);
		myGraph.dfs(reverse);
		myGraph.sortNodes();


		dependent = myGraph.dependentGraph();
		std::vector<SCC> sccs = myGraph.getSCC(dependent);
		myGraph.sortSCC(sccs);



		//Rule Evaluation
		
		std::cout << "Rule Evaluation\n";
		
			
		for (size_t i = 0; i < sccs.size(); ++i)
		{
			std::cout << "SCC: ";
			for (size_t ind = 0; ind < sccs.at(i).size(); ++ind)
			{
				if (ind != sccs.at(i).size() - 1) { std::cout << "R" << sccs.at(i).at(ind).nodeNum << ","; }
				else { std::cout << "R" << sccs.at(i).at(ind).nodeNum << std::endl; }
			}
			size_t passCount = 0;
			bool change = true;
			while (change)
			{
				++passCount;
				size_t dbSize = database.tupleCount();
				for (size_t ind = 0; ind < sccs.at(i).size(); ++ind)
				{
					

					std::cout << sccs.at(i).at(ind).nodeRule.toString() << std::endl;
					std::vector<Relation> joiningRealtions;
					//Query Eval
					for (size_t j = 0; j < sccs.at(i).at(ind).nodeRule.predicateList.size(); ++j)
					{
						joiningRealtions.push_back(evalQuery(sccs.at(i).at(ind).nodeRule.predicateList.at(j), database));
					}

					//Joining
					Relation joinResult;
					for (size_t j = 0; j < joiningRealtions.size(); ++j)
					{
						if (j == 0) { joinResult = joiningRealtions.at(j); }
						else { joinResult = joinResult.join(joiningRealtions.at(j)); }
					}

					//Project headpred
					std::vector<size_t> indexes;
					for (size_t j = 0; j < sccs.at(i).at(ind).nodeRule.idList.size(); ++j)
					{
						for (size_t k = 0; k < joinResult.scheme.size(); ++k)
						{
							if (sccs.at(i).at(ind).nodeRule.idList.at(j).toString() == joinResult.scheme.at(k))
							{
								indexes.push_back(k);
								break;
							}
						}
					}
					joinResult = joinResult.project(indexes);

					//Rename to header in db
					Scheme newScheme = database[sccs.at(i).at(ind).nodeRule.headId].scheme;
					joinResult = joinResult.rename(newScheme);

					//Union with database
					database[sccs.at(i).at(ind).nodeRule.headId].unite(joinResult);
				}
				if (database.tupleCount() == dbSize) { change = false; }
				else { change = true; }
				if (sccs.at(i).size() == 1 && !sccs.at(i).at(0).getRepeat()) { change = false; }
			}
			std::cout << passCount << " passes: ";
			for (size_t ind = 0; ind < sccs.at(i).size(); ++ind)
			{
				if (ind != sccs.at(i).size() - 1) { std::cout << "R" << sccs.at(i).at(ind).nodeNum << ","; }
				else { std::cout << "R" << sccs.at(i).at(ind).nodeNum << std::endl; }
			}
		}
		

		
		std::cout << "\nQuery Evaluation\n";
		for (size_t i = 0; i < data.queries.size(); ++i)
		{
			Relation queryResult = evalQuery(data.queries.at(i), database);
			std::cout << data.queries.at(i).toString() << "?";
			if (queryResult.tuples.empty()) { std::cout << " No\n"; }
			else { std::cout << " Yes(" << queryResult.tuples.size() << ")\n" << queryResult.toString(); }
		}
		return database;
	}

	Relation evalQuery(Predicate query, Database db)
	{
		std::map<std::string, size_t> indexMap;
		Relation dataTable;
		if (db.find(query.id) == db.end()) { std::cout << "Error in finding query in Database\n"; }
		else { dataTable = db[query.id]; }

		std::vector<size_t> projections;
		std::vector<std::string> newNames;
		for (size_t i = 0; i < query.params.size(); ++i)
		{
			if (query.params.at(i).isConstant) { dataTable = dataTable.select(i, query.params.at(i).paramVal); }
			else
			{
				if (indexMap.find(query.params.at(i).paramVal) == indexMap.end())
				{
					projections.push_back(i);
					newNames.push_back(query.params.at(i).paramVal);
					indexMap[query.params.at(i).paramVal] = i;
				}
				else { dataTable = dataTable.select(indexMap[query.params.at(i).paramVal], i); }
			}
		}

		
		
		if (!projections.empty()) { dataTable = dataTable.project(projections); }
		dataTable = dataTable.rename(newNames);

		return dataTable;
	}
};

#endif