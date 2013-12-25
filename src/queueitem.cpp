#include <stdlib.h>
#include <string.h>
#include "queueitem.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCQueueItem::FCQueueItem() {
	this->writable = false;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
FCQueueItem::FCQueueItem(int itemType) {
	this->init(itemType);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::init(int itemType) {
	this->itemType = itemType;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::write(char *bufferData, int bufferSize, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	buffer->write(bufferData, bufferSize);
	if(bufferType) this->writable = true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::write(char *bufferData, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	buffer->write(bufferData);
	if(bufferType) this->writable = true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::write(std::string bufferData, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	buffer->write(bufferData);
	if(bufferType) this->writable = true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::write(Json::Value bufferData, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	buffer->write(bufferData);
	if(bufferType) this->writable = true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCQueueItem::getItemType() {
	return this->itemType;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::advance(int bytes, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	if(buffer->advance(bytes)) {
		this->writable = false;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCQueueItem::getBuffer(int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	return buffer->getBuffer();
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCQueueItem::getLine(int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	return buffer->getLine();
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueueItem::setRawBuffer(char *bufferData, int bufferSize, int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	return buffer->setRawBuffer(bufferData, bufferSize);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
char *FCQueueItem::getRawBuffer(int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	return buffer->getRawBuffer();
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCQueueItem::getBufferSize(int bufferType) {
	FCBuffer *buffer = bufferType ? &this->output : &this->input;
	return buffer->getBufferSize();
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCQueueItem::isWritable() {
	return this->writable == 1;
}

