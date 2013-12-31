#include <stdio.h>
#include <stdlib.h>
#include "transactionin.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCTransactionIn::FCTransactionIn() {
	this->prevout_hash = 0;
	this->prevout_n = 0xffffffff;
	this->template0 = "";
	this->template1 = "";
	this->scriptSig = "";
	this->nSequence = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransactionIn::buildScriptSig(unsigned int height, std::string flags, unsigned int timestamp) {
	try {
		
		flags = "062f503253482f";
		
		this->template0 += FC::serializeNumber(height);
		this->template0 += flags;
		this->template0 += FC::serializeNumber(timestamp);
		this->template0 += "08";
		
		this->template1 = FC::serializeString("/stratumPool/");
		
		this->scriptSig = this->template0 + "f000000ff111111f" + this->template1;
		
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
bool FCTransactionIn::buildScriptSig(std::string extranonce) {
	this->scriptSig = this->template0 + extranonce + this->template1;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCTransactionIn::serialize() {
	std::string hex = "";
	
	hex += FC::numberToHex(this->prevout_hash);
	hex += FC::numberToHex(this->prevout_n);
	//printf("SCRIPT SIG %s\n", this->scriptSig.c_str());
	hex += FC::serializeHexString(this->scriptSig);
	hex += FC::numberToHex(this->nSequence);
	
	return hex;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCTransactionIn::deserialize(std::string &hex) {
	
	if(hex.size() < 72) {
		throw FCException("Unable to deserialize transaction in, invalid hex[1]");
	}
	//this->prevout_hash = this->get_number256(hex.substr(0, 64), true);
	hex = hex.substr(64);
	
	//this->prevout_n = this->get_number32(hex.substr(0, 8), true);
	hex = hex.substr(8);
	
	//this->scriptSig = this->deser_string(hex);
	
	if(hex.size() < 8) {
		throw FCException("Unable to deserialize transaction in, invalid hex[2]");
	}
	//this->nSequence = this->get_number32(hex.substr(0, 8), true);
	hex = hex.substr(8);
	
	return true;
}

