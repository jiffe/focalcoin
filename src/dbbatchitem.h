#ifndef _FCDBBATCHITEM_H
#define _FCDBBATCHITEM_H


#include <string>


class FCDBBatchItem {
	private:
		std::string key;
		char *value;
		int valueSize;
		
	public:
		FCDBBatchItem(std::string key, const char *value, int valueSize);
		~FCDBBatchItem();
		FCDBBatchItem(const FCDBBatchItem &copy);
		
		std::string getKey() { return this->key; }
		char *getValue() { return this->value; }
		int getValueSize() { return this->valueSize; }
};


#endif /* _FCDBBATCHITEM_H */
