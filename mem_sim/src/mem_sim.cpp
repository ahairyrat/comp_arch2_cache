
//////////////////////////////////////////////////////////////////////////////////////////////////
//This are default setting for initialisation should no parameters be given
#define ADDRESSBITS 9
#define BYTESPERWORD 4
#define WORDSPERBLOCK 4
#define BLOCKSPERSET 2
#define SETSPERCACHE 10
#define HITTIME 2
#define MEMREADTIME 3
#define MEMWRITETIME 4

#define DEBUGBUFFERSIZE 20
//////////////////////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream>
#include <vector>
#include "mem_sim_cache.h"
#include "mem_sim_memory.h"
#include "mem_sim_parser.h"
#include "mem_sim_debugger.h"
#include "mem_sim_exceptions.h"
#include "mem_sim_byte.h"
#include "mem_sim_utilities.h"

int convertToInt(char charIn);
int convertToInt(std::string stringIn);
void printByteString(Byte* string, int size);
std::string buildInvalidStringStream(std::string instruction, int inputParameters);

int main(int argc, char *argv[]){
	unsigned addressBits;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned blocksPerSet;
	unsigned setsPerCache;
	unsigned hitTime;
	unsigned memReadTime;
	unsigned memWriteTime;

	//Extracting of input information

	if (argc < 8)
	{
		addressBits = ADDRESSBITS;
		bytesPerWord = BYTESPERWORD;
		wordsPerBlock = WORDSPERBLOCK;
		blocksPerSet = BLOCKSPERSET;
		setsPerCache = SETSPERCACHE;
		hitTime = HITTIME;
		memReadTime = MEMREADTIME;
		memWriteTime = MEMWRITETIME;
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

	//initialisation of required objects
	Utilities utilities;

	Memory memory(
		addressBits,
		bytesPerWord,
		wordsPerBlock,
		memReadTime,
		memWriteTime,
		&utilities
		);

	Cache cache(
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet,
		setsPerCache,
		hitTime,
		&memory,
		&utilities
	);

	Debugger debugger;
	Parser parser;
	

	// end of initialisation


	bool endOfInput = false;
	std::string commandString;
	std::string command;
	std::vector<std::string> commandTokens;
	unsigned debugStored = DEBUGBUFFERSIZE;
	std::stringstream debugStream;

	while (!endOfInput)
	{
		getline(std::cin, commandString);
		utilities.globalHit = true;
		utilities.globalSetsUsed.str("");
		utilities.globalTime = 0;
		try{
			if (commandString.size() == 0)
				break;
			commandTokens = parser.parse(commandString);
			command = commandTokens[0];
			debugStream << commandString << std::endl;
			if (command == "read-req" || command == "READ-REQ")
			{
				if (commandTokens.size() != 2)
					throw invalidInputException(buildInvalidStringStream("read-req", commandTokens.size()-1).c_str());

				std::cout << "read-ack ";
				unsigned address = convertToInt(commandTokens[1]);

				Byte* dataOut = new Byte[2];
				cache.load(dataOut, address, 1);
				printByteString(dataOut, 1);
				std::cout << utilities.globalSetsUsed.str();
				if (utilities.globalHit)
					std::cout << "HIT ";
				else
					std::cout << "MISS ";
				std::cout << utilities.globalTime << std::endl;
				debugger.printCache(debugStream, &cache);
				debugStored--;
				delete[] dataOut;
			}
			else if (command == "write-req" || command == "WRITE-REQ")
			{
				if(commandTokens.size() != 3)
					throw invalidInputException(buildInvalidStringStream("write-req", commandTokens.size() - 1).c_str());

				std::cout << "write-ack ";

				unsigned address = convertToInt(commandTokens[1]);
				char * writable = new char[2*bytesPerWord + 1];
				Byte * dataIn = new Byte[bytesPerWord];
				strcpy(writable, commandTokens[2].c_str());
				unsigned count = 0;
				for (unsigned i = 0; i < bytesPerWord; i++)
				{
					dataIn[i].setByte(convertToInt(writable[count]), convertToInt(writable[count+1]));
					count += 2;
				}
				cache.store(dataIn, address, bytesPerWord);
				std::cout << utilities.globalSetsUsed.str();
				if (utilities.globalHit)
					std::cout << "HIT ";
				else
					std::cout << "MISS ";
				std::cout << utilities.globalTime << " " << commandTokens[2] << std::endl;
				debugger.printCache(debugStream, &cache);
				debugStored--;
				delete[] writable;
			}
			else if (command == "flush-req" || command == "FLUSH-REQ")
			{
				if (commandTokens.size() != 1)
					throw invalidInputException(buildInvalidStringStream("flush-req", commandTokens.size() - 1).c_str());

				std::cout << "flush-ack ";
				cache.flush();
				std::cout << utilities.globalTime << std::endl;
				debugger.printCache(debugStream, &cache);
			}
			else if (command == "debug-req" || command == "DEBUG-REQ")
			{
				if (commandTokens.size() != 1)
					throw invalidInputException(buildInvalidStringStream("debug-req", commandTokens.size() - 1).c_str());

				std::cout << "debug-ack-begin" << std::endl;
				std::cout << debugStream.str() << std::endl;
				std::cout << "debug-ack-end" << std::endl;
				debugStream.str("");
			}
		}
		catch (invalidInputException e)
		{
			std::cout << "# " << e.what() << std::endl;
		}
		catch (std::out_of_range e)
		{
			std::cout << "# " << e.what() << std::endl;
		}
		if (debugStored == 0)
			debugStream.str("");
		commandString.clear();
		command.clear();
	}
}

int convertToInt(char charIn)
{
	int x;
	std::stringstream ss;
	ss << std::hex << charIn;
	ss >> x;
	return x;
}

int convertToInt(std::string stringIn)
{
	int x;
	std::stringstream ss;
	ss << std::dec << stringIn;
	ss >> x;
	return x;

}

void printByteString(Byte* string, int size)
{
	for (int i = 0; i < size; i++)
		std::cout << std::hex << (unsigned)string[i].data[0] << (unsigned)string[i].data[1];
	std::cout << " ";
}

std::string buildInvalidStringStream(std::string instruction, int inputParameters)
{
	std::stringstream ss;
	ss << "# Error: " << instruction << " requires ";
	if (instruction == "read-req")
		ss << "1";
	else if (instruction == "write-req")
		ss << "2";
	else
		ss << "0";
	ss << " inputs, " << (inputParameters <= 0 ? 0 : inputParameters)<< " specified";
	return ss.str();
}