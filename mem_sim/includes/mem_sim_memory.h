
#ifndef MEM_SIM_MEMORY_H_
#define MEM_SIM_MEMORY_H_

class Memory{
	friend class Debugger;
public:
	Memory(
		unsigned addressBits,
		unsigned bytesPerWord,
		unsigned wordsPerBlock,
		unsigned memReadTime,
		unsigned memWriteTime
	);
	virtual ~Memory();

	//dataIn & dataOut must be at least the size of bytesPerBlock
	//dataIn & dataOut must be at least the size of 2*bytesPerBlock if multiple blocks are to be read/written
	void read(char dataOut[], unsigned address, unsigned loadLength);
	void write(char dataIn[], unsigned address, unsigned storeLength);

private:
	char** data;
	unsigned blockLength;
	unsigned memorySize;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;
	unsigned memReadTime;
	unsigned memWriteTime;
	unsigned bytesPerBlock;

	void readWord(char dataOut[], unsigned blockNumber);
	void writeWord(char dataIn[], unsigned blockNumber);
};

#endif /* MEM_SIM_MEMORY_H_ */
