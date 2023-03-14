#pragma once
#include <winsock2.h>

WSADATA getWSADATA();
addrinfo *getHostInfo(const char*);
SOCKET getSocket(addrinfo*);
void awaitClosing(SOCKET);
void disconnect(SOCKET);