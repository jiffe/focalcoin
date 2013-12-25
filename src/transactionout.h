#ifndef _FCTRANSACTIONOUT_H
#define _FCTRANSACTIONOUT_H


#include <stdint.h>
#include <vector>
#include <string>


class FCTransactionOut {
	private:
		uint64_t coinbasevalue;
		std::string scriptPubKey;
		
	public:
		FCTransactionOut();
		bool buildScriptPubKey(std::string walletAddress, uint64_t coinbasevalue);
		std::string serialize();
		bool deserialize(std::string &hex);
};


#endif /* _FCTRANSACTIONOUT_H */
