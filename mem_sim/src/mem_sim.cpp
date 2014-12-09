
#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream>
#include <vector>
#include "mem_sim_cache.h"
#include "mem_sim_memory.h"
#include "mem_sim_parser.h"
#include "mem_sim_debugger.h"
#include "mem_sim_exceptions.h"


int main(int argc, char *argv[]){
	unsigned addressBits;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned blocksPerSet;
	unsigned setsPerCache;
	unsigned hitTime;
	unsigned memReadTime;
	unsigned memWriteTime;
	if (argc < 8)
	{
		addressBits = 10;
		bytesPerWord = 8;
		wordsPerBlock = 4;
		blocksPerSet = 2;
		setsPerCache = 10;
		hitTime = 0;
		memReadTime = 0;
		memWriteTime = 0;
	}
	else
	{
		addressBits = *(unsigned*)argv[0];
		bytesPerWord = *(unsigned*)argv[1];
		wordsPerBlock = *(unsigned*)argv[2];
		blocksPerSet = *(unsigned*)argv[3];
		setsPerCache = *(unsigned*)argv[4];
		hitTime = *(unsigned*)argv[5];
		memReadTime = *(unsigned*)argv[6];
		memWriteTime = *(unsigned*)argv[7];
	}

	Memory memory(
		addressBits,
		bytesPerWord,
		wordsPerBlock,
		memReadTime,
		memWriteTime
		);

	Cache cache(
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet,
		setsPerCache,
		hitTime,
		&memory
	);
	Debugger debugger;
	Parser parser;
	bool endOfInput = false;
	std::string commandString;
	std::string command;
	std::vector<std::string> commandTokens;
	std::stringstream debugStream;
	while (!endOfInput)
	{
		getline(std::cin, commandString);
		try{
			commandTokens = parser.parse(commandString);
			command = commandTokens[0];
			debugStream << commandString << std::endl;
			if (command == "read-req" || command == "READ-REQ")
			{
				if (commandTokens.size() != 2)
				{
					std::stringstream ss;
					ss << "read-req requires 1 input, ";
					ss << (commandTokens.size() - 1 <= 0 ? 0 : commandTokens.size() - 1);
					ss << " specified";
					throw invalidinputException(ss.str().c_str());
				}
				char* dataOut = new char[bytesPerWord];
				std::cout << "read-ack" << std::endl;
				unsigned address = (unsigned)(stoi(commandTokens[1]));
				std::cout << address << std::endl;
				cache.load(dataOut, address, bytesPerWord);
				for (unsigned i = 0; i < bytesPerWord; i++)
					std::cout << std::hex << (unsigned)dataOut[i];
				std::cout << std::endl;
				debugger.printCache(debugStream, &cache);
				delete[] dataOut;
			}
			else if (command == "write-req" || command == "WRITE-REQ")
			{
				if(commandTokens.size() != 3)
				{
					std::stringstream ss;
					ss << "write-req requires 2 inputs, ";
					ss << (commandTokens.size() - 1 <= 0 ? 0 : commandTokens.size() - 1);
					ss << " specified";
					throw invalidinputException(ss.str().c_str());
				}
				std::cout << "write-ack" << std::endl;
				unsigned address = (unsigned)(stoi(commandTokens[1]));
				std::cout << address << std::endl;
				char * writable = new char[commandTokens[2].size() + 1];
				strcpy(writable, commandTokens[2].c_str());
				writable[commandTokens[2].size()] = '\0';
				cache.store(writable, address, bytesPerWord);
				debugger.printCache(debugStream, &cache);
				delete[] writable;
			}
			else if (command == "flush-req" || command == "FLUSH-REQ")
			{
				if (commandTokens.size() != 1)
				{
					std::stringstream ss;
					ss << "flush-req requires 0 inputs, ";
					ss << (commandTokens.size() - 1 <= 0 ? 0 : commandTokens.size() - 1);
					ss << " specified";
					throw invalidinputException(ss.str().c_str());
				}
				std::cout << "flush-ack" << std::endl;
			}
			else if (command == "debug-req" || command == "DEBUG-REQ")
			{
				if (commandTokens.size() != 1)
				{
					std::stringstream ss;
					ss << "debug-req requires 0 inputs, ";
					ss << (commandTokens.size() - 1 <= 0 ? 0 : commandTokens.size() - 1);
					ss << " specified";
					throw invalidinputException(ss.str().c_str());
				}
				std::cout << "debug-ack" << std::endl;
				std::cout << debugStream.str() << std::endl;
				debugStream.str("");
			}
			else if (commandString == "\n")
			{
				endOfInput = true;
			}
		}
		catch (invalidinputException e)
		{
			std::cout << e.what() << std::endl;
		}
		commandString.clear();
		command.clear();
	}
}