#include <leveldb/db.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "wallet.h"
#include "db.h"
#include "exception.h"


extern leveldb::DB* db;


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCWallet::init() {
	try {
		
		if(this->loadAccounts() == false) {
			throw FCException();
		}
		
		if(this->accounts.size() == 0) {
			if(this->createNewAddress("", true) == "") {
				throw FCException();
			}
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
bool FCWallet::loadAccounts() {
	try {
		leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
		for(it->Seek("A#"); it->Valid(); it->Next()) {
			std::string accountName = it->key().ToString().substr(2);
			std::string accountJSON = it->value().ToString();
			
			this->accounts[accountName] = FCWalletAccount(accountName, accountJSON);
			if(this->accounts[accountName].loadAddresses() == false) {
				throw FCException();
			}
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
std::string FCWallet::createNewAddress(std::string accountName, bool primary) {
	try {
		std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
		if(it == this->accounts.end()) {
			this->accounts[accountName] = FCWalletAccount(accountName);
		}
		
		return this->accounts[accountName].createNewAddress(primary);
	}
	catch(FCException &e) {
		return "";
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
uint64_t FCWallet::getBalance() {
	return this->balance;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
double FCWallet::getFormattedBalance() {
	double balance = this->balance;
	return balance / 100000000;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
Json::Value FCWallet::getAccounts() {
	Json::Value response = Json::Value(Json::objectValue);
	
	for(std::map<std::string, FCWalletAccount>::iterator iter = this->accounts.begin(); iter != this->accounts.end(); ++iter) {
		response[iter->first] = iter->second.getFormattedBalance();
	}
	
	return response;
}


/***************************************************************************************************
* 
***************************************************************************************************/
Json::Value FCWallet::getAccountAddresses(std::string accountName) {
	std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
	if(it != this->accounts.end()) {
		return this->accounts[accountName].getAccountAddresses();
	}
	return Json::Value(Json::arrayValue);
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCWallet::getAccountAddress(std::string accountName, bool rebalance) {
	std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
	if(it == this->accounts.end() || (rebalance && this->accounts[accountName].getBalance())) {
		return this->createNewAddress(accountName, true);
	}
	return this->accounts[accountName].getPrimaryAddress();
}

