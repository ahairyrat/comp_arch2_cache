
#include "mem_sim_set.h"
#include "mem_sim_exceptions.h"

Set::Set(
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet
	) : blocksPerSet(blocksPerSet)
{
	block = new Block*[blocksPerSet+1];
	for (unsigned i = 0; i < blocksPerSet; i++)
		block[i] = new Block(
		bytesPerWord,
		wordsPerBlock
		);
	block[blocksPerSet] = '\0';
}

Set::~Set() {
	for (unsigned i = 0; i < blocksPerSet; i++)
		delete block[i];
	delete[] block;
}

//Normal version
void Set::storeFromCpu(char dataIn[], unsigned tag, unsigned byteOffset, int bytesToStore)
{
	Block* storeBlock = findBlock(tag);
	if (storeBlock == NULL)
		throw dataNotAvailableException("Unable to store, data not in cache");
	storeBlock->store(dataIn, byteOffset, bytesToStore);
	incrementUnused(storeBlock);
}

//version for access to memory
void Set::storeFromMemory(char dataIn[], unsigned tag, void* blockRef)
{
	Block* storeBlock;
	if (blockRef == NULL)
	{
		storeBlock = findBlock(tag);
		if(storeBlock == NULL)
		{
			storeBlock = findLRU();
			if (storeBlock == NULL)
				storeBlock = findEmpty();
		}
	}
	else
		storeBlock = (Block*)blockRef;
	if (storeBlock->isDirty() && storeBlock->isValid())
		throw dataIsDirtyException("Unable to store, data must be updated", storeBlock, storeBlock ->getTag());
	storeBlock->update(dataIn, tag);
}

//Normal version
void Set::loadToCpu(char dataOut[], unsigned tag, unsigned byteOffset, int bytesToLoad)
{
	Block* loadBlock = findBlock(tag);
	if (loadBlock == NULL)
		throw dataNotAvailableException("Unable to load, data not in cache");
	loadBlock->load(dataOut, byteOffset, bytesToLoad);
	incrementUnused(loadBlock);
}

//version for access to memory
void Set::loadToMemory(char dataOut[], void* blockRef, int bytesToLoad)
{
	Block* loadBlock = (Block*)blockRef;
	loadBlock->load(dataOut, 0, bytesToLoad);
	loadBlock->setDirty(false);
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