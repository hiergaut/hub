#pragma once

// #include "io/output/OutputBase.hpp"

// #include "io/output/OutputImpl.hpp"
// #include "io/output/OutputZppBits.hpp"
#include <array>

#include "core/Info.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"
#include "core/io/Memory.hpp"
// #include "ios.hpp"

namespace hub {
namespace io {

class Header
{
  public:
    using MagicNumber = std::array<char, 128>;
    // clang-format off
    // static constexpr MagicNumber s_localMagicNumber { 'H', 'U', 'B',
    //                                             (Data_t)s_versionMajor,
    //                                             (Data_t)s_versionMinor,
    //                                             (Data_t)s_versionPatch,
    //                                             ' ', 'C', 'o', 'p', 'y', 'r', 'i', 'g', 'h', 't', ' ' , '2', '0', '2', '4'
                                                 // };
    // clang-format on

    auto getSize() const {
        // return sizeof( m_headerSize ) + sizeof( m_magicNumber ) + sizeof( m_dataSize ) + 4 +
        return sizeof( m_headerSize ) + sizeof( m_dataSize ) + 4 + m_userDefined.size();
    }

    Header( Size_t dataSize = 0, const Datas_t& userDefined = {} ) :
        m_dataSize( dataSize ), m_userDefined( userDefined ) {
        m_headerSize = getSize();

        // memset(m_magicNumber.data(), ' ', 31);
        const auto joker = ' ';
        std::fill( m_magicNumber.begin(), m_magicNumber.end(), joker );
        m_magicNumber.back() = '\n';
        // m_magicNumber.back() = 0;

#ifdef WIN32
        sprintf_s( m_magicNumber,
                   "%c%c%c %d.%d.%d",
                   'H',
                   'U',
                   'B',
                   s_versionMajor,
                   s_versionMinor,
                   s_versionPatch );
#else
#    ifdef OS_MACOS
        snprintf( m_magicNumber,
                  80,
                  "%c%c%c %d.%d.%d",
                  'H',
                  'U',
                  'B',
                  s_versionMajor,
                  s_versionMinor,
                  s_versionPatch );
#    else
        sprintf( m_magicNumber.data(),
                 "%c%c%c %d.%d.%d * %s",
                 'H',
                 'U',
                 'B',
                 s_versionMajor,
                 s_versionMinor,
                 s_versionPatch,
                 "Copyright 2022-2023 * author : gauthierbouyjou@aol.com" );
#    endif
#endif

        assert( strlen( m_magicNumber.data() ) < sizeof( m_magicNumber ) );
        m_magicNumber[strlen( m_magicNumber.data() )] = joker;
        //    memory.write( reinterpret_cast<unsigned char*>(m_magicNumber), 80 );
        // write( reinterpret_cast<unsigned char*>( m_magicNumber ), 80 );
    }

    Header( const Header& )            = default;
    Header& operator=( const Header& ) = default;

    bool empty() const { return m_userDefined.empty(); }
    bool operator==( const Header& other ) const {
        return m_magicNumber == other.m_magicNumber && m_headerSize == other.m_headerSize &&
               m_dataSize == other.m_dataSize && m_userDefined == other.m_userDefined;
    }
    auto toString() const {
        return std::to_string( m_headerSize ) + ":" + std::to_string( m_dataSize ) + ":" +
               ::toString( m_userDefined );
    }
    template <class Output>
    void write( Output& output ) {
        output.write( (Data_t*)m_magicNumber.data(), m_magicNumber.size() );
        output.writeAll( m_headerSize, m_dataSize, m_userDefined );
    }
    template <class Input>
    void read( Input& input ) {
        input.read( (Data_t*)m_magicNumber.data(), m_magicNumber.size() );

        int versionMajor;
        int versionMinor;
        int versionPatch;
        char h;
        char u;
        char b;
#ifdef WIN32
        sscanf_s( magicNumber,
                  "%c%c%c %d.%d.%d",
                  &h,
                  1,
                  &u,
                  1,
                  &b,
                  1,
                  &versionMajor,
                  &versionMinor,
                  &versionPatch );
#else
        sscanf( m_magicNumber.data(),
                "%c%c%c %d.%d.%d",
                &h,
                &u,
                &b,
                &versionMajor,
                &versionMinor,
                &versionPatch );
#endif
        assert( h == 'H' );
        assert( u == 'U' );
        assert( b == 'B' );
        assert( versionMajor <= s_versionMajor );
        assert( versionMinor <= s_versionMinor );
        assert( versionPatch <= s_versionPatch );

        input.readAll( m_headerSize, m_dataSize, m_userDefined );

        // #ifdef DEBUG
        //     std::cout <<  "[InputImpl] read(magic number) : '" << magicNumber << "'" <<
        //     std::endl;
        // #endif

        // assert( m_magicNumber[0] == 'H' );
        // assert( m_magicNumber[1] == 'U' );
        // assert( m_magicNumber[2] == 'B' );
        // assert( m_magicNumber[3] <= s_versionMajor );
        // assert( m_magicNumber[4] <= s_versionMinor );
        // assert( m_magicNumber[5] <= s_versionPatch );
    }
    // static constexpr auto serialize( auto& archive, auto& self ) {
    // return archive( self.m_headerSize, self.m_dataSize, self.m_userDefined );
    // }

    //     void write( Serializer& serializer ) const {
    //         // serializer.writeAll( m_magicNumber, m_headerSize, m_dataSize, m_userDefined );
    //         serializer.writeAll( m_magicNumber);
    //     }
    //     void read( Serializer& serializer ) {
    //         serializer.readAll( m_magicNumber);

    //     }

    Size_t getDataSize() const { return m_dataSize; }
    const Datas_t& getUserDefined() const { return m_userDefined; }

    // const Datas_t & getUserDefined() const;

  private:
    MagicNumber m_magicNumber;

    Size_t m_headerSize;
    // MagicNumber m_magicNumber = s_localMagicNumber;
    Size_t m_dataSize;
    Datas_t m_userDefined;

    // static_assert( sizeof( m_magicNumber ) == 64 );

    static_assert( sizeof( m_headerSize ) == 8 );
    static_assert( sizeof( m_dataSize ) == 8 );
    static_assert( sizeof( m_userDefined ) == 24 );
};
// static_assert( sizeof( Header ) == 8 + 8 + 8 + 24 );

template <class T>
Header make_header( const T& t ) {

    Memory memory;
    memory.write( t );

    Header header( t.dataSize(), memory.getData() );

    return header;
}

// template <class... Ts>
// Header make_header( const Ts&... ts ) {

//     Memory memory;
//     memory.writeAll( ts... );

//     Header header( 0, memory.getData() );

//     return header;
// }

} // namespace io
} // namespace hub
