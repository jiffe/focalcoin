#ifndef _FCPEERS_H
#define _FCPEERS_H


#include <map>
#include <vector>
#include <string>
#include "peer.h"


class FCPeers {
	private:
		std::vector<FCPeer> peerlist;
		
	public:
		FCPeer *createPeer();
		int getPeerCount(bool connected = true);
		static bool start();
		static bool finish();
};


#endif /* _FCPEERS_H */
