#include "mem_sim_memory.h"
#include <iostream>


Memory::Memory(
	unsigned addressBits,
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned memReadTime,
	unsigned memWriteTime
	) : bytesPerWord(bytesPerWord), wordsPerBlock(wordsPerBlock), memReadTime(memReadTime), memWriteTime(memWriteTime)
{
	blockLength = wordsPerBlock*bytesPerWord;
	memorySize = (1 << addressBits) / blockLength;
	bytesPerBlock = bytesPerWord*wordsPerBlock;
	data = new char*[memorySize];
	for (unsigned i = 0; i < memorySize; i++)
	{
		data[i] = new char[blockLength];
		for (unsigned j = 0; j < blockLength; j++)
			data[i][j] = 0;
	}
}

Memory::~Memory()
{
	for (unsigned i = 0; i < memorySize; i++)
		delete[] data[i];
	delete[] data;
}

void Memory::read(char dataOut[], unsigned address, unsigned loadLength)
{
	unsigned blockNumber = address / bytesPerBlock;
	unsigned offset = address - (blockNumber*bytesPerBlock);
	int blocksToLoad = ((offset + loadLength) / bytesPerBlock) + ((offset + loadLength) % bytesPerBlock != 0);
	while (blocksToLoad > 0)
	{
		readWord(dataOut, blockNumber);
		blocksToLoad--;
		dataOut += bytesPerBlock;
		blockNumber++;
	}

}

void Memory::write(char dataIn[], unsigned address, unsigned storeLength)
{
	unsigned blockNumber = address / bytesPerBlock;
	unsigned offset = address - (blockNumber*bytesPerBlock);
	int blocksToLoad = ((offset + storeLength) / bytesPerBlock) + ((offset + storeLength) % bytesPerBlock != 0);
	std::cout << std::dec << "Address " << address << " Storing in block " << blockNumber << " in word " << offset / bytesPerWord << std::endl;
	while (blocksToLoad > 0)
	{
		writeWord(dataIn, blockNumber);
		blocksToLoad--;
		dataIn += bytesPerBlock;
		blockNumber++;
	}
}

void Memory::readWord(char dataOut[], unsigned blockNumber)
{
	for (unsigned i = 0; i < bytesPerBlock; i++)
		dataOut[i] = data[blockNumber][i];
}

void Memory::writeWord(char dataIn[], unsigned blockNumber)
{
	for (unsigned i = 0; i < bytesPerBlock; i++)
		data[blockNumber][i] = dataIn[i];
}