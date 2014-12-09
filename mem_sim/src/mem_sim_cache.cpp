
#include <iostream>
#include "mem_sim_cache.h"
#include "mem_sim_exceptions.h"

Cache::Cache(
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet,
	unsigned setsPerCache,
	unsigned hitTime,
	Memory* memory
	) : bytesPerWord(bytesPerWord), setsPerCache(setsPerCache),blocksPerSet(blocksPerSet), wordsPerBlock(wordsPerBlock), memory(memory){
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

void Cache::store(char dataIn[], unsigned byteAddress, unsigned numBytes)
{
	unsigned setIndex = (byteAddress/(blocksPerSet*wordsPerBlock*bytesPerWord))%setsPerCache;
	unsigned tag = byteAddress;
	unsigned offset = byteAddress%(wordsPerBlock*bytesPerWord);
	try{
		set[setIndex]->storeFromCpu(dataIn, tag, offset, numBytes);
	}
	catch (dataNotAvailableException e)
	{
		std::cout << e.what() << std::endl;
		storeFromMemory(byteAddress);
		store(dataIn, byteAddress, numBytes);
	}
	catch (dataSplitException e)
	{
		std::cout << e.what() << std::endl;
		unsigned remByteAddress = byteAddress + e.dataRead();
		store(dataIn + e.dataRead(), remByteAddress, numBytes - e.dataRead())
	}
}

void Cache::load(char dataOut[], unsigned byteAddress, unsigned numBytes)
{
	//Fix this
	/*
	unsigned setIndex = (byteAddress/(blocksPerSet*wordsPerBlock*bytesPerWord))%setsPerCache;
	unsigned tag = byteAddress;
	unsigned offset = byteAddress%(wordsPerBlock*bytesPerWord);
	set[setIndex]->loadToCpu(dataOut, tag, offset, bytesPerWord);
	*/
}

void Cache::storeFromMemory(unsigned byteAddress)
{
	unsigned setIndex = (byteAddress / (blocksPerSet*wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress;
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	char* data = new char[bytesPerWord*wordsPerBlock];
	memory->read(data, byteAddress - offset, (bytesPerWord*wordsPerBlock));
	try{
		set[setIndex]->storeFromMemory(data, tag, NULL);
	}
	catch (dataIsDirtyException e)
	{
		std::cout << e.what() << std::endl;
		loadToMemory(byteAddress, e.dirtyLocation());
		set[setIndex]->storeFromMemory(data, tag, e.dirtyLocation());
	}
	delete[] data;
}

void Cache::loadToMemory(unsigned byteAddress, void* block)
{
	unsigned setIndex = (byteAddress / (blocksPerSet*wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress;
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	char* data = new char[bytesPerWord*wordsPerBlock];
	set[setIndex]->loadToMemory(data, block, bytesPerWord*wordsPerBlock);
	memory->write(data, byteAddress, bytesPerWord*wordsPerBlock);
}