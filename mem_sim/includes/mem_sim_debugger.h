

#ifndef MEM_SIM_DEBUGGER_
#define MEM_SIM_DEBUGGER_

class Debugger
{
public:
	Debugger();
	virtual ~Debugger();
	bool debug(bool cache, bool memory);
};

#endif