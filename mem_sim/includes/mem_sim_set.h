
#ifndef MEM_SIM_SET_H_
#define MEM_SIM_SET_H_

#include "mem_sim_block.h"
#include "mem_sim_byte.h"

class Set {
	//This is done to disconnect the debugger from the member functions.
	//This allows easier testing of load/store instructions via comparisons
	friend class Debugger;
public:
	Set(
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned blocksPerSet
		);
	virtual ~Set();

	void storeFromCpu(Byte dataIn[], unsigned tag, unsigned byteOffset, int bytesToStore);
	void storeFromMemory(Byte dataIn[], unsigned tag, void* blockRef);
	void loadToCpu(Byte dataOut[], unsigned tag, unsigned byteOffset, int bytesToLoad);
	void loadToMemory(Byte dataOut[], void* blockRef, int bytesToLoad);
	void incrementUnused(Block* usedBlock);
	void flush();
	
private:
	Block** block;

	Block* findBlock(unsigned tag);
	Block* findLRU();
	Block* findEmpty();

	unsigned blocksPerSet;
};

#endif /* MEM_SIM_SET_H_ */
