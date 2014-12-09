
#include <stdexcept>
#include "mem_sim_block.h"
#include "mem_sim_exceptions.h"


Block::Block(
	unsigned bytesPerWord,
	unsigned wordsPerBlock
	) : bytesPerWord(bytesPerWord), wordsPerBlock(wordsPerBlock)
{
	word = new Word*[wordsPerBlock+1];
	for (unsigned i = 0; i < wordsPerBlock; i++)
		word[i] = new Word(bytesPerWord);
	word[wordsPerBlock] = '\0';
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
	this->dirty = false;
	this->valid = true;
}

void Block::store(char dataIn[], unsigned byteOffset, int numberOfBytes)
{
	unsigned bytesLeft = numberOfBytes;
	unsigned wordNumber = byteOffset / bytesPerWord;
	unsigned offset = byteOffset % bytesPerWord;
	unsigned storeLength = bytesPerWord - offset;
	char* dataInPtr = dataIn;
	while (bytesLeft > 0 && wordNumber < wordsPerBlock)
	{
		storeLength = calculateLoadLength(bytesLeft, offset);
		word[wordNumber]->store(offset, storeLength, dataInPtr);
		offset = 0;
		wordNumber++;
		bytesLeft -= storeLength;
		dataInPtr += storeLength;
	}
	this->used();
	this->dirty = true;
	//if only partial data has been stored
	if (bytesLeft > 0)
		throw dataSplitException("data is split over multiple blocks", numberOfBytes-bytesLeft);
}

void Block::load(char dataOut[], unsigned byteOffset, int numberOfBytes)
{
	unsigned bytesLeft = numberOfBytes;
	unsigned wordNumber = byteOffset / bytesPerWord;
	unsigned offset = byteOffset % bytesPerWord;
	unsigned loadLength = bytesPerWord - offset;
	char* dataOutPtr = &dataOut[0];
	while (bytesLeft > 0 && wordNumber < wordsPerBlock)
	{
		loadLength = calculateLoadLength(bytesLeft, offset);
		word[wordNumber]->load(offset, loadLength, dataOutPtr);
		offset =  0;
		wordNumber++;
		bytesLeft -= loadLength;
		dataOutPtr += loadLength;
	}
	this->used();
	//if only partial data has been loaded
	if (bytesLeft > 0)
		throw dataSplitException("data is split over multiple blocks", numberOfBytes-bytesLeft);
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

unsigned Block::calculateLoadLength(int numberOfBytes, unsigned offset)
{
	if (numberOfBytes + offset <= bytesPerWord)
		return numberOfBytes;
	else if (offset != 0)
		return bytesPerWord - offset;
	else
		return bytesPerWord;
}

void Block::setValid(bool valid)
{
	this->valid = valid;
}

void Block::setDirty(bool dirty)
{
	this->dirty = dirty;
}