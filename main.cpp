#include "Scanner.h"
#include "Parser.h"
#include "Deconstructor.h"

#include "DatalogProgram.h"

#include "Interpreter.h"
#include "Database.h"

#include <fstream>
#include <iostream>
#include <string>


int main(int argc, char** argv)
{

	Scanner myScanner = Scanner();
	myScanner.setFile(argv[1]);
	myScanner.scan();
	Parser myDatalog = Parser();
	myDatalog.setTokens(myScanner.getTokens());
	DatalogProgram myLog;
	try
	{
		std::vector<std::vector<Token>> data = myDatalog.parse();
		Deconstrucor convertor;
		convertor.setData(data);
		convertor.sort();
		myLog = convertor.getLog();
	}
	catch (Token error) { std::cout << "Failure!\n  " << error.toString(); }

	Interpreter myInterpreter =  Interpreter(myLog);
	Database myDatabase = myInterpreter.makeDatabase();


	return 0;
}