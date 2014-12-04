
#ifndef MEM_SIM_CACHE_H_
#define MEM_SIM_CACHE_H_

#include "mem_sim_set.h"


class Cache {
public:
	Cache(
		unsigned addressBits,
		unsigned bytesdPerWord,
		unsigned wordsPerBlock,
		unsigned blocksPerSet,
		unsigned setsPerCache,
		unsigned hitTime
		);
	virtual ~Cache();

	//add parameters
	void load();
	void store();

private:
	Set** set;
	unsigned setsPerCache;
};

#endif /* MEM_SIM_CACHE_H_ */
