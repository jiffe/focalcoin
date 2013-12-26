#include <stdio.h>
#include <stdint.h>
#include "walletaccount.h"
#include "uint256.h"
#include "db.h"


extern FCDB db;


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAccount::FCWalletAccount(std::string accountName) {
	this->balance = 0;
	this->accountName = accountName;
	this->createDate = time(0);
	this->lastDepositDate = 0;
	this->lastWithdrawalDate = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAccount::FCWalletAccount(std::string accountName, std::string accountJSON) {
	Json::Reader reader;
	Json::Value json;
	
	this->accountName = accountName;
	this->balance = 0;
	
	if(!reader.parse(accountJSON, json)) {
		return;
	}
	
	if(json["name"].isString()) {
		this->accountName = json["name"].asString();
	}
	if(json["balance"].isInt()) {
		this->balance = json["balance"].asUInt64();
	}
	if(json["createDate"].isInt()) {
		this->createDate = json["createDate"].asUInt();
	}
	if(json["lastDepositDate"].isInt()) {
		this->lastDepositDate = json["lastDepositDate"].asUInt();
	}
	if(json["lastWithdrawalDate"].isInt()) {
		this->lastWithdrawalDate = json["lastWithdrawalDate"].asUInt();
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
Json::Value FCWalletAccount::getJSON() {
	Json::Value json = Json::Value(Json::objectValue);
	json["name"] = this->accountName;
	json["balance"] = Json::UInt64(this->balance);
	json["createDate"] = Json::UInt(this->createDate);
	json["lastDepositDate"] = Json::UInt(this->lastDepositDate);
	json["lastWithdrawalDate"] = Json::UInt(this->lastWithdrawalDate);
	return json;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCWalletAccount::importAddress(FCWalletAddress address) {
	this->addresses[address.getPublicKey()] = address;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCWalletAccount::createNewAddress(std::string notes, bool primary) {
	FCWalletAddress address(this->accountName, notes);

	this->addresses[address.getPublicKey()] = address;
	if(primary) {
		this->primaryAddress = address.getPublicKey();
	}
	db.write("A." + address.getPublicKey(), address.getJSON());
	
	return address.getPublicKey();
}


/***************************************************************************************************
*
*
***************************************************************************************************/
uint64_t FCWalletAccount::getBalance() {
	return this->balance;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
double FCWalletAccount::getFormattedBalance() {
	double balance = this->balance;
	return balance / 100000000;
}


/***************************************************************************************************
* 
***************************************************************************************************/
Json::Value FCWalletAccount::getAccountAddresses(bool verbose, bool display) {
	Json::Value response = Json::Value(Json::arrayValue);
	
	for(std::map<std::string, FCWalletAddress>::iterator it = this->addresses.begin(); it != this->addresses.end(); ++it) {
		if(verbose) {
			response.append(it->second.getJSON(display));
		}
		else {
			response.append(it->first);
		}
	}
	
	return response;
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCWalletAccount::getPrimaryAddress() {
	return this->primaryAddress;
}

