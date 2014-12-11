
#ifndef MEM_SIM_PARSER_
#define MEM_SIM_PARSER_

#include <string>
#include <vector>

//A basic parser/tokenizer for input strings
class Parser
{
public:
	Parser();
	virtual ~Parser();
	std::vector<std::string> parse(std::string inputString);
private:
	std::vector<std::string> tokenize(std::string inputString);
};

#endif
