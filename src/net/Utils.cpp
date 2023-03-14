
#include "net/Utils.hpp"

#include <cassert>
#include <cstring>
#include <regex>
#include <stdio.h>
#include <stdlib.h>

static const std::regex s_ipv4Regex { "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" };

#ifdef WIN32

//#ifdef MIN
#    ifdef __MINGW64__
#        undef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WS03 // Windows Server 2003
#        define _WIN32_WINNT _WIN32_WINNT_VISTA // Windows Vista
#    endif

// #include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
// #include <WS2tcpip.h>

#    pragma comment( lib, "ws2_32.lib" )
// #pragma comment(lib, "Ws2_32.lib")

// using socklen_t = int;
// using socket_fd = SOCKET;

// #include <basetsd.h>

#else // #ifdef WIN32
#    define INVALID_SOCKET -1ul
#    define closesocket close
// using socket_fd = int;
#    include <arpa/inet.h>
#    include <csignal>
#    include <sys/socket.h>
#    include <unistd.h>
#endif // #ifdef WIN32

namespace hub {
namespace net {
namespace utils {

static bool s_inited = false;

socket_fd invalidSocket() {
    return INVALID_SOCKET;
}

static inline void init() {
#ifdef DEBUG_NET
    std::cout << getHeader() << "init()" << std::endl;
#endif
#if defined WIN32
    if ( !s_inited ) {
        WSADATA wsaData;
        int iResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
        if ( iResult != 0 ) {
            printf( "error at WSAStartup\n" );
            exit( 1 );
        }
    }
#else

#endif
    s_inited = true;
}

bool isValid( socket_fd sock ) {
    return sock != INVALID_SOCKET;
}

void closeSocket( socket_fd& sock ) {
    assert( sock != INVALID_SOCKET );

#ifdef DEBUG_NET
    std::cout << getHeader() << "closeSocket(" << sock << ") close socket" << std::endl;
    std::cout << getHeader() << "s_sockets = ";
    for ( const auto& socket : s_sockets ) {
        std::cout << socket << " ";
    }
    std::cout << std::endl;
#endif
    ::closesocket( sock );
    // assert(s_sockets.size() == size - 1);

    // #ifdef WIN32
    // #endif

    sock = INVALID_SOCKET;
}

bool isConnected( socket_fd sock ) {
    assert( net::utils::isValid( sock ) );

#ifdef WIN32
    // not sure how to check this in windows
    return true;
#else

    // TODO - make this better, because its probably wrong (but seems to work)

    int optval;
    socklen_t optlen = sizeof( optval );

    int res;
    res = getsockopt( sock, SOL_SOCKET, SO_ERROR, &optval, &optlen );

    if ( optval == 0 && res == 0 ) return true;
#endif

    return false;
}

/////////////////////////////// SERVER ADDRESS /////////////////////////////////////////////

class ServerAddrImpl
{
  public:
    void init( int port ) {
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family      = AF_INET;
        m_sockAddr.sin_addr.s_addr = INADDR_ANY;
        m_sockAddr.sin_port        = htons( port ); // Server port
    }

    struct sockaddr_in m_sockAddr;
};

ServerAddr::ServerAddr() : m_pimpl( new ServerAddrImpl ) {}

ServerAddr::~ServerAddr() = default;

void ServerAddr::init( int port ) {
    m_pimpl->init( port );
}

socket_fd serverSocket() {
    if ( !s_inited ) init();
    return ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
}

int bind( socket_fd sock, ServerAddr& addr ) {
    return ::bind(
        sock, (struct sockaddr*)&addr.m_pimpl->m_sockAddr, sizeof( struct sockaddr_in ) );
}

int listen( socket_fd sock, int backlog ) {
    return ::listen( sock, backlog );
}

socket_fd accept( socket_fd sock, ServerAddr& addr ) {
    auto& sockAddr       = addr.m_pimpl->m_sockAddr;
    socklen_t addrlen    = sizeof( sockAddr );
    socket_fd new_socket = accept( sock, (struct sockaddr*)&sockAddr, &addrlen );
    return new_socket;
}

// socket_fd socket()
//{
// }

///////////////////////////////// CLIENT ADDRESS ///////////////////////////////////////////

class ClientAddrImpl
{
  public:
    void init( const std::string& ipv4, int port ) {
        // Server address construction
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ) );
        m_sockAddr.sin_family = AF_INET;
        // m_sockAddr.sin_addr.s_addr = inet_addr(m_ipv4.c_str()); // winsock 1.0
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
        m_sockAddr.sin_port = htons( port );                             // Server port
    }

