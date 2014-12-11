
#include "mem_sim_cache.h"
#include "mem_sim_exceptions.h"
#include "mem_sim_utilities.h"

Cache::Cache(
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet,
	unsigned setsPerCache,
	unsigned hitTime,
	Memory* memory,
	Utilities* utilities
	) : bytesPerWord(bytesPerWord), setsPerCache(setsPerCache),blocksPerSet(blocksPerSet), wordsPerBlock(wordsPerBlock), hitTime(hitTime), memory(memory), utilities(utilities){
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

void Cache::store(Byte dataIn[], unsigned byteAddress, unsigned numBytes)
{
	unsigned setIndex = (byteAddress/(wordsPerBlock*bytesPerWord))%setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress%(wordsPerBlock*bytesPerWord);
	try{
		set[setIndex]->storeFromCpu(dataIn, tag, offset, numBytes);
		utilities -> globalSetsUsed << (int)setIndex << " ";
		utilities->globalHit = true;
		utilities->globalTime += hitTime;
	}
	catch (dataNotAvailableException e)
	{
		storeFromMemory(byteAddress);
		store(dataIn, byteAddress, numBytes);
		utilities->globalHit = false;
	}
	catch (dataSplitException e)
	{
		store(dataIn + e.dataUsed(), byteAddress + e.dataUsed(), numBytes - e.dataUsed());
	}
}

void Cache::load(Byte dataOut[], unsigned byteAddress, unsigned numBytes)
{
	unsigned setIndex = (byteAddress / (wordsPerBlock*bytesPerWord)) % setsPerCache;
	unsigned tag = byteAddress / (wordsPerBlock*bytesPerWord);
	unsigned offset = byteAddress % (wordsPerBlock*bytesPerWord);
	try{
		set[setIndex]->loadToCpu(dataOut, tag, offset, numBytes);
		utilities->globalSetsUsed << (int)setIndex << " ";
		utilities->globalHit = true;
		utilities->globalTime += hitTime;
	}
	catch (dataNotAvailableException e)
	{
		storeFromMemory(byteAddress);
		load(dataOut, byteAddress, numBytes);
		utilities->globalHit = false;
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
	Byte* data = new Byte[bytesPerWord*wordsPerBlock];
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
	Byte* data = new Byte[bytesPerWord*wordsPerBlock];
	set[setIndex]->loadToMemory(data, block, bytesPerWord*wordsPerBlock);
	memory->write(data, byteAddress, bytesPerWord*wordsPerBlock);
}