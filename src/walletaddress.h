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
		
		std::string base58Encode(unsigned char *address, int addressSize);
		std::string buildPublicKey(unsigned char *buffer);
		std::string buildPrivateKey(unsigned char *buffer);
		
	public:
		FCWalletAddress();
		std::string getPublicKey() { return this->pubKey; }
};


#endif /* _FCWALLETADDRESS_H */
