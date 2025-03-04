
#ifdef __cplusplus
#error "not c wrapper"
#endif

// #include <sensor/OutputSensor.hpp>
#include <native/sensor/native_OutputSensor.hpp>

int main() {


    // using Resolution = hub::format::Dof6;
    // hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution() );

    // hub::sensor::OutputSensor outputSensor( sensorSpec, "streamName" );

    // auto acq            = outputSensor.acqMsg();
    // auto [start, end]   = acq.clocks();
    // auto & dof6 = acq.get<Resolution&>();

    // size_t iFrame = 0;
    // while ( 1 ) {
    //     start = iFrame;
    //     end = iFrame;

    //     dof6.x = iFrame % 100;
    //     dof6.y = iFrame % 100;
    //     dof6.z = iFrame % 100;

    //     outputSensor << acq;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    return 0;
}
