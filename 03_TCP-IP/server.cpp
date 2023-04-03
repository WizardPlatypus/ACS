#include <iostream>
#include "socks.hpp"

#define DEFAULT_PORT "27016"

using std::cin, std::cout, std::cerr, std::endl;

int main() {
    int status;

    socks::SocksData data;
    status = socks::init(socks::version, &data);
    if (status == socks::error) {
        cerr << "socks::init failed with error " << socks::lastError() << endl;
        return 1;
    }

    socks::AddressInfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = static_cast<int>(socks::AddressFamily::unspec);
    hints.ai_socktype = static_cast<int>(socks::SockType::stream);
    hints.ai_protocol = static_cast<int>(socks::Protocol::tcp);

    socks::AddressInfo* info;
    // Resolve the server address and port
    status = socks::getAddressInfo(nullptr, DEFAULT_PORT, &hints, &info);
    if (status == socks::error) {
        cerr << "getAddressInfo failed with error #" << socks::lastError() << endl;
        socks::drop();
        return 1;
    }

    uint64_t guard = socks::get(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (guard == socks::invalid_sock) {
        cerr << "socks::get failed with error " << socks::lastError() << endl;
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    status = socks::bint(guard, info->ai_addr, (int)info->ai_addrlen);
    if (status == socks::error) {
        cerr << "socks::bind failed with error " << socks::lastError() << endl;
        socks::freeAddressInfo(info);
        socks::close(guard);
        socks::drop();
        return 1;
    }

    socks::freeAddressInfo(info);

    status = socks::lizten(guard, socks::max_queue_size);
    if (status == socks::error) {
        cerr << "socks::listen failed with error " << socks::lastError() << endl;
        socks::close(guard);
        socks::drop();
        return 1;
    }

    uint64_t pilgrim = socks::axcept(guard, nullptr, nullptr);
    if (pilgrim == socks::invalid_sock) {
        cerr << "socks::axcept failed with error " << socks::lastError() << endl;
        socks::close(guard);
        socks::drop();
        return 1;
    }

    socks::close(guard);

    const int size = 512;
    unsigned char buffer[size];

    while (1) {
        status = socks::receive(pilgrim, (void*)buffer, size);
        if (status == socks::error) {
            cerr << "socks::receive failed with error " << socks::lastError() << endl;
            socks::close(pilgrim);
            socks::drop();
            return 1;
        }
        if (status == 0) {
            cout << "connection was closed" << endl;
            break;
        }
        cout << "received " << status << " bytes" << endl;
    }


    #if 0
    status = socks::send(pilgrim, (void*)buffer, status);
    if (status == socks::error) {
        cerr << "socks::send failed with error " << socks::lastError() << endl;
        socks::close(pilgrim);
        socks::drop();
        return 1;
    }

    if (status == 0) {
        cout << "connection was closed" << endl;
    } else {
        cout << "sent " << status << " bytes" << endl;
    }
    #endif

    socks::close(pilgrim);
    socks::drop();

    return 0;
}