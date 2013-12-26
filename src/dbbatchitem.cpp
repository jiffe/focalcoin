#include <stdlib.h>
#include <string.h>
#include "dbbatchitem.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCDBBatchItem::FCDBBatchItem(std::string key, const char *value, int valueSize) {
	this->key = key;
	this->value = new char[valueSize];
	this->valueSize = 0;
	if(this->value) {
		memcpy(this->value, value, valueSize);
		this->valueSize = valueSize;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCDBBatchItem::FCDBBatchItem(const FCDBBatchItem &copy) {
	this->key = copy.key;
	this->value = new char[copy.valueSize];
	this->valueSize = 0;
	if(this->value) {
		memcpy(this->value, copy.value, copy.valueSize);
		this->valueSize = copy.valueSize;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCDBBatchItem::~FCDBBatchItem() {
	if(this->value) {
		delete [] this->value;
	}
	this->value = NULL;
}

