

#include "mem_sim_parser.h"
#include "mem_sim_exceptions.h"


Parser::Parser()
{
}


Parser::~Parser()
{
}


std::vector<std::string> Parser::parse(std::string inputString)
{
	std::vector <std::string> result;
	result = tokenize(inputString);
	if (result[0] != "read-req" &&
		result[0] != "write-req" &&
		result[0] != "flush-req" &&
		result[0] != "debug-req" &&
		result[0] != "#")
		throw invalidinputException(inputString.c_str());
	return result;
}


std::vector<std::string> Parser::tokenize(std::string inputString)
{
	std::vector<std::string> result;
	unsigned begin = 0;
	unsigned end = 0;
	while (begin < inputString.length() && end < inputString.length())
	{
		end = inputString.find(" ");
		result.push_back(inputString.substr(begin, end - begin));
		begin = end + 1;
	}
	return result;
}
