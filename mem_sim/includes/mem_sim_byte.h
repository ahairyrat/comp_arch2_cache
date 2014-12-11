
#ifndef MEM_SIM_BYTE_
#define MEM_SIM_BYTE_

class Byte
{
public:
	Byte();
	Byte(unsigned a, unsigned b);
	~Byte();
	char data[2];
	char* c_str();

	void setByte(char a, char b);
};

#endif