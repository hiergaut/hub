/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/15

#pragma once

#include <array>

#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/io/Memory.hpp"

namespace hub {
namespace io {

class SRC_API Header
{
  public:
    using MagicNumber = std::array<char, 128>;

    auto getSize() const {
        return sizeof( m_headerSize ) + sizeof( m_dataSize ) + 4 + m_userDefined.size();
    }

    Header( Size_t dataSize = 0, const Datas_t& userDefined = {} );

    Header( const Header& ) = default;
    Header& operator=( const Header& ) = default;

    bool operator==( const Header& other ) const {
        return m_magicNumber == other.m_magicNumber && m_headerSize == other.m_headerSize &&
               m_dataSize == other.m_dataSize && m_userDefined == other.m_userDefined;
    }
    auto toString() const {
        std::string str;
        for (const auto & number : m_magicNumber) {
            str += number;
        }
        return str + "\n" + std::to_string( m_headerSize ) + "\n" + std::to_string( m_dataSize ) + "\n" +
               hub::to_string( m_userDefined );
    }
    template <class Output>
    void write( Output& output ) {
        output.write( (Data_t*)m_magicNumber.data(), m_magicNumber.size() );
        output.writeAll( m_headerSize, m_dataSize, m_userDefined );
    }
    template <class Input>
    void read( Input& input ) {
        input.read( (Data_t*)m_magicNumber.data(), m_magicNumber.size() );

        checkMagicNumber();

        input.readAll( m_headerSize, m_dataSize, m_userDefined );
    }

    Size_t getDataSize() const { return m_dataSize; }
    const Datas_t& getUserDefined() const { return m_userDefined; }

  private:
    void checkMagicNumber() const;

  private:
    MagicNumber m_magicNumber;

    Size_t m_headerSize;
    Size_t m_dataSize;
    Datas_t m_userDefined;

    static_assert( sizeof( m_headerSize ) == 8 );
    static_assert( sizeof( m_dataSize ) == 8 );
};

template <class T>
Header make_header( const T& t ) {

    Memory memory;
    memory.write( t );

    Header header( t.dataSize(), memory.getData() );

    return header;
}

} // namespace io
} // namespace hub
