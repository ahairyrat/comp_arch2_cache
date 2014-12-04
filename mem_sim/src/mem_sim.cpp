
#include "mem_sim_cache.h"
#include "mem_sim_memory.h"
#include "mem_sim_parser.h"
#include "mem_sim_debugger.h"


int main(int argc, char *argv[]){
	unsigned addressBits = *(unsigned*)argv[0];
	unsigned bytesPerWord = *(unsigned*)argv[1];
	unsigned wordsPerBlock = *(unsigned*)argv[2];
	unsigned blocksPerSet = *(unsigned*)argv[3];
	unsigned setsPerCache = *(unsigned*)argv[4];
	unsigned hitTime = *(unsigned*)argv[5];
	unsigned memReadTime = *(unsigned*)argv[6];
	unsigned memWriteTime = *(unsigned*)argv[7];

	Cache cache(
		addressBits,
		bytesPerWord,
		wordsPerBlock,
		blocksPerSet,
		setsPerCache,
		hitTime
	);

	Memory memory(
		addressBits,
		bytesPerWord,
		wordsPerBlock,
		memReadTime,
		memWriteTime
		);

}