
#pragma once

#ifdef BUILD_SERVER
#    include "OutputStreamServer.hpp"
#endif

#    ifdef USE_MQTT
#        include "OutputStreamMqtt.hpp"
#    endif

namespace hub {
using namespace io;
namespace output {

#ifdef BUILD_SERVER
using OutputStream = OutputStreamServer;
#else
using OutputStream = OutputStreamMqtt;
#endif

static_assert( std::is_base_of<OutputStreamInterface, OutputStream>::value,
               "OutputStreamInterface is base class of OutputStream" );

} // namespace output
} // namespace hub
