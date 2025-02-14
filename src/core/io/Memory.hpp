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

#include "InputOutput.hpp"
#include "core/Base.hpp"

namespace hub {
namespace io {

///
/// \brief The MemoryT class
/// implements input/output features with a any serializer to save data locally in a vector.
/// It's allow to pack/serialize many objects into one vector.
///
template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class MemoryT : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    ///
    /// \brief Construct with any container
    /// \param container [in] Copy all data from container to internal data
    ///
    template <class Container>
    explicit MemoryT( const Container& container ) {
        m_data.clear();
        m_data.insert( m_data.end(), container.begin(), container.end() );
    }

    ///
    /// \brief MemoryT
    ///
    MemoryT() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_data.empty() );

        std::copy( m_data.begin(), std::next( m_data.begin(), size ), data );
        m_data.erase( m_data.begin(), std::next( m_data.begin(), size ) );

#ifdef DEBUG_MEMORY
        std::vector<Data_t> vector( data, data + size );
        std::cout << "[Memory] read " << vector << std::endl;
#endif
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
#ifdef DEBUG_MEMORY
        std::vector<Data_t> vector( data, data + size );
        std::cout << "[Memory] write " << vector << std::endl;
#endif

        assert( size > 0 );
        const auto prevSize = m_data.size();
        m_data.resize( prevSize + size );
        std::copy( data, data + size, std::next( m_data.begin(), prevSize ) );
    }

    void close() override {};

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override { return true; }

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override { return m_data.empty(); }

    ///
    /// \brief clear
    ///
    void clear() override { m_data.clear(); }

    void setRetain( bool retained ) override {}

    ///
    /// \brief Getter function
    /// \return Internal data
    ///
    const Datas_t& getData() const {
        return m_data;
        // Datas_t datas( m_data.begin(), m_data.end() );
    }

  private:
    Datas_t m_data;
};

///
/// \brief Memory
///
using Memory = MemoryT<>;

} // namespace io
} // namespace hub
