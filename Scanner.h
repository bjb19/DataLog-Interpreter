#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "Token.h"
#include <fstream>
#include <vector>
#include <iostream>


class Scanner 
{
public:
	Scanner() : file("") {}
	void setFile(std::string file) { this->file = file;  }
	size_t getCount() { return myTokens.size();  }

	bool isLetter(char letter)
	{
		//ACII Key for lowercase
		if (letter > 96 && letter < 123) { return true; }
		//ACII Key for uppercase
		if (letter > 64 && letter < 91) { return true; }
		return false;
	}
	bool isNumber(char check)
	{
		if (check > 47 && check < 58) { return true; }
		return false;
	}

	std::ostream& display(std::ostream& out)
	{
		for (size_t i = 0; i < myTokens.size(); ++i)
		{
			out << myTokens.at(i).toString() << std::endl;
		}
		out << "Total Tokens = " << myTokens.size() << std::endl;
		return out;
	}

	void scan()
	{
		std::ifstream in;
		in.open(file);
		if (!in) { std::cerr << "ERROR OPENING FILE" << std::endl; }
		
		int line = 1;
		int readChar = 0;
		bool moreToRead = true;
		bool needNewChar = true;
		while (moreToRead)
		{
			if (needNewChar) { readChar = in.get(); }
			needNewChar = true;

			if (readChar == '\n')
			{
				++line;
			}

			else if (readChar == EOF)
			{
				moreToRead = false;
				//Do not need this token either
				//myTokens.push_back(Token(TokenType::END, "", line));
			}

			else if (readChar == ',') { myTokens.push_back(Token(TokenType::COMMA, ",", line)); }

			else if (readChar == '.') { myTokens.push_back(Token(TokenType::PERIOD, ".", line)); }

			else if (readChar == '?') { myTokens.push_back(Token(TokenType::Q_MARK, "?", line)); }

			else if (readChar == '(') { myTokens.push_back(Token(TokenType::LEFT_PAREN, "(", line)); }

			else if (readChar == ')') { myTokens.push_back(Token(TokenType::RIGHT_PAREN, ")", line)); }

			else if (readChar == ':') 
			{
				readChar = in.get();
				if (readChar == '-') { myTokens.push_back(Token(TokenType::COLON_DASH, ":-", line)); }
				else 
				{
					myTokens.push_back(Token(TokenType::COLON, ":", line));
					needNewChar = false;
				}
			}

			else if (readChar == '*') { myTokens.push_back(Token(TokenType::MULTIPLY, "*", line)); }

			else if (readChar == '+') { myTokens.push_back(Token(TokenType::ADD, "+", line)); }

			else if (isLetter(readChar))
			{
				std::string id = "";
				do
				{
					id += static_cast<char>(readChar);
					readChar = in.get();
				} while (isNumber(readChar) || isLetter(readChar));
				needNewChar = false;
				if (id == "Facts") { myTokens.push_back(Token(TokenType::FACTS, id, line)); }
				else if(id == "Rules") { myTokens.push_back(Token(TokenType::RULES, id, line)); }
				else if (id == "Queries") { myTokens.push_back(Token(TokenType::QUERIES, id, line)); }
				else if (id == "Schemes") { myTokens.push_back(Token(TokenType::SCHEMES, id, line)); }
				else { myTokens.push_back(Token(TokenType::ID, id, line)); }
			}

			else if (readChar == '#')
			{
				int startLine = line;
				std::string comment = "#";
				readChar = in.get();
				bool isUndefined = false;
				needNewChar = false;
				if (readChar == '|')
				{
					while (readChar != EOF)
					{
						comment += static_cast<char>(readChar);
						if (readChar == '\n') 
						{ ++line; }
						if (readChar == '|')
						{
							readChar = in.get();
							if (readChar == '#')
							{
								comment += static_cast<char>(readChar);
								needNewChar = true;
								break;
							}
						}
						else { readChar = in.get(); }

						if (readChar == EOF) { isUndefined = true; }
					}
				}
				else
				{
					while (readChar != '\n' && readChar != EOF)
					{
						comment += static_cast<char>(readChar);
						readChar = in.get();
					}
				}
				if (isUndefined) { myTokens.push_back(Token(TokenType::UNDEFINED, comment, startLine)); }
				//Comments are Ignored
				//else { myTokens.push_back(Token(TokenType::COMMENT, comment, startLine)); }
			}

			else if (readChar == '\'')
			{
				std::string strVal = "\'";
				needNewChar = false;
				int startLine = line;
				bool isUndefined = true;
				readChar = in.get();
				while (readChar != EOF)
				{
					strVal += static_cast<char>(readChar);
					if (readChar == '\n')
					{ ++line; }
					if (readChar == '\'') 
					{
						readChar = in.get();
						if (readChar != '\'')
						{
							isUndefined = false;
							break;
						}
						else
						{
							strVal += static_cast<char>(readChar);
							readChar = in.get();
						}
					}
					else { readChar = in.get(); }
				}
				if (isUndefined) { myTokens.push_back(Token(TokenType::UNDEFINED, strVal, startLine)); }
				else { myTokens.push_back(Token(TokenType::STRING, strVal, startLine)); }
			}

			else
			{
				if (readChar != ' ' && readChar != '\t')
				{
					std::string undefStr = "";
					undefStr += static_cast<char>(readChar);
					myTokens.push_back(Token(TokenType::UNDEFINED, undefStr, line));
				}
			}
		}
		in.close();
		return;
	}
	
	std::vector<Token> getTokens() { return myTokens; }
private:
	std::vector<Token> myTokens;
	std::string file;
};

#endif