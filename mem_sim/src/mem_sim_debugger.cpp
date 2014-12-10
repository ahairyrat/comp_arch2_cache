
#include "mem_sim_debugger.h"

#define TEST_SUCCESS 1
#define TEST_FAILURE 0


Debugger::Debugger()
{
}


Debugger::~Debugger()
{
}


bool Debugger::debug(Cache* cache, Memory* memory)
{
	if (cache == NULL && memory == NULL)
		return TEST_SUCCESS;
	return TEST_FAILURE;
}



void Debugger::forceWord(Word* word)
{
	unsigned count = 0;
	for (unsigned i = 0; i < word->bytesPerWord; i++)
	{
		word->byte[i] = count;
		count++;
		if (count > 9)
			count = 0;
	}
}

void Debugger::forceWord(Word* word, unsigned &count)
{
	for (unsigned i = 0; i < word->bytesPerWord; i++)
	{
		word->byte[i] = count;
		count++;
		if (count > 9)
			count = 0;
	}
}

void Debugger::printWord(std::stringstream& dataOut, Word* word)
{
	for (unsigned i = 0; i < word->bytesPerWord; i++)
		dataOut << std::hex << (unsigned)word->byte[i];
}

void Debugger::forceBlock(Block* block)
{
	unsigned count = 0;
	block->valid = true;
	for (unsigned i = 0; i < block->wordsPerBlock; i++)
		forceWord(block->word[i], count);
}

void Debugger::forceBlock(Block* block,unsigned &count)
{
	block->valid = true;
	for (unsigned i = 0; i < block->wordsPerBlock; i++)
		forceWord(block->word[i], count);
}

void Debugger::printBlock(std::stringstream& dataOut, Block* block)
{
	dataOut << "(V" << block->isValid() << ",D" << block->isDirty() << ",T" << block->getTag() << ")";
	for (unsigned i = 0; i < block->wordsPerBlock; i++)
	{
		printWord(dataOut, block->word[i]);
		dataOut << '|';
	}
}

void Debugger::forceSet(Set* set)
{
	unsigned count = 0;
	for (unsigned i = 0; i < set->blocksPerSet; i++)
		forceBlock(set->block[i], count);
}

void Debugger::forceSet(Set* set, unsigned &count)
{
	for (unsigned i = 0; i < set->blocksPerSet; i++)
		forceBlock(set->block[i], count);
}

void Debugger::printSet(std::stringstream& dataOut, Set* set)
{
	for (unsigned i = 0; i < set->blocksPerSet; i++)
	{
		printBlock(dataOut, set->block[i]);
		dataOut << '|';
	}
}

void Debugger::forceCache(Cache* cache)
{
	unsigned count = 0;
	for (unsigned i = 0; i < cache ->setsPerCache; i++)
		forceSet(cache->set[i], count);
}

void Debugger::printCache(std::stringstream& dataOut, Cache* cache)
{
	//rework dash number
	dataOut << std::hex;
	unsigned dashNumber = (cache->set[0]->blocksPerSet)*(cache->set[0]->block[0]->wordsPerBlock)*(cache->set[0]->block[0]->word[0]->bytesPerWord);
	dashNumber += cache->set[0]->blocksPerSet;
	dashNumber += 7;
	for (unsigned i = 0; i < cache->setsPerCache; i++)
	{
		printSet(dataOut, cache->set[i]);
		dataOut << '\n';
		for (int j = dashNumber; j > 0; j--)
			dataOut << '-';
		dataOut << '\n';
	}
}

void Debugger::printMemory(std::stringstream& dataOut, Memory* memory)
{
	for (unsigned i = 0; i < memory->memoryBlockSize; i++)
	{
		for (unsigned j = 0; j < memory->bytesPerBlock; j++)
		{
			if (j%memory->bytesPerWord == 0)
				dataOut << '|';
			dataOut << (unsigned)memory->data[i][j];	
		}
		dataOut << '\n';
	}
}

