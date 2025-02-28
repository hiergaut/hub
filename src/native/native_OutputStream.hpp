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

#include "core/Base.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE
namespace output {
class OutputStream;
}
namespace io {
class Header;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createOutputStream
    /// \param header
    /// \param streamName
    /// \param port
    /// \param ipv4
    /// \return
    ///
    SRC_API output::OutputStream* createOutputStream( const io::Header* header,
                                                      const char* streamName,
                                                      int port,
                                                      const char* ipv4 );

    // SRC_API bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input

    ///
    /// \brief outputStream_write_int
    /// \param outputStream
    /// \param value
    ///
    SRC_API void outputStream_write_int( output::OutputStream* outputStream, int value );

    ///
    /// \brief freeOutputStream
    /// \param outputStream
    ///
    SRC_API void freeOutputStream( output::OutputStream* outputStream );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
