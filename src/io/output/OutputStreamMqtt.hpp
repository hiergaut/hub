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
 *   - Gauthier Bouyjou
 *******************************************************************************/

#pragma once

#ifdef HUB_USE_MQTT

#    include <mutex>
#    include <thread>

#    include <mqtt/client.h>

#    include "core/Output.hpp"
#    include "io/StreamMqtt.hpp"
#    include "sensor/Acquisition.hpp"
#    include "sensor/SensorSpec.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputStreamMqtt class
///
class SRC_API OutputStreamMqtt : public Output, public io::StreamMqtt
{
  public:
    ///
    /// \brief OutputStreamMqtt
    /// \param streamName
    /// \param ipv4
    /// \param port
    ///
    explicit OutputStreamMqtt( const std::string& streamName,
                               const std::string& ipv4 = s_defaultIpv4,
                               int port                = s_defaultPort );

    ///
    /// \brief write
    /// \param acq
    ///
    void write( const sensor::Acquisition& acq );

    ///
    /// \brief write
    /// \param sensorSpec
    ///
    void write( const sensor::SensorSpec& sensorSpec );

  protected:
    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    void write( const unsigned char* data, size_t len ) override;

    void close() override;
    bool isOpen() const override;

  private:
    std::unique_ptr<mqtt::client> m_client;
    mqtt::message_ptr m_msgPtr;

    std::string m_currentTopic;

    uint64_t m_acqSize = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void OutputStreamMqtt::write( const unsigned char* data, size_t len ) {
#    ifdef HUB_DEBUG_OUTPUT
    std::cout << "[OutputStreamMqtt] write(data, len = " << len << ")" << std::endl;
#    endif
    assert( m_client->is_connected() );
    assert( m_msgPtr != nullptr );
    assert( m_msgPtr->get_qos() == 2 );
    assert( m_msgPtr->is_retained() );
#    ifdef DEBUG_OUTPUT_STREAM
    std::cout << DEBUG_OUTPUT_STREAM + m_name + " write(const unsigned char*, size_t) len = " << len
              << ", current topic = " << m_currentTopic << std::endl;
#    endif
    m_msgPtr->set_payload( reinterpret_cast<const char*>( data ), len );
    assert( m_msgPtr->get_qos() == 2 );
    assert( m_msgPtr->is_retained() );
    m_client->publish( m_msgPtr );
}

inline void OutputStreamMqtt::close() {
    assert( m_client->is_connected() );

    m_msgPtr->set_payload( "none" );

    m_msgPtr->set_topic( s_topicStream + m_name + "/header/size" );
    m_client->publish( m_msgPtr );

    m_msgPtr->set_topic( s_topicStream + m_name + "/header/data" );
    m_client->publish( m_msgPtr );

    m_msgPtr->set_topic( s_topicStream + m_name + "/acqs/size" );
    m_client->publish( m_msgPtr );

    m_msgPtr->set_topic( s_topicStream + m_name + "/acqs/data0" );
    m_client->publish( m_msgPtr );

    m_msgPtr->set_payload( "inactive" );
    m_msgPtr->set_topic( s_topicStream + m_name );
    m_client->publish( m_msgPtr );

    m_msgPtr->set_retained( false );
    m_msgPtr->set_topic( s_topicEvents );
    m_msgPtr->set_payload( std::to_string( (int)ClientMessage::STREAMER_CLIENT_CLOSED ) + m_name );
    m_client->publish( m_msgPtr );
    m_msgPtr->set_retained( true );

    m_client->disconnect();
    assert( !m_client->is_connected() );
}

inline bool OutputStreamMqtt::isOpen() const {
    return m_client->is_connected();
}

} // namespace output
} // namespace hub

#endif
