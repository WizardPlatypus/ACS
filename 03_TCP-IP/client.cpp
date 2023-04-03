#include <iostream>
#include <string>
#include "socks.hpp"

#define DEFAULT_PORT "27016"
#define BUFFER_SIZE 512
//#define DEBUG

using std::cout, std::cin, std::cerr, std::endl;

int main() {
    int result;
    socks::SocksData data;
    result = socks::init(socks::version, &data);
    if(result == socks::error) {
        cerr << "socks::init failed" << endl;
        return 1;
    }
    #ifdef DEBUG
    cout << "init success" << endl;
    #endif

    socks::AddressInfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = static_cast<int>(socks::AddressFamily::unspec);
    hints.ai_socktype = static_cast<int>(socks::SockType::stream);
    hints.ai_protocol = static_cast<int>(socks::Protocol::tcp);

    #ifdef DEBUG
    cout << "hints success" << endl;
    #endif

    socks::AddressInfo* info = nullptr;
    // Resolve the server address and port
    result = socks::getAddressInfo(/*serverName*/nullptr, DEFAULT_PORT, &hints, &info);
    if (result == socks::error) {
        cerr << "getAddressInfo failed with error #" << socks::lastError() << endl;
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "info success" << endl;
    #endif

    uint64_t socket = socks::get(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (socket == socks::invalid_sock) {
        cerr << "socks::get failed with error " << socks::lastError() << endl;
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "get success" << endl;
    #endif

    result = socks::comect(socket, info->ai_addr, info->ai_addrlen);
    if (result == socks::error) {
        cerr << "socks::comect failed with error " << socks::lastError() << endl;
        socks::close(socket);
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "comect success" << endl;
    #endif

    socks::freeAddressInfo(info);

    cout << "Enter messages to be sent. To exit, enter message \"exit\"" << endl;
    char buffer[BUFFER_SIZE];
    while (1) {
        std::string input;
        std::getline(cin, input);

        if (input == "exit") {
            break;
        }

        result = socks::cend(socket, input.c_str(), input.size() + 1);
        if (result == socks::error) {
            cerr << "socks::cend failed with error " << socks::lastError() << endl;
            socks::close(socket);
            socks::drop();
            return 1;
        }
        if (result == 0) {
            cout << "connection was closed" << endl;
            break;
        }

        #ifdef DEBUG
        cout << "sent " << result << " bytes out of " << strlen(buffer) + 1 << endl;
        #endif

        result = socks::receive(socket, buffer, BUFFER_SIZE);
        if (result == socks::error) {
            cerr << "socks::cend failed with error " << socks::lastError() << endl;
            socks::close(socket);
            socks::drop();
            return 1;
        }
        if (result == 0) {
            cout << "connection was closed" << endl;
            break;
        }

        #ifdef DEBUG
        cout << "received " << result << " bytes" << endl;
        #endif

        for (int i = 0; i < (result / sizeof(char)); i++) {
            cout << buffer[i];
        }
        cout << endl;
    }

    socks::close(socket);
    socks::drop();
}
