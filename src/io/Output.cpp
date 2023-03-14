#include "io/Output.hpp"

// #include <any>
// #include <cstring>
// #include <iostream>
// #include <typeindex>
// #include <typeinfo>

// #include "Acquisition.hpp"
// #include "SensorSpec.hpp"
#include "Any.hpp"

namespace hub {
namespace io {

void Output::write( const std::string& str ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(std::string) : '" << str << "'" << std::endl;
#endif

    int strLen = static_cast<int>( str.size() );
    write( strLen );

    if ( strLen > 0 ) {
        const char* data = str.data();
        write( reinterpret_cast<const unsigned char*>( data ), strLen );
    }
}

void Output::write( const SensorSpec& sensorSpec ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(SensorSpec)" << std::endl;
#endif

    write( sensorSpec.getSensorName() );
    write( sensorSpec.getResolutions() );
    write( sensorSpec.getMetaData() );
}

void Output::write( const data::Measure& measure ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Measure)" << std::endl;
#endif

    assert( measure.m_size != 0 );
    assert( measure.m_data != nullptr );

    write( measure.m_size );
    write( measure.m_data, measure.m_size );
}

// void Output::write( const data::UserData& userData ) {
//}

void Output::write( const Acquisition& acq ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(Acquisition)" << std::endl;
#endif

    write( acq.m_start );
    write( acq.m_end );
    const auto& measures = acq.getMeasures();
    assert( measures.size() > 0 );
    write( measures );
}

void Output::write( const char* str ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << "[Output] write(const char*)" << std::endl;
#endif

    int strLen = static_cast<int>( strlen( str ) );
    write( strLen );

    if ( strLen > 0 ) { write( reinterpret_cast<const unsigned char*>( str ), strLen ); }
}

// Output::~Output()
//{
// }

// void Output::write( const std::any& any )  {
void Output::write( const Any& any ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
#    ifdef WIN32
    std::cout << "[Output] write(std::any) : '" << any.type().raw_name() << "'" << std::endl;
#    else
    std::cout << "[Output] write(std::any) : '" << any << "'" << std::endl;
#    endif
#endif

    assert( any.has_value() );
    const auto& anyType = any.type();
    write( anyType );
    switch ( anyType ) {

    case Any::Type::INT: {
        const auto& val = any.getInt();
        write( val );
        break;
    }

    case Any::Type::DOUBLE: {
        const auto& val = any.getDouble();
        write( val );
        break;
    }

    case Any::Type::STRING: {
        const auto& val = any.getStdString();
        write( val );
        break;
    }

    case Any::Type::CONST_CHAR_PTR: {
        const auto& val = any.getConstCharPtr();
        write( val );
        break;
    }

    case Any::Type::MAT4: {
        const auto& val = any.getMat4();
        write( reinterpret_cast<const unsigned char*>( val.m_data ), 64 );
        break;
    }

    default:
        std::cerr << "non supported type : '" << any.type() << "'" << std::endl;
        assert( false );
    }
}

/////////////////////////////////////////////////////////////////////////////

} // namespace io
} // namespace hub