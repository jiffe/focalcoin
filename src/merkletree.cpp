#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "merkletree.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCMerkleTree::addTransaction(Json::Value transaction) {
	std::string hash = transaction["hash"].asString();
	uint256_t hashvalue = 0;
	
	for(int i = 0; i < hash.length(); i += 16) {
		hashvalue <<= 64;
		hashvalue += strtoull(hash.substr(i, 16).c_str(), NULL, 16);
	}
	
	this->transactions.push_back( FC::numberToHex(hashvalue) );
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCMerkleTree::recalculate() {
	std::vector <std::string> transactions = this->transactions;
	std::vector <std::string> transactions_next;
	std::vector <std::string> branches;
	int len = transactions.size();
	
	while(len > 0) {
		branches.push_back( transactions[0] );
		
		if(len % 2 == 0) {
			transactions.push_back( transactions[ transactions.size() - 1 ] );
		}
		
		transactions_next.clear();
		for(int i = 1; i < len; i += 2) {
			transactions_next.push_back( FC::doublesha256( transactions[i] + transactions[i+1] ) );
		}
		
		transactions = transactions_next;
		len = transactions.size();
	}
	
	this->branches = branches;
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCMerkleTree::build(std::string hash) {
	for(int i = 0; i < this->branches.size(); i++) {
		hash = FC::doublesha256(hash + this->branches[i]);
	}
	
	return hash;
}

