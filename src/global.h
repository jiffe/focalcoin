#ifndef _FCGLOBAL_H
#define _FCGLOBAL_H


#include "namespace.h"
#include "exception.h"
#include "inventory.h"
#include "config.h"
#include "peers.h"
#include "db.h"


extern int exit_signal;
extern FCConfig config;
extern FCInventory inv;
extern FCPeers peers;
extern FCDB db;


#endif