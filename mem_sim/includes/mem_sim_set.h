
#ifndef MEM_SIM_SET_H_
#define MEM_SIM_SET_H_

#include "mem_sim_block.h"

class Set {
public:
	Set(
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned blocksPerSet
		);
	virtual ~Set();

	void storeFromCpu(char dataIn[], unsigned tag, unsigned byteOffset);
	void storeFromMemory(char dataIn[], unsigned tag, void* blockRef);
	void load(char dataOut[], unsigned tag, unsigned byteOffset);
	void load(char dataOut[], void* blockRef);
	void incrementUnused(Block* usedBlock);
	
private:
	Block** block;

	Block* findBlock(unsigned tag);
	Block* findLRU();
	Block* findEmpty();

	unsigned blocksPerSet;
};

#endif /* MEM_SIM_SET_H_ */
