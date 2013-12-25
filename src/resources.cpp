#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <include/template.hpp>
#include <iostream>
#include "resources.h"
#include "exception.h"
#include "resources.asm"


FCResources resources;


/***************************************************************************************************
*
*
***************************************************************************************************/
FCResource *FCResources::get(std::string path) {
	FCResource *r = new FCResource();
	char key[32];
	char *error;
	
	if(path[path.size()-1] == '/') {
		path += "index.tmpl";
	}
	path = path.substr(1);
	
	if(this->resource_handle == NULL) {
		this->resource_handle = dlopen(NULL, RTLD_NOW);
	}
	if(this->resource_handle == NULL) {
		return r;
	}
	
	for(int i = 0; i < numResources; i++) {
		
		sprintf(key, "resource_name%d", i);
		char *name = (char *)dlsym(this->resource_handle, key);
		
		if(name != NULL && strcmp(name, path.c_str()) == 0) {
			r->name = name;
			
			sprintf(key, "resource%d", i);
			r->data = (char *)dlsym(this->resource_handle, key);
			
			sprintf(key, "resource_size%d", i);
			r->size = *(int *)dlsym(this->resource_handle, key);
			
			this->render(r);
			
			return r;
		}
	}
	
	return r;
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCResources::render(FCResource *r) {
	PlustacheTypes::ObjectType ctx;
	Plustache::template_t t;
	
	ctx["coin_name"] = FC_COINNAME;
	ctx["coin_version"] = FC::buildVersion(FC_VERSION_MAJOR, FC_VERSION_MINOR, FC_VERSION_REVISION);
	
	char *ext = strrchr(r->name, '.');
	r->contentType = FC_RESOURCETYPE_UNKNOWN;
	if(ext++) {
		if(strcasecmp(ext, "tmpl") == 0) { r->contentType = FC_RESOURCETYPE_TMPL; }
		else if(strcasecmp(ext, "css") == 0) { r->contentType = FC_RESOURCETYPE_CSS; }
		else if(strcasecmp(ext, "js") == 0) { r->contentType = FC_RESOURCETYPE_JS; }
	}
	
	if(r->contentType == FC_RESOURCETYPE_TMPL) {
		char *buffer1 = (char *)malloc(r->size + 1);
		memcpy(buffer1, r->data, r->size);
		buffer1[r->size] = '\0';
		
		std::string tmpl(buffer1);
		std::string result = t.render(tmpl, ctx);
		
		free(buffer1);
		
		r->buffer = (char *)malloc(result.size() + 1);
		strcpy(r->buffer, result.c_str());
		r->size = result.size();
		r->data = r->buffer;
	}
	
	return true;
}

