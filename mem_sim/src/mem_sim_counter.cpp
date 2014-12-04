
#include "mem_sim_counter.h"


Counter::Counter()
{
	count = 0;
}


Counter::~Counter()
{
}

void Counter::reset()
{
	count = 0;
}

void Counter::increment()
{
	count++;
}

void Counter::decrement()
{
	count--;
}

int Counter::getCount()
{
	return count;
}