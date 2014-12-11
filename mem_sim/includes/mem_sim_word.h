
#ifndef MEM_SIM_WORD_H_
#define MEM_SIM_WORD_H_

#include "mem_sim_byte.h"

class Word {
	friend class Debugger;
public:
	Word(unsigned bytesPerWord);
	virtual ~Word();

	//storeLength/loadLength can be any value.
	//This allows for arbitrarily length loads and stores
	//The dataIn/dataOut must be at least the size of storeLength/loadLength
	//
	void Word::store(unsigned offset, unsigned storeLength, Byte dataIn[]);
	void Word::load(unsigned offset, unsigned loadLength, Byte dataOut[]);

private:
	unsigned bytesPerWord;
	Byte* byte;
};

#endif /* MEM_SIM_WORD_H_ */
