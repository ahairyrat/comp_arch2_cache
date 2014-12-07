
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
	unsigned count = 65;
	for (unsigned i = 0; i < word->bytesPerWord; i++)
	{
		word->byte[i] = count;
		count++;
		if (count > 90)
			count = 65;
	}
}

void Debugger::forceWord(Word* word, unsigned &count)
{
	for (unsigned i = 0; i < word->bytesPerWord; i++)
	{
		word->byte[i] = count;
		count++;
		if (count > 90)
			count = 65;
	}
}

void Debugger::printWord(std::stringstream& dataOut, Word* word)
{
	for (unsigned i = 0; i < word->bytesPerWord; i++)
		dataOut << word->byte[i];
}

void Debugger::forceBlock(Block* block)
{
	unsigned count = 65;
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
	for (unsigned i = 0; i < block->wordsPerBlock; i++)
	{
		printWord(dataOut, block->word[i]);
		dataOut << '|';
	}
}

void Debugger::forceSet(Set* set)
{
	unsigned count = 65;
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
	unsigned count = 65;
	for (unsigned i = 0; i < cache ->setsPerCache; i++)
		forceSet(cache->set[i], count);
}

void Debugger::printCache(std::stringstream& dataOut, Cache* cache)
{
	unsigned dashNumber = (cache->set[0]->blocksPerSet)*(cache->set[0]->block[0]->wordsPerBlock)*(cache->set[0]->block[0]->word[0]->bytesPerWord);
	dashNumber += cache->set[0]->blocksPerSet;
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

}

