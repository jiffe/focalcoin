#ifndef _FCTRANSACTION_H
#define _FCTRANSACTION_H


#include <stdint.h>
#include <vector>
#include <string>
#include "transactionin.h"
#include "transactionout.h"


class FCTransaction {
	private:
		std::vector <FCTransactionIn> txin;
		std::vector <FCTransactionOut> txout;
		unsigned int version;
		unsigned int lockTime;
		std::vector <std::string> values;
		
	public:
		FCTransaction();
		bool build(std::string walletAddress, uint64_t coinbasevalue, std::string flags, unsigned int height, std::string data);
		bool deserialize(std::string transaction);
		std::string serialize();
		std::string get(int index) { return this->values[index]; }
		//bool set_extranonce(std::string extranonce);
};


#endif /* _FCTRANSACTION_H */
