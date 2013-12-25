#include "web.h"
#include "resources.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWeb::FCWeb() : FCRPC() {
	this->headerCreated = false;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCWeb::setContentType(int contentType) {
	this->contentType = contentType;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCWeb::encapsulate(int contentType) {
	try {
		char contentLength[12];
		
		if(!this->headerCreated) {
			sprintf(contentLength, "%d", this->getBufferSize(FCBUFFERTYPE_OUTPUT));
			std::string header = "HTTP/1.1 200 OK\r\n";
			
			switch(contentType) {
				case FC_RESOURCETYPE_TMPL:
					header += "Content-Type: text/html\r\n";
					break;
				case FC_RESOURCETYPE_CSS:
					header += "Content-Type: text/css\r\n";
					break;
				case FC_RESOURCETYPE_JS:
					header += "Content-Type: text/javascript\r\n";
					break;
				case FC_RESOURCETYPE_JSON:
					header += "Content-Type: application/json\r\n";
					break;
				default:
					header += "Content-Type: text/html\r\n";
					break;
			}
			
			header += "Content-Length: " + std::string(contentLength) + "\r\n\r\n";
			
			char *buffer = (char *)malloc(header.size() + this->getBufferSize(FCBUFFERTYPE_OUTPUT) + 1);
			if(buffer == NULL) {
				throw FCException("Failed to allocate memory");
			}
			strcpy(buffer, header.c_str());
			memcpy(buffer + header.size(), this->getRawBuffer(FCBUFFERTYPE_OUTPUT), this->getBufferSize(FCBUFFERTYPE_OUTPUT));
			
			this->setRawBuffer(buffer, header.size() + this->getBufferSize(FCBUFFERTYPE_OUTPUT), FCBUFFERTYPE_OUTPUT);
			
			this->headerCreated = true;
		}
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}

