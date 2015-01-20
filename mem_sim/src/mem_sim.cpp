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

//stops windows from warning about unsafe functions
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

int convertToInt(char* charIn, bool hex);
int convertToInt(std::string stringIn, bool hex);
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

	if (argc < 9)
	{
		std::cout << "#Initialisation values not found. Defaulting ..." << std::endl;
		addressBits = ADDRESSBITS;
		bytesPerWord = BYTESPERWORD;
		wordsPerBlock = WORDSPERBLOCK;
		blocksPerSet = BLOCKSPERSET;
		setsPerCache = SETSPERCACHE;
		hitTime = HITTIME;
		memReadTime = MEMREADTIME;
		memWriteTime = MEMWRITETIME;
		std::cout << "#Default values loaded" << std::endl;
	}
	else
	{
		std::cout << "#Initialisation values found. Loading ..." << std::endl;
		addressBits = convertToInt(argv[1], false);
		bytesPerWord = convertToInt(argv[2], false);
		wordsPerBlock = convertToInt(argv[3], false);
		blocksPerSet = convertToInt(argv[4], false);
		setsPerCache = convertToInt(argv[5], false);
		hitTime = convertToInt(argv[6], false);
		memReadTime = convertToInt(argv[7], false);
		memWriteTime = convertToInt(argv[8], false);
		std::cout << "#Initialisation values loaded" << std::endl;
	}

	//initialisation of required objects
	Utilities utilities;
	std::cout << "#Initialising memory ..." << std::endl;
	Memory memory(
		addressBits,
		bytesPerWord,
		wordsPerBlock,
		memReadTime,
		memWriteTime,
		&utilities
		);
	std::cout << "#Memory initialised" << std::endl;
	std::cout << "#Initialising cache ..." << std::endl;
	Cache cache(
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet,
		setsPerCache,
		hitTime,
		&memory,
		&utilities
		);
	std::cout << "#Cache initialised" << std::endl;
	Debugger debugger;
	Parser parser;

	//All required class constructed

	//Creating various data
	bool endOfInput = false;
	std::string commandString;
	std::string command;
	std::vector<std::string> commandTokens;
	unsigned debugStored = DEBUGBUFFERSIZE;
	std::stringstream debugStream;
	std::cout << "#Initialisation complete\n#Beginning command processing" << std::endl;

	// end of initialisation

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
					throw invalidInputException(buildInvalidStringStream("read-req", commandTokens.size() - 1).c_str());

				std::cout << "read-ack ";
				unsigned address = convertToInt(commandTokens[1], false);

				Byte* dataOut = new Byte[2];
				cache.load(dataOut, address, 1);
				std::cout << utilities.globalSetsUsed.str();
				if (utilities.globalHit)
					std::cout << "hit ";
				else
					std::cout << "miss ";
				std::cout << utilities.globalTime << " ";
				printByteString(dataOut, 1);
				std::cout << std::endl;
				debugger.printCache(debugStream, &cache);
				debugStored--;
				delete[] dataOut;
			}
			else if (command == "write-req" || command == "WRITE-REQ")
			{
				if (commandTokens.size() != 3)
					throw invalidInputException(buildInvalidStringStream("write-req", commandTokens.size() - 1).c_str());

				std::cout << "write-ack ";

				unsigned address = convertToInt(commandTokens[1], false);
				char * writable = new char[2 * bytesPerWord + 1];
				Byte * dataIn = new Byte[bytesPerWord];
				strcpy(writable, commandTokens[2].c_str());
				unsigned count = 0;
				for (unsigned i = 0; i < bytesPerWord; i++)
				{
					dataIn[i].setByte(convertToInt(&writable[count], true), convertToInt(&writable[count + 1], true));
					count += 2;
				}
				cache.store(dataIn, address, bytesPerWord);
				std::cout << utilities.globalSetsUsed.str();
				if (utilities.globalHit)
					std::cout << "hit ";
				else
					std::cout << "miss ";
				std::cout << utilities.globalTime << std::endl;
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
				//debug will print all instructions and chache states since the last debug request/ beginning of program
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
			std::cout << "\n" << e.what() << std::endl;
		}
		catch (std::out_of_range e)
		{
			std::cout << "\n" << e.what() << std::endl;
		}
		//If the buffer is full, clear it and restart
		if (debugStored == 0)
			debugStream.str("");
		commandString.clear();
		command.clear();
	}
	std::cout << "#Exiting ..." << std::endl;
}

//Allows only values 0 - f hex as inputs
int convertToInt(char *charIn, bool hex)
{
	int x;
	std::stringstream ss;
	for (unsigned i = 0; i < sizeof(charIn) / sizeof(charIn[0]); i++)
	{
		if (hex)
			ss << std::hex << charIn[i];
		else
			ss << std::dec << charIn[i];
	}
	ss >> x;
	return x;
}

int convertToInt(std::string stringIn, bool hex)
{
	int x;
	std::stringstream ss;
	if (hex)
		ss << std::hex << stringIn;
	else
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
	ss << "#Error: " << instruction << " requires ";
	if (instruction == "read-req")
		ss << "1";
	else if (instruction == "write-req")
		ss << "2";
	else
		ss << "0";
	ss << " inputs, " << (inputParameters <= 0 ? 0 : inputParameters) << " specified";
	return ss.str();
}