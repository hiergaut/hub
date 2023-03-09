#include "IO/Input.hpp"

//#include <any>
//#include <cstring>
//#include <iostream>
//#include <typeindex>
//#include <typeinfo>

//#include "Acquisition.hpp"
//#include "SensorSpec.hpp"
#include "AnyUtils.hpp"

namespace hub {
namespace io {


/////////////////////////////////////////////////////////////////////////////

void Input::read( std::string& str ) const {
    assert( isOpen() );

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str = ""; }
    else {
        const int buffSize = strLen + 1;
        unsigned char* tmp = new unsigned char[buffSize];
        read( tmp, strLen );
        tmp[strLen] = 0;
        str         = std::string( reinterpret_cast<char*>( tmp ) );
        delete[] tmp;
    }
#ifdef DEBUG_IOSTREAM
    std::cout << "[Input] read(std::string) : '" << str << "'" << std::endl;
#endif
}


SensorSpec Input::getSensorSpec() const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Input] getSensorSpec()" << std::endl;
#endif


    std::string sensorName;
    Resolutions resolutions;
    hub::SensorSpec::MetaData metaData;
    read( sensorName );
    read( resolutions );
    read( metaData );

    //        sensorSpec.m_acquisitionSize =
    //        SensorSpec::computeAcquisitionSize(sensorSpec.m_resolutions); return sensorSpec;
    return SensorSpec( std::move( sensorName ), std::move( resolutions ), std::move( metaData ) );
}


Acquisition Input::getAcquisition( const SensorSpec& sensorSpec ) const
//    Acquisition Input::getAcquisition(int acquisitionSize) const
{
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Input] getAcquisition()" << std::endl;
#endif

    long long start, end;

    read( start );
    read( end );

    Acquisition acq { start, end };

    int nMeasures;
    read( nMeasures );
    assert( nMeasures > 0 );

    const auto& resolutions = sensorSpec.getResolutions();
    assert( resolutions.size() == nMeasures );

    for ( int iMeasure = 0; iMeasure < nMeasures; ++iMeasure ) {
//        const auto & resolution = resolutions.at(iMeasure);
//        const auto & format = resolution.second;

//        if (format == Format::USER_DATA) {

//            std::cout << "[Input] getAcquisition: user_data" << std::endl;
////            std::string name;
////            read(name);
////            std::any value;
////            read(value);

////            UserData userData(name, value);
//            auto userData = get<UserData>();
//            acq.pushBack(std::move(userData));

//        }
//        else {

        //            acq << Input::getMeasure();
        uint64_t size; // compatibility 32/64 bits
        read( size );
        unsigned char* data = new unsigned char[size];
        read( data, size );

        //             Measure measure {data, size, resolutions.at(iMeasure)};

        //            acq.op(data, size, resolutions.at(iMeasure));
        //            acq << Measure(data, size, resolutions.at(iMeasure));
        //            acq << (data, size, resolutions.at(iMeasure));
        //            Acquisition::operator<<(acq, data, size, resolutions.at(iMeasure));
        //            acq << measure;

        acq.emplaceMeasure( data, size, resolutions.at( iMeasure ), true );
        //    acq << { data, size};
//        }
    }

    assert( ! acq.hasFixedSize() || sensorSpec.getAcquisitionSize() == acq.getSize() );
    //    assert( nMeasures == 0 || acquisitionSize == acq.getSize() );

    return acq;
}

void Input::read( char* str ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Input] read char*" << std::endl;
#endif

    int strLen = 0;
    read( strLen );

    if ( strLen == 0 ) { str[0] = 0; }
    else {
        read( reinterpret_cast<unsigned char*>( str ), strLen );

        str[strLen] = 0;
    }
}

void Input::read( std::any& any ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Input] read(std::any)" << std::endl;
#endif

    Any::Type type;
    read( type );

    switch ( type ) {
    case Any::Type::INT: {
        assert( sizeof( int ) == 4 );
        int val;
        read( val );
        any = std::any_cast<int>( val );
    } break;

    case Any::Type::DOUBLE: {
        double val;
        assert( sizeof( double ) == 8 );
        read( val );
        any = std::any_cast<double>( val );
    } break;

    case Any::Type::STRING: {
        std::string val;
        read( val );
        any = std::any_cast<std::string>( val );
    } break;

    case Any::Type::CONST_CHAR_PTR: {
        assert( sizeof( char ) == 1 );
        char* buff = new char[256];
        memset( buff, 0, 256 );
        read( buff );
        int len = static_cast<int>( strlen( buff ) );

        const int buffSize = len + 1;
        char* str          = new char[buffSize];
        memcpy( str, buff, len );
        str[len] = 0;
        delete[] buff;

        any = std::any_cast<const char*>( reinterpret_cast<const char*>( str ) );

    } break;

    case Any::Type::VECTOR_FLOAT: {
        std::vector<float> val;
        read( val );
        any = std::any_cast<std::vector<float>>( val );
    } break;

    case Any::Type::UINT: {
        unsigned int val;
        read( val );
        any = std::any_cast<unsigned int>( val );
    } break;

    case Any::Type::CONST_FLOAT_PTR: {
        float* buff = new float[16];
        read( reinterpret_cast<unsigned char*>( buff ), 64 );

        any = std::any_cast<const float*>( reinterpret_cast<const float*>( buff ) );

    } break;

    default:
        assert( false );
    }
    assert( any.has_value() );
}

/////////////////////////////////////////////////////////////////////////////



} // namespace io
} // namespace hub
