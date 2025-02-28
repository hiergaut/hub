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

class Any;

namespace io {
class Header;
}

namespace client {
class ViewerHandler;
}

namespace sensor {
class SensorSpec;
class Acquisition;
} // namespace sensor

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief on server not found viewer event
    ///
    typedef void ( *onServerNotFoundFunc )( const char* ipv4, int port );

    ///
    /// \brief on server connected viewer event
    ///
    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );

    ///
    /// \brief on server disconnected viewer event
    ///
    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );

    ///
    /// \brief on new stream viewer event
    ///
    typedef bool ( *onNewStreamFunc )( const char* streamName, const io::Header* header );

#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief on new sensor viewer event
    ///
    typedef bool ( *onNewSensorFunc )( const char* streamName,
                                       const sensor::SensorSpec* sensorSpec );
#endif

    ///
    /// \brief on new data viewer event
    ///
    typedef void ( *onNewDataFunc )( const char* streamName, const Datas_t* datas );

#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief on new acq viewer event
    ///
    typedef void ( *onNewAcqFunc )( const char* streamName, const sensor::Acquisition* acq );
#endif

    ///
    /// \brief on del stream viewer event
    ///
    typedef void ( *onDelStreamFunc )( const char* streamName );

    ///
    /// \brief on set property viewer event
    ///
    typedef void ( *onSetPropertyFunc )( const char* streamName,
                                         const char* objectName,
                                         int property,
                                         const Any* value );

    ///
    /// \brief on log message viewer event
    ///
    typedef void ( *onLogMessageFunc )( const char* logMessage );

    ///
    /// \brief createViewerHandler
    /// \param onServerNotFound
    /// \param onServerConnected
    /// \param onServerDisconnected
    /// \param onNewStream
    /// \param onNewSensor
    /// \param onNewData
    /// \param onNewAcq
    /// \param onDelStream
    /// \param onSetProperty
    /// \param onLogMessage
    /// \return
    ///
    SRC_API client::ViewerHandler*
    createViewerHandler( onServerNotFoundFunc onServerNotFound,
                         onServerConnectedFunc onServerConnected,
                         onServerDisconnectedFunc onServerDisconnected,
                         onNewStreamFunc onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                         onNewSensorFunc onNewSensor,
#endif
                         onNewDataFunc onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                         onNewAcqFunc onNewAcq,
#endif
                         onDelStreamFunc onDelStream,
                         onSetPropertyFunc onSetProperty,
                         onLogMessageFunc onLogMessage );

    ///
    /// \brief freeViewer
    /// \param viewerHandler
    ///
    SRC_API void freeViewerHandler( client::ViewerHandler* viewerHandler );

    ///
    /// \brief printStatus
    /// \return
    ///
    SRC_API int printStatus();

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
