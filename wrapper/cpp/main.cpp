
#ifndef __cplusplus
#    error "not cpp wrapper"
#endif

#include <chrono>
#include <thread>
// #include <sensor/OutputSensor.hpp>
#include <native/sensor/native_OutputSensor.hpp>

using namespace hub::native;

int main() {

    HUB_OUTPUT_SENSOR * outputSensor = createOutputSensorDof6( "sensorName", "streamName" );

    HUB_ACQUISITION * acq = outputSensorGetAcq( outputSensor );

    HUB_DOF6 * dof6 = acquisition_getDof6(acq);
    HUB_CLOCK * start = acquisition_getStart(acq);
    HUB_CLOCK * end = acquisition_getEnd(acq);

    for ( int i = 0; i < 10; ++i ) {
        *start = i;
        *end = i;
        dof6->x = i;
        dof6->y = i;
        dof6->z = i;
        dof6->w0 = i;
        dof6->w1 = i;
        dof6->w2 = i;
        dof6->w3 = i;
        outputSensorSendAcq( outputSensor, acq );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    freeAcquisition( acq );
    freeOutputSensor( outputSensor );

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
