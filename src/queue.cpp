#include <stdio.h>
#include <unistd.h>
#include "exception.h"
#include "queue.h"
#include "rpc.h"


extern int exit_signal;


/***************************************************************************************************
*
*
***************************************************************************************************/
void FCQueue::run() {
	FCQueueItem *item = NULL;
	
	while(!exit_signal) {
		{ // Critical Section
			this->lock.lock();
			if(this->items.size()) {
				item = this->items.front();
				this->items.pop_front();
			}
			else {
				item = NULL;
			}
			this->lock.unlock();
		}
		
		if(item) {
			item->handle();
		}
		
		usleep(100000);
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCQueue::enqueue(FCQueueItem *item) {
	{ // Critical Section
		this->lock.lock();
		this->items.push_back(item);
		this->lock.unlock();
	}
	return true;
}

