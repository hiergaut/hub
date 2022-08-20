#pragma once

#include <type_traits>

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

class SRC_API OutputSensor : public Sensor
{
  public:
    template <class OutputInterface,
              typename = typename std::enable_if<
                  std::is_base_of<io::OutputInterface, OutputInterface>::value>::type>
    OutputSensor( const SensorSpec&& sensorSpec, OutputInterface&& outputInterface ) :

        Sensor( std::move( sensorSpec ),
                *std::move( new OutputInterface( std::move( outputInterface ) ) ) ) {

        static_assert( std::is_base_of<io::OutputInterface, OutputInterface>::value,
                       "not a base class" );
        m_interface.write( spec );
    }

    template <class OutputInterface>
    OutputSensor( const SensorSpec&& sensorSpec, OutputInterface& outputInterface ) = delete;

    OutputSensor( const OutputSensor& outputSensor )           = delete;
    OutputSensor operator=( const OutputSensor& outputSensor ) = delete;

  public:
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

} // namespace hub
