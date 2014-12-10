
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
	unsigned setIndex = (byteAddress/(wordsPerBlock*bytesPerWord))%setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress%(wordsPerBlock*bytesPerWord);
	try{
		set[setIndex]->storeFromCpu(dataIn, tag, offset, numBytes);
	}
	catch (dataNotAvailableException e)
	{
		storeFromMemory(byteAddress);
		store(dataIn, byteAddress, numBytes);
	}
	catch (dataSplitException e)
	{
		store(dataIn + e.dataUsed(), byteAddress + e.dataUsed(), numBytes - e.dataUsed());
	}
}

void Cache::load(char dataOut[], unsigned byteAddress, unsigned numBytes)
{
	unsigned setIndex = (byteAddress / (wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	try{
		set[setIndex]->loadToCpu(dataOut, tag, offset, numBytes);
	}
	catch (dataNotAvailableException e)
	{
		storeFromMemory(byteAddress);
		load(dataOut, byteAddress, numBytes);
	}
	catch (dataSplitException e)
	{
		load(dataOut + e.dataUsed(), byteAddress + e.dataUsed(), numBytes - e.dataUsed());
	}
}

void Cache::flush()
{
	for (unsigned i = 0; i < setsPerCache; i++)
	{
		try
		{
			set[i]->flush();
		}
		catch (dataIsDirtyException e)
		{
			unsigned storeAddress = e.getTag()*wordsPerBlock*bytesPerWord;
			loadToMemory(storeAddress, e.dirtyLocation());
			i--;
		}
	}
}

void Cache::storeFromMemory(unsigned byteAddress)
{
	unsigned setIndex = (byteAddress / (wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	char* data = new char[bytesPerWord*wordsPerBlock];
	memory->read(data, byteAddress - offset, (bytesPerWord*wordsPerBlock));
	try{
		set[setIndex]->storeFromMemory(data, tag, NULL);
	}
	catch (dataIsDirtyException e)
	{
		unsigned storeAddress = e.getTag()*wordsPerBlock*bytesPerWord;
		loadToMemory(storeAddress, e.dirtyLocation());
		set[setIndex]->storeFromMemory(data, tag, e.dirtyLocation());
	}
	delete[] data;
}

void Cache::loadToMemory(unsigned byteAddress, void* block)
{
	unsigned setIndex = (byteAddress / (wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	char* data = new char[bytesPerWord*wordsPerBlock];
	set[setIndex]->loadToMemory(data, block, bytesPerWord*wordsPerBlock);
	memory->write(data, byteAddress, bytesPerWord*wordsPerBlock);
}