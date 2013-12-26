#ifndef _FCDB_H
#define _FCDB_H


#include <leveldb/db.h>
#include <json/json.h>
#include <vector>
#include "dbbatchitem.h"


class FCDB {
	private:
		leveldb::DB  *db;
		std::vector <FCDBBatchItem> batchItems;
		
	public:
		FCDB();
		bool init();
		int read(std::string key, std::string &value);
		int read(std::string key, int &value);
		leveldb::Iterator *iterator();
		int write(std::string key, std::string value);
		int write(std::string key, Json::Value value);
		int writeBatch();
};


#endif /* _FCDB_H */
