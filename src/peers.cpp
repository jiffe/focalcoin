#include "peers.h"
#include "exception.h"


extern FCPeers peers;


/***************************************************************************************************
*
*
***************************************************************************************************/
FCPeer *FCPeers::createPeer() {
	try {
		// LOCK TODO
		this->peerlist.push_back(FCPeer());
		FCPeer *peer = &this->peerlist[this->peerlist.size() - 1];
		return peer;
	}
	catch(FCException &e) {
		return NULL;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCPeers::getPeerCount(bool connected) {
	try {
		int count = 0;
		
		return count;
	}
	catch(FCException &e) {
		return -1;
	}
}


/***************************************************************************************************
* Start the peer handling thread
***************************************************************************************************/
bool FCPeers::start() {
	try {
		
		
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* Wait for the rejoin the peer handling thread
***************************************************************************************************/
bool FCPeers::finish() {
	try {
		
		
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}

