#ifndef _FCQUEUE_H
#define _FCQUEUE_H


#include <list>
#include <boost/thread.hpp>
#include "queueitem.h"


class FCQueue {
	private:
		boost::mutex lock;
		std::list <FCQueueItem *> items;
		
	public:
		void run();
		bool enqueue(FCQueueItem *item);
};


#endif /* _FCQUEUE_H */
