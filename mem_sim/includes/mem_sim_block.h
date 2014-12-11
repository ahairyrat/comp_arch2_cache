
#ifndef MEM_SIM_BLOCK_H_
#define MEM_SIM_BLOCK_H_

#include "mem_sim_word.h"
#include "mem_sim_byte.h"
#include "mem_sim_counter.h"

class Block {
	//This is done to disconnect the debugger from the member functions.
	//This allows easier testing of load/store instructions via comparisons
	friend class Debugger;
public:
	Block(
		unsigned bytesPerWord,
		unsigned wordsPerBlock
		);
	virtual ~Block();

	void update(Byte dataIn[], unsigned tag);

	void store(Byte dataIn[], unsigned byteOffset, int numberOfBytes);
	void load(Byte dataOut[], unsigned byteOffset, int numberOfBytes);

	void Block::used();
	void Block::notUsed();
	
	bool isValid();
	bool isDirty();

	int getUnusedTime();

	void setTag(unsigned tag);
	unsigned getTag();

	void setValid(bool valid);
	void setDirty(bool dirty);

private:
	Word** word;
	unsigned tag;
	bool valid;
	bool dirty;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;

	unsigned calculateLoadLength(int numberOfBytes, unsigned offset);

	Counter counter;
	
};

#endif /* MEM_SIM_BLOCK_H_ */
