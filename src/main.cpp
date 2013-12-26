#include <stdio.h>
#include <string>
#include <iostream>
#include <leveldb/db.h>
#include <boost/filesystem.hpp>
#include "exception.h"
#include "config.h"
#include "peers.h"
#include "inventory.h"
#include "network.h"
#include "rpc.h"
#include "queue.h"
#include "db.h"


bool exit_signal = false;

// Main configuration object
FCConfig config;

// Main inventory object holding the blockchain and transactions
FCInventory inv;

// Network items
FCNetwork network;

// Task queue
FCQueue queue;

// Database Access
FCDB db;


int main(int argc, char **argv) {
	
	// Read command line and config options
	if(config.build(argc, argv) == false) {
		return -1;
	}
	
	if(network.start() == false) {
		return -1;
	}
	
	if(db.init() == false) {
		return -1;
	}
	
	// Initialize block chain
	if(inv.init() == false) {
		return -1;
	}
	
	// Daemonize
	if(!config.getBool("foreground", false)) {
		//printf("Daemonize\n");
	}
	
	queue.run();
	
	return 0;
}

