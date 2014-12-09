
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

	void store(char dataIn[], unsigned byteAddress, unsigned numBytes);
	void load(char dataOut[], unsigned byteAddress, unsigned numBytes);


private:
	Set** set;
	unsigned setsPerCache;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned blocksPerSet;
	Memory* memory;

	void storeFromMemory(unsigned byteAddress);
	void loadToMemory(unsigned byteAddress, void* block);
};

#endif /* MEM_SIM_CACHE_H_ */
