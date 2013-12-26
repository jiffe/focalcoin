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
		bool loadAddresses();
		
	public:
		FCWallet() { this->balance = 0; }
		bool init();
		uint64_t getBalance();
		double getFormattedBalance();
		std::string createNewAddress(std::string accountName, std::string notes, bool primary = false);
		Json::Value getAccounts();
		Json::Value getAccountAddresses(std::string accountName, bool verbose, bool display);
		std::string getAccountAddress(std::string accountName, std::string notes, bool rebalance);
		
};


#endif /* _FCWALLET_H */
