#include <stdio.h>
#include <stdarg.h>
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCException::FCException(std::string format, ...) {
	char buffer[8192];
	this->format = format;
	
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
	va_end(args);
	
	this->msg = std::string(buffer);
	this->type = FC::ERROR;
	
	FCLogger::log_b(FC::ERROR, this->msg);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCException::FCException(int type, std::string format, ...) {
	char buffer[8192];
	this->format = format;
	this->type = type;
	
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
	va_end(args);
	
	this->msg = std::string(buffer);
	
	FCLogger::log_b(type, this->msg);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCException::~FCException() throw() {
	
}


/***************************************************************************************************
*
*
***************************************************************************************************/
const char *FCException::what() const throw() {
	//return this->format.c_str();
	return (char *)"";
}



