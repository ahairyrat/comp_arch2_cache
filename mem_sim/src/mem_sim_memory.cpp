#include <stdexcept>
#include <sstream>
#include "mem_sim_memory.h"

Memory::Memory(
	unsigned addressBits,
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned memReadTime,
	unsigned memWriteTime
	) : bytesPerWord(bytesPerWord), wordsPerBlock(wordsPerBlock), memReadTime(memReadTime), memWriteTime(memWriteTime)
{
	blockLength = wordsPerBlock*bytesPerWord;
	memoryCapacity = 1 << addressBits;
	memoryBlockSize = memoryCapacity / blockLength;
	bytesPerBlock = bytesPerWord*wordsPerBlock;
	data = new char*[memoryBlockSize];
	for (unsigned i = 0; i < memoryBlockSize; i++)
	{
		data[i] = new char[blockLength];
		for (unsigned j = 0; j < blockLength; j++)
			data[i][j] = 0;
	}
}

Memory::~Memory()
{
	for (unsigned i = 0; i < memoryBlockSize; i++)
		delete[] data[i];
	delete[] data;
}

void Memory::read(char dataOut[], unsigned address, unsigned loadLength)
{
	if (address + loadLength > memoryCapacity)
		throw std::out_of_range(buildOutOfMemoryString(address, loadLength));
	unsigned blockNumber = address / bytesPerBlock;
	unsigned offset = address - (blockNumber*bytesPerBlock);
	int blocksToLoad = ((offset + loadLength) / bytesPerBlock)
		+ ((offset + loadLength) % bytesPerBlock != 0);
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
	if (address + storeLength > memoryCapacity)
		throw std::out_of_range(buildOutOfMemoryString(address, storeLength));
	unsigned blockNumber = address / bytesPerBlock;
	unsigned offset = address - (blockNumber*bytesPerBlock);
	int blocksToLoad = ((offset + storeLength) / bytesPerBlock)
		+ ((offset + storeLength) % bytesPerBlock != 0);
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

std::string Memory::buildOutOfMemoryString(int address, int length)
{
	std::stringstream ss;
	ss << "Error: Trying to access address " << address << " for " << length << " bytes.";
	ss << "\nMemory is only " << memoryCapacity << " bytes large." << std::endl;
	return ss.str();
}