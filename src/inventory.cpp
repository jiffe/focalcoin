#include <boost/filesystem.hpp>
#include <leveldb/db.h>
#include <math.h>
#include "inventory.h"
#include "config.h"
#include "exception.h"


extern FCConfig config;


/***************************************************************************************************
* Initialize an empty inventory set
***************************************************************************************************/
FCInventory::FCInventory() {
	this->prevBlock = NULL;
}


/***************************************************************************************************
* 
***************************************************************************************************/
int FCInventory::getCurrentHeight(bool locked) {
	try {
		/*
		int blockchainHeight = -1;
		
		{ // Critical Section
			if(!locked) this->lock.lock();
			blockchainHeight = this->blockchainHeight;
			if(!locked) this->lock.unlock();
		}
		
		if(blockchainHeight < 0) {
			std::string blockcount_str;
			boost::filesystem::path dataPath = FC::expandUser(config.getString("datapath", ""));
			boost::filesystem::path blockchainPath = dataPath / "blockchain";
			leveldb::DB* db;
			leveldb::Options options;
			options.create_if_missing = true;
			leveldb::Status status = leveldb::DB::Open(options, blockchainPath.string(), &db);
			if(!status.ok()) {
				throw FCException("Failed to create blockchain database file " + blockchainPath.string());
			}
			
			status = db->Get(leveldb::ReadOptions(), "blockcount", &blockcount_str);
			if(!status.ok()) {
				status = db->Put(leveldb::WriteOptions(), "blockcount", "0");
				if(!status.ok()) {
				printf("t2\n");
					throw FCException("Failed to create write blockcount to blockchain database file " + blockchainPath.string());
				}
			}
			
			blockchainHeight = atoi(blockcount_str.c_str());
			
			{ // Critical Section
				if(!locked) this->lock.lock();
				this->blockchainHeight = blockchainHeight;
				if(!locked) this->lock.unlock();
			}
		}
		
		return blockchainHeight;
		*/
		return this->currBlock.getHeight();
	}
	catch(FCException &e) {
		return -1;
	}
}


/***************************************************************************************************
* 
***************************************************************************************************/
uint256_t FCInventory::getCurrentTarget() {
	return this->currBlock.getTarget();
}


/***************************************************************************************************
* 
***************************************************************************************************/
unsigned int FCInventory::getCurrentNBits() {
	return this->currBlock.getNBits();
}


/***************************************************************************************************
* 
***************************************************************************************************/
float FCInventory::getCurrentDifficulty() {
	return this->currBlock.getDifficulty();
}


/***************************************************************************************************
* 
***************************************************************************************************/
uint64_t FCInventory::getCurrentReward() {
	return this->currBlock.getReward();
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCInventory::getCurrentWork() {
	return this->currBlock.getWork(this->prevBlock);
}


/***************************************************************************************************
* 
***************************************************************************************************/
Json::Value FCInventory::getBlockTemplate() {
	Json::Value tmpl = Json::Value(Json::objectValue);
	
	/*
	{
		"version" : 5,
		"previousblockhash" : "000000007e60d0a8e48d7d5dbf3e04e83e23c371cbe3a50635b7338328dddf0d",
		"transactions" : [
		],
		"coinbaseaux" : {
			"flags" : "062f503253482f"
		},
		                2511426767
		"coinbasevalue" : 64000000,
		"target" : "000000015a460000000000000000000000000000000000000000000000000000",
		"mintime" : 1387238812,
		"mutable" : [
			"time",
			"transactions",
			"prevblock"
		],
		"noncerange" : "00000000ffffffff",
		"sigoplimit" : 20000,
		"sizelimit" : 1000000,
		"curtime" : 1387239672,
		"bits" : "1d015a46",
		"height" : 177285
	}

	*/
	
	{ // Critical Section
		this->lock.lock();
		
		tmpl["version"] = FC_INVENTORYVERSION;
		tmpl["transactions"] = Json::Value(Json::arrayValue);
		tmpl["coinbaseaux"] = Json::Value(Json::objectValue);
		tmpl["coinbaseaux"]["flags"] = "062f503253482f";
		tmpl["mutable"] = Json::Value(Json::arrayValue);
		tmpl["mutable"][0] = "time";
		tmpl["mutable"][1] = "transactions";
		tmpl["mutable"][2] = "prevblock";
		tmpl["curtime"] = (unsigned int)time(0);
		tmpl["height"] = this->getCurrentHeight(true);
		tmpl["target"] = FC::numberToHex(this->getCurrentTarget());
		tmpl["bits"] = FC::numberToHex(this->getCurrentNBits());
		tmpl["coinbasevalue"] = Json::UInt64(this->getCurrentReward());
		tmpl["sizelimit"] = 1048576;
		tmpl["sigoplimit"] = 10000;
		
		if(this->prevBlock) {
			
		}
		else {
			tmpl["mintime"] = (unsigned int)time(0);
			tmpl["previousblockhash"] = "0000000000000000000000000000000000000000000000000000000000000000";
		}
		
		this->lock.unlock();
	}
	
	return tmpl;
}


/***************************************************************************************************
* 
***************************************************************************************************/
Json::Value FCInventory::getAccounts() {
	return this->wallet.getAccounts();
}


/***************************************************************************************************
* 
***************************************************************************************************/
Json::Value FCInventory::getAccountAddresses(std::string accountName) {
	return this->wallet.getAccountAddresses(accountName);
}


/***************************************************************************************************
* 
***************************************************************************************************/
double FCInventory::getBalance() {
	return this->wallet.getFormattedBalance();
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCInventory::getNewAddress(std::string accountName) {
	return this->wallet.createNewAddress(accountName);
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCInventory::getAccountAddress(std::string accountName, bool rebalance) {
	return this->wallet.getAccountAddress(accountName, rebalance);
}


/***************************************************************************************************
* Initialize the block chain and transaction inventory
***************************************************************************************************/
bool FCInventory::init() {
	try {
		boost::filesystem::path dataPath = FC::expandUser(config.getString("datapath", ""));
		if(dataPath == "") {
			throw FCException("Missing data path");
		}
		
		std::cout << dataPath.string() << std::endl;
		
		if(!boost::filesystem::exists(dataPath)) {
		std::cout << dataPath.string() << std::endl;
			if(!boost::filesystem::create_directories(dataPath)) {
				throw FCException("Failed to create data path directory structure");
			}
		}
		else {
			if(!boost::filesystem::is_directory(dataPath)) {
				throw FCException("Invalid data path, not a directory");
			}
		}
		
		if(this->wallet.init() == false) {
			throw FCException("");
		}
		this->currBlock.update(0, FCDEFAULT_NBITS);
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}

