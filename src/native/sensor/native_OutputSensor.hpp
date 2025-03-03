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

#ifdef __cplusplus
namespace hub {

#ifndef HUB_CPP_SOURCE
namespace sensor {
class OutputSensor;
}
#endif

namespace native {

extern "C"
{
#endif

    ///
    /// \brief freeOutputSensor
    /// \param outputSensor
    ///
#ifdef __cplusplus
    SRC_API void freeOutputSensor( sensor::OutputSensor* outputSensor );
#else
    SRC_API void freeOutputSensor( void* outputSensor );
#endif

#ifdef __cplusplus
} // end extern "C"

} // namespace native
} // namespace hub
#endif
