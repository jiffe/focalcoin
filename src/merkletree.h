#ifndef _FCMERKLETREE_H
#define _FCMERKLETREE_H


#include <map>
#include <string>
#include <json/json.h>
#include "uint256.h"


class FCMerkleTree {
	private:
		std::vector <std::string> transactions;
		std::vector <std::string> branches;
		
	public:
		bool addTransaction(Json::Value transaction);
		bool recalculate();
		std::string build(std::string hash);
		std::vector <std::string> getBranches() { return this->branches; }
};


#endif /* _FCMERKLETREE_H */
