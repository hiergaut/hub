
#pragma once

#include <thread>

//#include "Input.hpp"
#include "InputStreamInterface.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputStreamServer class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
//class SRC_API InputStreamServer : public Input
class SRC_API InputStreamServer : public InputStreamInterface
{
  public:
    ///
    /// \brief InputStreamServer
    /// is used to instantiate an InputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream.
    /// \param clientSocket
    /// [in] is an existing connection to a hub server.
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to link (by their names) needs to be active in the server hub.
    /// \exception hub::net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is not connected to the server.
    ///
//    explicit InputStreamServer( const std::string& streamName,
//                          net::ClientSocket&& clientSocket = net::ClientSocket() );
    InputStreamServer( const std::string& streamName, const std::string & ipv4, int port);

    ///
    /// \brief InputStreamServer
    /// \param inputStream
    ///
    InputStreamServer( InputStreamServer&& inputStream );

    ~InputStreamServer();

  protected:
    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t len ) override;

    ///
    /// \brief close
    ///
    void close() override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

    void read( Acquisition& acq ) override;
    void read( SensorSpec& sensorSpec ) override;

  private:
    net::ClientSocket m_clientSocket;
    bool m_streamViewerClientClosed = false;
    bool m_streamerClosed           = false;

    bool m_moved = false;

    bool m_readAcqWaiting = false;

    friend class InputSyncStream;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamServer::isOpen() const {
    return m_clientSocket.isOpen();
}

inline void InputStreamServer::read( unsigned char* data, size_t len ) {
    m_clientSocket.read( data, len );
}

inline void InputStreamServer::close() {
    std::cout << "[InputStreamServer] close() started" << std::endl;

    // inputSensor closing, prevent server this stream is done
    if ( !m_streamerClosed && !m_streamViewerClientClosed ) {
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
    }

    if ( m_readAcqWaiting ) {
        while ( !m_streamerClosed && !m_streamViewerClientClosed ) {
            std::cout << "[InputStreamServer] close() waiting for server/streamer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
    }
    else {

        net::ClientSocket::Message message;
        m_clientSocket.read( message );
        while ( message == net::ClientSocket::Message::NEW_ACQ ) {
            auto acq = m_clientSocket.get<Acquisition>();
            m_clientSocket.read( message );
        }
        assert( message == net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED );
        std::cout << "[InputStreamServer] stream viewer client closed" << std::endl;
    }

    if ( m_clientSocket.isOpen() ) m_clientSocket.close();

    std::cout << "[InputStreamServer] close() ended" << std::endl;
}

inline bool InputStreamServer::isEnd() const {
    return m_clientSocket.isEnd();
}

} // namespace io
} // namespace hub