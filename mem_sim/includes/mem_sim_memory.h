
#ifndef MEM_SIM_MEMORY_H_
#define MEM_SIM_MEMORY_H_

#include <string>
#include "mem_sim_byte.h"
#include "mem_sim_utilities.h"

class Memory{
	friend class Debugger;
public:
	Memory(
		unsigned addressBits,
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned memReadTime,
		unsigned memWriteTime,
		Utilities* Utilities
	);
	virtual ~Memory();

	//dataIn & dataOut must be at least the size of bytesPerBlock
	//dataIn & dataOut must be at least the size of 2*bytesPerBlock if multiple blocks are to be read/written
	void read(Byte dataOut[], unsigned address, unsigned loadLength);
	void write(Byte dataIn[], unsigned address, unsigned storeLength);

private:
	Byte** data;
	unsigned blockLength;
	unsigned memoryCapacity;
	unsigned memoryBlockSize;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned memReadTime;
	unsigned memWriteTime;
	unsigned bytesPerBlock;

	void readWord(Byte dataOut[], unsigned blockNumber);
	void writeWord(Byte dataIn[], unsigned blockNumber);

	std::string buildOutOfMemoryString(int address, int loadLength);

	Utilities* utilities;
};

#endif /* MEM_SIM_MEMORY_H_ */
