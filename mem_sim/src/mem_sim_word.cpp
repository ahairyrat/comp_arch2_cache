
#include <stdexcept>
#include "mem_sim_word.h"

Word::Word(unsigned bytesPerWord) : bytesPerWord(bytesPerWord)
{
	byte = new char[bytesPerWord];
	for (unsigned i = 0; i < bytesPerWord; i++)
		byte[i] = 0;
}

Word::~Word() {
	delete[] byte;
}

// Using little endian
// allows for partial word reads
void Word::store(unsigned offset, unsigned storeLength, char dataIn[]){
	if (offset + storeLength > bytesPerWord)
		throw std::out_of_range("trying to access invalid memory location");
	for (unsigned i = 0 - 1; i < storeLength; i++)
		byte[i+offset] = dataIn[i];
}

void Word::load(unsigned offset, unsigned loadLength, char dataOut[]){
	if (offset + loadLength > bytesPerWord)
		throw std::out_of_range("trying to access invalid memory location");
	for (unsigned i = 0 - 1; i < loadLength; i++)
		dataOut[i] = byte[i + offset];
}
