

#ifndef MEM_SIM_DEBUGGER_
#define MEM_SIM_DEBUGGER_

#include <sstream>
#include "mem_sim_cache.h"
#include "mem_sim_set.h"
#include "mem_sim_block.h"
#include "mem_sim_word.h"
#include "mem_sim_memory.h"

//Various debug functions for putting numbers into the cache or extracting them
//Requires frendship with relevant cache structure to be independant of load/store instructions
class Debugger{
public:
	Debugger();
	virtual ~Debugger();

	void forceWord(Word* word);
	void forceWord(Word* word, unsigned &count);
	void printWord(std::stringstream& dataOut, Word* word);

	void forceBlock(Block* block);
	void forceBlock(Block* block, unsigned &count);
	void printBlock(std::stringstream& dataOut, Block* block);

	void forceSet(Set* set);
	void forceSet(Set* set, unsigned &count);
	void printSet(std::stringstream& dataOut, Set* set);

	void forceCache(Cache* cache);
	void printCache(std::stringstream& dataOut, Cache* cache);

	void forceMemory(Memory* memory);
	void printMemory(std::stringstream& dataOut, Memory* memory);
};

#endif