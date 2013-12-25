#ifndef _FCPEER_H
#define _FCPEER_H


#include <map>
#include <string>


class FCPeer {
	private:
		bool onConnect();
		bool onClose();
		bool onRead(char *buffer, int bufferSize);
		
	public:
};


#endif /* _FCPEER_H */
