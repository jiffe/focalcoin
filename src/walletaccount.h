#ifndef _FCWALLETACCOUNT_H
#define _FCWALLETACCOUNT_H


#include <json/json.h>
#include <stdint.h>
#include <string>
#include "walletaddress.h"


class FCWalletAccount {
	private:
		std::string accountName;
		uint64_t balance;
		int numAddresses;
		std::map<std::string,FCWalletAddress> addresses;
		std::string primaryAddress;
		unsigned int createDate;
		unsigned int lastDepositDate;
		unsigned int lastWithdrawalDate;
		
	public:
		FCWalletAccount() { }
		FCWalletAccount(std::string accountName);
		FCWalletAccount(std::string accountName, std::string accountJSON);
		
		Json::Value getJSON();
		
		uint64_t getBalance();
		double getFormattedBalance();
		Json::Value getAccountAddresses(bool verbose, bool display);
		void importAddress(FCWalletAddress address);
		
		std::string createNewAddress(std::string notes = "", bool primary = false);
		std::string getPrimaryAddress();
		
};


#endif /* _FCWALLETACCOUNT_H */
