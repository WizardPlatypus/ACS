#include <iostream>
#include "socks.hpp"

#define DEFAULT_PORT "27016"
#define DEBUG

using std::cout, std::cin, std::cerr, std::endl;

int main() {
    int status;
    socks::SocksData data;
    status = socks::init(socks::version, &data);
    if(status == socks::error) {
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
    status = socks::getAddressInfo(/*serverName*/nullptr, DEFAULT_PORT, &hints, &info);
    if (status == socks::error) {
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

    status = socks::comect(socket, info->ai_addr, info->ai_addrlen);
    if (status == socks::error) {
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

    const char* message = "Test message";
    status = socks::semd(socket, (void*) message, sizeof(message));
    if (status == socks::error) {
        cerr << "socks::send failed with error " << socks::lastError() << endl;
        socks::close(socket);
        socks::drop();
        return 1;
    }
    if (status < sizeof(message)) {
        cout << "not all the bytes have been sent, " << sizeof(message) - status << " left" << endl;
    }

    #ifdef DEBUG
    cout << "semd success" << endl;
    #endif

    socks::limit(socket, static_cast<int>(socks::LimitFlags::send));

    const int size = 512;
    unsigned char buffer[size];
    while (1) {
        status = socks::receive(socket, (void*)buffer, size);
        if (status == 0) {
            cout << "connection closed" << endl;
            break;
        }
        if (status == socks::error) {
            cerr << "socks::receive failed with error " << socks::lastError() << endl;
            socks::close(socket);
            socks::drop();
            return 1;
        }
        cout << "received " << status << " bytes" << endl;
    }

    socks::close(socket);
    socks::drop();
}
