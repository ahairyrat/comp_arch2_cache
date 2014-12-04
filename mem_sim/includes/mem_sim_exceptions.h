
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

public:
	dataIsDirtyException(const char* msg, void* dirtylocation) : err_msg(msg), location(dirtylocation){}
	~dataIsDirtyException() {}
	const char *what() { return this->err_msg.c_str(); }
	const void* dirtyLocation(){ return location; }
};

class dataSplitException : public std::exception {
private:
	std::string errorMessage;

public:
	dataSplitException(const char* msg) : errorMessage(msg){}
	~dataSplitException() {}
	const char *what() { return this->errorMessage.c_str(); }
};

class invalidinputException : public std::exception {
private:
	std::string errorMessage;

public:
	invalidinputException(const char* msg) : errorMessage(msg){}
	~invalidinputException() {}
	const char *what() { return this->errorMessage.c_str(); }
};

#endif