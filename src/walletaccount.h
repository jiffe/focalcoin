#ifndef _FCWALLETACCOUNT_H
#define _FCWALLETACCOUNT_H


#include <json/json.h>
#include <stdint.h>
#include <string>
#include "walletaddress.h"


class FCWalletAccount {
	private:
		uint64_t balance;
		int numAddresses;
		std::map<std::string,FCWalletAddress> addresses;
		std::string primaryAddress;
		
	public:
		FCWalletAccount() { }
		FCWalletAccount(std::string accountName);
		FCWalletAccount(std::string accountName, std::string accountJSON);
		
		uint64_t getBalance();
		double getFormattedBalance();
		Json::Value getAccountAddresses();
		
		bool loadAddresses();
		std::string createNewAddress(bool primary);
		std::string getPrimaryAddress();
		
};


#endif /* _FCWALLETACCOUNT_H */
