#ifndef _FCINVENTORY_H
#define _FCINVENTORY_H


#include <map>
#include <string>
#include <json/json.h>
#include <boost/thread.hpp>
#include "block.h"
#include "wallet.h"
#include "uint256.h"


class FCInventory {
	private:
		boost::mutex lock;
		std::map <int, FCBlock> blockchain;
		FCWallet wallet;
		
		FCBlock *prevBlock;
		FCBlock currBlock;
		
		// Transactions
		
	public:
		FCInventory();
		bool init();
		
		Json::Value getBlockTemplate();
		Json::Value getAccounts();
		double getBalance();
		Json::Value getAccountAddresses(std::string accountName);
		
		int getCurrentHeight(bool locked = false);
		unsigned int getCurrentNBits();
		float getCurrentDifficulty();
		uint256_t getCurrentTarget();
		uint64_t getCurrentReward();
		std::string getCurrentWork();
		std::string getNewAddress(std::string accountName);
		std::string getAccountAddress(std::string accountName, bool rebalance = false);
};


#endif /* _FCINVENTORY_H */
