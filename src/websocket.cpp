#include <libwebsockets.h>
#include "websocket.h"
#include "resources.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWebSocket::FCWebSocket() : FCRPC() {
	this->encapsulated = false;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCWebSocket::encapsulate() {
	try {
		if(!this->encapsulated) {
			int bufferSize = this->getBufferSize(FCBUFFERTYPE_OUTPUT);
			int paddedSize = bufferSize + LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING;
			char *buffer = (char *)malloc(paddedSize);
			if(buffer == NULL) {
				throw FCException("Failed to allocate memory");
			}
			memcpy(buffer + LWS_SEND_BUFFER_PRE_PADDING, this->getRawBuffer(FCBUFFERTYPE_OUTPUT), bufferSize);
			
			this->setRawBuffer(buffer, bufferSize + LWS_SEND_BUFFER_PRE_PADDING, FCBUFFERTYPE_OUTPUT);
			this->advance(LWS_SEND_BUFFER_PRE_PADDING, FCBUFFERTYPE_OUTPUT);
			
			this->encapsulated = true;
		}
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}

