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

#include <iostream>
#include <string>

#include "Header.hpp"
#include "core/Base.hpp"

namespace hub {
namespace io {

///////////////////////////////////////////////////////////////

///
/// \brief The StreamBase class
/// is a helper class generalizing what a stream looks like with the current server implement.
///
class SRC_API StreamBase
{
  public:
    ///
    /// \brief The ClientType enum
    /// defines all type of clients able to connect to the server
    ///
    enum class ClientType { NONE = 0, STREAMER, STREAM_VIEWER, VIEWER, ASKER, KILLER, COUNT };

    ///
    /// \brief operator <<
    /// \param os
    /// \param clientType
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ClientType& clientType );

    ///
    /// \brief The ClientMessage enum
    ///
    enum class ClientMessage {
        NONE = 0,

        STREAMER_CLIENT_START,
        STREAMER_CLIENT_CLOSED,
        STREAMER_CLIENT_NEW_ACQ,
        STREAMER_CLIENT_INIT_SENSOR,

        STREAM_VIEWER_CLIENT_CLOSED,
        STREAM_VIEWER_CLIENT,

        VIEWER_CLIENT_CLOSED,
        VIEWER_CLIENT_SET_PROPERTY,

        ASKER_CLIENT_CLOSED,
        ASKER_CLIENT_GET_LIST_STREAMS,
        ASKER_CLIENT_GET_ACQ,

        CLIENT_SERVER_DOWN,
        STREAMER_CLIENT_NEW_STREAM_VIEWER,
        STREAMER_CLIENT_INITED,
        STREAMER_CLIENT_STREAM_VIEWER_INITED,

        NEW_RETAIN_DATA,
        FULLY_RETAINED_DATA,
        STREAMER_CLIENT_DEL_STREAM_VIEWER,

        VIEWER_CLIENT_STREAM_ADDED,

        COUNT
    };

    ///
    /// \brief operator <<
    /// \param os
    /// \param msg
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ClientMessage& msg );

    ///
    /// \brief The ServerMessage enum
    ///
    enum class ServerMessage {
        NONE = 0,

        FOUND,
        NOT_FOUND,
        OK,

        SERVER_CLOSED,

        STREAMER_CLOSED,
        STREAMER_INITED,

        STREAM_VIEWER_CLOSED,
        STREAM_VIEWER_NEW_ACQ,

        VIEWER_CLOSED,
        VIEWER_NEW_STREAMER,
        VIEWER_DEL_STREAMER,
        VIEWER_SET_PROPERTY,

        ASKER_CLOSED,
        ASKER_LIST_STREAMS,

        STREAM_VIEWER_INITED,

        RETAINED_DATA_START,
        RETAINED_DATA_END,
        RETAINED_SHARED_TO_VIEWER,

        COUNT
    };

    ///
    /// \brief operator <<
    /// \param os
    /// \param msg
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const ServerMessage& msg );

    ///
    /// \brief The exception class
    /// is thrown when some server specific processes failed
    ///
    class exception : public std::runtime_error
    {
      public:
        ///
        /// \brief exception
        /// launched when peer connection is lost or invalid.
        /// \param message
        /// shown by the client application when exception has been launched.
        ///
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}

        ///
        /// \brief what
        /// allow the user to catch message exception.
        /// \return
        /// message describe the exception.
        ///
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  protected:
    ///
    /// \brief
    /// \param name [in] unique id of stream for log message
    /// \param ipv4 [in] ip of stream
    /// \param port [in] port of stream
    ///
    StreamBase( const std::string& name, const std::string& ipv4, int port );
    // StreamBase( const char* name, const char* ipv4, int port, const Header& header );

    // Header m_header;
    ///
    /// \brief Unique name of stream for log message
    ///
    const std::string m_name;

    ///
    /// \brief Stream ip
    ///
    const std::string m_ipv4;

    ///
    /// \brief Stream port
    ///
    const int m_port;
};

} // namespace io
} // namespace hub
