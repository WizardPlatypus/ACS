#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "shared.hpp"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(void) 
{
    int status;

    SOCKET clientSocket = INVALID_SOCKET;


    
    // Initialize Winsock
    getWSADATA();

    addrinfo *hostInfo = getHostInfo(NULL);
    // Create a SOCKET for the server to listen for client connections.
    SOCKET listenSocket = socket(hostInfo->ai_family, hostInfo->ai_socktype, hostInfo->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(hostInfo);
        WSACleanup();
        return 1;
    }
    // Setup the TCP listening socket
    status = bind(listenSocket, hostInfo->ai_addr, (int)hostInfo->ai_addrlen);
    if (status == SOCKET_ERROR) {
        std::cerr << "bind failed with error #" << WSAGetLastError() << std::endl;
        freeaddrinfo(hostInfo);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(hostInfo);

    status = listen(listenSocket, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(listenSocket);

    awaitClosing(clientSocket);

    // shutdown the connection since we're done
    disconnect(clientSocket);

    // cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}