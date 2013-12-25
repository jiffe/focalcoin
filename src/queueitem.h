#ifndef _FCQUEUEITEM_H
#define _FCQUEUEITEM_H


#include <stdio.h>
#include <json/json.h>
#include <string>
#include "buffer.h"


enum {
	FCQUEUEITEMTYPE_NONE,
	FCQUEUEITEMTYPE_WEB,
	FCQUEUEITEMTYPE_RPC
};


enum {
	FCBUFFERTYPE_INPUT,
	FCBUFFERTYPE_OUTPUT
};


class FCQueueItem {
	private:
		FCBuffer input;
		FCBuffer output;
		int itemType;
		bool writable;
		
	public:
		FCQueueItem();
		FCQueueItem(int itemType);
		void init(int itemType);
		void write(char *bufferData, int bufferSize, int bufferType);
		void write(char *bufferData, int bufferType);
		void write(std::string bufferData, int bufferType);
		void write(Json::Value bufferData, int bufferType);
		int getItemType();
		void advance(int bytes, int bufferType);
		char *getRawBuffer(int bufferType);
		void setRawBuffer(char *bufferData, int bufferSize, int bufferType);
		std::string getBuffer(int bufferType);
		std::string getLine(int bufferType);
		int getBufferSize(int bufferType);
		bool isWritable();
		virtual void handle()=0;
};


#endif /* _FCQUEUEITEM_H */
