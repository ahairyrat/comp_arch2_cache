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
	memorySize = addressBits / blockLength;
	data = new Block*[memorySize];
	for (unsigned i = 0; i < memorySize; i++)
		data[i] = new Block(bytesPerWord, wordsPerBlock);
}


Memory::~Memory()
{
	for (unsigned i = 0; i < memorySize; i++)
		delete data[i];
	delete[] data;
}

void Memory::read(char dataOut[], unsigned address)
{
	unsigned blockNumber = address / blockLength;
	unsigned byteOffset = address % blockLength;
	for (int i = 0; i < )
	data[blockNumber]->load(dataOut, 0);
}

void Memory::write(char dataIn[], unsigned address)
{

}