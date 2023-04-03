#include <iostream>
#include <string>
#include <windows.h>
#include "socks.hpp"

#define DEFAULT_PORT "27016"
#define BUFFER_SIZE 1024
#define DEBUG

int main() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    auto yellow = FOREGROUND_RED | FOREGROUND_GREEN;
    auto red = FOREGROUND_RED;
    auto blue = FOREGROUND_BLUE;
    auto green = FOREGROUND_GREEN;
    auto white = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

    int result;
    socks::SocksData data;
    result = socks::init(socks::version, &data);
    if(result == socks::error) {
        SetConsoleTextAttribute(console, red);
        std::cerr << "socks::init failed" << std::endl;
        return 1;
    }
    #ifdef DEBUG
    SetConsoleTextAttribute(console, yellow);
    std::cout << "init success" << std::endl;
    #endif

    socks::AddressInfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = static_cast<int>(socks::AddressFamily::unspec);
    hints.ai_socktype = static_cast<int>(socks::SockType::stream);
    hints.ai_protocol = static_cast<int>(socks::Protocol::tcp);

    #ifdef DEBUG
    SetConsoleTextAttribute(console, yellow);
    std::cout << "hints success" << std::endl;
    #endif

    socks::AddressInfo* info = nullptr;
    // Resolve the server address and port
    result = socks::getAddressInfo(/*serverName*/nullptr, DEFAULT_PORT, &hints, &info);
    if (result == socks::error) {
        SetConsoleTextAttribute(console, red);
        std::cerr << "getAddressInfo failed with error #" << socks::lastError() << std::endl;
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    SetConsoleTextAttribute(console, yellow);
    std::cout << "info success" << std::endl;
    #endif

    uint64_t socket = socks::get(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (socket == socks::invalid_sock) {
        SetConsoleTextAttribute(console, red);
        std::cerr << "socks::get failed with error " << socks::lastError() << std::endl;
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    SetConsoleTextAttribute(console, yellow);
    std::cout << "get success" << std::endl;
    #endif

    result = socks::comect(socket, info->ai_addr, info->ai_addrlen);
    if (result == socks::error) {
        SetConsoleTextAttribute(console, red);
        std::cerr << "socks::comect failed with error " << socks::lastError() << std::endl;
        socks::close(socket);
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    SetConsoleTextAttribute(console, yellow);
    std::cout << "comect success" << std::endl;
    #endif

    socks::freeAddressInfo(info);


    SetConsoleTextAttribute(console, yellow);
    std::cout << "Enter messages to be sent. To exit, enter message \"exit\"" << std::endl;
    char buffer[BUFFER_SIZE];
    while (1) {
        std::string input;
        SetConsoleTextAttribute(console, green);
        std::cout << "client> ";
        SetConsoleTextAttribute(console, white);
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        result = socks::cend(socket, input.c_str(), input.size() + 1);
        if (result == socks::error) {
            SetConsoleTextAttribute(console, red);
            std::cerr << "socks::cend failed with error " << socks::lastError() << std::endl;
            socks::close(socket);
            socks::drop();
            return 1;
        }
        if (result == 0) {
            SetConsoleTextAttribute(console, yellow);
            std::cout << "connection was closed" << std::endl;
            break;
        }

        #ifdef DEBUG
        SetConsoleTextAttribute(console, yellow);
        std::cout << "sent " << result << " bytes out of " << strlen(buffer) + 1 << std::endl;
        #endif

        result = socks::receive(socket, buffer, BUFFER_SIZE);
        if (result == socks::error) {
            SetConsoleTextAttribute(console, red);
            std::cerr << "socks::cend failed with error " << socks::lastError() << std::endl;
            socks::close(socket);
            socks::drop();
            return 1;
        }
        if (result == 0) {
            SetConsoleTextAttribute(console, yellow);
            std::cout << "connection was closed" << std::endl;
            break;
        }

        #ifdef DEBUG
        SetConsoleTextAttribute(console, yellow);
        std::cout << "received " << result << " bytes" << std::endl;
        #endif

        int size = result / sizeof(char);
        if (size == 1) {
            continue;
        }
        SetConsoleTextAttribute(console, blue);
        std::cout << "server> ";
        SetConsoleTextAttribute(console, white);
        std::cout << buffer[0];
        for (int i = 1; i < size - 1; i++) {
            if (buffer[i - 1] == '\n') {
                std::cout << "        ";
            }
            std::cout << buffer[i];
        }
        if (buffer[size - 1 - 1] != '\n') {
            std::cout << std::endl;
        }
    }

    socks::close(socket);
    socks::drop();
}
