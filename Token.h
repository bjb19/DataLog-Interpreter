#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
	COMMA, PERIOD, Q_MARK, LEFT_PAREN,
	RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY,
	ADD, SCHEMES, FACTS, RULES,
	QUERIES, ID, STRING, COMMENT,
	UNDEFINED, END
};

struct Token
{
	TokenType type;
	std::string value;
	int line;

	Token(TokenType type, std::string value, int line)
	{
		this->type = type;
		this->value = value;
		this->line = line;
	}
	std::string toString()
	{
		std::string returnStr = "(" + convertEnum() + ",\"" + value + "\"," + std::to_string(line) + ")";
		return returnStr;
	}
	std::string convertEnum()
	{
		switch(static_cast<int>(type))
		{
			case 0 :
				return "COMMA";
			case 1 :
				return "PERIOD";
			case 2 :
				return "Q_MARK";
			case 3 :
				return "LEFT_PAREN";
			case 4 :
				return "RIGHT_PAREN";
			case 5 :
				return "COLON";
			case 6 :
				return "COLON_DASH";
			case 7 :
				return "MULTIPLY";
			case 8:
				return "ADD";
			case 9 :
				return "SCHEMES";
			case 10 :
				return "FACTS";
			case 11 :
				return "RULES";
			case 12 :
				return "QUERIES";
			case 13 :
				return "ID";
			case 14 :
				return "STRING";
			case 15 :
				return "COMMENT";
			case 16 :
				return "UNDEFINED";
			case 17 :
				return "EOF";
;			default :
				return "ERROR";
		}
	}
};
#endif