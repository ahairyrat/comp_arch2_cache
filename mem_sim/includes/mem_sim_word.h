
#ifndef MEM_SIM_WORD_H_
#define MEM_SIM_WORD_H_

class Word {
public:
	Word(unsigned bytesPerWord);
	virtual ~Word();

	void Word::store(unsigned offset, unsigned storeLength, char dataIn[]);
	void Word::load(unsigned offset, unsigned loadLength, char dataOut[]);

private:
	unsigned bytesPerWord;
	char* byte;
};

#endif /* MEM_SIM_WORD_H_ */
