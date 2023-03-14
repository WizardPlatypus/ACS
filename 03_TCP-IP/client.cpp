#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "shared.hpp"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


void sendBuffer(SOCKET, const char*, int);

int __cdecl main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " server-name(localhost)" << std::endl;
        return 1;
    }
    auto serverName = argv[1];
    getWSADATA();

    addrinfo *hostInfo = getHostInfo(serverName);
    SOCKET targetSocket = getSocket(hostInfo);
    freeaddrinfo(hostInfo);

    // Send an initial buffer
    const char *message = "Test message";
    sendBuffer(targetSocket, message, (int)strlen(message));
    // shutdown the connection since no more data will be sent
    disconnect(targetSocket);
    // Receive until the peer closes the connection
    awaitClosing(targetSocket);
    // cleanup
    closesocket(targetSocket);

    WSACleanup();
    return 0;
}


void sendBuffer(SOCKET s, const char* buffer, const int length) {
    int status = send(s, buffer, length, 0);
    if (status == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        exit(1);
    }
    std::cout << "Sent " << status << " bytes" << std::endl;
}


