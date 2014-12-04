
#ifndef MEM_SIM_MEMORY_H_
#define MEM_SIM_MEMORY_H_

#include "mem_sim_block.h"

class Memory
{
public:
	Memory(
		unsigned addressBits,
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned memReadTime,
		unsigned memWriteTime
	);
	virtual ~Memory();

	//dataIn & datOut must be at least the size of bytesPerWord if address is aligned
	//dataIn & datOut must be at least the size of 2*bytesPerWord if address is unaligned
	//If address is unaligned all blocks in which data is contained will be loaded
	void read(char dataOut[], unsigned address);
	void write(char dataIn[], unsigned address);

private:
	Block** data;
	unsigned blockLength;
	unsigned memorySize;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned memReadTime;
	unsigned memWriteTime;
	int bytesPerBlock;
};

#endif /* MEM_SIM_MEMORY_H_ */
