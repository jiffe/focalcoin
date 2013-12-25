#ifndef _FCDB_H
#define _FCDB_H


#include <leveldb/db.h>


class FCDB {
	private:
		leveldb::DB* db;
		
	public:
		FCDB();
		bool init();
		int read(std::string key, std::string &value);
		int read(std::string key, int &value);

};


#endif /* _FCDB_H */
