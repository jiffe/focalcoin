#ifndef _FCEXCEPTION_H
#define _FCEXCEPTION_H


#include <iostream>
#include "logger.h"
#include "namespace.h"


class FCException : public std::exception {
	private:
		std::string format;
		std::string msg;
		int type;

	public:
		FCException(std::string format = "", ...);
		FCException(int type, std::string format, ...);
		~FCException() throw();
		const char* what() const throw();
		int getType() { return this->type; }
};


#endif /* _FCEXCEPTION_H */

