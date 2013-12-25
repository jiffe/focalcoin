#include <stdlib.h>
#include <string.h>
#include "buffer.h"


/***************************************************************************************************
*
*
***************************************************************************************************/
FCBuffer::FCBuffer() {
	this->buffer = NULL;
	this->bufferSize = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::reset() {
	if(this->buffer) free(this->buffer);
	this->buffer = NULL;
	this->bufferSize = 0;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::write(char *buffer, int bufferSize) {
	this->buffer = (char *)realloc(this->buffer, this->bufferSize + bufferSize + 1);
	memcpy(this->buffer + this->bufferSize, buffer, bufferSize);
	printf("Write %d bytes at offset %d\n", bufferSize, this->bufferSize);
	this->bufferPtr = this->buffer;
	this->bufferSize += bufferSize;
	this->buffer[this->bufferSize] = 0;
	printf("BUFFER %s\n", this->buffer);
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::write(char *buffer) {
	this->write(buffer, strlen(buffer));
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::write(std::string buffer) {
	this->write((char *)buffer.c_str());
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::write(Json::Value buffer) {
	Json::FastWriter writer;
	this->write(writer.write(buffer));
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCBuffer::advance(int bytes) {
	if(bytes < this->bufferSize) {
		this->bufferPtr += bytes;
		this->bufferSize -= bytes;
		return false;
	}
	this->reset();
	return true;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCBuffer::getBuffer() {
	return this->bufferPtr;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
std::string FCBuffer::getLine() {
	char *newline, *line;
	if((newline = strtok_r(this->bufferPtr, "\n", &newline))) {
		line = this->bufferPtr;
		this->bufferSize -= (newline - this->bufferPtr) + 1;
		this->bufferPtr = newline + 1;
		return std::string(line);
	}
	return "";
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCBuffer::setRawBuffer(char *buffer, int bufferSize) {
	this->reset();
	this->buffer = buffer;
	this->bufferSize = bufferSize;
	this->bufferPtr = this->buffer;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
char *FCBuffer::getRawBuffer() {
	return this->bufferPtr;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCBuffer::getBufferSize() {
	return this->bufferSize;
}
