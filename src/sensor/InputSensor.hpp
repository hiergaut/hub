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

#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"
#include "core/Base.hpp"
#include "core/Input.hpp"

// User friendly headers (classes can be used as input parameter of constructor)
#include "io/input/InputFile.hpp"
#include "io/input/InputStream.hpp"
#include "io/input/InputStreamMqtt.hpp"
#include "io/input/InputStreamServer2.hpp"

namespace hub {
namespace sensor {

///
/// \brief The InputSensorT class
/// represents the remote sensor.
/// From any communication bus (file, RAM, socket) you have access to all the information specific
/// to the sensors as well as the acquisition flow coming from the physical sensor.
/// This class allows client applications to completely abstract from the notion
/// of sensor and to be interested only in the carried data.
/// This enables several applications to work simultaneously on the same data in real time.
/// This also avoids the need to manage the drivers of each of the manufacturers of the sensors you
/// wish to use.
///
/// todo: template class
///
class InputSensor : public Sensor
{
    template <class Input, class... Inputs>
    void initSensorSpec( const Input& input, const Inputs&... inputs ) {
        const auto& header = input.getHeader();
        hub::io::Memory memory( input.getHeader().getUserDefined() );

        SensorSpec sensorSpec;
        const auto start = std::chrono::high_resolution_clock::now();
        memory.read( sensorSpec );
        const auto end = std::chrono::high_resolution_clock::now();
        const auto period =
            std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
        const auto nBytes         = header.getSize();
        const auto bytesPerSecond = ( 1'000'000.0 * nBytes ) / period;
        std::cout << "\033[35m[InputSensor] unpacked sensor spec speedup : " << period / 1000.0
                  << " ms, " << PRETTY_BYTES( bytesPerSecond ) << "/s (" << PRETTY_BYTES( nBytes )
                  << ") " << sensorSpec << "\033[0m" << std::endl;
        m_spec += sensorSpec;
        m_specs.push_back( std::move( sensorSpec ) );

        if constexpr ( sizeof...( inputs ) > 0 ) { initSensorSpec( inputs... ); }
    }

  public:
    ///
    /// \brief acqMsg
    ///

    ///
    /// \brief InputSensor
    /// \param input
    ///
    template <class InputT>
    explicit InputSensor( InputT& input ) : Sensor( SensorSpec {} ), m_inputs( { &input } ) {
        static_assert( !std::is_same_v<InputSensor, InputT> );
        static_assert( std::is_base_of_v<Input, std::remove_cvref_t<InputT>> );
        initSensorSpec( input );
    }

    ///
    /// \brief InputSensor
    /// \param input
    /// \param input2
    ///
    template <class InputT>
    InputSensor( InputT& input, InputT& input2 ) :
        Sensor( SensorSpec {} ), m_inputs( { &input, &input2 } ) {
        static_assert( std::is_base_of_v<Input, std::remove_cvref_t<InputT>> );
        initSensorSpec( input, input2 );
    }

    ///
    /// \brief InputSensor
    /// \param input
    ///
    template <class InputT>
#if CPP_VERSION >= 20
        requires std::is_base_of_v<Input, std::remove_cvref_t<InputT>>
#endif
    // REQUIRES (std::is_base_of_v<Input, std::remove_cvref_t<InputT>>)
    explicit InputSensor( InputT&& input ) : Sensor( SensorSpec {} ) {
        static_assert( std::is_base_of_v<Input, std::remove_cvref_t<InputT>> );
        initSensorSpec( input );
        m_inputs.push_back( new std::remove_cvref_t<InputT>( std::move( input ) ) );
        m_inputOwner = true;
    }

    ///
    /// \brief InputSensor
    /// \param input
    /// \param input2
    ///
    template <class InputT>
#if CPP_VERSION >= 20
        requires std::is_base_of_v<Input, std::remove_cvref_t<InputT>>
#endif
    InputSensor( InputT&& input, InputT&& input2 ) : Sensor( SensorSpec {} ) {
        static_assert( std::is_base_of_v<Input, std::remove_cvref_t<InputT>> );
        initSensorSpec( input, input2 );
        m_inputs.push_back( new std::remove_cvref_t<InputT>( std::move( input ) ) );
        m_inputs.push_back( new std::remove_cvref_t<InputT>( std::move( input2 ) ) );
        m_inputOwner = true;
    }

