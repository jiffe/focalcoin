#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include "config.h"
#include "db.h"
#include "exception.h"


extern FCConfig config;


/***************************************************************************************************
*
*
***************************************************************************************************/
FCDB::FCDB() {
	this->db = NULL;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCDB::init() {
	try {
		std::string blockcount_str;
		leveldb::Options options;
		
		boost::filesystem::path dataPath = FC::expandUser(config.getString("datapath", ""));
		boost::filesystem::path dbPath = dataPath / "db";
		
		options.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(options, dbPath.string(), &this->db);
		if(!status.ok()) {
			throw FCException("Failed to create/open database under '%s': %s", dbPath.string().c_str(), status.ToString().c_str());
		}
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCDB::read(std::string key, std::string &value) {
	try {
		printf("key2 %s\n", key.c_str());
		if(this->db == NULL) {
			throw FCException("Failed to read key '%s', database not open", key.c_str());
		}
		
		leveldb::Status status = this->db->Get(leveldb::ReadOptions(), key, &value);
		if(!status.ok()) {
			if(status.IsNotFound()) {
				throw FCException(FC::INFO, "Key '%s' was not found", key.c_str());
			}
			throw FCException("Failed to read key: %s", status.ToString().c_str());
		}
		
		return 0;
	}
	catch(FCException &e) {
		return e.getType();
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCDB::read(std::string key, int &value) {
	try {
		std::string tmp;
		int status;
		
		printf("key1 %s\n", key.c_str());
		if((status = this->read(key, tmp))) {
			throw FCException(status, "");
		}
		value = atoi(tmp.c_str());
		
		return 0;
	}
	catch(FCException &e) {
		return e.getType();
	}
}

