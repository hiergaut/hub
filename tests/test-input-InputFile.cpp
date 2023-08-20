#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <io/File.hpp>

#include <filesystem>
#include <thread>

TEST_CASE( "File test" ) {

    const std::string filename = "file.txt";

#if CPLUSPLUS_VERSION <= 14
    // todo cpp 17 -> 14
#else
    std::filesystem::remove( filename );
#endif

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs = 100;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        const unsigned char data[3] = {
            (unsigned char)iAcq, (unsigned char)( iAcq + 1 ), (unsigned char)( iAcq + 2 ) };
        acqs.push_back( hub::Acquisition( iAcq, iAcq ) );
        acqs.back() << hub::data::Measure( data, 3, { { 1 }, hub::Format::BGR8 } );
        CHECK( acqs.back().getSize() == 3 );
    }
    CHECK( acqs[0] != acqs[1] );

    std::cout << "outputStream start" << std::endl;
    INFO( "OutputStream" );
    {
//        hub::output::OutputFile outputFile(filename);

        hub::OutputSensor outputSensor(
            hub::SensorSpec { "sensorName", { { { 1 }, hub::Format::BGR8 } } },
//            hub::io::File(
//                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc ) ) );
            hub::output::OutputFile(filename));


        const auto& sensorSpec = outputSensor.getSpec();
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::Format::BGR8 );

        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
        assert( outputSensor.getOutput().isOpen() );
        std::cout << "outputStream start end" << std::endl;
    }
    std::cout << "outputStream end ################################" << std::endl;

    std::cout << "inputStream start" << std::endl;
    INFO( "InputStream" );
    {
//        hub::input::InputFile inputFile(filename);
//        hub::InputSensor inputSensor(
//            hub::input::InputFile(filename));
        auto inputSensor = hub::InputSensor(hub::input::InputFile(filename));
//            hub::io::File( std::fstream( filename, std::ios::in | std::ios::binary ) ) );
        const auto& input = inputSensor.getInput();

        const auto& sensorSpec = inputSensor.getSpec();
        CHECK( sensorSpec.getAcquisitionSize() == 3 );
        CHECK( sensorSpec.getSensorName() == "sensorName" );
        CHECK( sensorSpec.getResolutions().size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( sensorSpec.getResolutions()[0].first.at( 0 ) == 1 );
        CHECK( sensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "####### compare acqs" << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            hub::Acquisition acq;
            inputSensor >> acq;
            assert( acq == acqs[iAcq] );
            CHECK( acq == acqs[iAcq] );
        }
        CHECK( input.isEnd() );
    }
    std::cout << "inputStream end #################################" << std::endl;
}