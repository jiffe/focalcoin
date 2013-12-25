#ifndef _FCRPC_H
#define _FCRPC_H


#include <stdio.h>
#include <json/json.h>
#include <string>
#include <boost/thread.hpp>
#include "queueitem.h"


class FCRPC : public FCQueueItem {
	private:
		
	public:
		FCRPC();
		void handle();
		static bool handleGetInfo(Json::Value &request, Json::Value &response);
		static bool handleGetBlockTemplate(Json::Value &request, Json::Value &response);
		static bool handleGetWork(Json::Value &request, Json::Value &response);
		static bool handleListAccounts(Json::Value &request, Json::Value &response);
		static bool handleGetBalance(Json::Value &request, Json::Value &response);
		static bool handleGetBlockCount(Json::Value &request, Json::Value &response);
		static bool handleGetAddressesByAccount(Json::Value &request, Json::Value &response);
		static bool handleGetNewAddress(Json::Value &request, Json::Value &response);
		static bool handleGetAccountAddress(Json::Value &request, Json::Value &response);
};


#endif /* _FCRPC_H */
