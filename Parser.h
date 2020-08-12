#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>

#include "Parameter.h"
#include "DatalogProgram.h"
#include "Predicate.h"


class Parser
{
public:
	Parser() {};
	void setTokens(std::vector<Token> newTokens)
	{
		for (size_t i = 0; i < newTokens.size(); ++i)
		{
			myTokens.push_back(newTokens.at(i));
		}
	}
	std::vector<std::vector<Token>> parse()
	{
		std::vector<std::vector<Token>> data;
		int vectorToPlace = -1;
		std::deque<std::string> parseDeque;
		parseDeque.push_back("DLP");
		while (!myTokens.empty())
		{
			match(parseDeque);
			if (isType(parseDeque)) 
			{
				++vectorToPlace;
				std::vector<Token> myVector;
				data.push_back(myVector);
			}
			bool someThingChanged = clear(parseDeque, vectorToPlace, data);
			if (!parseDeque.empty() && isTerminal(parseDeque.at(0)) && !someThingChanged) { throw myTokens.at(0); }
		}
		return data;
	}
private:
	std::deque<Token> myTokens;

	void match(std::deque<std::string>& myDeque)
	{
		if (myDeque.empty() && !myTokens.empty()) { throw myTokens.at(0); }

		if (myDeque.at(0) == "DLP") { dlp(myDeque); }

		else if (myDeque.at(0) == "SL") { sl(myDeque); }
		else if (myDeque.at(0) == "FL") { fl(myDeque); }
		else if (myDeque.at(0) == "RL") { rl(myDeque); }
		else if (myDeque.at(0) == "QL") { ql(myDeque); }

		else if (myDeque.at(0) == "S") { s(myDeque); }
		else if (myDeque.at(0) == "F") { f(myDeque); }
		else if (myDeque.at(0) == "R") { r(myDeque); }
		else if (myDeque.at(0) == "Q") { q(myDeque); }

		else if (myDeque.at(0) == "HPR") { hpr(myDeque); }
		else if (myDeque.at(0) == "PR") { pr(myDeque); }

		else if (myDeque.at(0) == "PRL") { prl(myDeque); }
		else if (myDeque.at(0) == "PL") { pl(myDeque); }
		else if (myDeque.at(0) == "STRL") { strl(myDeque); }
		else if (myDeque.at(0) == "IDL") { idl(myDeque); }

		else if (myDeque.at(0) == "P") { p(myDeque); }
		else if (myDeque.at(0) == "EXP") { exp(myDeque); }
		else if (myDeque.at(0) == "OPR") { opr(myDeque); }
	}
	

