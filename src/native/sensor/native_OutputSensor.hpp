/*******************************************************************************
 * Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
 * Please visit https://www.irit.fr/tplay/.
 *
 * All rights reserved.
 * This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Initial Contributors:
 *   - Nicolas Mellado (IRIT)
 *   - Gauthier Bouyjou (IRIT)
 *******************************************************************************/

#pragma once

// #include "core/Base.hpp"
#include "core/base/Macros.hpp"
#include "native_Acquisition.hpp"
// #include "core/Format.hpp"

#ifdef __cplusplus
#define HUB_OUTPUT_SENSOR hub::sensor::OutputSensor

namespace hub {

#    ifndef HUB_CPP_SOURCE
namespace sensor {
class OutputSensor;
}
#    endif

namespace native {

extern "C"
{
#else
#define HUB_OUTPUT_SENSOR void
#endif

    SRC_API void helloWorld();

    SRC_API HUB_OUTPUT_SENSOR* createOutputSensorDof6( const char* sensorName, const char* streamName );

    SRC_API HUB_ACQUISITION* outputSensorGetAcq( HUB_OUTPUT_SENSOR* outputSensor );

    SRC_API void outputSensorSendAcq( HUB_OUTPUT_SENSOR* outputSensor, HUB_ACQUISITION * acquisition );


    // SRC_API void freeAcquisition( void* acquisition );

    ///
    /// \brief freeOutputSensor
    /// \param outputSensor
    ///
    SRC_API void freeOutputSensor( HUB_OUTPUT_SENSOR* outputSensor );

#ifdef __cplusplus
} // end extern "C"

} // namespace native
} // namespace hub
#endif
