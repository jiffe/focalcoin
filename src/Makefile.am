INCLUDES = -I/usr/local/include/ -I/usr/include/jsoncpp -I/home/jiffe/libwebsockets.patched/lib -I/home/jiffe/plustache/ -I/usr/include/libircclient
bin_PROGRAMS = focalcoind

focalcoind_LDFLAGS = -rdynamic

focalcoind_LDADD = 		-lboost_system \
						-lboost_filesystem \
						-lboost_thread \
						-lleveldb \
						-lsnappy \
						-ljsoncpp \
						/home/jiffe/libwebsockets.patched/build/lib/libwebsockets.a \
						/home/jiffe/plustache/.libs/libplustache.a \
						-lssl \
						-lboost_regex \
						-lircclient \
						-lm

focalcoind_SOURCES = 	main.cpp \
						config.cpp \
						namespace.cpp \
						exception.cpp \
						logger.cpp \
						inventory.cpp \
						block.cpp \
						peers.cpp \
						network.cpp \
						peer.cpp \
						rpc.cpp \
						wallet.cpp \
						resources.cpp \
						queue.cpp \
						queueitem.cpp \
						web.cpp \
						buffer.cpp \
						websocket.cpp \
						uint256.cpp \
						uint128.cpp \
						db.cpp \
						walletaccount.cpp \
						walletaddress.cpp \
						transaction.cpp \
						transactionin.cpp \
						transactionout.cpp \
						merkletree.cpp
						

