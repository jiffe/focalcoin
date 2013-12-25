#include <stdio.h>
#include <stdint.h>
#include "walletaccount.h"
#include "uint256.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAccount::FCWalletAccount(std::string accountName) {
	this->balance = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAccount::FCWalletAccount(std::string accountName, std::string accountJSON) {
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCWalletAccount::loadAddresses() {
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCWalletAccount::createNewAddress(bool primary) {
	FCWalletAddress address;

	this->addresses[address.getPublicKey()] = address;
	if(primary) {
		this->primaryAddress = address.getPublicKey();
	}
	
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
Json::Value FCWalletAccount::getAccountAddresses() {
	Json::Value response = Json::Value(Json::arrayValue);
	
	for(std::map<std::string, FCWalletAddress>::iterator it = this->addresses.begin(); it != this->addresses.end(); ++it) {
		response.append(it->first);
	}
	
	return response;
}


/***************************************************************************************************
* 
***************************************************************************************************/
std::string FCWalletAccount::getPrimaryAddress() {
	return this->primaryAddress;
}

