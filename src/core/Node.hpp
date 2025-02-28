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

#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Base.hpp"

namespace hub {

///
/// \brief Dims
///
using Dims = std::vector<int>;

///
/// \brief The Node class
/// represents matrix for unique type.
/// Allows to vectorize matrices into single matrix.
///
// Todo hide this class from the user
class Node
{
  public:
    Node() = default;
    // Node( HashType hash, Dims&& dims, std::string typeName, Size_t size );

    ///
    /// \brief Node
    /// \param dims
    /// \param typeName
    /// \param size
    /// \param id
    ///
    Node( Dims&& dims, const std::string& typeName, Size_t size, TypeId_t id );

    ///
    /// \brief toString
    /// \param pretty
    /// \return
    ///
    std::string toString( bool pretty ) const;

    // Size_t size() const;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Node& other ) const;

#if CPLUSPLUSVERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) {
        return archive( self.m_dims, self.m_typeName, self.m_size );
    }
#endif
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_dims, self.m_typeName, self.m_size, self.m_id );
    }

    friend class Matrix;

    ///
    /// \brief getDims
    /// \return
    ///
    const Dims& getDims() const;

    ///
    /// \brief getTypeName
    /// \return
    ///
    const std::string& getTypeName() const;

    ///
    /// \brief getSize
    /// \return
    ///
    Size_t getSize() const;

    ///
    /// \brief getTypeId
    /// \return
    ///
    TypeId_t getTypeId() const;

  private:
    // HashType m_hashCode;
    Dims m_dims;
    std::string m_typeName;
    Size_t m_size = 0;
    TypeId_t m_id = Types::NONE;
    // Data_t * m_data = nullptr;
};

/////////////////////////////////////// MAKER /////////////////////////////////////////////////////

template <class Type, Size_t N = 1, Size_t... Ns>
#if CPLUSPLUSVERSION >= 20
    requires( N > 0 && ( ( Ns > 1 ) && ... ) )
#endif
static Node make_node(
    // Data_t * data
) {
    auto size = hub::sizeOf<Type>() * N;
    if constexpr ( sizeof...( Ns ) > 0 ) { size *= ( ... * Ns ); }
    return Node( std::move( Dims { N, Ns... } ), TYPE_NAME( Type() ), size, TYPE_ID( Type ) );
}

template <class Type, class... Dims>
static Node make_node(
    // Data_t * data,
    const Dims&... dims ) {
    const auto size = hub::sizeOf<Type>() * ( ... * dims );
    return Node( hub::Dims { dims... }, TYPE_NAME( Type() ), size, TYPE_ID( Type ) );
}

/////////////////////////////////////// INLINE ////////////////////////////////////////////////////

inline Node::Node( Dims&& dims, const std::string& typeName, Size_t size, TypeId_t id ) :
    m_dims { std::move( dims ) }, m_typeName { typeName }, m_size { size }, m_id { id } {}

inline std::string Node::toString( bool pretty ) const {
    std::string str;
    str += m_typeName;
    str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );
    if ( !( m_dims.size() == 1 && m_dims.at( 0 ) == 1 ) ) {
        if ( pretty )
            str += " : ";
        else
            str += ":";
        Size_t i = 0;
        for ( auto dim : m_dims ) {
            str += std::to_string( dim );
            if ( i != m_dims.size() - 1 ) {
                if ( pretty )
                    str += " x ";
                else
                    str += "x";
            }
            ++i;
        }
    }
    return str;
}

inline bool Node::operator==( const Node& other ) const {
    return m_dims == other.m_dims && m_typeName == other.m_typeName && m_size == other.m_size;
}

inline const Dims& Node::getDims() const {
    return m_dims;
}

inline const std::string& Node::getTypeName() const {
    return m_typeName;
}

inline TypeId_t Node::getTypeId() const {
    return m_id;
}

inline Size_t Node::getSize() const {
    return m_size;
}

} // namespace hub
