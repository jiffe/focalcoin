#ifndef _FCBLOCK_H
#define _FCBLOCK_H


#include <stdint.h>
#include <map>
#include <string>
#include "merkletree.h"
#include "uint256.h"
#include "transaction.h"


class FCBlock: private FCMerkleTree {
	private:
		unsigned int version;
		int height;
		unsigned int nbits;
		uint64_t baseReward;
		uint32_t extranonce;
		FCTransaction coinbase;
		FCMerkleTree merkle;
		
		std::string buildCoinbaseHash(std::string extranonce);
		
	public:
		FCBlock();
		void update(int height, unsigned int nbits);
		unsigned int getHeight();
		unsigned int getNBits();
		uint256_t getTarget();
		float getDifficulty();
		uint64_t getReward();
		uint256_t getHash();
		std::string getWork(FCBlock *prevBlock);
		
		bool serializeHeader(FCBlock *prevBlock, std::string coinbaseHash, unsigned int timestamp, unsigned int nonce, char *buffer);
};


#endif /* _FCBLOCK_H */
