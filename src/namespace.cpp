#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <iomanip>
#include <ctime>
#include "namespace.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
namespace FC {
	
	// Logging constants
	const int ERROR = -1;
	const int SUCCESS = 0;
	const int INFO = 1;
	const int WARN2 = 2;
	const int WARN1 = 3;
	const int DEBUG1 = 4;
	const int DEBUG2 = 5;
	const int DEBUG3 = 6;
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string expandUser(std::string path) {
		if(not path.empty() and path[0] == '~') {
			if(path.size() != 1 and path[1] != '/') return "";
			char const* home = getenv("HOME");
			if(home || (home = getenv("USERPROFILE"))) {
				path.replace(0, 1, home);
			}
			else {
				char const *hdrive = getenv("HOMEDRIVE"),
				*hpath = getenv("HOMEPATH");
				if(!hdrive) return "";
				if(!hpath) return "";
				path.replace(0, 1, std::string(hdrive) + hpath);
			}
		}
		return path;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string buildVersion(int major, int minor, int revision, bool flat) {
		char buffer[64];
		
		if(flat) {
			sprintf(buffer, "%d%02d%02d", major, minor, revision);
		}
		else {
			sprintf(buffer, "%d.%02d.%02d", major, minor, revision);
		}
		
		return std::string(buffer);
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string timestampToString(unsigned int timestamp) {
		if(timestamp == 0) return "Never";
		std::time_t now = timestamp;
		std::tm *ptm = std::localtime(&now);
		char buffer[32];
		// Format: Mo, 15.06.2009 20:20:00
		std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S %z", ptm);
		return std::string(buffer);
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string numberToHex(uint32_t value) {
		char c[9];
		
		sprintf(c, "%08x", value);
		
		return std::string(c);
	}


	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string numberToHex(uint64_t number) {
		std::string hex = "";
		
		hex = numberToHex((uint32_t)(number & 0xffffffff));
		number >>= 32;
		hex = numberToHex((uint32_t)(number & 0xffffffff)) + hex;
		
		return hex;
	}


	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string numberToHex(uint256_t number) {
		std::string hex = "";
		char c[3];
		int i;
		
		for(i = 0; i < 8; i++) {
			hex = numberToHex((uint32_t)(number & 0xffffffff)) + hex;
			number >>= 32;
		}
		
		return hex;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string swapEndian(std::string number) {
		std::string swappedNumber = "";
		
		for(int i = number.size() - 2; i >= 0; i-=2) {
			swappedNumber += number.substr(i, 2);
		}
		
		return swappedNumber;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string bufferToHex(const unsigned char *buffer, unsigned int buffer_len) {
		std::string hex = "";
		char c[3];
		int i;
		
		for(i = 0; i < buffer_len; i++) {
			sprintf(c, "%02x", buffer[i]);
			hex += std::string(c);
		}
		
		return hex;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	char hexToChar(std::string hex) {
		char v = 0;
		
		for(int i = 0; i < 2; i++) {
			v <<= 4;
			char d = hex[i];
			if(d >= '0' && d <= '9') { d -= '0'; }
			else if(d >= 'A' && d <= 'F') { d -= 55; }
			else if(d >= 'a' && d <= 'f') { d -= 87; }
			v += d;
		}
		
		return v;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	int hexToBuffer(std::string hex, unsigned char *buffer) {
		if(hex.size() % 2) return 0;
		for(int i = 0; i < hex.size(); i+=2) {
			*buffer++ = FC::hexToChar(hex.substr(i));
		}
		return hex.size() >> 1;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string doublesha256(std::string str) {
		unsigned char hash[SHA256_DIGEST_LENGTH];
		char *buffer;
		SHA256_CTX sha256;
		int i;
		
		buffer = (char *)malloc(str.size() / 2);
		if(!buffer) {
			return "";
		}
		for(i = 0; i < str.size() / 2; i++) {
			buffer[i] = FC::hexToChar(str.substr(i * 2, 2));
		}
		
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, buffer, i);
		SHA256_Final(hash, &sha256);
		
		memcpy(buffer, hash, SHA256_DIGEST_LENGTH);
		
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, buffer, SHA256_DIGEST_LENGTH);
		SHA256_Final(hash, &sha256);
		
		free(buffer);
		
		std::stringstream ss;
		for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
		}
		
		return ss.str();
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string scriptToAddress(std::string address) {
		address = FC::base58Decode(address);
		if(address.size() < 12) {
			return "";
		}
		address = address.substr(2, address.length() - 10);
		return "76a914" + address + "88ac";
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string base58Decode(std::string str) {
		std::string b58chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
		std::string hex = "";
		uint256_t v;
		char c[3];
		std::string padding = "";
		bool pad = true;

		uint256_t multiplier = 1;
		for(int i = 0; i < str.length(); i++) {
			char x = str[str.length() - i - 1];
			if(b58chars.find(x) == std::string::npos) {
				return "";
			}
			v += multiplier * b58chars.find(x);
			if(pad && str[i] == '1') padding += "00";
			else pad = false;
			multiplier *= 58;
		}

		while(v > 256) {
			sprintf(c, "%02x", (unsigned char)(v % 256));
			hex = std::string(c) + hex;
			v /= 256;
		}
		
		sprintf(c, "%02x", (unsigned char)(v % 256));
		hex = std::string(c) + hex;
		
		return padding + hex;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string serializeString(std::string str) {
		std::string hex;
		int size = str.length();
		int i;
		char c[3];
		
		if(size < 253) {
			sprintf(c, "%02x", size);
			hex = std::string(c);
		}
		
		for(i = 0; i < str.length(); i++) {
			sprintf(c, "%02x", (char)str[i]);
			hex += std::string(c);
		}
		
		return hex;
	}
	
	
	/***************************************************************************************************
	*
	*
	***************************************************************************************************/
	std::string serializeHexString(std::string str) {
		std::string hex;
		int size = str.length() / 2;
		int i;
		char c[3];
		
		if(size < 253) {
			sprintf(c, "%02x", size);
			hex = std::string(c);
		}
		else if(size < 0x10000) {
			//return chr(253) + struct.pack("<H", len(s)) + s
		}
		else if(size < 0x100000000L) {
			//return chr(254) + struct.pack("<I", len(s)) + s
		}
		else {
			//return chr(255) + struct.pack("<Q", len(s)) + s
		}
		
		return hex + str;
	}
	
	
}

