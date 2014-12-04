
#include "mem_sim_cache.h"

Cache::Cache(
	unsigned addressBits,
	unsigned bytesPerWord,
	unsigned wordsPerBlock,
	unsigned blocksPerSet,
	unsigned setsPerCache,
	unsigned hitTime
	) : setsPerCache(setsPerCache){
	set = new Set*[setsPerCache];
	for (unsigned i = 0; i < setsPerCache; i++)
		set[i] = new Set(
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet
		);
}

Cache::~Cache() {
	for (unsigned i = 0; i < setsPerCache; i++)
		delete set[i];
	delete[] set;
}

void Cache::load()
{

}

void Cache::store()
{

}