#ifndef _FCWALLET_H
#define _FCWALLET_H


#include <json/json.h>
#include <stdint.h>
#include <string>
#include "walletaccount.h"


class FCWallet {
	private:
		uint64_t balance;
		std::map <std::string, FCWalletAccount> accounts;
		
		bool loadAccounts();
		
	public:
		FCWallet() { this->balance = 0; }
		bool init();
		uint64_t getBalance();
		double getFormattedBalance();
		std::string createNewAddress(std::string accountName, bool primary = false);
		Json::Value getAccounts();
		Json::Value getAccountAddresses(std::string accountName);
		std::string getAccountAddress(std::string accountName, bool rebalance);
		
};


#endif /* _FCWALLET_H */
