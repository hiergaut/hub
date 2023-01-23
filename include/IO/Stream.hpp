#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// Describes an output communication to the server.
///
class SRC_API OutputStream : public OutputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief OutputStream
    /// is used to instantiate an OutputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \attention no stream with streamName id is present in the server hub.
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    ///
    OutputStream( const std::string& streamName,
                  net::ClientSocket&& clientSocket = net::ClientSocket() );

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() const override;
    bool isOpen() const override;
    bool isEnd() const override;

#endif
};

///
/// \brief The InputStream class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the server.
/// That implies an OutputStream communicating data through the hub.
///
class SRC_API InputStream : public InputInterface, public net::ClientSocket
{
  public:
    ///
    /// \brief InputStream
    /// is used to instantiate an InputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \attention the stream with streamName id needs to be active in the server hub,
    /// OutputStream with the same streamName is sharing.
    /// \param syncStreamName
    /// [in] is an unique identifier name of stream
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    /// \param mergeSyncAcqs
    /// [in] if you want merge acq during the synchronization
    ///
    InputStream( const std::string& streamName,
                 const std::string& syncStreamName = "",
                 net::ClientSocket&& clientSocket  = net::ClientSocket(),
                 bool mergeSyncAcqs                = true );

#ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;
    void close() const override;
    bool isOpen() const override;
    bool isEnd() const override;
#endif

  protected:
    //    Acquisition getAcquisition( int acquisitionSize ) const override;
    ///
    /// \brief getAcquisition
    /// \param sensorSpec
    /// \return
    ///
    Acquisition getAcquisition( const SensorSpec& sensorSpec ) const override;
};

} // namespace io
} // namespace hub