#ifndef _FCNAMESPACE_H
#define _FCNAMESPACE_H


#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "uint256.h"


#define FC_COINNAME "FocalCoin"

#define FC_DEFAULT_CONFIG_PATH "~/.focalcoin/focalcoin.conf"
#define FC_DEFAULT_FOREGROUND "false"
#define FC_DEFAULT_DATA_DIRECTORY "~/.focalcoin/data"
#define FC_PIDFILE "~/.focalcoin/focalcoin.pid"
#define FC_DEFAULT_LISTEN "14742"
#define FC_DEFAULT_DEBUG "0"
#define FC_DEFAULT_USERNAME "focalcoin"

#define FCDEFAULT_NBITS 0x1e00ffff

#define FC_VERSION_MAJOR 0
#define FC_VERSION_MINOR 0
#define FC_VERSION_REVISION 1

#define FC_PROTOCOLVERSION 1

#define FC_INVENTORYVERSION 1


// Namespace prototype
namespace FC {
	
	// Logging constants
	extern const int ERROR;
	extern const int SUCCESS;
	extern const int INFO;
	extern const int WARN2;
	extern const int WARN1;
	extern const int DEBUG1;
	extern const int DEBUG2;
	extern const int DEBUG3;
	
	// Some miscellaneous functions
	std::string expandUser(std::string path);
	std::string buildVersion(int major, int minor, int revision, bool flat = false);
	std::string timestampToString(unsigned int timestamp);
	
	// Conversion functions
	std::string numberToHex(uint32_t value);
	std::string numberToHex(uint64_t number);
	std::string numberToHex(uint256_t number);
	char hexToChar(std::string hex);
	std::string doublesha256(std::string str);
	std::string bufferToHex(const unsigned char *buffer, unsigned int buffer_len);
	int hexToBuffer(std::string hex, unsigned char *buffer);
	std::string scriptToAddress(std::string address);
	std::string base58Decode(std::string str);
	
	std::string serializeString(std::string str);
	std::string serializeHexString(std::string str);
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	template < typename T > std::string toString(const T& v) {
		std::ostringstream stm ;
		return ( stm << v ) ? stm.str() : "" ;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	template <class T> std::string serializeVector(std::vector<T> v) {
		std::string hex = "";
		char c[3];
		
		if(v.size() < 253) {
			sprintf(c, "%02x", (unsigned int)v.size());
			hex = std::string(c);
		}
		
		for(int i = 0; i < v.size(); i++) {
			hex += v[i].serialize();
		}
		
		return hex;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	template <class T> std::string serializeNumber(T number) {
		std::string hex;
		int size = 1;
		char c[3];
		
		while(number > 127) {
			size++;
			sprintf(c, "%02x", (unsigned char)(number % 256));
			hex += std::string(c);
			number /= 256;
		}
		sprintf(c, "%02x", (unsigned char)(number % 256));
		hex += std::string(c);
		
		sprintf(c, "%02x", size);
		hex = std::string(c) + hex;
		
		return hex;
	}
	
	
}


#endif /* _FCNAMESPACE_H */
