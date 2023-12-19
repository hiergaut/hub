#include "ViewerClient.hpp"

#include "ServerImpl.hpp"
#include "StreamerClient.hpp"

namespace hub {
namespace server {
namespace impl {

ViewerClient::ViewerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock ) :
    Client( server, iClient ), m_socket( std::move( sock ) ) {

    assert( m_server != nullptr );
    m_server->addViewer( this );

    m_thread = std::thread( [this]() {
        try {
            // check client still alive
            // ping viewer client to know if the connection of this one still alive

            io::StreamBase::ClientMessage message;
            m_socket.read( message );
            while ( message == io::StreamBase::ClientMessage::VIEWER_CLIENT_SET_PROPERTY ) {
                std::string streamName;
                std::string objectName;
                int property;
                Any value;
                m_socket.read( streamName );
                m_socket.read( objectName );
                m_socket.read( property );
                m_socket.read( value );

                m_server->setProperty( streamName, objectName, property, value );

                m_socket.read( message );
            };
            assert( message == io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
            m_viewerClosed = true;

            if ( m_socket.isOpen() )
                m_socket.write( io::StreamBase::ServerMessage::VIEWER_CLOSED );
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
            m_socket.write( io::StreamBase::ServerMessage::VIEWER_CLOSED );
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
    return Client::headerMsg() + "[Viewer] ";
}

void ViewerClient::notifyNewStreamer( const std::string& streamName,
                                      const sensor::SensorSpec& sensorSpec ) {

    m_socket.write( io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER );

    m_socket.write( streamName );
    m_socket.write( sensorSpec );
}

void ViewerClient::notifyDelStreamer( const std::string& streamName,
                                      const sensor::SensorSpec& sensorSpec ) {
    // void ViewerClient::notifyDelStreamer( const StreamerClient& streamer ) {
    if ( m_viewerClosed ) return;

    try {
        m_socket.write( io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER );
        m_socket.write( streamName );
        m_socket.write( sensorSpec );
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

void ViewerClient::end( io::StreamBase::ServerMessage message ) {
    std::cout << headerMsg() << "end(" << message << ")" << std::endl;
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

void ViewerClient::notifyProperty( const std::string& streamName,
                                   const std::string& objectName,
                                   int property,
                                   const Any& value ) {

    assert( m_socket.isOpen() );
    m_socket.write( io::StreamBase::ServerMessage::VIEWER_SET_PROPERTY );
    m_socket.write( streamName );
    m_socket.write( objectName );
    m_socket.write( property );
    m_socket.write( value );
}

} // namespace impl
} // namespace server
} // namespace hub