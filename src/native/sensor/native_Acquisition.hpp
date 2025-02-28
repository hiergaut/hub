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

#include "core/Base.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE

namespace sensor {
class Acquisition;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief freeAcquisition
    /// \param acquisition
    ///
    SRC_API void freeAcquisition( sensor::Acquisition* acquisition );

    ///
    /// \brief acquisition_getMeasure
    /// \param acquisition
    /// \param data
    /// \param iMeasure
    ///
    SRC_API void acquisition_getMeasure( const sensor::Acquisition* acquisition,
                                         unsigned char* data,
                                         int iMeasure = 0 );

    ///
    /// \brief acquisition_getStart
    /// \param acquisition
    /// \return
    ///
    SRC_API long long acquisition_getStart( const sensor::Acquisition* acquisition );

    ///
    /// \brief acquisition_to_string
    /// \param acquisition
    /// \param str
    /// \param strLen
    ///
    SRC_API void
    acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
