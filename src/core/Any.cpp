#include "Any.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <typeinfo>
#include <vector>

// #include "data/Mat4.hpp"
// #include "data/Mesh.hpp"

namespace hub {

// Any::Any( const Any& any ) : m_type( any.m_type ), m_hasValue( any.m_hasValue ) {
//     assert( any.m_hasValue );

//    switch ( m_type ) {
//    case Any::Type::INT: {
//        m_int = any.m_int;
//    } break;

//    case Any::Type::DOUBLE: {
//        m_double = any.m_double;
//    } break;

//    case Any::Type::STRING: {
//        m_std_string = any.m_std_string;
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        m_const_char_ptr = any.m_const_char_ptr;
//    } break;

//    case Any::Type::MAT4: {
//        m_mat4 = new data::Mat4( *any.m_mat4 );
//    } break;

//    case Any::Type::MESH: {
//        m_mesh = new data::Mesh( *any.m_mesh );
//    } break;

//    default:
//        assert( false );
//    }
//}

// Any& Any::operator=( const Any& any ) {
//     m_type     = any.m_type;
//     m_hasValue = any.m_hasValue;
//     assert( any.m_hasValue );

//    switch ( m_type ) {
//    case Any::Type::INT: {
//        m_int = any.m_int;
//    } break;

//    case Any::Type::DOUBLE: {
//        m_double = any.m_double;
//    } break;

//    case Any::Type::STRING: {
//        m_std_string = any.m_std_string;
//    } break;

//    case Any::Type::CONST_CHAR_PTR: {
//        m_const_char_ptr = any.m_const_char_ptr;
//    } break;

//    case Any::Type::MAT4: {
//        m_mat4 = any.m_mat4;
//    } break;

//    case Any::Type::MESH: {
//        m_mesh = any.m_mesh;
//    } break;

//    default:
//        assert( false );
//    }

//    return *this;
//}

// Any::Any( int value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::INT;
//     m_int      = value;
//     m_hasValue = true;
// }
// Any::Any( double value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::DOUBLE;
//     m_double   = value;
//     m_hasValue = true;
// }
// Any::Any( const std::string& value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type       = Type::STRING;
//     m_std_string = value;
//     m_hasValue   = true;
// }
// Any::Any( const char* value ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type           = Type::CONST_CHAR_PTR;
//     m_const_char_ptr = value;
//     m_hasValue       = true;
// }
// Any::Any( const data::Mat4& value ) : m_mat4( new data::Mat4( value ) ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::MAT4;
//     m_hasValue = true;
// }

// Any::Any( const data::Mesh& value ) : m_mesh( new data::Mesh( value ) ) {
//     assert( m_type == Type::NONE );
//     assert( !m_hasValue );
//     m_type     = Type::MESH;
//     m_hasValue = true;
// }

// const int& Any::get<int>() const {
//     assert( m_type == Type::INT );
//     return m_int;
// }
// const double& Any::get<double>() const {
//     assert( m_type == Type::DOUBLE );
//     return m_double;
// }
// const std::string& Any::getStdString() const {
//     assert( m_type == Type::STRING );
//     return m_std_string;
// }
// const char* Any::getConstCharPtr() const {
//     assert( m_type == Type::CONST_CHAR_PTR );
//     return m_const_char_ptr;
// }
// const data::Mat4& Any::getMat4() const {
//     assert( m_type == Type::MAT4 );
//     assert( m_mat4 != nullptr );
//     return *m_mat4;
// }

// const data::Mesh& Any::getMesh() const {
//     assert( m_type == Type::MESH );
//     assert( m_mesh != nullptr );
//     return *m_mesh;
// }

// std::string Any::anyValue2string() const {

//    const auto& any = *this;

//    if ( any.hasValue() ) {

//        return m_any2valueStr(m_any);
//    }
//    return "";

//        const auto& anyType = any.type();

////        return m_anyToString(m_any);

//        if ( anyType == typeid( int ) ) {
//            const auto& val = any.get<int>();
//            return std::to_string( val );
//        }
//        else if ( anyType == typeid( double ) ) {
//            const auto& val = any.get<double>();
//            return std::to_string( val );
//        }
//        else if ( anyType == typeid( std::string ) ) {
//            const auto& val = "'" + any.get<std::string>() + "'";
//            return val;
//        }
//        else if ( anyType == typeid( const char* ) ) {
//            const auto& val = any.get<const char*>();
//            return "'" + std::string( val ) + "'";
//        }
//        else {
//            return m_any2valueStr(m_any);
//            //        assert(false);
//            return "";
//        }

//        switch ( any.type() ) {
//        case Any::Type::INT: {
//            const auto& val = any.get<int>();
//            return std::to_string( val );
//        } break;

//        case Any::Type::DOUBLE: {
//            const auto& val = any.get<double>();
//            return std::to_string( val );
//        } break;

//        case Any::Type::STRING: {
//            const auto& val = "'" + any.get<std::string>() + "'";
//            return val;
//        } break;

//        case Any::Type::CONST_CHAR_PTR: {
//            const auto& val = any.get<const char*>();
//            return "'" + std::string( val ) + "'";
//        } break;

//            // todo any
////        case Any::Type::MAT4: {
////            const auto& val = any.get<data::Mat4>();
////            return val.to_string();
////        } break;

////        case Any::Type::MESH: {
////            const auto& val = any.get<data::Mesh>();
////            return val.to_string();
////        } break;

//        default:
//            assert( false );
//        }
//    }

//    return "";
//}

// static const std::string s_anyType2string[static_cast<int>( Any::Type::COUNT )] = {
//     "none",
//     "int",
//     "double",
//     "string",
//     "cst_char_ptr",
//     "mat4",
//     "mesh",
// };

// Any::Type Any::type() const
//{
//     return m_type;
// }

// const std::type_info & Any::typeId() const
//{
//     return m_any.type();
// }

// const std::string& Any::typeName() const {
//     return s_anyType2string[static_cast<int>( m_type )];
// }

// std::ostream& operator<<( std::ostream& os, const Any::Type& type ) {
//     os << s_anyType2string[(int)type];
//     return os;
// }

std::string Any::to_string() const {
    //    std::string ret = "'" + Any::typeName() + "' = " + anyValue2string( *this );

    //    const auto& any = *this;

    if ( hasValue() ) {
        //    return m_any2valueStr(m_any);
        return "*'" + std::string( m_any.type().name() ) + "' : " + m_any2valueStr( m_any ) + "*";
        //    return ret;
    }
    else {
        return "*'" + std::string( m_any.type().name() ) + "' : none*";
    }
}

