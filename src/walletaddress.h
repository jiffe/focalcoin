#ifndef _FCWALLETADDRESS_H
#define _FCWALLETADDRESS_H


#include <json/json.h>
#include <stdint.h>
#include <string>


class FCWalletAddress {
	private:
		uint64_t balance;
		std::string privKey;
		std::string pubKey;
		std::string notes;
		std::string accountName;
		unsigned int createDate;
		unsigned int lastDepositDate;
		unsigned int lastWithdrawalDate;
		
		std::string base58Encode(unsigned char *address, int addressSize);
		std::string buildPublicKey(unsigned char *buffer);
		std::string buildPrivateKey(unsigned char *buffer);
		
	public:
		FCWalletAddress() { }
		FCWalletAddress(std::string accountName, std::string notes);
		FCWalletAddress(std::string json);
		
		std::string getPublicKey() { return this->pubKey; }
		std::string getAccountName() { return this->accountName; }
		
		Json::Value getJSON(bool display = false);
};


#endif /* _FCWALLETADDRESS_H */
