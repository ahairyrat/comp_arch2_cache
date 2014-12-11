
#ifndef MEM_SIM_UTILITIES_
#define MEM_SIM_UTILITIES_

#include <sstream>

class Utilities{
public:
	unsigned globalTime;
	std::stringstream globalSetsUsed;
	bool globalHit = true;
};

#endif