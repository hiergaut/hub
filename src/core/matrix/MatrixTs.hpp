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

#include "core/Matrix.hpp"
#include "core/MatrixBase.hpp"

namespace hub {

//////////////////////////////////////////// TEMPLATES /////////////////////////////////////////

///
/// \brief The MatrixTs class
/// represents a bunch of type, matrix
/// able to merge type or matrices into single matrix
/// everything is matrix at the end
///
template <class... Types>
#if CPP_VERSION >= 20
    requires( sizeof...( Types ) > 1 )
#endif
class MatrixTs
{
    static_assert( sizeof...( Types ) > 1 );

  public:
    ///
    /// \brief matrix
    ///
    static struct {
    } matrix;

    ///
    /// \brief Capacity
    ///
    static constexpr auto Capacity = 1;

    ///
    /// \brief Size
    ///
    static constexpr auto Size = sizeOf<Types...>();

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
    static constexpr auto nType() { return sizeof...( Types ); };

    ///
    /// \brief nDim
    /// \return
    ///
    static constexpr auto nDim() { return 1; };

    ///
    /// \brief getDim
    /// \return
    ///
    template <Size_t i>
#if CPP_VERSION >= 20
        requires( i == 0 )
#endif
    static constexpr auto getDim() {
        static_assert( i == 0 );
        return 1;
    }

  private:
    Buffer<Data_t, Size> m_buffer;

  public:
    ///
    /// \brief data
    /// \return
    ///
    const Data_t* data() const { return m_buffer.data(); }

    ///
    /// \brief data
    /// \return
    ///
    Data_t* data() { return m_buffer.data(); };

    ///
    /// \brief hasType
    /// \return
    ///
    template <class Type>
    static constexpr auto hasType() {
        return ( isSame<Type, Types>() || ... );
    }

    ///
    /// \brief hasType
    /// \return
    ///
    template <class... Types_>
    // REQUIRES( static constexpr, sizeof...( Types_ ) > 1, bool )
    static constexpr typename std::enable_if_t<( sizeof...( Types_ ) > 1 ), bool> hasType() {
        return ( hasType<Types_>() && ... );
    }

    ///
    /// \brief nType
    /// \return
    ///
    template <class Type>
    static constexpr int nType() {
        return ( nType<Type, Types>() + ... );
    }

