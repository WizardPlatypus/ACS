#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


WSADATA getWSADATA();
addrinfo *getHostInfo(const char*);
SOCKET getSocket(addrinfo*);
void sendBuffer(SOCKET, const char*, int);
void disconnect(SOCKET);
void awaitClosing(SOCKET);

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

    if (targetSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }

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

WSADATA getWSADATA() {
    WSADATA wsaData;
    int status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (status) {
        std::cerr << "WSAStartup failed with error #" << status << std::endl;
        exit(1);
    }
    return wsaData;
}

addrinfo *getHostInfo(const char *serverName) {
    addrinfo hints;
    // set hints to zeros
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo *hostInfo = NULL;
    // Resolve the server address and port
    int status = getaddrinfo(serverName, DEFAULT_PORT, &hints, &hostInfo);
    if (status != 0 ) {
        std::cerr << "WSAStartup failed with error #" << status << std::endl;
        WSACleanup();
        exit(1);
    }

    return hostInfo;
}

SOCKET getSocket(addrinfo *hostInfo) {
    SOCKET targetSocket = INVALID_SOCKET;
    // Attempt to connect to an address until one succeeds
    for(addrinfo *info = hostInfo; info != NULL; info = info->ai_next) {
        // Create a SOCKET for connecting to server
        targetSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        if (targetSocket == INVALID_SOCKET) {
            std::cerr << "socket failed with error #" << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        int status = connect(targetSocket, info->ai_addr, (int)info->ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(targetSocket);
            targetSocket = INVALID_SOCKET;
            continue;
        }

        break;
    }
    return targetSocket;
}

void sendBuffer(SOCKET s, const char* buffer, const int length) {
    int status = send(s, buffer, length, 0 );
    if (status == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        exit(1);
    }
    std::cout << "Sent " << status << " bytes" << std::endl;

}

void disconnect(SOCKET s) {
    int status = shutdown(s, SD_SEND);
    if (status == SOCKET_ERROR) {
        std::cerr << "shutdown failed with error #" << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        exit(1);
    }
}

void awaitClosing(SOCKET s) {
    char buffer[DEFAULT_BUFLEN];
    int bufferLength = DEFAULT_BUFLEN;

    while (1) {
        int status = recv(s, buffer, bufferLength, 0);

        if (status > 0) {
            std::cout << "Received " << status << " bytes." << std::endl;
            std::cout << "The bytes: ";
            for (int i = 0; i < bufferLength; i++) {
                if (buffer[i] == '\0') {
                    break;
                }
                std::cout << buffer[i];
            }
            std::cout << std::endl;
            continue;
        }

        if (status < 0) {
            std::cerr << "recv failed with error #" << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Connection closed." << std::endl;
        break;
    }
}


