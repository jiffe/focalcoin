#ifndef _FCWEB_H
#define _FCWEB_H


#include "resources.h"
#include "rpc.h"


class FCWeb : public FCRPC {
	private:
		bool headerCreated;
		int contentType;
		
	public:
		FCWeb();
		void setContentType(int contentType);
		bool encapsulate(int contentType=0);
};


#endif /* _FCWEB_H */
