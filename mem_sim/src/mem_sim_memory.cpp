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
	memorySize = (2 << addressBits) / blockLength;
	bytesPerBlock = bytesPerWord*wordsPerBlock;
	char* initial = new char[bytesPerBlock];
	for (int i = 0; i < bytesPerBlock; i++)
		initial[i] = 0;
	data = new Block*[memorySize];
	for (unsigned i = 0; i < memorySize; i++)
	{
		data[i] = new Block(bytesPerWord, wordsPerBlock);
		data[i]->store(initial, 0, bytesPerBlock);
		data[i]->setValid(true);
		data[i]->setDirty(false);
	}
	delete[] initial;
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
	int bytesPerBlockCopy = bytesPerBlock;
	data[blockNumber]->load(dataOut, 0, bytesPerBlockCopy);
}

void Memory::write(char dataIn[], unsigned address)
{

}