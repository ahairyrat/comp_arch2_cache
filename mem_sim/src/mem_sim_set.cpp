
#include "mem_sim_set.h"
#include "mem_sim_exceptions.h"

Set::Set(
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet
	) : blocksPerSet(blocksPerSet)
{
	block = new Block*[blocksPerSet];
	for (unsigned i = 0; i < blocksPerSet; i++)
		block[i] = new Block(
		bytesPerWord,
		wordsPerBlock
		);
}
Set::~Set() {
	for (unsigned i = 0; i < blocksPerSet; i++)
		delete block[i];
	delete[] block;
}
//Normal version
void Set::storeFromCpu(char dataIn[], unsigned tag, unsigned byteOffset)
{
	Block* storeBlock = findBlock(tag);
	if (storeBlock == NULL)
		throw dataNotAvailableException("Unable to store, data not in cache");
	storeBlock->store(dataIn, byteOffset);
}

//version for access to memory
void Set::storeFromMemory(char dataIn[], unsigned tag, void* blockRef)
{
	Block* storeBlock;
	if (blockRef == NULL)
	{
		storeBlock = findLRU();
		if (storeBlock == NULL)
			storeBlock = findEmpty();
	}
	else
		storeBlock = (Block*)blockRef;
	if (storeBlock->isDirty())
		throw dataIsDirtyException("Unable to store, data must be updated", storeBlock);
	storeBlock->update(dataIn, tag);
}

void Set::load(char dataOut[], unsigned tag, unsigned byteOffset)
{
	Block* loadBlock = findBlock(tag);
	if (loadBlock == NULL)
		throw dataNotAvailableException("Unable to load, data not in cache");
	loadBlock->load(dataOut, byteOffset);
}

void Set::load(char dataOut[], void* blockRef)
{
	Block* loadBlock = (Block*)blockRef;
	loadBlock->load(dataOut, 0);
}

Block* Set::findBlock(unsigned tag)
{
	for (unsigned i = 0; i < blocksPerSet; i++)
		if (block[i]->isValid() && block[i]->getTag() == tag)
			return block[i];
	return NULL;
}

Block* Set::findLRU()
{
	int maxUnusedTime = 0;
	Block* lastUsedBlock = NULL;
	for (unsigned i = 0; i < blocksPerSet; i++)
		if (block[i]->isValid() && block[i]->getUnusedTime() > maxUnusedTime)
		{
		lastUsedBlock = block[i];
		maxUnusedTime = block[i]->getUnusedTime();
		}
	return lastUsedBlock;
}

Block* Set::findEmpty()
{
	for (unsigned i = 0; i < blocksPerSet; i++)
		if (!block[i]->isValid())
			return block[i];
	return block[0];
}

void Set::incrementUnused(Block* usedBlock)
{
	for (unsigned i = 0; i < blocksPerSet; i++)
		if (block[i] != usedBlock)
			block[i]->notUsed();
}