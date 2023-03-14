#pragma once

#include "InputSensor.hpp"
#include "Macros.hpp"
#include "client/Viewer.hpp"

namespace hub {

///
/// @brief native
/// Export these "C" functions usable by Unity C++ plugin or Matlab (loadlibrary).
///
namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createInputSensor
    /// \param streamName
    /// \param ipv4
    /// \param port
    /// \return
    ///
    SRC_API InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port );

    ///
    /// \brief freeInputSensor
    /// \param inputSensor
    ///
    SRC_API void freeInputSensor( InputSensor* inputSensor );

    ///
    /// \brief getAcquisitionSize
    /// \param inputSensor
    /// \return
    ///
    SRC_API int getAcquisitionSize( const InputSensor* inputSensor );

    ///
    /// \brief getAcquisition
    /// \param inputSensor
    /// \return
    ///
    SRC_API Acquisition* getAcquisition( const InputSensor* inputSensor );

    ///
    /// \brief freeAcquisition
    /// \param acquisition
    ///
    SRC_API void freeAcquisition( Acquisition* acquisition );

    ///
    /// \brief acquisition_getMeasure
    /// \param acquisition
    /// \param data
    /// \param iMeasure
    ///
    SRC_API void
    acquisition_getMeasure( const Acquisition* acquisition, unsigned char* data, int iMeasure = 0 );

    ///
    /// \brief acquisition_getStart
    /// \param acquisition
    /// \return
    ///
    SRC_API long long acquisition_getStart( const Acquisition* acquisition );

    ///
    /// \brief acquisition_to_string
    /// \param acquisition
    /// \param str
    /// \param strLen
    ///
    SRC_API void acquisition_to_string( const Acquisition* acquisition, char* str, int* strLen );

    /// \brief onNewStreamerFunc
    /// \param streamName
    /// \param sensorSpec
    typedef bool ( *onNewStreamerFunc )( const char* streamName, const SensorSpec* sensorSpec );

    /// \brief onDelStreamerFunc
    /// \param streamName
    /// \param sensorSpec
    typedef void ( *onDelStreamerFunc )( const char* streamName, const SensorSpec* sensorSpec );

    /// \brief onServerConnectedFunc
    /// \param ipv4
    /// \param port
    typedef void ( *onServerConnectedFunc )( const char* ipv4, int port );

    /// \brief onServerDisconnectedFunc
    /// \param ipv4
    /// \param port
    typedef void ( *onServerDisconnectedFunc )( const char* ipv4, int port );

    /// \brief onNewAcquisitionFunc
    /// \param streamName
    /// \param acq
    typedef void ( *onNewAcquisitionFunc )( const char* streamName, const Acquisition* acq );

    /// \brief onLogMessageFunc
    /// \param logMessage
    typedef void ( *onLogMessageFunc )( const char* logMessage );

    ///
    /// \brief createViewer
    /// \param onNewStreamer
    /// \param onDelStreamer
    /// \param onServerConnected
    /// \param onServerDisconnected
    /// \param onNewAcquisition
    /// \param ipv4
    /// \param port
    /// \param onLogMessage
    /// \return
    ///
    SRC_API client::Viewer* createViewer( onNewStreamerFunc onNewStreamer,
                                          onDelStreamerFunc onDelStreamer,
                                          onServerConnectedFunc onServerConnected,
                                          onServerDisconnectedFunc onServerDisconnected,
                                          onNewAcquisitionFunc onNewAcquisition,
                                          const char* ipv4,
                                          int port,
                                          onLogMessageFunc onLogMessage );

    ///
    /// \brief freeViewer
    /// \param viewer
    ///
    SRC_API void freeViewer( client::Viewer* viewer );

    ///
    /// \brief viewer_setIpv4
    /// \param viewer
    /// \param ipv4
    ///
    SRC_API void viewer_setIpv4( client::Viewer* viewer, const char* ipv4 );

    ///
    /// \brief viewer_setPort
    /// \param viewer
    /// \param port
    ///
    SRC_API void viewer_setPort( client::Viewer* viewer, int port );

    ///
    /// \brief viewer_isConnected
    /// \param viewer
    /// \return
    ///
    SRC_API bool viewer_isConnected( client::Viewer* viewer );

    ///
    /// \brief sensorSpec_getSensorName
    /// \param sensorSpec
    /// \param sensorName
    /// \param strLen
    ///
    SRC_API void
    sensorSpec_getSensorName( const SensorSpec* sensorSpec, char* sensorName, int* strLen );

    ///
    /// \brief sensorSpec_getResolutionsSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionsSize( const SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getResolutionSize
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionSize( const SensorSpec* sensorSpec, int iResolution = 0 );

    ///
    /// \brief sensorSpec_getResolutionsStr
    /// \param sensorSpec
    /// \param resolutionsStr
    ///
    SRC_API void sensorSpec_getResolutionsStr( const SensorSpec* sensorSpec, char* resolutionsStr );

    ///
    /// \brief sensorSpec_getFormat
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getFormat( const SensorSpec* sensorSpec, int iResolution );

    ///
    /// \brief sensorSpec_getDimensionsSize
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getDimensionsSize( const SensorSpec* sensorSpec, int iResolution );

    ///
    /// \brief sensorSpec_getDimension
    /// \param sensorSpec
    /// \param iResolution
    /// \param iDimension
    /// \return
    ///
    SRC_API int
    sensorSpec_getDimension( const SensorSpec* sensorSpec, int iResolution, int iDimension );

    ///
    /// \brief sensorSpec_getAcquisitionSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getAcquisitionSize( const SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getMetaDataStr
    /// \param sensorSpec
    /// \param metaDataStr
    ///
    SRC_API void sensorSpec_getMetaDataStr( const SensorSpec* sensorSpec, char* metaDataStr );

    ///
    /// \brief sensorSpec_copy
    /// \param source
    /// \return
    ///
    SRC_API SensorSpec* sensorSpec_copy( const SensorSpec* source );

    ///
    /// \brief sensorSpec_getMetaData
    /// \param sensorSpec
    /// \return
    ///
    SRC_API const SensorSpec::MetaData* sensorSpec_getMetaData( const SensorSpec* sensorSpec );

    ///
    /// \brief metaData_exists
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API bool metaData_exists( const SensorSpec::MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getString
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \param strLen
    /// \return
    ///
    SRC_API bool metaData_getString( const SensorSpec::MetaData* metaData,
                                     const char* metaName,
                                     char* output,
                                     int* strLen );

    ///
    /// \brief metaData_getMat4
    /// \param metaData
    /// \param metaName
    /// \param output
    /// \return
    ///
    SRC_API bool
    metaData_getMat4( const SensorSpec::MetaData* metaData, const char* metaName, float* output );

    ///
    /// \brief metaData_getInt
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API int metaData_getInt( const SensorSpec::MetaData* metaData, const char* metaName );

    ///
    /// \brief metaData_getUInt
    /// \param metaData
    /// \param metaName
    /// \return
    ///
    SRC_API double metaData_getDouble( const SensorSpec::MetaData* metaData, const char* metaName );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub