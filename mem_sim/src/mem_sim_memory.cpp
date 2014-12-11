#include <stdexcept>
#include <sstream>
#include "mem_sim_memory.h"

Memory::Memory(
	unsigned addressBits,
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned memReadTime,
	unsigned memWriteTime,
	Utilities* utilities
	) : bytesPerWord(bytesPerWord), wordsPerBlock(wordsPerBlock), memReadTime(memReadTime), memWriteTime(memWriteTime), utilities(utilities)
{
	blockLength = wordsPerBlock*bytesPerWord;
	memoryCapacity = 1 << addressBits;
	memoryBlockSize = memoryCapacity / blockLength;
	bytesPerBlock = bytesPerWord*wordsPerBlock;
	data = new Byte*[memoryBlockSize];
	for (unsigned i = 0; i < memoryBlockSize; i++)
		data[i] = new Byte[blockLength];
}

Memory::~Memory()
{
	for (unsigned i = 0; i < memoryBlockSize; i++)
		delete[] data[i];
	delete[] data;
}

void Memory::read(Byte dataOut[], unsigned address, unsigned loadLength)
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
	utilities -> globalTime += memReadTime;

}

void Memory::write(Byte dataIn[], unsigned address, unsigned storeLength)
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
	utilities -> globalTime += memWriteTime;
}

void Memory::readWord(Byte dataOut[], unsigned blockNumber)
{
	for (unsigned i = 0; i < bytesPerBlock; i++)
		dataOut[i] = data[blockNumber][i];
}

void Memory::writeWord(Byte dataIn[], unsigned blockNumber)
{
	for (unsigned i = 0; i < bytesPerBlock; i++)
		data[blockNumber][i] = dataIn[i];
}

std::string Memory::buildOutOfMemoryString(int address, int length)
{
	std::stringstream ss;
	ss << "Error: Trying to access address " << address << " for " << length/2 << " bytes.";
	ss << "\n#Memory is only " << memoryCapacity/2 << " bytes large." << std::endl;
	return ss.str();
}