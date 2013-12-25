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
leveldb::DB* db;


int main(int argc, char **argv) {
	
	// Read command line and config options
	if(config.build(argc, argv) == false) {
		return -1;
	}
	
	if(network.start() == false) {
		return -1;
	}
	
	boost::filesystem::path dataPath = FC::expandUser(config.getString("datapath", ""));
	boost::filesystem::path dbPath = dataPath / "db";
	
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, dbPath.string(), &db);
	if(!status.ok()) {
		throw FCException("Failed to create/open database under '%s': %s", dbPath.string().c_str(), status.ToString().c_str());
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

