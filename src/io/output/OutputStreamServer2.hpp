#pragma once

#include <mutex>
#include <thread>

// #include "Output.hpp"
// #include "OutputStreamInterface.hpp"
// #include "net/ClientSocket.hpp"
#include "core/Output.hpp"
// #include "../StreamServer.hpp"
// #include "impl/server2/io/StreamServer.hpp"
#include "io/StreamServer2.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputStreamServer2 class
/// Describes an output communication to the server.
///
// class SRC_API OutputStreamServer2 : public Output
class SRC_API OutputStreamServer2 : public Output, public io::StreamServer2
{
  public:
    using Output::write;

    ///
    /// \brief OutputStreamServer2
    /// is used to instantiate an OutputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to start linking (by it name) needs to not exist in the server hub.
    /// Streaming with most of one sender is not allowed.
    /// \exception net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is already started in the server.
    ///
    //    explicit OutputStreamServer2( const std::string& streamName,
    //                           net::ClientSocket&& clientSocket = net::ClientSocket() );
    OutputStreamServer2( int streamPort );

    explicit OutputStreamServer2( const std::string& streamName,
                                 int port                = s_defaultPort,
                                 const std::string& ipv4 = s_defaultIpv4 );
    ///
    /// \brief OutputStreamServer2
    /// \param outputStream
    ///
    OutputStreamServer2( OutputStreamServer2&& outputStream );

    ~OutputStreamServer2();

    // void streamProcess();
    void startStreaming();
    void initStream();
    //    void write( const sensor::Acquisition& acq );
    //    void write( const sensor::SensorSpec& sensorSpec );

    // #ifdef WIN32 // msvc warning C4250
    //  protected:
    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;
    void setRetain( bool retain ) override;

    // #endif

  private:
    void stop();

    struct SharedData {
        std::unique_ptr<hub::io::InputOutputSocket> m_serverSocket;
        int m_streamPort = 0;
        std::vector<hub::io::InputOutputSocket> m_streamSockets;
        std::mutex m_mtxClientSockets;
        std::vector<hub::Data_t> m_retainedData;
        bool m_killed                          = false;
        std::atomic<bool> m_serverStarted      = false;
        std::atomic<bool> m_streamConnected    = false;
        std::atomic<bool> m_streamViewerInited = false;
        std::function<void( const Data_t*, Size_t )> m_writingFun;
        std::unique_ptr<std::thread> m_streamThread;
        std::unique_ptr<std::thread> m_serverThread;
        bool m_shutdown = false;
        SharedData(std::unique_ptr<hub::io::InputOutputSocket> && ioSocket) : m_serverSocket{std::move(ioSocket)} {};
        SharedData() = default;
    };
    std::unique_ptr<SharedData> m_data;
    bool m_moved = false;

    //    std::unique_ptr<std::vector<io::InputOutputSocket>> m_clientSockets;
    //    hub::Buffer<> m_buffer;

    //    std::unique_ptr<bool> m_serverClosed   = std::make_unique<bool>(false);
    //    std::unique_ptr<bool> m_streamerClosed = std::make_unique<bool>(false);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// inline void OutputStreamServer2::write( const sensor::Acquisition& acq ) {
//     Output::write( io::StreamInterface::ClientMessage::STREAMER_CLIENT_NEW_ACQ );
//     Output::write( acq );
// }

// inline void OutputStreamServer2::write(const sensor::SensorSpec &sensorSpec)
//{
////    Output::write( io::StreamInterface::ClientMessage::STREAMER_CLIENT_INIT_SENSOR );
//    Output::write( sensorSpec );
//}

// void OutputStreamServer2::write( const unsigned char* data, size_t len );

inline void OutputStreamServer2::close() {
    m_data->m_serverSocket->close();
    // std::cout << "[OutputStreamServer2] close() started" << std::endl;
    //    assert( m_serverSocket->isOpen() );
    //    if ( ! *m_serverClosed && ! *m_streamerClosed ) {
    //        m_serverSocket->write( io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED );
    //    }
    //    int iTry = 0;
    //    while ( ! *m_serverClosed && ! *m_streamerClosed && iTry < 10 ) {
    //        std::cout << "[OutputStreamServer2] close() waiting for server/streamer closing" <<
    //        std::endl; std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //        ++iTry;
    //    }
    //    assert(iTry < 10);

    // std::cout << "[OutputStreamServer2] closing connection ended" << std::endl;
    // m_serverSocket->close();
    // std::cout << "[OutputStreamServer2] close() ended" << std::endl;
}

inline bool OutputStreamServer2::isOpen() const {
    return true;
    // return m_serverSocket->isOpen();
}

} // namespace output
} // namespace hub
