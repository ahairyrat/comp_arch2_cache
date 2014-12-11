
#ifndef MEM_SIM_UTILITIES_
#define MEM_SIM_UTILITIES_

#include <sstream>
//A set of global variables for storing internal data
//Internal data is not natively returned from the cache
class Utilities{
public:
	//total time taken for a transaction
	unsigned globalTime;
	//a group of all set indexes used in a transaction 
	std::stringstream globalSetsUsed;
	//a flag to show if the trnasaction hit or missed
	//should be set to false if at least one miss is detected
	bool globalHit = true;
};

#endif