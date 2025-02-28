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

#include <type_traits>

#include "core/Matrix.hpp"
#include "core/MatrixBase.hpp"

namespace hub {

/////////////////////////////////////////// TEMPLATES
/////////////////////////////////////////////////////

///
/// \brief The MatrixXDBase class
/// defined basic features of nd array
///
template <class Type, Size_t... Ns>
#if CPP_VERSION >= 20
    requires( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) )
#endif
class MatrixXDBase
{
    static_assert( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) );

  public:
    ///
    /// \brief matrix
    ///
    static struct {
    } matrix;

    ///
    /// \brief Capacity
    ///
    static constexpr auto Capacity = ( Ns * ... );

    ///
    /// \brief Size
    ///
    static constexpr auto Size = sizeOf<Type>() * Capacity;

    ///
    /// \brief capacity
    /// \return
    ///
    static constexpr auto capacity() { return Capacity; };

    ///
    /// \brief size
    /// \return
    ///
    static constexpr auto size() { return Size; };

    ///
    /// \brief nType
    /// \return
    ///
    static constexpr auto nType() { return 1; };

    ///
    /// \brief nDim
    /// \return
    ///
    static constexpr auto nDim() { return sizeof...( Ns ); };

    ///
    /// \brief getDim
    /// \return
    ///
    template <Size_t i>
#if CPP_VERSION >= 20
        requires( 0 <= i && i < nDim() )
#endif
    static constexpr auto getDim() {
        static_assert( 0 <= i && i < nDim() );
        auto j = 0;
        for ( auto dim : { Ns... } ) {
            if ( j == i ) return dim;
            ++j;
        }
        return (Size_t)0;
    }

    ///
    /// \brief data
    /// \return
    ///
    const Data_t* data() const { return m_buffer.data(); }

    ///
    /// \brief hasType
    /// \return
    ///
    template <class Type_>
    static constexpr auto hasType() {
        return std::is_same<Type, Type_>();
    }

    ///
    /// \brief hasType
    /// \return
    ///
    template <class... Types>
    // REQUIRES( static constexpr, sizeof...( Types ) > 1, bool )
    static constexpr typename std::enable_if_t<( sizeof...( Types ) > 1 ), bool> hasType() {
        return ( hasType<Types>() && ... );
    }

    ///
    /// \brief nType
    /// \return
    ///
    template <class Type_>
    static constexpr int nType() {
        return std::is_same_v<Type, Type_>;
    }

    ///
    /// \brief getOffset
    /// \return
    ///
    template <class Type_, int i = 0>
            // REQUIRES( static constexpr, hasType<Type_>() && i < nType<Type_>(), Size_t )
            static constexpr typename std::enable_if_t < hasType<Type_>() &&
        i<nType<Type_>(), Size_t> getOffset() {
        return 0;
    }

    template <Size_t ith>
#if CPP_VERSION >= 20
        requires( ith == 0 )
#endif
    using getType = Type;

  public:
    template <class... Args>
    // cppcheck-suppress noExplicitConstructor
    constexpr MatrixXDBase( Args&&... args ) : m_buffer { std::forward<Type&&>( args )... } {}

    static CONSTEXPR20 auto name() {
        std::string str;
        str += TYPE_NAME( Type() );

        if ( !( nDim() == 1 && getDim<0>() == 1 ) ) {
            str += ":";
            Size_t i = 0;
            for ( auto dim : { Ns... } ) {
                str += std::to_string( dim );
                if ( i != nDim() - 1 ) str += "x";
                ++i;
            }
        }
        return str;
    }

    ///
    /// \brief toString
    ///
    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    ///
    /// \brief get
    /// \return
    ///
    template <int i = 0>
    const Type& get() {
        static_assert( 0 <= i && i < Capacity );
        return reinterpret_cast<const Type&>( *( m_buffer.data() + i * sizeof( Type ) ) );
    }

    ///
    /// \brief serialize
    /// \param mat
    ///
    void serialize( Matrix& mat ) const {
        assert( !mat.hasValue() );
        auto matrix2 = make_matrix<Type, Ns...>();
        mat |= matrix2;
    }

    ///
    /// \brief getMatrix
    /// \return
    ///
    Matrix getMatrix() const {
        Matrix mat;
        serialize( mat );
        mat.setData( m_buffer.data(), m_buffer.size() );
        return mat;
    }

    ///
    /// \brief operator ==
    /// \param mat
    /// \return
    ///
    bool operator==( const Matrix& mat ) { return getMatrix() == mat; }

  private:
    // Buffer<Type, Capacity> m_buffer;
    Buffer<Data_t, Size> m_buffer;
};
static_assert( isMatrix<MatrixXDBase<int, 2>> );

