
#ifndef MEM_SIM_CACHE_H_
#define MEM_SIM_CACHE_H_

#include "mem_sim_set.h"
#include "mem_sim_memory.h"
#include "mem_sim_byte.h"
#include "mem_sim_utilities.h"

class Cache {
	friend class Debugger;
public:
	Cache(
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned blocksPerSet,
		unsigned setsPerCache,
		unsigned hitTime,
		Memory* memory,
		Utilities* utilities
		);
	virtual ~Cache();

	void store(Byte dataIn[], unsigned byteAddress, unsigned numBytes);
	void load(Byte dataOut[], unsigned byteAddress, unsigned numBytes);

	void flush();


private:
	Set** set;
	unsigned setsPerCache;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned blocksPerSet;

	unsigned hitTime;

	Memory* memory;
	Utilities* utilities;

	void storeFromMemory(unsigned byteAddress);
	void loadToMemory(unsigned byteAddress, void* block);
};

#endif /* MEM_SIM_CACHE_H_ */
