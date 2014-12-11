
#ifndef MEM_SIM_BYTE_
#define MEM_SIM_BYTE_

//A data storage for two 8 bit numbers
//A byte is usually only 8 bits which stores 4 bit 2 hexadecimal numbers
//C++ does not have smaller sized data types, thus it is easier to double
//the memory usage for increased performance
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