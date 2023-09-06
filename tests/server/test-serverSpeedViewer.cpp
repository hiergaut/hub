#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <atomic>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <client/Viewer.hpp>
// #include <server/Server.hpp>
#include <net/ServerSocket.hpp>
#include <utils/Utils.hpp>

TEST_CASE( "Server test : viewer" ) {
    const auto hostname = hub::utils::getHostname();

    constexpr int nAcqs       = 100;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    unsigned char* data       = new unsigned char[dataSize];
    unsigned char* data2      = new unsigned char[dataSize];

    std::vector<hub::Acquisition> acqs( nAcqs );

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq % 256;
        }
        hub::Acquisition acq( iAcq, iAcq );
        acq << hub::Measure( reinterpret_cast<unsigned const char*>( data ),
                                   dataSize,
                                   { { width, height }, hub::Format::BGR8 } );
        acqs.at( iAcq ) = std::move( acq );
    }

    double megaBytesPerSeconds;
    double megaBytesPerSeconds2;

    {
        const std::string ipv4 = "127.0.0.1";
        const int port         = GET_RANDOM_PORT;

        std::cout << "[test][ClientSocket] start streaming" << std::endl;
        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        auto clientServerSocket = serverSocket.waitNewClient();
        //const int packetSize    = 2'000'000; // 2Go network memory buffer
        const int packetSize    = 500'000; // 500Mo network memory buffer MacOS
        const int nPart         = dataSize / packetSize;
        const auto& start       = std::chrono::high_resolution_clock::now();
        {
            for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                int uploadSize = 0;

                for ( int i = 0; i < nPart - 1; ++i ) {
                    clientSocket.write( data + uploadSize, packetSize );
                    clientServerSocket.read( data2 + uploadSize, packetSize );

                    uploadSize += packetSize;
                }

                clientSocket.write( data + uploadSize, dataSize - uploadSize );
                clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize );

                assert( !memcmp( data, data2, dataSize ) );
            }
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto& duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        const auto& bytes           = dataSize * nAcqs;
        const auto& bytesPerSeconds = 1000.0 * bytes / duration;
        megaBytesPerSeconds         = bytesPerSeconds / 1000'000.0;

        std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
                  << std::endl;
        std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds
                  << " Mo/s" << std::endl;
    }

    {
        //        const int port2 = port + 1;

        std::cout << "[test][InputOutputSensor] ############################### server start"
                  << std::endl;
        //        hub::Server server( port2 );
        //        server.setMaxClients( 3 );
        //        server.asyncRun();
        std::cout << "[test][InputOutputSensor] server end ------------------------------"
                  << std::endl;

        {
            std::cout
                << "[test][InputOutputSensor] ############################### outputStream start"
                << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                OutputStream( FILE_NAME )
                //                "streamName",
                //                hub::net::ClientSocket( ipv4, port2 ) );
                //                ipv4,
                //                port2
            );

            std::cout
                << "[test][InputOutputSensor] ############################### inputStream start"
                << std::endl;

            {
                // startConstruction
                bool newStreamerAdded = false;
                auto onNewStreamer    = [&]( const std::string& streamName,
                                          const hub::SensorSpec& sensorSpec ) {
                    std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;

                    CHECK( sensorSpec.getAcquisitionSize() == dataSize );
                    CHECK( sensorSpec.getSensorName() == "sensorName" );

                    const auto& resolutions = sensorSpec.getResolutions();
                    CHECK( resolutions.size() == 1 );
                    CHECK( resolutions[0].first.size() == 2 );
                    CHECK( resolutions[0].first.at( 0 ) == width );
                    CHECK( resolutions[0].first.at( 1 ) == height );
                    CHECK( resolutions[0].second == hub::Format::BGR8 );
                    newStreamerAdded = true;
                    return true;
                };
                auto onDelStreamer = []( const std::string& streamName,
                                         const hub::SensorSpec& sensorSpec ) {
                    std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
                };
                auto onServerNotFound = []( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerNotFound : " << ipv4 << " " << port
                              << std::endl;
                };
                bool serverConnected   = false;
                auto onServerConnected = [&]( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port
                              << std::endl;
                    serverConnected = true;
                };
                auto onServerDisconnected = []( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port
                              << std::endl;
                };
//                int nAcqSended        = 0;
                std::atomic_int nAcqSended;
//                nAcqSended.store(0);
                nAcqSended = 0;
                auto onNewAcquisition = [&]( const std::string& streamName,
                                             const hub::Acquisition& acq ) {
//                    std::cout << "[example-viewer] onNewAcquisition : " << streamName << " " << acq
//                              << std::endl;
                                        std::cout << "+";
                    CHECK( acq == acqs.at( nAcqSended ) );
                    ++nAcqSended;
                };
                auto onSetProperty = []( const std::string& streamName,
                                         const std::string& objectName,
                                         int property,
                                         const hub::Any& value ) {
                    std::cout << "[example-viewer] onSetProperty " << streamName << std::endl;
                };
                auto onLogMessage = []( const std::string& logMessage ) {
                    std::cout << "[example-viewer] onLogMessage '" << logMessage << "'"
                              << std::endl;
                };

                std::cout << "[Test] ############################### viewer start" << std::endl;
                hub::client::Viewer viewer {
                    FILE_NAME,
                    onNewStreamer,
                    onDelStreamer,
                    onServerNotFound,
                    onServerConnected,
                    onServerDisconnected,
                    onNewAcquisition,
                    onSetProperty,
                    onLogMessage
                    //                    ipv4,
                    //                    port2,
                };

                while ( !serverConnected ) {
                    std::cout << "[test] waiting for server started" << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                }
                assert( viewer.isConnected() );

                std::cout
                    << "[test][InputOutputSensor] ############################### send acquisitions"
                    << std::endl;
                const auto& start2 = std::chrono::high_resolution_clock::now();
                for ( int i = 0; i < nAcqs; ++i ) {
//                    std::cout << "[test] send acq " << acqs.at( i ) << std::endl;
                                        std::cout << ".";
                    outputSensor << acqs.at( i );
                    while ( nAcqSended <= i ) {
                        //                        std::this_thread::sleep_for(
                        //                        std::chrono::milliseconds( 1 ) ); std::cout <<
                        //                        "[test] waiting for receive " << i << std::endl;
                    }
                }

                std::cout << std::endl;

                assert( nAcqSended == nAcqs );
                //                while ( nAcqSended != nAcqs ) {
                //                    std::cout << "[test] waiting for viewer read all acqs" <<
                //                    std::endl; std::this_thread::sleep_for(
                //                    std::chrono::milliseconds( 25 ) );
                //                }
                const auto& end2 = std::chrono::high_resolution_clock::now();
                const auto& duration2 =
                    std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
                const auto& bytes2           = dataSize * nAcqs;
                const auto& bytesPerSeconds2 = 1000.0 * bytes2 / duration2;
                megaBytesPerSeconds2         = bytesPerSeconds2 / 1000'000.0;

                std::cout << "[test][InputOutputSensor] Mega byte wrote : " << bytes2 / 1000'000.0
                          << " Mo" << std::endl;
                std::cout << "[test][InputOutputSensor] Mega byte per second : "
                          << megaBytesPerSeconds2 << " Mo/s" << std::endl;

                // todo : add read acq timeout to exit infinite loop in detached thread
                nAcqSended = 0;
//                std::cout << "[test] send exit acq " << acqs.front() << std::endl;
                                        std::cout << ".";
                outputSensor << acqs.front(); // to exit read acq loop from stream
            }                                 // end viewer

            std::cout << "######################### end viewer" << std::endl;
            //            outputSensor << acqs.front(); // to exit read acq loop from stream

        } // end outputSensor
    }

    std::cout << std::endl;

    std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds << " Mo/s"
              << std::endl;
    std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds2
              << " Mo/s" << std::endl;

    std::cout << std::endl;

    const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
    std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %" << std::endl;

    // #ifdef WIN32
    // #    ifdef DEBUG
    //     checkRatio( ratio, 20 );
    // #    else
    //     checkRatio( ratio, 40 );
    // #    endif
    // #else
    //     if ( hostname == "msi" ) { checkRatio( ratio, 50, 10 ); }
    //     else {
    // #    ifdef DEBUG
    //         checkRatio( ratio, 55, 10 );
    // #    else
    //         checkRatio( ratio, 40, 10 );
    // #    endif
    //     }
    // #endif

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] data;
    delete[] data2;
}
