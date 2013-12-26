#include <string.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include "walletaddress.h"
#include "uint256.h"
#include "exception.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAddress::FCWalletAddress(std::string accountName, std::string notes) {
	unsigned char publicBuffer[65], privateBuffer[32];
	EC_KEY *eckey = NULL;
	EC_POINT *pub_key = NULL;
	const EC_GROUP *group = NULL;
	BIGNUM start, x, y;
	BIGNUM priv;
	BIGNUM *res;
	BN_CTX *ctx;
	
	
	BN_init(&start);
	BN_init(&x);
	BN_init(&y);
	
	ctx = BN_CTX_new();
	
	res = &start;
	
start:
	
	BN_rand(res, 256, -1, 0);
	
	eckey = EC_KEY_new_by_curve_name(NID_secp256k1);
	group = EC_KEY_get0_group(eckey);
	pub_key = EC_POINT_new(group);
	
	EC_KEY_set_private_key(eckey, res);
	
	if(!EC_POINT_mul(group, pub_key, res, NULL, NULL, ctx)) {
		goto start;
	}
	
	EC_KEY_set_public_key(eckey, pub_key);
	
	EC_POINT_get_affine_coordinates_GFp(group, pub_key, &x, &y, ctx);
	publicBuffer[0] = 4;
	BN_bn2bin(&x, publicBuffer + 1);
	BN_bn2bin(&y, publicBuffer + 33);
	BN_bn2bin(res, privateBuffer);
	
	this->privKey = this->buildPrivateKey(privateBuffer);
	this->pubKey = this->buildPublicKey(publicBuffer);
	
	BN_CTX_free(ctx);
	
	this->balance = 0;
	this->accountName = accountName;
	this->notes = notes;
	this->createDate = time(0);
	this->lastDepositDate = 0;
	this->lastWithdrawalDate = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCWalletAddress::FCWalletAddress(std::string addressJSON) {
	Json::Reader reader;
	Json::Value json;
	
	this->accountName = "";
	this->privKey = "";
	this->pubKey = "";
	this->notes = "";
	this->balance = 0;
	
	if(!reader.parse(addressJSON, json)) {
		return;
	}
	
	if(json["accountName"].isString()) {
		this->accountName = json["accountName"].asString();
	}
	if(json["public"].isString()) {
		this->pubKey = json["public"].asString();
	}
	if(json["public"].isString()) {
		this->privKey = json["public"].asString();
	}
	if(json["balance"].isInt()) {
		this->balance = json["balance"].asUInt64();
	}
	if(json["createDate"].isInt()) {
		this->createDate = json["createDate"].asUInt();
	}
	if(json["lastDepositDate"].isInt()) {
		this->lastDepositDate = json["lastDepositDate"].asUInt();
	}
	if(json["lastWithdrawalDate"].isInt()) {
		this->lastWithdrawalDate = json["lastWithdrawalDate"].asUInt();
	}
	if(json["notes"].isString()) {
		this->privKey = json["notes"].asString();
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
Json::Value FCWalletAddress::getJSON(bool display) {
	Json::Value json = Json::Value(Json::objectValue);
	json["accountName"] = this->accountName;
	json["public"] = this->pubKey;
	json["private"] = this->privKey;
	json["balance"] = display ? (double)this->balance / 1000000.0 : Json::UInt64(this->balance);
	json["notes"] = this->notes;
	json["createDate"] = display ? Json::Value(FC::timestampToString(this->createDate)) : Json::Value(this->createDate);
	json["lastDepositDate"] = display ? Json::Value(FC::timestampToString(this->lastDepositDate)) : Json::UInt(this->lastDepositDate);
	json["lastWithdrawalDate"] = display ? Json::Value(FC::timestampToString(this->lastWithdrawalDate)) : Json::UInt(this->lastWithdrawalDate);
	return json;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCWalletAddress::buildPublicKey(unsigned char *buffer) {
	unsigned char digest1[32], digest2[32];
	unsigned int dgstlen;
	
	// SHA256
	
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_sha256());
	
	EVP_DigestUpdate(&md_ctx, buffer, 65);
	
	EVP_DigestFinal(&md_ctx, digest1, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	// RIPEMD160
	
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_ripemd160());
	
	EVP_DigestUpdate(&md_ctx, digest1, 32);
	
	EVP_DigestFinal(&md_ctx, digest1 + 1, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	digest1[0] = 0;
	
	// SHA256
	
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_sha256());
	
	EVP_DigestUpdate(&md_ctx, digest1, 21);
	
	EVP_DigestFinal(&md_ctx, digest2, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	// SHA256
	
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_sha256());
	
	EVP_DigestUpdate(&md_ctx, digest2, 32);
	
	EVP_DigestFinal(&md_ctx, digest2, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	digest1[21] = digest2[0];
	digest1[22] = digest2[1];
	digest1[23] = digest2[2];
	digest1[24] = digest2[3];
	
	return this->base58Encode(digest1, 25);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCWalletAddress::buildPrivateKey(unsigned char *buffer) {
	unsigned char digest1[37], digest2[32];
	unsigned int dgstlen;
	
	digest1[0] = 0x80;
	memcpy(digest1 + 1, buffer, 32);
	
	// SHA256
	
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_sha256());
	
	EVP_DigestUpdate(&md_ctx, digest1, 33);
	
	EVP_DigestFinal(&md_ctx, digest2, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	//for(int i = 0; i < 32; i++) { printf("%02x", digest2[i]); } printf("\n");
	
	// SHA256
	
	EVP_MD_CTX_init(&md_ctx);
	EVP_DigestInit(&md_ctx, EVP_sha256());
	
	EVP_DigestUpdate(&md_ctx, digest2, 32);
	
	EVP_DigestFinal(&md_ctx, digest2, &dgstlen); 
	EVP_MD_CTX_cleanup(&md_ctx);
	
	digest1[33] = digest2[0];
	digest1[34] = digest2[1];
	digest1[35] = digest2[2];
	digest1[36] = digest2[3];
	
	return this->base58Encode(digest1, 37);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCWalletAddress::base58Encode(unsigned char *address, int addressSize) {
	static const std::string digits = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	std::string result = "";
	BIGNUM n, mod, base;
	BN_CTX *ctx = BN_CTX_new();;
	
	BN_init(&n);
	BN_init(&mod);
	BN_init(&base);
	
	BN_bin2bn(address, addressSize, &n);
	BN_set_word(&base, 58);
	
	while(!BN_is_zero(&n)) {
		BN_div(&n, &mod, &n, &base, ctx);
		result = digits[BN_get_word(&mod)] + result;
	}
	
	BN_CTX_free(ctx);
	
	for(int i = 0; i < addressSize && address[i] == 0; i++) {
		result = digits[0] + result;
	}
	
	return result;
}

