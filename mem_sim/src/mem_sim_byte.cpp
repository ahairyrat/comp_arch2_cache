
#include "mem_sim_byte.h"

Byte::Byte()
{
	data[0] = 0;
	data[1] = 0;
}

Byte::Byte(unsigned a, unsigned b)
{
	data[0] = (char)a;
	data[1] = (char)b;
}

Byte::~Byte()
{

}

char* Byte::c_str()
{
	return data;
}

void Byte::setByte(char a, char b)
{
	data[0] = a;
	data[1]= b;
}
