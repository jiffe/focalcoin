#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdint.h>
#include "network.h"
#include "config.h"
#include "resources.h"
#include "exception.h"
#include "peers.h"
#include "web.h"
#include "rpc.h"
#include "websocket.h"
#include "config.h"
#include "queue.h"


extern bool exit_signal;
extern FCNetwork network;
extern FCConfig config;
extern FCResources resources;
extern FCQueue queue;
extern FCConfig config;


struct FCWebSession {
	union {
		FCWeb *web;
		FCWebSocket *websocket;
	};
};

/***************************************************************************************************
*
*
***************************************************************************************************/
static struct libwebsocket_protocols protocols[] = {
	{
		"http-only",
		FCNetwork::callback_web,
		sizeof (struct FCWebSession),
		12,
	},
	{
		"jsonrpc",
		FCNetwork::callback_jsonrpc,
		sizeof(struct FCWebSession),
		0,
	},/*
	{
		"peer",
		FCNetwork::callback_peer,
		sizeof(struct per_session_data__dumb_increment),
		0,
	},
	*/
	{ NULL, NULL, 0, 0 } /* terminator */
};

static void
dump_handshake_info(struct libwebsocket *wsi)
{
	int n;
	static const char *token_names[] = {
		/*[WSI_TOKEN_GET_URI]		=*/ "GET URI",
		/*[WSI_TOKEN_POST_URI]		=*/ "POST URI",
		/*[WSI_TOKEN_HOST]		=*/ "Host",
		/*[WSI_TOKEN_CONNECTION]	=*/ "Connection",
		/*[WSI_TOKEN_KEY1]		=*/ "key 1",
		/*[WSI_TOKEN_KEY2]		=*/ "key 2",
		/*[WSI_TOKEN_PROTOCOL]		=*/ "Protocol",
		/*[WSI_TOKEN_UPGRADE]		=*/ "Upgrade",
		/*[WSI_TOKEN_ORIGIN]		=*/ "Origin",
		/*[WSI_TOKEN_DRAFT]		=*/ "Draft",
		/*[WSI_TOKEN_CHALLENGE]		=*/ "Challenge",

		/* new for 04 */
		/*[WSI_TOKEN_KEY]		=*/ "Key",
		/*[WSI_TOKEN_VERSION]		=*/ "Version",
		/*[WSI_TOKEN_SWORIGIN]		=*/ "Sworigin",

		/* new for 05 */
		/*[WSI_TOKEN_EXTENSIONS]	=*/ "Extensions",

		/* client receives these */
		/*[WSI_TOKEN_ACCEPT]		=*/ "Accept",
		/*[WSI_TOKEN_NONCE]		=*/ "Nonce",
		/*[WSI_TOKEN_HTTP]		=*/ "Http",

		"Accept:",
		"If-Modified-Since:",
		"Accept-Encoding:",
		"Accept-Language:",
		"Pragma:",
		"Cache-Control:",
		"Authorization:",
		"Cookie:",
		"Content-Length:",
		"Content-Type:",
		"Date:",
		"Range:",
		"Referer:",
		"Uri-Args:",

		/*[WSI_TOKEN_MUXURL]	=*/ "MuxURL",
	};
	char buf[256];

	for (n = 0; n < sizeof(token_names) / sizeof(token_names[0]); n++) {
		if(!lws_hdr_total_length(wsi, (lws_token_indexes)n))
			continue;

		lws_hdr_copy(wsi, buf, sizeof(buf), (lws_token_indexes)n);

		fprintf(stderr, "    %s = %s\n", token_names[n], buf);
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCNetwork::callback_web(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *obj, void *in, size_t len) {
	try {
		FCWebSession *session = (FCWebSession *)obj;
		
		switch(reason) {
			case LWS_CALLBACK_PROTOCOL_INIT:
				if(wsi) {
					printf("Set timeout\n");
					//libwebsocket_set_timeout(wsi, PENDING_TIMEOUT_HTTP_CONTENT, 5);
				}
				break;
			case LWS_CALLBACK_CLOSED:
				// todo free
				break;
			case LWS_CALLBACK_HTTP: {
				dump_handshake_info(wsi);
				
				if(len < 1) {
					libwebsockets_return_http_status(context, wsi, HTTP_STATUS_BAD_REQUEST, NULL);
					throw FCException("Invalid http request length");
				}
				
				session->web = new FCWeb();
				if(lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI)) {
					return 0;
				}
				
				FCResource *r = resources.get(std::string((char *)in));
				if(r->size == 0) {
					libwebsockets_return_http_status(context, wsi, HTTP_STATUS_NOT_FOUND, NULL);
					throw FCException("Resource %s not found", (char *)in);
				}
				
				session->web->setContentType(r->contentType);
				session->web->write(r->data, r->size, FCBUFFERTYPE_OUTPUT);
				session->web->encapsulate(r->contentType);
				
				delete r;
				
				break;
			}
			case LWS_CALLBACK_HTTP_BODY:
			printf("HTTP BODY %d\n", len);
				session->web->write((char *)in, len, FCBUFFERTYPE_INPUT);
				
				break;
			case LWS_CALLBACK_HTTP_BODY_COMPLETION:
			printf("HTTP BODY COMPLETION\n");
				session->web->setContentType(FC_RESOURCETYPE_JSON);
				queue.enqueue(session->web);
				
				break;
			case LWS_CALLBACK_HTTP_WRITEABLE:
				if(session && session->web && session->web->isWritable()) {
					session->web->encapsulate();
					do {
						int bytes_written = libwebsocket_write(wsi, (unsigned char *)session->web->getRawBuffer(FCBUFFERTYPE_OUTPUT), (size_t)session->web->getBufferSize(FCBUFFERTYPE_OUTPUT), LWS_WRITE_HTTP);
						
						if(bytes_written < 0) {
							throw FCException("Failed to write HTTP response");
						}
						else {
							session->web->advance(bytes_written, FCBUFFERTYPE_OUTPUT);
						}
					}while(session->web->isWritable() && !lws_send_pipe_choked(wsi));
					
					if(!session->web->isWritable()) return -1;
				}
				break;
		}
		
		return 0;
	}
	catch(FCException &e) {
		return - 1;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
int FCNetwork::callback_jsonrpc(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *obj, void *in, size_t len) {
	try {
		FCWebSession *session = (FCWebSession *)obj;
		
		switch(reason) {
			case LWS_CALLBACK_ESTABLISHED:
				session->websocket = new FCWebSocket();
				break;
			case LWS_CALLBACK_CLIENT_ESTABLISHED: {
				
				session->websocket = new FCWebSocket();
				Json::Value request = Json::Value(Json::objectValue);
				request["id"] = 0;
				request["method"] = config.getCommand();
				request["params"] = Json::Value(Json::arrayValue);
				for(int i = 0; i < config.getCommandArgCount(); i++) {
					request["params"][i] = config.getCommandArg(i);
				}
				
				Json::FastWriter writer;
				session->websocket->write(writer.write(request), FCBUFFERTYPE_OUTPUT);
				
				break;
			}
			case LWS_CALLBACK_RECEIVE:
				session->websocket->write((char *)in, len, FCBUFFERTYPE_INPUT);
				queue.enqueue(session->websocket);
				break;
			case LWS_CALLBACK_CLIENT_RECEIVE: {
				session->websocket->write((char *)in, len, FCBUFFERTYPE_INPUT);
				std::string line = session->websocket->getLine(FCBUFFERTYPE_INPUT);
				if(line != "") {
					Json::Reader reader;
					Json::Value response;
					
					if(!reader.parse(line, response)) {
						// failure
						exit(-1);
					}
					if(!response["error"].isNull()) {
						exit(-1);
					}
					if(response["result"].isNull()) {
						exit(-1);
					}
					
					std::cout << response["result"] << std::endl;
					
					exit(0);
				}
				break;
			}
			case LWS_CALLBACK_CLIENT_WRITEABLE:
			case LWS_CALLBACK_SERVER_WRITEABLE:
				if(session && session->websocket && session->websocket->isWritable()) {
				//printf("LWS_CALLBACK_SERVER_WRITEABLE %d\n", session->rpc->isWritable());
					session->websocket->encapsulate();
					do {
						int bytes_written = libwebsocket_write(wsi, (unsigned char *)session->websocket->getRawBuffer(FCBUFFERTYPE_OUTPUT), (size_t)session->websocket->getBufferSize(FCBUFFERTYPE_OUTPUT), LWS_WRITE_TEXT);
						
						if(bytes_written < 0) {
							throw FCException("Failed to write HTTP response");
						}
						else {
							session->websocket->advance(bytes_written, FCBUFFERTYPE_OUTPUT);
						}
					}while(session->websocket->isWritable() && !lws_send_pipe_choked(wsi));
				}
				break;
		}
		
		return 0;
	}
	catch(FCException &e) {
		return - 1;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCNetwork::reconnect_irc() {
	try {
		if(irc_connect(this->irc_session, "irc.example.com", 6667, 0, "mynick", "myusername", "myrealname")) {
			throw FCException("Failed to create IRC connection attempt");
		}
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
void *FCNetwork::run(void *obj) {
	try {
		FCNetwork *self = (FCNetwork *)obj;
		uint64_t time1, time2;
		fd_set in_set, out_set;
		struct timeval tv;
		int maxfd = 0;
		int n;
		
		while(!exit_signal) {
			
			// Handle web sockets
			
			gettimeofday(&tv, NULL);
			time2 = (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
			
			if(time2 - time1 > 50000) {
				for(libwebsocket_protocols *p = protocols; p->name != NULL; p++) {
					libwebsocket_callback_on_writable_all_protocol(p);
				}
				time1 = time2;
			}
			
			n = libwebsocket_service(self->websocket_context, 50);
			
			
			// Handle IRC
			if(config.getBool("irc", false)) {
				if(!irc_is_connected(self->irc_session)) {
					self->reconnect_irc();
				}
				else {
					maxfd = 0;
					tv.tv_usec = 250000;
					tv.tv_sec = 0;
					
					FD_ZERO(&in_set);
					FD_ZERO(&out_set);
					
					irc_add_select_descriptors(self->irc_session, &in_set, &out_set, &maxfd);
					
					if(select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0) {
						FCLogger::log(FC::ERROR, "Failed to SELECT IRC descriptors");
					}
					else {
						if(irc_process_select_descriptors(self->irc_session, &in_set, &out_set)) {
							FCLogger::log(FC::ERROR, "Failed to process IRC descriptors");
						}
					}
				}
			}
		}
		
		libwebsocket_context_destroy(self->websocket_context);
		if(config.getBool("irc", false)) {
			irc_destroy_session(self->irc_session);
		}
		
		return NULL;
	}
	catch(FCException &e) {
		return NULL;
	}
}


/***************************************************************************************************
*
*
***************************************************************************************************/
bool FCNetwork::start() {
	try {
		struct lws_context_creation_info info;
		irc_callbacks_t callbacks;
		
		memset(&info, 0, sizeof info);
		info.gid = -1;
		info.uid = -1;
		info.options = 0;
		info.protocols = protocols;
		
		lws_set_log_level(0, lwsl_emit_syslog);
		
		if(config.getCommand() != "") {
			info.port = CONTEXT_PORT_NO_LISTEN;
			
			this->websocket_context = libwebsocket_create_context(&info);
			if(this->websocket_context == NULL) {
				throw FCException("Failed to create libwebsocket context");
			}
			
			std::string host = "localhost";
			int port = config.getInt("listen", 0);
			
			libwebsocket_client_connect(this->websocket_context, host.c_str(), port, 0, "/", host.c_str(), host.c_str(), "jsonrpc", -1);
			run(this);
		}
		else {
			
			// Setup web sockets
			
			info.port = config.getInt("listen", 0);
			info.iface = NULL;
			
			this->websocket_context = libwebsocket_create_context(&info);
			if(this->websocket_context == NULL) {
				throw FCException("Failed to create libwebsocket context");
			}
			
			
			// Setup IRC
			if(config.getBool("irc", false)) {
				bzero(&callbacks, sizeof(callbacks));
				
				this->irc_session = irc_create_session(&callbacks);
				if(this->irc_session == NULL) {
					throw FCException("Failed to create irc session");
				}
				
				this->reconnect_irc();
			}
			
			if(pthread_create(&this->thread, NULL, FCNetwork::run, (void*)this)) {
				throw FCException("Failed to create network thread");
			}
		}
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}

