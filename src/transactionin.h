#ifndef _FCTRANSACTIONIN_H
#define _FCTRANSACTIONIN_H


#include <stdint.h>
#include <vector>
#include <string>
#include "uint256.h"


class FCTransactionIn {
	private:
		uint256_t prevout_hash;
		unsigned int prevout_n;
		unsigned int nSequence;
		std::string template0;
		std::string template1;
		std::string scriptSig;
		
	public:
		FCTransactionIn();
		bool buildScriptSig(unsigned int height, std::string flags, unsigned int timestamp);
		bool buildScriptSig(std::string extranonce);
		std::string serialize();
		bool deserialize(std::string &hex);
		
};


#endif /* _FCTRANSACTIONIN_H */
