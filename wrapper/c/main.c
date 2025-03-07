
#ifdef __cplusplus
#error "not c wrapper"
#endif

#include <unistd.h>

#include <native/sensor/native_OutputSensor.hpp>

int main() {
    helloWorld();

    HUB_OUTPUT_SENSOR* outputSensor = createOutputSensorDof6( "sensorName", "streamName" );

    HUB_ACQUISITION* acq = outputSensorGetAcq( outputSensor );

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
        usleep(100000);
    }

    freeAcquisition( acq );
    freeOutputSensor( outputSensor );


    return 0;
}
