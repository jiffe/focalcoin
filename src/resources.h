#ifndef _FCRESOURCES_H
#define _FCRESOURCES_H


#include <string>
#include "resource.h"


enum {
	FC_RESOURCETYPE_UNKNOWN,
	FC_RESOURCETYPE_TMPL,
	FC_RESOURCETYPE_JS,
	FC_RESOURCETYPE_CSS,
	FC_RESOURCETYPE_JPEG,
	FC_RESOURCETYPE_GIF,
	FC_RESOURCETYPE_PNG,
	FC_RESOURCETYPE_JSON
};


class FCResources {
	private:
		void *resource_handle;
		
		bool render(FCResource *r);
		
	public:
		FCResources() { this->resource_handle = NULL; }
		FCResource *get(std::string path);
};

#endif /* _FCRESOURCES_H */

