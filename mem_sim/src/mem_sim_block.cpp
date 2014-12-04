
#include <stdexcept>
#include "mem_sim_block.h"
#include "mem_sim_exceptions.h"


Block::Block(
	unsigned bytesPerWord,
	unsigned wordsPerBlock
	) : bytesPerWord(bytesPerWord), wordsPerBlock(wordsPerBlock)
{
	word = new Word*[wordsPerBlock];
	for (unsigned i = 0; i < wordsPerBlock; i++)
		word[i] = new Word(bytesPerWord);
	valid = false;
	dirty = false;
}

Block::~Block() {
	for (unsigned i = 0; i < wordsPerBlock; i++)
		delete word[i];
	delete[] word;
}

void Block::update(char dataIn[], unsigned tag)
{
	this->tag = tag;
	for (unsigned i = 0; i < wordsPerBlock; i++)
		word[i]->store(0, bytesPerWord, dataIn+(i*bytesPerWord));
	counter.reset();
}

void Block::store(char dataIn[], unsigned byteOffset)
{
	unsigned wordNumber = byteOffset / bytesPerWord;
	unsigned offset = byteOffset % bytesPerWord;
	unsigned storeLength = bytesPerWord - offset;
	word[wordNumber]->store(offset, storeLength, dataIn);
	this->used();
	this->dirty = true;
	//if only partial word has been stored
	if (storeLength < bytesPerWord){
		try{
			word[wordNumber + 1]->store(0, bytesPerWord - storeLength, dataIn + offset);
		}
		catch (const std::out_of_range& e){
			throw dataSplitException("data is split  over multiple blocks", offset);
		}

	}
}

void Block::load(char dataOut[], unsigned byteOffset, int numberOfBytes)
{
	unsigned wordNumber = byteOffset / bytesPerWord;
	unsigned offset = byteOffset % bytesPerWord;
	unsigned loadLength = bytesPerWord-offset;
	while (numberOfBytes > 0)
	{
		numberOfBytes -= loadLength;
		word[wordNumber]->load(offset, loadLength, dataOut);
		offset =  0;
		wordNumber++;
		loadLength 
	}
	this->used();
	//if only partial word has been loaded
	if (loadLength < bytesPerWord){
		try{
			word[wordNumber + 1]->load(0, numberOfBytes - loadLength, dataOut + offset);
		}
		catch (const std::out_of_range& e){
			throw dataSplitException("data is split  over multiple blocks", offset);
		}
	}
}

void Block::used()
{
	counter.reset();
}

void Block::notUsed()
{
	counter.increment();
}



bool Block::isValid()
{
	return valid;
}

bool Block::isDirty()
{
	return dirty;
}

int Block::getUnusedTime()
{
	return counter.getCount();
}

void Block::setTag(unsigned tag)
{
	this -> tag = tag;
}
unsigned Block::getTag()
{
	return this->tag;
}