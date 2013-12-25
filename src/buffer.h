#ifndef _FCBUFFER_H
#define _FCBUFFER_H


#include <stdio.h>
#include <json/json.h>
#include <string>


class FCBuffer {
	private:
		char *buffer;
		char *bufferPtr;
		int bufferSize;
		
	public:
		FCBuffer();
		void reset();
		void write(char *buffer);
		void write(char *buffer, int bufferSize);
		void write(std::string buffer);
		void write(Json::Value buffer);
		bool advance(int bytes);
		std::string getBuffer();
		std::string getLine();
		void setRawBuffer(char *buffer, int bufferSize);
		char *getRawBuffer();
		int getBufferSize();
};


#endif /* _FCBUFFER_H */