    ~InputSensor() {
        if ( m_inputOwner ) {
            auto it = m_inputs.begin();
            while ( it != m_inputs.end() ) {
                auto cur = it;
                it++;
                delete ( *cur );
            }
        }
    }

    ///
    /// \brief operator >>
    /// \param acq
    ///
    void operator>>( Acquisition& acq ) {
        assert( m_spec.getResolution() == acq.getResolution() );

        if ( m_inputs.size() == 1 ) {
            Input& input = *m_inputs.at( 0 );
            input.read( acq );
        }
        else {
            Input& leftInput  = *m_inputs.at( 0 );
            Input& rightInput = *m_inputs.at( 1 );

            Acquisition rightAcq = make_acquisition( m_specs.at( 1 ).getResolution() );
            rightInput.read( rightAcq );

            auto& leftLastAcqs = m_lastAcqs;
            assert( leftLastAcqs.size() < 20 );
            Acquisition leftAcq = make_acquisition( m_specs.at( 0 ).getResolution() );

            if ( leftLastAcqs.empty() ) {
                leftInput.read( leftAcq );
                leftLastAcqs.push_back( leftAcq.copy() );
            }

            while ( rightAcq.getStart() < leftLastAcqs.front().getStart() ) {
#ifdef HUB_DEBUG_INPUT
                std::cout << "[InputSensor] operator>>(InputSensor&) shift rightAcq : " << rightAcq
                          << std::endl;
#endif
                assert( !rightInput.isEnd() );
                rightInput.read( rightAcq );
            }

            while ( leftLastAcqs.back().getStart() < rightAcq.getStart() && !leftInput.isEnd() ) {
                assert( !leftInput.isEnd() );
                leftInput.read( leftAcq );
                leftLastAcqs.push_back( leftAcq.copy() );
            }

            while ( leftLastAcqs.size() > 2 ) {
                leftLastAcqs.pop_front();
            }

            const auto& leftBeforeRightAcq = leftLastAcqs.front();
            const auto& leftAfterRightAcq  = leftLastAcqs.back();

            assert( leftInput.isEnd() || leftBeforeRightAcq.getStart() <= rightAcq.getStart() );
            assert( leftInput.isEnd() || rightAcq.getStart() <= leftAfterRightAcq.getStart() );

            const auto& closestAcq =
                ( std::abs( leftBeforeRightAcq.getStart() - rightAcq.getStart() ) >
                  std::abs( leftAfterRightAcq.getStart() - rightAcq.getStart() ) )
                    ? ( leftAfterRightAcq )
                    : ( leftBeforeRightAcq );

            auto sync_acq    = closestAcq << rightAcq;
            sync_acq.start() = rightAcq.getStart();
            sync_acq.end()   = rightAcq.getEnd();
            assert( sync_acq.getResolution() == acq.getResolution() );

            acq = std::move( sync_acq );
        }

#ifdef HUB_DEBUG_INPUT
        std::cout << HEADER << "read(Acquisition&) : " << acq << std::endl;
#endif
    }

    ///
    /// \brief getAllAcquisitions
    /// \return
    ///
    std::vector<Acquisition> getAllAcquisitions() {
        std::vector<Acquisition> acqs;
        auto acq = acqMsg();
        while ( std::none_of( m_inputs.begin(), m_inputs.end(), []( const Input* input ) {
            return input->isEnd();
        } ) ) {
            *this >> acq;
            acqs.push_back( acq.copy() );
        }
        return acqs;
    }

    ///
    /// \brief fillAllAcquisitions
    /// \param ts
    ///
    template <class Container,
              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
    void fillAllAcquisitions( Container& ts ) {
        auto acq = acqMsg();
        while ( std::none_of( m_inputs.begin(), m_inputs.end(), []( const Input* input ) {
            return input->isEnd();
        } ) ) {
            *this >> acq;
            ts.push_back( acq.copy() );
        }
    }

    ///
    /// \brief getInput
    /// \return
    ///
    const Input& getInput() const { return *m_inputs.at( 0 ); }

    ///
    /// \brief acqMsg
    /// \return
    ///
    Acquisition acqMsg() const { return make_acquisition( m_spec.getResolution() ); }

    ///
    /// \brief getInput
    /// \return
    ///
    Input& getInput() { return *m_inputs.at( 0 ); }

  private:
    std::vector<Input*> m_inputs;
    bool m_inputOwner = false;
    std::list<sensor::Acquisition> m_lastAcqs;
    std::vector<hub::sensor::SensorSpec> m_specs;
};

} // namespace sensor
} // namespace hub