	void dlp(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::SCHEMES)
		{
			myDeque.pop_front();
			myDeque.push_front("QL");
			myDeque.push_front("Q");
			myDeque.push_front("COLON");
			myDeque.push_front("QUERIES");
			myDeque.push_front("RL");
			myDeque.push_front("COLON");
			myDeque.push_front("RULES");
			myDeque.push_front("FL");
			myDeque.push_front("COLON");
			myDeque.push_front("FACTS");
			myDeque.push_front("SL");
			myDeque.push_front("S");
			myDeque.push_front("COLON");
			myDeque.push_front("SCHEMES");
		}
		else { throw myTokens.at(0); }
		return;
	}

	//The following are all lists for
	//schemes facts rules queries predicates parameters strings and ids
	void sl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("SL");
			myDeque.push_front("S");
		}
		else { myDeque.pop_front(); }
		return;
	}
	void fl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("FL");
			myDeque.push_front("F");
		}
		else { myDeque.pop_front(); }
		return;
	}
	void rl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("RL");
			myDeque.push_front("R");
		}
		else { myDeque.pop_front(); }
		return;
	}
	void ql(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("QL");
			myDeque.push_front("Q");
		}
		else { myDeque.pop_front(); }
		return;
	}
	void prl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::COMMA)
		{
			myDeque.pop_front();
			myDeque.push_front("PRL");
			myDeque.push_front("PR");
			myDeque.push_front("COMMA");
		}
		else { myDeque.pop_front(); }
		return;
	}

	void pl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::COMMA)
		{
			myDeque.pop_front();
			myDeque.push_front("PL");
			myDeque.push_front("P");
			myDeque.push_front("COMMA");
		}
		else { myDeque.pop_front(); }
		return;
	}
	void strl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::COMMA)
		{
			myDeque.pop_front();
			myDeque.push_front("STRL");
			myDeque.push_front("STRING");
			myDeque.push_front("COMMA");
		}
		else { myDeque.pop_front(); }
	}
	void idl(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::COMMA)
		{
			myDeque.pop_front();
			myDeque.push_front("IDL");
			myDeque.push_front("ID");
			myDeque.push_front("COMMA");
		}
		else { myDeque.pop_front(); }
		return;
	}

	//scheme fact rule query
	void s(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("RIGHT_PAREN");
			myDeque.push_front("IDL");
			myDeque.push_front("ID");
			myDeque.push_front("LEFT_PAREN");
			myDeque.push_front("ID");
		}
		else { throw myTokens.at(0); }
	}
	void f(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("PERIOD");
			myDeque.push_front("RIGHT_PAREN");
			myDeque.push_front("STRL");
			myDeque.push_front("STRING");
			myDeque.push_front("LEFT_PAREN");
			myDeque.push_front("ID");
		}
		else { throw myTokens.at(0); }
	}
	void r(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("PERIOD");
			myDeque.push_front("PRL");
			myDeque.push_front("PR");
			myDeque.push_front("COLON_DASH");
			myDeque.push_front("HPR");
		}
		else { throw myTokens.at(0); }
	}
	void q(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("Q_MARK");
			myDeque.push_front("PR");
		}
		else { throw myTokens.at(0); }
	}

	//head predicate predicate expression and operator
	void hpr(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("RIGHT_PAREN");
			myDeque.push_front("IDL");
			myDeque.push_front("ID");
			myDeque.push_front("LEFT_PAREN");
			myDeque.push_front("ID");
		}
		else { throw myTokens.at(0); }
	}
	void pr(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("RIGHT_PAREN");
			myDeque.push_front("PL");
			myDeque.push_front("P");
			myDeque.push_front("LEFT_PAREN");
			myDeque.push_front("ID");
		}
		else { throw myTokens.at(0); }
	}
	void exp(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::LEFT_PAREN)
		{
			myDeque.pop_front();
			myDeque.push_front("RIGHT_PAREN");
			myDeque.push_front("P");
			myDeque.push_front("OPR");
			myDeque.push_front("P");
			myDeque.push_front("LEFT_PAREN");
		}

		else { throw myTokens.at(0); }
	}
	void opr(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ADD)
		{
			myDeque.pop_front();
			myDeque.push_front("ADD");
		}
		else if (myTokens.at(0).type == TokenType::MULTIPLY)
		{
			myDeque.pop_front();
			myDeque.push_front("MULTIPLY");
		}
		else { throw myTokens.at(0); }
	}

	void p(std::deque<std::string>& myDeque)
	{
		if (myTokens.at(0).type == TokenType::ID)
		{
			myDeque.pop_front();
			myDeque.push_front("ID");
		}
		else if (myTokens.at(0).type == TokenType::STRING)
		{
			myDeque.pop_front();
			myDeque.push_front("STRING");
		}
		else if (myTokens.at(0).type == TokenType::LEFT_PAREN)
		{
			myDeque.pop_front();
			myDeque.push_front("EXP");
		}
		else { throw myTokens.at(0); }
	}
	bool clear(std::deque<std::string>& myDeque, int& index, std::vector<std::vector<Token>>& data)
	{
		bool somethingChanged = false;
		if (!myTokens.empty() && !myDeque.empty())
		{
			if (myDeque.at(0) == myTokens.at(0).convertEnum())
			{
				data.at(index).push_back(myTokens.at(0));
				myDeque.pop_front();
				myTokens.pop_front();
				somethingChanged = true;
			}
		}
		return somethingChanged;
	}
	bool isTerminal(std::string val)
	{
		const std::vector<std::string> TERMINALS =
		{
			"SCHEMES", "COLON", "FACTS",
			"RULES", "QUERIES", "ID",
			"LEFT_PAREN", "RIGHT_PAREN",
			"STRING", "COLON_DASH", "COMMA",
			"PERIOD", "ADD", "MULTIPLY", "Q_MARK"
		};
		if (std::find(TERMINALS.begin(), TERMINALS.end(), val) != TERMINALS.end())
		{
			return true;
		}
		else { return false; }
	}
	bool isType(std::deque<std::string>& parseDeque)
	{
		if (parseDeque.empty()) { return false; }
		else if (
			parseDeque.at(0) == "SCHEMES" ||
			parseDeque.at(0) == "FACTS" ||
			parseDeque.at(0) == "RULES" ||
			parseDeque.at(0) == "QUERIES"
			) { return true; }
		return false;
	}
};

#endif