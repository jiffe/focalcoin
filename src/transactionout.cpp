#include <stdio.h>
#include <stdlib.h>
#include "transactionout.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCTransactionOut::FCTransactionOut() {
	this->scriptPubKey = "";
	this->coinbasevalue = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransactionOut::buildScriptPubKey(std::string walletAddress, uint64_t coinbasevalue) {
	try {
		
		this->coinbasevalue = coinbasevalue;
		this->scriptPubKey = FC::scriptToAddress(walletAddress);
		
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
std::string FCTransactionOut::serialize() {
	std::string hex = "";
	
	hex += FC::numberToHex(this->coinbasevalue);
	hex += FC::serializeHexString(this->scriptPubKey);
	
	return hex;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransactionOut::deserialize(std::string &hex) {
	if(hex.size() < 16) {
		throw FCException("Unable to deserialize transaction out, invalid hex[1]");
	}
	//this->coinbasevalue = this->get_number64(hex.substr(0, 16), true);
	hex = hex.substr(16);
	
	//this->scriptPubKey = this->deser_string(hex);
	
	return true;
}