///
/// \brief The MatrixXD class
/// represent nd array basic features
///
template <class Type, Size_t... Ns>
#if CPP_VERSION >= 20
    requires( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) )
#endif
class MatrixXD : public MatrixXDBase<Type, Ns...>
{
    static_assert( sizeof...( Ns ) > 0 && ( ( Ns > 1 ) && ... ) );

  public:
    ///
    /// \brief n
    /// \return
    ///
    template <int i>
    static constexpr Size_t nDim() {
        static_assert( 0 <= i && i < MatrixXDBase<Type, Ns...>::nDim() );
        int j = 0;
        for ( const auto& n : { Ns... } ) {
            if ( i == j ) return n;
            ++j;
        }
        return 0;
    }
};

///
/// \brief The MatrixXD class
/// is a 1D matrix
/// vector with constant length
///
template <class Type, Size_t N>
#if CPP_VERSION >= 20
    requires( N > 1 )
#endif
class MatrixXD<Type, N> : public MatrixXDBase<Type, N>
{
    static_assert( N > 1 );

  public:
    ///
    /// \brief length
    /// \return
    ///
    static constexpr auto length() { return N; }
};

///
/// \brief The MatrixXD class
/// is a 2D matrix
/// similar to image with constant width and height
///
template <class Type, Size_t N, Size_t N2>
class MatrixXD<Type, N, N2> : public MatrixXDBase<Type, N, N2>
{
  public:
    ///
    /// \brief width
    /// \return
    ///
    static constexpr Size_t width() { return N; }

    ///
    /// \brief height
    /// \return
    ///
    static constexpr Size_t height() { return N2; }
};

///
/// \brief The MatrixXD class
/// is a 3D matrix
///
template <class Type, Size_t N, Size_t N2, Size_t N3>
class MatrixXD<Type, N, N2, N3> : public MatrixXDBase<Type, N, N2, N3>
{
  public:
    ///
    /// \brief nx
    /// \return
    ///
    static constexpr Size_t nx() { return N; }

    ///
    /// \brief ny
    /// \return
    ///
    static constexpr Size_t ny() { return N2; }

    ///
    /// \brief nz
    /// \return
    ///
    static constexpr Size_t nz() { return N3; }
};

///
/// \brief The MatrixXD class
/// is a 4D matrix
///
template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
class MatrixXD<Type, N, N2, N3, N4> : public MatrixXDBase<Type, N, N2, N3, N4>
{
  public:
    ///
    /// \brief nx
    /// \return
    ///
    static constexpr Size_t nx() { return N; }

    ///
    /// \brief ny
    /// \return
    ///
    static constexpr Size_t ny() { return N2; }

    ///
    /// \brief nz
    /// \return
    ///
    static constexpr Size_t nz() { return N3; }

    ///
    /// \brief nt
    /// \return
    ///
    static constexpr Size_t nt() { return N4; }
};

///////////////////////////// 1D

///
/// \brief Matrix1D
///
template <class Type, Size_t N>
using Matrix1D = MatrixXD<Type, N>;

///
/// \brief Vector
///
template <class Type, Size_t N>
using Vector = MatrixXD<Type, N>;

///////////////////////////// 2D

///
/// \brief Matrix2D
///
template <class Type, Size_t N, Size_t N2>
using Matrix2D = MatrixXD<Type, N, N2>;

///
/// \brief Image
///
template <class Type, Size_t N, Size_t N2>
using Image = MatrixXD<Type, N, N2>;

///////////////////////////// 3D

///
/// \brief Matrix3D
///
template <class Type, Size_t N, Size_t N2, Size_t N3>
using Matrix3D = MatrixXD<Type, N, N2, N3>;

///
/// \brief Volume
///
template <class Type, Size_t N, Size_t N2, Size_t N3>
using Volume = MatrixXD<Type, N, N2, N3>;

///////////////////////////// 4D

///
/// \brief Matrix4D
///
template <class Type, Size_t N, Size_t N2, Size_t N3, Size_t N4>
using Matrix4D = MatrixXD<Type, N, N2, N3, N4>;

} // namespace hub
