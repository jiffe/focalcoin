#include <stdio.h>
#include <algorithm>
#include "resources.h"
#include "global.h"
#include "rpc.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCRPC::FCRPC() : FCQueueItem() {
	
}


/***************************************************************************************************
* Default rpc command option structure
***************************************************************************************************/
struct FCRPCCommandOption {
	std::string name;
	std::string type;
};


/***************************************************************************************************
* Default rpc command structure
***************************************************************************************************/
struct FCRPCCommand {
	std::string name;
	FCRPCCommandOption options[];
	bool (*handler)(Json::Value &, Json::Value &);
	std::string description;
};


/***************************************************************************************************
* Default set of rpc commands
***************************************************************************************************/
static const FCRPCCommand rpccommands[] = {
	{ "getinfo",				{},	FCRPC::handleGetInfo, "Get general information about this coin and its block chain" },
	{ "getblocktemplate",		{},	FCRPC::handleGetBlockTemplate, "Get data needed to construct a block to work on" },
	{ "getwork",				{},	FCRPC::handleGetWork, "Get data needed to construct a block to work on" },
	{ "listaccounts",			{},	FCRPC::handleListAccounts, "Returns Object that has account names as keys, account balances as values" },
	{ "getbalance",				{},	FCRPC::handleGetBalance, "If [account] is not specified, returns the server's total available balance\nIf [account] is specified, returns the balance in the account" },
	{ "getblockcount",			{},	FCRPC::handleGetBlockCount, "Returns the number of blocks in the longest block chain" },
	{ "getaddressesbyaccount",	{},	FCRPC::handleGetAddressesByAccount, "Returns the list of addresses for the given account" },
	{ "getnewaddress",			{},	FCRPC::handleGetNewAddress, "Returns the list of addresses for the given account" },
	{ "getaccountaddress",		{},	FCRPC::handleGetAccountAddress, "Returns the list of addresses for the given account" },
	{ "",						{},	NULL, "" }
};


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetInfo(Json::Value &request, Json::Value &response) {
	
	/*
	{
		"result": {
			"version": 60400,
			"protocolversion": 60001,
			"walletversion": 60000,
			"balance": 5404.5,
			"blocks": 173263,
			"connections": 3,
			"proxy": "",
			"difficulty": 0.18426824,
			"testnet": false,
			"keypoololdest": 1382542386,
			"keypoolsize": 101,
			"paytxfee": 0,
			"mininput": 0.0001,
			"errors": ""
		},
		"error": null,
		"id": null
	}
	*/
	
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = Json::Value(Json::objectValue);
	response["result"]["version"] = FC::buildVersion(FC_VERSION_MAJOR, FC_VERSION_MINOR, FC_VERSION_REVISION, true);
	response["result"]["protocolversion"] = FC_PROTOCOLVERSION;
	response["result"]["difficulty"] = inv.getCurrentDifficulty();
	//response["result"]["connections"] = peers.getPeerCount(true);
	response["result"]["blocks"] = inv.getCurrentHeight();
	//response["result"]["balance"] = wallet.getFormattedBalance();

	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetBlockTemplate(Json::Value &request, Json::Value &response) {
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getBlockTemplate();
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleListAccounts(Json::Value &request, Json::Value &response) {
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getAccounts();
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetBalance(Json::Value &request, Json::Value &response) {
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getBalance();
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetBlockCount(Json::Value &request, Json::Value &response) {
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getCurrentHeight();
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetAddressesByAccount(Json::Value &request, Json::Value &response) {
	std::string account = "";
	bool verbose = false;
	
	if(request["params"].size() > 0) {
		account = request["params"][0].asString();
	}
	if(request["params"].size() > 1) {
		std::string w = request["params"][1].asString();
		std::transform(w.begin(), w.end(), w.begin(), ::tolower);
		if(w == "true") {
			verbose = true;
		}
	}
	
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getAccountAddresses(account, verbose, true);
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetNewAddress(Json::Value &request, Json::Value &response) {
	std::string account = "";
	std::string notes = "Internally generated address";
	
	if(request["params"].size() > 0) {
		account = request["params"][0].asString();
	}
	if(request["params"].size() > 1) {
		notes = request["params"][1].asString();
	}
	
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = inv.getNewAddress(account, notes);
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetAccountAddress(Json::Value &request, Json::Value &response) {
	
	// error checking todo
	
	if(request["params"].size() > 0) {
		printf("GETWORK WITH PARAMS\n");
	}
	else {
		response["error"] = Json::Value(Json::nullValue);
		response["result"] = inv.getAccountAddress(request["params"][0].asString(), true);
	}
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCRPC::handleGetWork(Json::Value &request, Json::Value &response) {
	Json::Value tmpl = inv.getBlockTemplate();
	
	response["error"] = Json::Value(Json::nullValue);
	response["result"] = Json::Value(Json::objectValue);
	
	response["result"]["target"] = tmpl["target"];
	response["result"]["data"] = inv.getCurrentWork();
	
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCRPC::handle() {
	Json::Reader reader;
	Json::Value response = Json::Value(Json::objectValue);
	Json::Value request;
	
	if(!reader.parse(this->getBuffer(FCBUFFERTYPE_INPUT), request)) {
		printf("failed to parse '%s'\n", this->getBuffer(FCBUFFERTYPE_INPUT).c_str());
		return;
	}
	
	if(!request.isObject()) {
		// {"result":null,"error":{"code":-32700,"message":"Parse error"},"id":null}
		
		response["result"] = Json::Value(Json::nullValue);
		response["error"] = Json::Value(Json::objectValue);
		response["error"]["code"] = -32700;
		response["error"]["message"] = "Parse error";
		response["id"] = Json::Value(Json::nullValue);
		
		return this->write(response, FCBUFFERTYPE_OUTPUT);
	}
	
	response["id"] = request["id"];
	
	if(!request["method"].isString()) {
		// {"result":null,"error":{"code":-32601,"message":"Method not found"},"id":null}
		
		response["result"] = Json::Value(Json::nullValue);
		response["error"] = Json::Value(Json::objectValue);
		response["error"]["code"] = -32601;
		response["error"]["message"] = "Method not found";
		
		return this->write(response, FCBUFFERTYPE_OUTPUT);
	}
	
	std::string method = request["method"].asString();
	
	bool found = false;
	for(const FCRPCCommand *cmd = rpccommands; cmd->name != ""; cmd++) {
		if(method != "getwork") printf("Calling method %s\n", method.c_str());
		if(method == cmd->name) {
			cmd->handler(request, response);
			found = true;
		}
	}
	if(!found) {
		response["result"] = Json::Value(Json::nullValue);
		response["error"] = Json::Value(Json::objectValue);
		response["error"]["code"] = -32601;
		response["error"]["message"] = "Method not found";
		printf("METHOD %s not found\n", method.c_str());
	}
	
	return this->write(response, FCBUFFERTYPE_OUTPUT);
}

