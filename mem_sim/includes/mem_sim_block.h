
#ifndef MEM_SIM_BLOCK_H_
#define MEM_SIM_BLOCK_H_

#include "mem_sim_word.h"
#include "mem_sim_counter.h"

class Block {
public:
	Block(
		unsigned bytesPerWord,
		unsigned wordsPerBlock
		);
	virtual ~Block();

	void update(char dataIn[], unsigned tag);

	void store(char dataIn[], unsigned byteOffset);
	void load(char dataOut[], unsigned byteOffset);

	void Block::used();
	void Block::notUsed();
	
	bool isValid();
	bool isDirty();

	int getUnusedTime();

	void setTag(unsigned tag);
	unsigned getTag();

private:
	Word** word;
	unsigned tag;
	bool valid;
	bool dirty;
	unsigned bytesPerWord;
	unsigned wordsPerBlock;

	Counter counter;
	
};

#endif /* MEM_SIM_BLOCK_H_ */
