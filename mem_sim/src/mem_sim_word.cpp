
#include <stdexcept>
#include "mem_sim_word.h"

Word::Word(unsigned bytesPerWord) : bytesPerWord(bytesPerWord)
{
	byte = new Byte[bytesPerWord];
}

Word::~Word() {
	delete[] byte;
}

void Word::store(unsigned offset, unsigned storeLength, Byte dataIn[]){
	if (offset + storeLength > bytesPerWord)
		throw std::out_of_range("trying to access invalid memory location");
	for (unsigned i = 0; i < storeLength; i++)
		byte[i+offset] = dataIn[i];
}

void Word::load(unsigned offset, unsigned loadLength, Byte dataOut[]){
	if (offset + loadLength > bytesPerWord)
		throw std::out_of_range("trying to access invalid memory location");
	for (unsigned i = 0; i < loadLength; i++)
		dataOut[i] = byte[i + offset];
}
