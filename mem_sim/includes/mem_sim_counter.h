

#ifndef MEM_SIM_COUNTER_H_
#define MEM_SIM_COUNTER_H_

//A basic counter class
class Counter
{
public:
	Counter();
	virtual ~Counter();

	void Counter::reset();
	void Counter::increment();
	void Counter::decrement();
	int Counter::getCount();

private:
	int count;
};

#endif /* MEM_SIM_COUNTER_H_ */