    void setPort( int port ) {
        m_sockAddr.sin_port = htons( port ); // Server port
    }
    void setIpv4( const std::string& ipv4 ) {
        inet_pton( AF_INET, ipv4.c_str(), &m_sockAddr.sin_addr.s_addr ); // winsock 2.0
    }

    struct sockaddr_in m_sockAddr;
};

ClientAddr::ClientAddr() : m_pimpl( new ClientAddrImpl ) {}

ClientAddr::~ClientAddr()                                  = default;
ClientAddr::ClientAddr( ClientAddr&& clientAddr ) noexcept = default;

void ClientAddr::init( const std::string& ipv4, int port ) {
    m_pimpl->init( ipv4, port );
}

void ClientAddr::setPort( int port ) {
    m_pimpl->setPort( port );
}

void ClientAddr::setIpv4( const std::string& ipv4 ) {
    m_pimpl->setIpv4( ipv4 );
}

socket_fd clientSocket() {
    if ( !s_inited ) init();
    return ::socket( PF_INET, SOCK_STREAM, 0 );
}

int connect( socket_fd sock, ClientAddr& addr ) {
    return ::connect(
        sock, (struct sockaddr*)&addr.m_pimpl->m_sockAddr, sizeof( struct sockaddr_in ) );
}

int send( socket_fd sock, const char* buf, int len, int flags ) {
    return ::send( sock, buf, len, flags );
}

int recv( socket_fd sock, char* buf, int len, int flags ) {
    return ::recv( sock, buf, len, flags );
}

bool isValid( const std::string& ipv4 ) {
    return std::regex_match( ipv4, s_ipv4Regex );
}

bool isValid( int port ) {
    return ( 0 <= port && port <= 65535 );
}

} // namespace utils
} // namespace net
} // namespace hub

// #include <cassert>
// #include <cstring>
// #include <functional>
// #include <iostream>
// #include <list>
// #include <set>
// #include <mutex>
// #include <string>
// #include <thread>

// #include <winsock2.h>

// #define UDT_API // no dllimport/export

// #include <winsock.h>
// #pragma comment(lib, "ws2_32.lib") // Winsock Library
// #include <WinSock2.h>
// #pragma comment( lib, "Ws2_32.lib" )

// #include <winsock2.h>
// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #endif

// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #endif
// #include <WinSock2.h>
// #include <WS2tcpip.h>
// #include <Windows.h>
////#include <WS2tcpip.h>

// #include <winsock.h>
// #include <windows.h>
// #endif

// #include <iphlpapi.h>
// #include <stdio.h>
// #include <windows.h>
// #include <ws2tcpip.h>

// #include <WS2tcpip.h>

// #pragma comment(lib, "Ws2_32.lib")

// source:
// https://www.zachburlingame.com/2011/05/resolving-redefinition-errors-betwen-ws2def-h-and-winsock-h/
// #ifndef _WINDOWS_

// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
// #ifndef _WINDOWS_
// #undef WIN32_LEAN_AND_MEAN

// #endif
// #define _WINSOCKAPI_
// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #undef WIN32_LEAN_AND_MEAN
// #endif

// #include <windows.h>
// #include <ws2tcpip.h>
// #include <WS2tcpip.h>

// #pragma comment( lib, "ws2_32.lib" )
//// end source
///
// #ifndef WIN32
// #    ifdef DEBUG_NET
// #    endif

//}
// #endif

// socket_fd createSocket(int af, int type, int protocol)
//{
// }