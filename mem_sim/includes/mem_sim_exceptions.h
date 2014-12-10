
#ifndef MEM_SIM_EXCEPTION_H_
#define MEM_SIM_EXCEPTION_H_

#include <exception>
#include <string>

class dataNotAvailableException : public std::exception {
private:
	std::string errorMessage;

public:
	dataNotAvailableException(const char* msg) : errorMessage(msg) {}
	~dataNotAvailableException() {}
	const char *what() { return this->errorMessage.c_str(); }
};

class dataIsDirtyException : public std::exception {
private:
	std::string err_msg;
	void* location;
	unsigned tag;

public:
	dataIsDirtyException(const char* msg, void* dirtylocation, unsigned tag) : err_msg(msg), location(dirtylocation), tag(tag){}
	~dataIsDirtyException() {}
	const char *what() { return this->err_msg.c_str(); }
	void* dirtyLocation(){ return location; }
	unsigned getTag(){ return tag; }
};

class dataSplitException : public std::exception {
private:
	std::string errorMessage;
	unsigned dataUsedVal;

public:
	dataSplitException(const char* msg, const unsigned dataUsed) : errorMessage(msg), dataUsedVal(dataUsed){}
	~dataSplitException() {}
	const char *what() { return this->errorMessage.c_str(); }
	const unsigned dataUsed(){ return dataUsedVal; }
};

class invalidInputException : public std::exception {
private:
	std::string errorMessage;

public:
	invalidInputException(const char* msg) : errorMessage(msg){}
	~invalidInputException() {}
	const char *what() { return this->errorMessage.c_str(); }
};

#endif