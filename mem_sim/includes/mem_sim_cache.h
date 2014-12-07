
#ifndef MEM_SIM_CACHE_H_
#define MEM_SIM_CACHE_H_

#include "mem_sim_set.h"
#include "mem_sim_memory.h"

class Cache {
	friend class Debugger;
public:
	Cache(
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned blocksPerSet,
		unsigned setsPerCache,
		unsigned hitTime,
		Memory* memory
		);
	virtual ~Cache();

	void store(char dataIn[], unsigned address, unsigned byteoffset);
	void load(char dataOut[], unsigned address, unsigned byteoffset);


private:
	Set** set;
	unsigned setsPerCache;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned blocksPerSet;
	Memory* memory;

	void storeFromMemory(unsigned address);
	void loadToMemory(unsigned address, void* block);
};

#endif /* MEM_SIM_CACHE_H_ */
