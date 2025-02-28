#include "ViewerClient.hpp"

#include "ServerImpl.hpp"
#include "StreamerClient.hpp"
#include "core/Any.hpp"

namespace hub {
namespace server {

ViewerClient::ViewerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    assert( m_server != nullptr );
    m_server->addViewer( this );

    std::cout << headerMsg() << "new viewer at " << m_socket << std::endl;

    m_thread = std::thread( [this]() {
        try {

            assert( !m_viewerClosed );
            while ( !m_viewerClosed ) {
                hub::io::StreamBase::ClientMessage message;
                m_socket.read( message );
                if ( message == hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_SET_PROPERTY ) {
                    std::string streamName;
                    std::string objectName;
                    int property;
                    Any value;
                    m_socket.read( streamName );
                    m_socket.read( objectName );
                    m_socket.read( property );
                    m_socket.read( value );

                    m_socket.read( message );
                }
                else if ( message ==
                          hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_STREAM_ADDED ) {
                    // Todo fix, some tests failed with assert below
                    /// assert( !m_clientStreamAdded );
                    m_clientStreamAdded = true;
                }
                else if ( message == hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED ) {
                    m_viewerClosed = true;
                }
                else { assert( false ); }
            }

            if ( m_socket.isOpen() )
                m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_CLOSED );
        }
        catch ( std::exception& ex ) {
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }
        std::thread( [this]() { delete this; } ).detach();
    } );

    printStatusMessage( "new viewer" );
}

ViewerClient::~ViewerClient() {

    assert( m_thread.joinable() );
    m_thread.join();

    assert( m_server != nullptr );
    m_server->delViewer( this );

    if ( m_socket.isOpen() ) {
        if ( !m_viewerClosed ) {
            m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_CLOSED );
        }
        int iTry = 0;
        while ( !m_viewerClosed && iTry < 10 ) {
            std::cout << "[ViewerClient] close() waiting for server/viewer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        assert( iTry < 10 );

        m_socket.close();
    }
}

std::string ViewerClient::headerMsg() const {
    return Client::clientMsg() + "[Viewer] ";
}

void ViewerClient::notifyNewStreamer( const StreamerClient* streamer ) {

    m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER );
    m_socket.write( streamer->m_streamName );

    m_socket.write( streamer->m_streamIpv4 );
    m_socket.write( streamer->m_streamPort );

    m_socket.write( streamer->m_header );

    m_clientStreamAdded = false;
    int iTry            = 0;
    // For some reason, it has been decided here that we must wait for the client to connect and
    // connect is that after the end of this function, if the client is not available and functions
    while ( iTry < 10 && !m_clientStreamAdded ) // timeout: 100 ms
    {
        std::cout << "[ViewerClient] waiting for streamer added ..." << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
        ++iTry;
    }
}

void ViewerClient::notifyDelStreamer( const std::string& streamName ) {
    if ( m_viewerClosed ) return;

    if ( !m_socket.isOpen() ) return;

    try {
        m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER );
        m_socket.write( streamName );
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

void ViewerClient::end( hub::io::StreamBase::ServerMessage message ) {
    if ( m_socket.isOpen() ) {
        try {
            assert( m_socket.isOpen() );
            m_socket.write( message );
        }
        catch ( std::exception& ex ) {
            std::cout << headerMsg() << "catch exception " << ex.what() << std::endl;
        }
    }
}

void ViewerClient::notifyInited() {
    assert( m_socket.isOpen() );
}

} // namespace server
} // namespace hub
