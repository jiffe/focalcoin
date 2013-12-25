#ifndef _FCNETWORK_H
#define _FCNETWORK_H


#include <pthread.h>
#include <vector>
#include <string>
#include <libwebsockets.h>
#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include "web.h"


class FCNetwork {
	private:
		pthread_t thread;
		struct libwebsocket_context *websocket_context;
		irc_session_t *irc_session;
		
		bool reconnect_irc();
		
	public:
		bool start();
		
		static int callback_web(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);
		static int callback_jsonrpc(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *obj, void *in, size_t len);
		static void *run(void *obj);
};


#endif /* _FCNETWORK_H */
