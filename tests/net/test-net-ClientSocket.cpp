#include "test_common.hpp"

#include <algorithm>
#include <array>

#include <core/Utils.hpp>
#include <net/ServerSocket.hpp>

TEST_CASE( "Net test : ClientSocket" ) {
    const auto hostname = hub::utils::getHostname();

    // initing datum
    constexpr auto bigDataSize = 1'000'000'000; // 1 Go
                                                //    constexpr auto bigDataSize = 1000; // 1 Go
                                                //    assert(sizeof(size_t) == 8);

    //    unsigned char* bigData = new unsigned char[bigDataSize * 2u];
    unsigned char* bigData = new unsigned char[bigDataSize];
    //    std::array<unsigned char, bigDataSize> bigData;

    //    srand( (unsigned)time( NULL ) );
    memset( bigData, 65, bigDataSize );
    //        memset( bigData, 65, bigDataSize * 2u );
    //        for ( int i = 0; i < bigDataSize; ++i ) {
    //            bigData[i] = rand() % 256;
    //            bigData[j] = 65;
    //        }
    //    dataIsOk( bigData, bigDataSize );

    const int port = GET_RANDOM_PORT;
    hub::net::ServerSocket serverSocket( port );

    constexpr auto nWrite = 1;

    //    std::shuffle(bigData, bigData + bigDataSize, std::default_random_engine(11));
    // datum inited

    // raw socket
    {
        const std::string ipv4 = "127.0.0.1";
        //        constexpr int packetSize = MAX_NET_BUFFER_SIZE;
        //        constexpr int nPart      = bigDataSize / packetSize;
        std::cout << "[test][ClientSocket] sending big data size: " << bigDataSize / 1'000'000'000.0
                  << " Go" << std::endl;
        std::cout << "[test][ClientSocket] start streaming" << std::endl;

        hub::net::ClientSocket clientSocket( ipv4, port );

        assert( clientSocket.isConnected() );
        auto clientServerSocket = serverSocket.waitNewClient();
        assert( clientServerSocket.isConnected() );

        unsigned char* dataIn = new unsigned char[bigDataSize];
        memset( dataIn, 0, bigDataSize );

        size_t dataWrote  = 0;
        const auto& start = std::chrono::high_resolution_clock::now();
        {
            auto thread = std::thread( [&]() {
                try {
                    for ( int i = 0; i < nWrite; ++i ) {
                        std::cout << "[test][ClientSocket] sending data size: "
                                  << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
                        clientSocket.write( bigData, bigDataSize );
                        dataWrote += bigDataSize;
                    }
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test][ClientSocket] catch exception : " << ex.what()
                              << std::endl;
                }
            } );

            try {
                for ( int i = 0; i < nWrite; ++i ) {
                    clientServerSocket.read( dataIn, bigDataSize );
                    //                    dataIn[100] = 5;
                    // #ifdef DEBUG
//                    CHECK( memcmp( bigData, dataIn, bigDataSize ) == 0 );
                    // #endif
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test][ClientSocket] catch exception : " << ex.what() << std::endl;
            }

            assert( thread.joinable() );
            thread.join();
        }
        delete[] dataIn;
        const auto& end = std::chrono::high_resolution_clock::now();

        //        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        //                const auto& bytes               = bigDataSize;
        const auto bytesPerSecond     = 1000.0 * dataWrote / duration;
        const auto gigaBytesPerSecond = bytesPerSecond / 1'000'000'000.0;

        REPORT( "[test][ClientSocket] Giga byte wrote : " << dataWrote / 1'000'000'000.0 << "Go" );
        //                  << std::endl;
        REPORT( "[test][ClientSocket] Giga byte per second : " << gigaBytesPerSecond << " Go/s" );
    }

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] bigData;
}
