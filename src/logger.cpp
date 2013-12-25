#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include <string>
#include "logger.h"
#include "namespace.h"
#include "config.h"


extern FCConfig config;


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCLogger::log(int type, std::string msg, ...) {
	char buffer[8192] = "";
	std::string line = "";
	std::time_t rawtime;
    std::tm* timeinfo;
    char timebuffer[80];
	
	
	if(type > config.getInt("debug", 3) || msg == "") return;
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg.c_str(), args);
	va_end(args);
	
	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(timebuffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    line += std::string(timebuffer);
	
	switch(type) {
		case -1:
			line += " [ERROR]: ";
			break;
		case 0:
			line += " [SUCCESS]: ";
			break;
		case 1:
			line += " [INFO]: ";
			break;
		case 2:
			line += " [WARN2]: ";
			break;
		case 3:
			line += " [WARN1]: ";
			break;
		case 4:
			line += " [DEBUG1]: ";
			break;
		case 5:
			line += " [DEBUG2]: ";
			break;
		case 6:
			line += " [DEBUG3]: ";
			break;
		default:
			line += " [UNKNOWN]: ";
			break;
	}
	
	line += std::string(buffer);
	std::cout << line << std::endl;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCLogger::log_b(int type, std::string msg) {
	std::string line = "";
	std::time_t rawtime;
    std::tm* timeinfo;
    char timebuffer[80];
	
	
	if(type > config.getInt("debug", 3) || msg == "") return;
	
	std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(timebuffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    line += std::string(timebuffer);
	
	switch(type) {
		case -1:
			line += " [ERROR]: ";
			break;
		case 0:
			line += " [SUCCESS]: ";
			break;
		case 1:
			line += " [INFO]: ";
			break;
		case 2:
			line += " [WARN2]: ";
			break;
		case 3:
			line += " [WARN1]: ";
			break;
		case 4:
			line += " [DEBUG1]: ";
			break;
		case 5:
			line += " [DEBUG2]: ";
			break;
		case 6:
			line += " [DEBUG3]: ";
			break;
		default:
			line += " [UNKNOWN]: ";
			break;
	}
	
	line += msg;
	std::cout << line << std::endl;
}

