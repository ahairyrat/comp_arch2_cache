
#include "mem_sim_debugger.h"

#define TEST_SUCCESS 1
#define TEST_FAILURE 0


Debugger::Debugger()
{
}


Debugger::~Debugger()
{
}


bool Debugger::debug(bool cache, bool memory)
{
	if (!cache && !memory)
		return TEST_SUCCESS;
	return TEST_FAILURE;
}
