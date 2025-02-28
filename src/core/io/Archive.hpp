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

#include <queue>

#include "InputOutput.hpp"
#include "core/Base.hpp"

#ifndef HUB_NON_BUILD_IO
#    include "io/Header.hpp"
#endif

namespace hub {
namespace io {

///
/// \brief The ArchiveT class
/// implements input/output features with a any serializer to save data locally in the unique
/// archive. You can write or read in the archive.
/// \warning When you read in the archive, the information is lost. The data is not retained.
///
template <class SerializerT = Serializer, class InputOutputT = InputOutput<SerializerT>>
class ArchiveT : public InputOutputT
{
  public:
    using InputOutputT::read;
    using InputOutputT::write;

    ArchiveT() = default;

    ///
    /// \brief ArchiveT
    /// \param header
    ///
#ifndef HUB_NON_BUILD_IO
    explicit ArchiveT( const io::Header& header ) : m_header { header } {};
#endif

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        assert( !m_datas.empty() );
        const auto& vector = m_datas.front();
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "read(Data_t*, Size_t) : data = " << vector
                  << ", read size = " << size << std::endl;
#endif
        assert( vector.size() == size );

        memcpy( data, vector.data(), size );
        m_datas.pop();

#ifdef DEBUG
        ++m_nCall;
        m_lastCallSize = size;
#endif
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        assert( size > 0 );
        std::vector<hub::Data_t> vector( data, data + size );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif
        m_datas.push( std::move( vector ) );

#ifdef DEBUG
        ++m_nCall;
        m_lastCallSize = size;
#endif
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
    bool isEnd() const override { return m_datas.empty(); }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }

    void setRetain( bool retained ) override {}

    ///
    /// \brief getHeader
    /// \return
    ///
#ifndef HUB_NON_BUILD_IO
    const Header& getHeader() const { return m_header; }
#endif

#ifdef DEBUG
    ///
    /// \brief The number of call is used to check how many IO operations are done during the test.
    /// Able to compare stats between other IO implements.
    /// \return The number of io calls.
    ///
    size_t getNCall() const { return m_nCall; }

    ///
    /// \brief Get the latest io call size.
    /// \return The latest io call size.
    ///
    size_t getLastCallSize() const { return m_lastCallSize; }
#endif

  private:
    std::queue<std::vector<hub::Data_t>> m_datas;
#ifdef DEBUG
    size_t m_nCall        = 0;
    size_t m_lastCallSize = 0;
#endif
#ifndef HUB_NON_BUILD_IO
    io::Header m_header;
#endif
};

///
/// \brief Archive is a readable/writeable object saving data locally (RAM)
///
using Archive = ArchiveT<>;

} // namespace io
} // namespace hub
