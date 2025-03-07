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
 *   - Gauthier Bouyjou (IRIT, Toulouse Tech Transfer)
 *******************************************************************************/

#pragma once

#include "core/base/Macros.hpp"
// #include "core/Base.hpp"

#ifdef __cplusplus
#define HUB_ACQUISITION hub::sensor::Acquisition

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace sensor {
class Acquisition;
}
#endif

namespace native {

extern "C"
{
#else
#define HUB_ACQUISITION void
#endif

    typedef struct {
        float x;
        float y;
        float z;
        float w0; // w
        float w1; // x
        float w2; // y
        float w3; // z
    } HUB_DOF6;

    typedef long long HUB_CLOCK;

    ///
    /// \brief freeAcquisition
    /// \param acquisition
    ///
    SRC_API void freeAcquisition( HUB_ACQUISITION* acquisition );

    ///
    /// \brief acquisition_getMeasure
    /// \param acquisition
    /// \param data
    /// \param iMeasure
    ///
    SRC_API void acquisition_getMeasure( const HUB_ACQUISITION* acquisition,
                                         unsigned char* data,
                                         int iMeasure );

    SRC_API HUB_DOF6 * acquisition_getDof6( HUB_ACQUISITION* acquisition);
    ///
    /// \brief acquisition_getStart
    /// \param acquisition
    /// \return
    ///
    SRC_API HUB_CLOCK* acquisition_getStart( HUB_ACQUISITION* acquisition );

    SRC_API HUB_CLOCK* acquisition_getEnd( HUB_ACQUISITION* acquisition );

    ///
    /// \brief acquisition_to_string
    /// \param acquisition
    /// \param str
    /// \param strLen
    ///
    SRC_API void
    acquisition_to_string( const HUB_ACQUISITION* acquisition, char* str, int* strLen );

#ifdef __cplusplus
} // end extern "C"

} // namespace native
} // namespace hub
#endif
