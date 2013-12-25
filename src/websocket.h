#ifndef _FCWEBSOCKET_H
#define _FCWEBSOCKET_H


#include "rpc.h"


class FCWebSocket : public FCRPC {
	private:
		bool encapsulated;
		
	public:
		FCWebSocket();
		bool encapsulate();
};


#endif /* _FCWEBSOCKET_H */
