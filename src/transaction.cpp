#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "transaction.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCTransaction::FCTransaction() {
	this->version = 1;
	this->lockTime = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransaction::build(std::string walletAddress, uint64_t coinbasevalue, std::string flags, unsigned int height, std::string data) {
	try {
		uint32_t timestamp = time(0);
		// self.nVersion = 1
        // self.vin = []
        // self.vout = []
        // self.nLockTime = 0
        // self.sha256 = None
		
		this->txin.push_back(FCTransactionIn());
		if(this->txin[0].buildScriptSig(height, flags, timestamp) == false) {
			throw FCException("Failed to build TXIn Script Sig");
		}
		
		this->txout.push_back(FCTransactionOut());
		if(this->txout[0].buildScriptPubKey(walletAddress, coinbasevalue) == false) {
			throw FCException("Failed to build TXIn Script Sig");
		}
		
		std::string serialized = this->serialize();
		
		size_t pos = serialized.find("f000000ff111111f");
		if(pos == std::string::npos) {
			throw FCException("Failed to find separator");
		}
		this->values.push_back(serialized.substr(0, pos));
		this->values.push_back(serialized.substr(pos + 16));
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCTransaction::serialize() {
	try {
		std::string value = "";
		
		value += FC::swapEndian(FC::numberToHex(this->version));
		//std::cout << "TXIN " << FC::serializeVector(this->txin) << std::endl;
		value += FC::serializeVector(this->txin);
		
		//std::cout << "TXOUT " << FC::serializeVector(this->txout) << std::endl;
		value += FC::serializeVector(this->txout);
		value += FC::numberToHex(this->lockTime);
		
		return value;
	}
	catch(FCException &e) {
		return "";
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransaction::deserialize(std::string transaction) {
	if(transaction.size() < 8) {
		throw FCException("Unable to deserialize transaction, invalid hex[1]");
	}
	//this->version = this->get_number32(transaction.substr(0, 8), true);
	//transaction = transaction.substr(8);
	
	//this->txin = this->deser_vector(transaction, AMPTransactionIn());
	//this->txout = this->deser_vector(transaction, AMPTransactionOut());
	
	if(transaction.size() < 8) {
		throw FCException("Unable to deserialize transaction, invalid hex[2]");
	}
	//this->lockTime = this->get_number32(transaction.substr(0, 8), true);
	
	return true;
}

