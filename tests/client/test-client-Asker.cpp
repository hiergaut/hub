#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <sensor/OutputSensor.hpp>
#include <sensor/SensorSpec.hpp>
// #include <server/Server.hpp>

#include <client/Asker.hpp>

TEST_CASE( "Asker test" ) {

    const hub::sensor::Resolution resolution( { 1 }, hub::sensor::Format::BGR8 );
    const hub::sensor::SensorSpec sensorSpec( "hello", { resolution } );
    const std::string streamName = FILE_NAME;
    //    const std::string ipv4       = "127.0.0.1";
    //    const int port               = GET_RANDOM_PORT;
    //    const int port = hub::io::s_serverDefaultPort;

    //    hub::Server server( port );
    //    server.setMaxClients( 2 );
    //    server.asyncRun();

    {
        hub::sensor::OutputSensor outputSensor(
            //            sensorSpec, streamName, hub::net::ClientSocket( ipv4, port ) );
            //            sensorSpec, streamName, ipv4, port );
            //            sensorSpec, OutputStream(FILE_NAME));
            sensorSpec,
            hub::output::OutputStream( streamName ) );

        unsigned char data[3] = { 0, 1, 2 };
        const hub::sensor::Acquisition acq =
            std::move( hub::sensor::Acquisition( 0, 1 ) << hub::Measure( data, 3, resolution ) );
        outputSensor << acq;

//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        {
            //            hub::client::Asker asker( ipv4, port );
            hub::client::Asker asker;
            auto listStreams = asker.listStreams();

            std::cout << "nb stream : " << listStreams.size() << std::endl;
#if ( __cplusplus >= 201703L )
            for ( const auto& [streamName2, sensorSpec2] : listStreams ) {
#else
            for ( const auto& pair : listStreams ) {
                const auto& streamName2 = pair.first;
                const auto& sensorSpec2 = pair.second;
#endif
                std::cout << streamName2 << std::endl;
                CHECK( streamName == streamName2 );

                std::cout << sensorSpec2 << std::endl;
                CHECK( sensorSpec == sensorSpec2 );

                if ( streamName == streamName2 ) {
                    auto acq2 = asker.getAcquisition( streamName2 );
                    CHECK( acq == acq2 );
                    std::cout << acq2 << std::endl;
                }

                std::cout << "--------------------------------" << std::endl;
            }

            try {
                asker.getAcquisition( "fakeStream" );
                CHECK( false );
            }
            catch ( std::exception& ex ) {
                std::cout << "[test] catch execption : " << ex.what() << std::endl;
                CHECK( true );
            }

        } // end asker
        std::cout << "[test] end asker " << std::endl;
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    } // end outputSensor
    std::cout << "[test] end outputSensor " << std::endl;
//    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    //////////////////////////////////////////////////////////////
    std::cout << "[test] end server " << std::endl;
}