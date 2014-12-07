
#include <sstream>
#include <iostream>
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
		addressBits = 5;
		bytesPerWord = 4;
		wordsPerBlock = 4;
		blocksPerSet = 1;
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
	std::stringstream dataTest;
	int bytesPerBlock = bytesPerWord*wordsPerBlock;
	int bytesToRead = 12;
	char  dataIn[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C', 'D', 'E', 'F', 'G'};
	char *dataOut= new char[bytesToRead+1];
	debugger.forceCache(&cache);
	cache.store(dataIn, 485, 0);
	debugger.printCache(dataTest, &cache);
	std::cout << dataTest.str() << std::endl;
	char dataIn2[] = { '0', '1', '2', '3', '4', '5', '6', 'T', '8', '9', 'G', 'F', 'X', 'R', 'E', 'F', 'G' };
	
	cache.store(dataIn2, 487, 0);
	dataTest.str("");
	debugger.printCache(dataTest, &cache);
	std::cout << dataTest.str() << std::endl;
	try{
		cache.load(dataOut, 485, 0);
		for (int i = 0; i < bytesToRead; i++)
			std::cout << dataOut[i];
		std::cout << std::endl;
	}
	catch (dataNotAvailableException e)
	{
		std::cout << e.what() << std::endl;
	}

	getchar();
	delete[] dataOut;
	return 0;
}