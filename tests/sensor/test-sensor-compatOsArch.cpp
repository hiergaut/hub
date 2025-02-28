
#include <filesystem>

#include "test_common.hpp"
#include "test_sensor_common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "sensor compat os and arch test" ) {

    // Multi os/platform compatibility.
    // Able to load hub file through differents os and platform.
    // Test if raw stream data is same using different architectures like x64, x86 and arm64

    TEST_BEGIN()

    // INIT_SERVER

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh refMesh( meshPath + "sensor" );
    // CHECK( refMesh == refMesh );
    // const hub::data::Mesh mesh2( meshPath + "sensor" );
    // CHECK( refMesh == mesh2 );

    hub::MetaData refMetadata;
    refMetadata["asset"]  = refMesh;
    refMetadata["parent"] = "Stanford";

    using Resolution   = hub::MatrixTs<int, bool, hub::MatrixXD<char, 10>>;
    using OutputSensor = hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile>;

    using Acquisition = OutputSensor::Acquisition;
    std::vector<Acquisition> refAcqs;

    const hub::sensor::SensorSpec refSensorSpec( FILE_NAME, Resolution(), refMetadata );

    const std::string filePath = HUB_PROJECT_DIR "data/compat/sensor.hub";

    Acquisition acq;
    auto [start, end] = acq.clocks();
    auto& intRef      = acq.get<int&>();
    auto& boolRef     = acq.get<bool&>();
    auto* charPtr     = acq.get<char*>();

    for ( int i = 0; i < 10; ++i ) {
        start   = i;
        end     = i;
        intRef  = i;
        boolRef = i % 2;
        for ( int j = 0; j < 10; ++j ) {
            charPtr[j] = i;
        }
        refAcqs.push_back( acq );
    }

    // OutputSensor
    // Testing unique multi os/arch file
    if ( !std::filesystem::exists( filePath ) ) {
        std::cout << "--------------> generating ref file <----------------------" << std::endl;
        OutputSensor outputSensor( refSensorSpec, filePath );
        // OutputSensor outputSensor( refSensorSpec, FILE_NAME, SERVER_PORT );
        for ( const auto& acq : refAcqs ) {
            outputSensor << acq;
        }
    }

    // InputSensor
    {
        hub::sensor::InputSensor inputSensor( hub::input::InputFile { filePath } );
        const auto& sensorSpec = inputSensor.getSpec();
        std::cout << "ref sensor spec : " << refSensorSpec << std::endl;
        std::cout << "    sensor spec : " << sensorSpec << std::endl;

        CHECK( sensorSpec.getSensorName() == refSensorSpec.getSensorName() );
        CHECK( sensorSpec.getResolution() == refSensorSpec.getResolution() );
        const auto& metaData = sensorSpec.getMetaData();
        CHECK( metaData.at( "parent" ) == refMetadata.at( "parent" ) );

        /// todo fix mesh x64/arm64 compat vertices differ
        /// CHECK( sensorSpec == refSensorSpec );

        auto acqs = inputSensor.getAllAcquisitions();
        for ( int i = 0; i < acqs.size(); ++i ) {
            CHECK( refAcqs.at( i ) == acqs.at( i ) );
        }
    }

    TEST_END()
}
