#ifndef _FCLOGGER_H
#define _FCLOGGER_H


#include <string>


class FCLogger {
	private:
	
	public:
		static void log(int type, std::string msg, ...);
		static void log_b(int type, std::string msg);
};


#endif /* _FCLOGGER_H */

