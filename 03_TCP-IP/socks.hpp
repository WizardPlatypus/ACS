#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2def.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <cstdint>

namespace socks {
    using AddressInfo = addrinfo;
    using Address = sockaddr;
    using IPv4Address= sockaddr_in;
    using IPv6Address= sockaddr_in6;
    using AddressStorage = sockaddr_storage;
    using IPv4InternetAddress = in_addr;
    using IPv6InternetAddress = in6_addr;
    using SocksData = WSADATA;

    inline unsigned short hostToNetworkShort(unsigned short bytes) {
        return htons(bytes);
    }
    inline unsigned long hostToNetworkLong(unsigned long bytes) {
        return htonl(bytes);
    }
    inline unsigned short networkToHostShort(unsigned short bytes) {
        return ntohs(bytes);
    }
    inline unsigned long networkToHostLong(unsigned long bytes) {
        return ntohl(bytes);
    }


    #if 0 // this piece of code does not work for some reason
    inline int presToNetwork(int addressFamily, const char *ip, void *address) {
        return inet_pton(addressFamily, ip, address);
    }
    inline const char *networkToPres(int addressFamily, const void *address, char *buffer, size_t size) {
        return inet_ntop(addressFamily, address, buffer, size);
    }
    #endif


    inline int getAddressInfo(const char *host, const char *service, const AddressInfo *hints, AddressInfo **info) {
        return getaddrinfo(host, service, hints, info);
    }
    inline void freeAddressInfo(AddressInfo *info) {
        freeaddrinfo(info);
    }

    inline int get(int addressFamily, int sockType, int protocol) {
        return socket(addressFamily, sockType, protocol);
    }
    inline int bint(uint64_t sock, Address* address, int addressSize) {
        return bind(sock, address, addressSize);
    }
    inline int comect(uint64_t sock, Address* address, int addressSize) {
        return connect(sock, address, addressSize);
    }
    inline int close(int sock) {
        return closesocket(sock);
    }
    inline int limit(uint64_t sock, int limitFlag) {
        return shutdown(sock, limitFlag);
    }
    inline int lizten(uint64_t sock, int queueSize) {
        return listen(sock, queueSize);
    }
    inline int axcept(uint64_t sock, Address* buffer = nullptr, int* size = nullptr) {
        return accept(sock, buffer, size);
    }
    inline int semd(uint64_t sock, const void* data, int size, int flags = 0) {
        return send(sock, (const char*)data, size, flags);
    }
    inline int receive(uint64_t sock, void* buffer, int size, int flags = 0) {
        return recv(sock, (char*)buffer, size, flags);
    }

    inline int getPeerName(uint64_t sock, Address* address, int& size) {
        return getpeername(sock, address, &size);
    }
    inline int getHostName(char* buffer, int size) {
        return gethostname(buffer, size);
    }

    /// @brief Initializes WinSock2 infrastructure
    /// @return 0 on succes, some error code otherwise
    inline int init(unsigned short version, SocksData* data) {
        return WSAStartup(version, data);
    }
    inline int drop() {
        return WSACleanup();
    }

    inline int lastError() {
        return WSAGetLastError();
    }

    const int max_ipv4_length = INET_ADDRSTRLEN;
    const int max_ipv6_length = INET6_ADDRSTRLEN;
    const int max_queue_size = SOMAXCONN;
    const IPv6InternetAddress init_ipv6_internet_adress = IN6ADDR_ANY_INIT;
    const uint64_t invalid_sock = INVALID_SOCKET;
    const unsigned short version = MAKEWORD(2,2);
    const int error = SOCKET_ERROR;

    enum class Protocol {
        tcp = IPPROTO_TCP,
    };

    enum class SockType: int {
        stream = SOCK_STREAM,
        dgram = SOCK_DGRAM,
        raw = SOCK_RAW,
        rdm = SOCK_RDM,
        seqpacket = SOCK_SEQPACKET
    };

    enum class LimitFlags: int {
        receive = SD_RECEIVE,
        send = SD_SEND,
        both = SD_BOTH
    };

    enum class AddressFamily: int {
        ipv4  = AF_INET,
        ipv6 = AF_INET6,

        _12844 = AF_12844,
        appleTalk = AF_APPLETALK,
        atm = AF_ATM,
        ban = AF_BAN,
        bth = AF_BTH,
        ccitt = AF_CCITT,
        chaos = AF_CHAOS,
        cluster = AF_CLUSTER,
        datakit = AF_DATAKIT,
        decnet = AF_DECnet,
        dli = AF_DLI,
        ecma = AF_ECMA,
        firefox = AF_FIREFOX,
        hylink = AF_HYLINK,
        //hyperv = AF_HYPERV,
        iclfxbm = AF_ICLFXBM,
        implink = AF_IMPLINK,
        ipx = AF_IPX,
        irda = AF_IRDA,
        iso = AF_ISO,
        lat = AF_LAT,
        //link = AF_LINK,
        max = AF_MAX,
        netbios = AF_NETBIOS,
        netdes = AF_NETDES,
        ns = AF_NS,
        osi = AF_OSI,
        pup = AF_PUP,
        sna = AF_SNA,
        tcnMessage = AF_TCNMESSAGE,
        tcnProcess = AF_TCNPROCESS,
        unix = AF_UNIX,
        unknown1 = AF_UNKNOWN1,
        unspec = AF_UNSPEC,
        voiceView = AF_VOICEVIEW,
    };

    enum class HintsFlags: unsigned int {
        passive = AI_PASSIVE,
        canonName = AI_CANONNAME,
        numericHost = AI_NUMERICHOST,
        #if 0 // doesn't compile
        numericServer = AI_NUMERICSERV,
        dnsOnly = AI_DNS_ONLY,
        forceClearText = AI_FORCE_CLEAR_TEXT,
        bypassDnsCache = AI_BYPASS_DNS_CACHE,
        returnTtl = AI_RETURN_TTL,
        all = AI_ALL,
        addressConfig = AI_ADDRCONFIG,
        v4Mapped = AI_V4MAPPED,
        nonAuthoritativ = AI_NON_AUTHORITATIVE,
        secure = AI_SECURE,
        returnPreferredNames = AI_RETURN_PREFERRED_NAMES,
        fqdn = AI_FQDN,
        fileserver = AI_FILESERVER,
        disableIdnEncoding = AI_DISABLE_IDN_ENCODING,
        secureWithFallback = AI_SECURE_WITH_FALLBACK,
        exclusiveCustomServers = AI_EXCLUSIVE_CUSTOM_SERVERS,
        returnResponseFlags = AI_RETURN_RESPONSE_FLAGS,
        requireSecure = AI_REQUIRE_SECURE,
        resolutionHandle = AI_RESOLUTION_HANDLE,
        extended = AI_EXTENDED
        #endif
    };
}