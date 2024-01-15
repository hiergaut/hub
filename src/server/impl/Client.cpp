
#include "Client.hpp"
#ifdef HUB_USE_SERVER
#include <iomanip>
#include <iostream>

#include "ServerImpl.hpp"

namespace hub {
namespace server {
namespace impl {

Client::Client( ServerImpl* server, int iClient ) : m_server( server ), m_iClient( iClient ) {
    assert( m_server != nullptr );
}

Client::~Client() {
    if ( m_server != nullptr ) m_server->removeClient( this );
}

constexpr int g_margin  = 45;
constexpr int g_margin2 = 20;

std::string Client::headerMsg() const {
    const std::string str = "\t\033[" + std::to_string( 31 + m_iClient % 7 ) + "m" +
                            "[Client:" + std::to_string( m_iClient ) + "/" +
                            std::to_string( m_server->m_nActiveClient ) + "]\033[0m";
    return str;
}

void Client::printStatusMessage( const std::string& message ) const {
    if ( m_server == nullptr ) {
        std::cout << std::left << std::setw( g_margin ) << headerMsg() << std::setw( g_margin2 )
                  << message << "status : server exited" << std::endl
                  << "-------------------------------------------------------------------------"
                     "--------------------"
                  << std::endl;
    }
    else {
        std::cout << std::left << std::setw( g_margin ) << headerMsg() << std::setw( g_margin2 )
                  << message << m_server->getStatus() << std::endl
                  << "-------------------------------------------------------------------------"
                     "--------------------"
                  << std::endl;
    }
}

void Client::setServer( ServerImpl* newServer ) {
    assert( m_server != nullptr );
    m_server = newServer;
}

} // namespace impl
} // namespace server
} // namespace hub


#endif