    ///
    /// \brief nType
    /// \return
    ///
    template <class Type, class Matrix, class... Types_>
    // REQUIRES( static constexpr, isMatrix<Matrix>, int )
    static constexpr typename std::enable_if_t<isMatrix<Matrix>, int> nType() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template nType<Type>() + ( nType<Type, Types_>() + ... );
        }
        else { return Matrix::template nType<Type>(); }
    }

    ///
    /// \brief nType
    /// \return
    ///
    template <class Type, class Type2, class... Types_>
    // REQUIRES( static constexpr, !isMatrix<Type2>, int )
    static constexpr typename std::enable_if_t<!isMatrix<Type2>, int> nType() {
        if constexpr ( sizeof...( Types_ ) ) {
            return std::is_same_v<Type, Type2> + nType<Type, Types_...>();
        }
        else { return std::is_same_v<Type, Type2>; }
    }

    ///
    /// \brief getType
    ///
    template <Size_t i>
    using getType = typename std::tuple_element<i, std::tuple<Types...>>::type;

    ///
    /// \brief MatrixTs
    /// \param args
    ///
    template <class... Args>
    constexpr explicit MatrixTs( Args&&... args ) :
        m_buffer { std::forward<Data_t&&>( args )... } {}

    ///
    /// \brief get
    /// \return
    ///
    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    typename std::enable_if_t<(std::is_pointer_v<Type>), Type> get() {
        static_assert( hasType<RawType>() && i < nType<RawType>() );
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return (Type)( m_buffer.data() + offset );
    }

    ///
    /// \brief get
    /// \return
    ///
    template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    // REQUIRES(, std::is_pointer_v<Type>, Type )
    typename std::enable_if_t<std::is_pointer_v<Type>, Type> get() const {
        static_assert( hasType<RawType>() && i < nType<RawType>() );
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return (Type)( m_buffer.data() + offset );
    }

    ///
    /// \brief get
    /// \return
    ///
    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    typename std::enable_if_t<(!std::is_pointer_v<Type>), Type> get() {
        static_assert( hasType<RawType>() && i < nType<RawType>() );
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    ///
    /// \brief get
    /// \return
    ///
    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    // REQUIRES(, !std::is_pointer_v<Type>, Type )
    typename std::enable_if_t<!std::is_pointer_v<Type>, Type> get() const {
        static_assert( hasType<RawType>() && i < nType<RawType>() );
        const auto offset = getOffset<i, 0, RawType, Types...>();
        static_assert( 0 <= offset && offset < Size );
        return reinterpret_cast<Type>( *( m_buffer.data() + offset ) );
    }

    ///
    /// \brief name
    /// \return
    ///
    static CONSTEXPR20 std::string name() { return printName<Types...>(); }

    ///
    /// \brief toString
    /// \return
    ///
    constexpr auto toString() const { return name() + " = " + m_buffer.toString(); }

    ///
    /// \brief getOffset
    /// \return
    ///
    template <class Type, int i = 0>
            // REQUIRES( static constexpr, hasType<Type>() && i < nType<Type>(), Size_t )
            static constexpr typename std::enable_if_t < hasType<Type>() &&
        i<nType<Type>(), Size_t> getOffset() {
        return getOffset<i, 0, Type, Types...>();
    }

    ///
    /// \brief serialize
    /// \param mat
    ///
    void serialize( Matrix& mat ) const { serialize_<Types...>( mat ); }

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
    bool operator==( const Matrix& mat ) const { return getMatrix() == mat; }

    ///
    /// \brief serialize_
    /// \param mat
    ///
    template <class Type_, class... Types_>
    void serialize_( Matrix& mat ) const {
        // Matrix::serialize_( mat );
        if constexpr ( isMatrix<Type_> ) { mat |= Type_().getMatrix(); }
        else { mat |= make_matrix<Type_>(); }

        if constexpr ( sizeof...( Types_ ) > 0 ) { serialize_<Types_...>( mat ); }
    }

    ///
    /// \brief isSame
    /// \return
    ///
    template <class Type, class Matrix, class... Types_>
    // REQUIRES( static constexpr, isMatrix<Matrix>, bool )
    static constexpr typename std::enable_if_t<isMatrix<Matrix>, bool> isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return Matrix::template hasType<Type>() || isSame<Type, Types_...>();
        }
        else { return Matrix::template hasType<Type>(); }
    }

    ///
    /// \brief isSame
    /// \return
    ///
    template <class Type, class Type_, class... Types_>
    // REQUIRES( static constexpr, !isMatrix<Type_>, bool )
    static constexpr typename std::enable_if_t<!isMatrix<Type_>, bool> isSame() {
        if constexpr ( sizeof...( Types_ ) > 0 ) {
            return std::is_same<Type, Type_>() || isSame<Type, Types_...>();
        }
        else { return std::is_same<Type, Type_>(); }
    }

    ///
    /// \brief getOffset
    /// \return
    ///
    template <int ith, int i, class targetType, class Matrix, class... Types_>
    // REQUIRES( static constexpr, isMatrix<Matrix>, Size_t )
    static constexpr typename std::enable_if_t<isMatrix<Matrix>, Size_t> getOffset() {
        if constexpr ( Matrix::template hasType<targetType>() ) {
            if ( ith == i ) { return Matrix::template getOffset<targetType>(); }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return Matrix::Size + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else { return Matrix::Size; }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return Matrix::Size + getOffset<ith, i, targetType, Types_...>();
            }
            else { return Matrix::Size; }
        }
    }

    ///
    /// \brief getOffset
    /// \return
    ///
    template <int ith, int i, class targetType, class Type_, class... Types_>
    // REQUIRES( static constexpr, !isMatrix<Type_>, Size_t )
    static constexpr typename std::enable_if_t<!isMatrix<Type_>, Size_t> getOffset() {
        if constexpr ( std::is_same_v<targetType, Type_> ) {
            if ( ith == i ) { return 0; }
            else {
                if constexpr ( sizeof...( Types_ ) > 0 ) {
                    return sizeOf<Type_>() + getOffset<ith, i + 1, targetType, Types_...>();
                }
                else { return sizeOf<Type_>(); }
            }
        }
        else {
            if constexpr ( sizeof...( Types_ ) > 0 ) {
                return sizeOf<Type_>() + getOffset<ith, i, targetType, Types_...>();
            }
            else { return sizeOf<Type_>(); }
        }
    }

    ///
    /// \brief printName
    ///
    template <class Type_, class... Types_>
    static CONSTEXPR20 auto printName() {
        std::string str;
        str += TYPE_NAME( Type_() );
        str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );

        if constexpr ( sizeof...( Types_ ) > 0 ) { return str + "_" + printName<Types_...>(); }
        else { return str; }
    }
};
static_assert( isMatrix<MatrixTs<int, double>> );

} // namespace hub
