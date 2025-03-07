
#include <sensor/OutputSensor.hpp>

#define HUB_CPP_SOURCE
#include "native_OutputSensor.hpp"

namespace hub {
namespace native {

void helloWorld() {
	std::cout << "Hello world from C++" << std::endl;
}

void freeOutputSensor( HUB_OUTPUT_SENSOR* outputSensor ) {
    assert( outputSensor != nullptr );
    std::cout << "[Native] freeOutputSensor( " << outputSensor << ")" << std::endl;
    delete static_cast<hub::sensor::OutputSensor*>(outputSensor);
}

HUB_OUTPUT_SENSOR *createOutputSensorDof6(const char *sensorName, const char *streamName)
{
    using Resolution = hub::format::Dof6;
    hub::sensor::SensorSpec sensorSpec( sensorName, Resolution() );

    auto * outputSensor = new hub::sensor::OutputSensor( sensorSpec, streamName );
    std::cout << "[Native] createOutputSensorDof6( " << outputSensor << ")" << std::endl;
    return outputSensor;
}

HUB_ACQUISITION *outputSensorGetAcq(HUB_OUTPUT_SENSOR *outputSensor)
{
    hub::sensor::OutputSensor * outputSensor2 = static_cast<hub::sensor::OutputSensor*>(outputSensor);
    auto * acq = new hub::sensor::Acquisition{outputSensor2->acqMsg()};
    std::cout << "[Native] outputSensorGetAcq( " << acq << ")" << std::endl;
    return acq;
}

void outputSensorSendAcq(sensor::OutputSensor *outputSensor, sensor::Acquisition *acquisition)
{
    *outputSensor << *acquisition;
}

// void freeAcquisition(void *acquisition)
// {
//     delete static_cast<hub::sensor::Acquisition*>(acquisition);
// }

} // namespace native
} // namespace hub
