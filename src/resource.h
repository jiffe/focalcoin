#ifndef _FCRESOURCE_H
#define _FCRESOURCE_H


#include <stdlib.h>
#include <string.h>


struct FCResource {
	char *data;
	char *name;
	int size;
	char *buffer;
	int contentType;
	
	FCResource() { bzero(this, sizeof(struct FCResource)); }
	~FCResource() { if(this->buffer) free(this->buffer); }
};


#endif /* _FCRESOURCE_H */
