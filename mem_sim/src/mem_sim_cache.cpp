
#include "mem_sim_cache.h"
#include "mem_sim_exceptions.h"

Cache::Cache(
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet,
	unsigned setsPerCache,
	unsigned hitTime,
	Memory* memory
	) : setsPerCache(setsPerCache),blocksPerSet(blocksPerSet), wordsPerBlock(wordsPerBlock), memory(memory){
	set = new Set*[setsPerCache+1];
	for (unsigned i = 0; i < setsPerCache; i++)
		set[i] = new Set(
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet
		);
	set[setsPerCache] = '\0';
}

Cache::~Cache() {
	for (unsigned i = 0; i < setsPerCache; i++)
		delete set[i];
	delete[] set;
}

void Cache::store(char dataIn[], unsigned address, unsigned byteOffset)
{
	unsigned setIndex = address%setsPerCache;
	unsigned tag = address;
	unsigned wordOffset = address%wordsPerBlock;
	unsigned offset = (wordOffset*bytesPerWord) + byteOffset;
	try{
		set[setIndex]->storeFromCpu(dataIn, tag, offset, bytesPerWord);
	}
	catch (dataNotAvailableException e)
	{
		storeFromMemory(address);
		set[setIndex]->storeFromCpu(dataIn, tag, offset, bytesPerWord);
	}
}

void Cache::load(char dataOut[], unsigned address, unsigned byteOffset)
{
	unsigned setIndex = address%setsPerCache;
	unsigned tag = address%blocksPerSet;
	unsigned wordOffset = address%wordsPerBlock;
	unsigned offset = (wordOffset*bytesPerWord) + byteOffset;
	set[setIndex]->loadToCpu(dataOut, tag, offset, bytesPerWord);
}

void Cache::storeFromMemory(unsigned address)
{
	unsigned setIndex = address%setsPerCache;
	unsigned tag = address;
	unsigned wordOffset = address%wordsPerBlock;
	unsigned offset = wordOffset*bytesPerWord;
	char* data = new char[bytesPerWord*wordsPerBlock];
	memory->read(data, address-offset);
	try{
		set[setIndex]->storeFromMemory(data, tag, NULL);
	}
	catch (dataIsDirtyException e)
	{
		loadToMemory(address, e.dirtyLocation());
		set[setIndex]->storeFromMemory(data, tag, e.dirtyLocation());
	}
	delete[] data;
}

void Cache::loadToMemory(unsigned address, void* block)
{
	unsigned setIndex = address%setsPerCache;
	unsigned tag = address;
	unsigned wordOffset = address%wordsPerBlock;
	unsigned offset = wordOffset*bytesPerWord;
	char* data = new char[bytesPerWord*wordsPerBlock];
	set[setIndex]->loadToMemory(data, block, bytesPerWord*wordsPerBlock);
	memory->write(data, address);
}