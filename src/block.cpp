#include <stdio.h>
#include <string.h>
#include "block.h"
#include "inventory.h"
#include "exception.h"


extern FCInventory inv;


/***************************************************************************************************
* 
***************************************************************************************************/
FCBlock::FCBlock() {
	this->version = FC_INVENTORYVERSION;
	this->extranonce = 0;
}


/***************************************************************************************************
* 
***************************************************************************************************/
void FCBlock::update(int height, unsigned int nbits) {
	this->height = height;
	this->nbits = nbits;
	this->baseReward = (uint64_t)100000000 * (uint64_t)50000;
	this->coinbase.build(inv.getAccountAddress(""), this->baseReward, "", this->height, "");
}


/***************************************************************************************************
* 
***************************************************************************************************/
unsigned int FCBlock::getHeight() {
	return this->height;
}


/***************************************************************************************************
* 
***************************************************************************************************/
unsigned int FCBlock::getNBits() {
	return this->nbits;
}


/***************************************************************************************************
* 
***************************************************************************************************/
uint256_t FCBlock::getTarget() {
	uint256_t target = 0;
	
	int n_digits = (this->nbits & 0xff000000) >> 24;
	target = this->nbits & 0xffffff;
	if(n_digits > 2) {
		target <<= (n_digits - 3) * 8;
	}
	else {
		target >>= (3 - n_digits) * 8;
	}
	
	return target;
}


/***************************************************************************************************
* 
***************************************************************************************************/
float FCBlock::getDifficulty() {
	uint256_t ref_target = 0;
	
	for(int i = 0; i < 7; i++) {
		ref_target <<= 32;
		ref_target |= 0xffffffff;
	}
	
	uint256_t target = getTarget();
	
	return target ? (double)ref_target / (double)target : 0.0;
}


/***************************************************************************************************
* 
***************************************************************************************************/
uint64_t FCBlock::getReward() {
	return this->baseReward;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCBlock::buildCoinbaseHash(std::string extranonce) {
	std::string coinbase = this->coinbase.get(0) + extranonce + this->coinbase.get(1);
	return FC::doublesha256(coinbase);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCBlock::serializeHeader(FCBlock *prevBlock, std::string coinbaseHash, unsigned int timestamp, unsigned int nonce, char *buffer) {
	uint256_t prevHash = prevBlock ? prevBlock->getHash() : (uint256_t)0;
	
	buffer = (char *)memcpy(buffer, &this->version, sizeof(this->version)) + sizeof(this->version);
	buffer = (char *)memcpy(buffer, &prevHash, sizeof(prevHash)) + sizeof(prevHash); // reverse ?
	
	std::string merkle_root = this->merkle.build(coinbaseHash);
	buffer += FC::hexToBuffer(merkle_root, (unsigned char *)buffer);
	
	buffer = (char *)memcpy(buffer, &timestamp, sizeof(timestamp)) + sizeof(timestamp);
	buffer = (char *)memcpy(buffer, &this->nbits, sizeof(this->nbits)) + sizeof(this->nbits);
	buffer = (char *)memcpy(buffer, &nonce, sizeof(nonce)) + sizeof(nonce);
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
uint256_t FCBlock::getHash() {
	return 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCBlock::getWork(FCBlock *prevBlock) {
	char buffer[80];
	uint32_t tm = time(0);
	
	std::string coinbaseHash = this->buildCoinbaseHash(FC::numberToHex(this->extranonce++));
	//std::string coinbaseHash = this->buildCoinbaseHash(FC::numberToHex(this->extranonce));
	this->serializeHeader(prevBlock, coinbaseHash, tm, 0, buffer);
	
	return FC::bufferToHex((unsigned char *)buffer, 80) + "000000800000000000000000000000000000000000000000000000000000000000000000000000000000000080020000";
}

