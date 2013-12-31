#include <leveldb/db.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "wallet.h"
#include "global.h"


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
			if(this->createNewAddress("", "Internally generated address", true) == "") {
				throw FCException();
			}
		}
		
		if(this->loadAddresses() == false) {
			throw FCException();
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
		leveldb::Iterator *it = db.iterator();
		for(it->Seek("A#"); it->Valid() && it->key().ToString().substr(0, 2) == "A#"; it->Next()) {
			std::string accountName = it->key().ToString().substr(2);
			std::string accountJSON = it->value().ToString();
			
			//printf("%s\n", it->key().ToString().c_str());
			
			this->accounts[accountName] = FCWalletAccount(accountName, accountJSON);
			//printf("add acct '%s'\n", accountName.c_str());
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
bool FCWallet::loadAddresses() {
	try {
		leveldb::Iterator *it = db.iterator();
		for(it->Seek("A."); it->Valid() && it->key().ToString().substr(0, 2) == "A."; it->Next()) {
			std::string addressPublicKey = it->key().ToString().substr(2);
			std::string addressJSON = it->value().ToString();
			
			FCWalletAddress address = FCWalletAddress(addressJSON);
			
			//printf("load address acct '%s'\n", addressJSON.c_str());
			
			std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(address.getAccountName());
			if(it == this->accounts.end()) {
				this->accounts[address.getAccountName()] = FCWalletAccount(address.getAccountName());
				db.write("A#" + address.getAccountName(), this->accounts[address.getAccountName()].getJSON());
			}
			
			//printf("here '%s' '%s'\n", address.getPublicKey().c_str(), address.getAccountName().c_str());
			this->accounts[address.getAccountName()].importAddress(address);
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
std::string FCWallet::createNewAddress(std::string accountName, std::string notes, bool primary) {
	try {
		std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
		if(it == this->accounts.end()) {
			this->accounts[accountName] = FCWalletAccount(accountName);
			db.write("A#" + accountName, this->accounts[accountName].getJSON());
		}
		
		return this->accounts[accountName].createNewAddress(notes, primary);
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
Json::Value FCWallet::getAccountAddresses(std::string accountName, bool verbose, bool display) {
	std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
	if(it != this->accounts.end()) {
		return this->accounts[accountName].getAccountAddresses(verbose, display);
	}
	return Json::Value(Json::arrayValue);
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCWallet::getAccountAddress(std::string accountName, std::string notes, bool rebalance) {
	std::map<std::string, FCWalletAccount>::const_iterator it = this->accounts.find(accountName);
	if(it == this->accounts.end() || (rebalance && this->accounts[accountName].getBalance())) {
		return this->createNewAddress(accountName, notes, true);
	}
	return this->accounts[accountName].getPrimaryAddress();
}