 std::string Any::getValueStr() const {
     return m_any2valueStr( m_any );
 }

bool Any::operator==( const Any& any ) const {
    assert( hasValue() );
    //    assert( m_type == any.m_type );
    assert( m_any.type() == any.m_any.type() );
    assert( any.hasValue() );
    //    if ( m_type == any.m_type && hasValue() == any.m_hasValue() ) {
    //    if ( m_type == any.m_type) {

    assert( m_anyCompare != nullptr );
    return m_anyCompare( m_any, any.m_any );

    //    const auto& anyType = m_any.type();

    //    if ( anyType == typeid( int ) ) { return get<int>() == any.get<int>(); }
    //    else if ( anyType == typeid( double ) ) { return get<double>() == any.get<double>(); }
    //    else if ( anyType == typeid( std::string ) ) {
    //        return get<std::string>() == any.get<std::string>();
    //    }
    //    else if ( anyType == typeid( const char* ) ) {
    //        return !strcmp( get<const char*>(), any.get<const char*>() );
    //    }
    //    else {
    //        //        assert(false);
    //        return false;
    //    }

    //        switch ( m_type ) {
    //        case Any::Type::INT: {
    //            return get<int>() == any.get<int>();
    ////            return m_int == any.m_int;
    //        } break;

    //        case Any::Type::DOUBLE: {
    //            return get<double>() == any.get<double>();
    ////            return m_double == any.m_double;
    //        } break;

    //        case Any::Type::STRING: {
    //            return get<std::string>() == any.get<std::string>();
    ////            return m_std_string == any.m_std_string;
    //        } break;

    //        case Any::Type::CONST_CHAR_PTR: {
    ////            return get<const char *>() == any.get<const char *>();
    //            return !strcmp( get<const char*>(), any.get<const char*>() );
    ////            return !strcmp( m_const_char_ptr, any.m_const_char_ptr );
    //        } break;

    //            // todo any
    ////        case Any::Type::MAT4: {
    ////            return get<data::Mat4>() == any.get<data::Mat4>();
    //////            return *m_mat4 == *any.m_mat4;
    ////        } break;

    ////        case Any::Type::MESH: {
    ////            return get<data::Mesh>() == any.get<data::Mesh>();
    //////            return *m_mesh == *any.m_mesh;
    ////        } break;

    //        default:
    //            assert( false );
    //        }
    ////    }

    return false;
}

std::ostream& operator<<( std::ostream& os, const Any& any ) {
    os << any.to_string();
    return os;
}

} // namespace hub