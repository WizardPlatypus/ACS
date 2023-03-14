#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

#define DEFAULT_PORT "27015"
#define DEFAULT_LENGTH 512

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
            freeaddrinfo(hostInfo);
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
    if (targetSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error #" << WSAGetLastError() << std::endl;
        freeaddrinfo(hostInfo);
        WSACleanup();
        exit(1);
    }
    return targetSocket;
}

void awaitClosing(SOCKET s) {
    char buffer[DEFAULT_LENGTH];
    int bufferLength = DEFAULT_LENGTH;

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

void disconnect(SOCKET s) {
    int status = shutdown(s, SD_SEND);
    if (status == SOCKET_ERROR) {
        std::cerr << "shutdown failed with error #" << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        exit(1);
    }
}
