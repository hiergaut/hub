

#include <cassert>
#include <sstream>

#include <sensor/Acquisition.hpp>

#define HUB_CPP_SOURCE
#include "native_Acquisition.hpp"

namespace hub {
namespace native {

void freeAcquisition( sensor::Acquisition* acquisition ) {
    assert( acquisition != nullptr );
    std::cout << "[Native] freeAcquisition( " << acquisition << ")" << std::endl;
    delete acquisition;
}

void acquisition_getMeasure( const sensor::Acquisition* acquisition,
                             unsigned char* data,
                             int iMeasure ) {
    assert( acquisition != nullptr );

    const auto& nodes = acquisition->getNodes();
    assert( iMeasure < nodes.size() - 2 );
    const auto& node = nodes.at( 2 + iMeasure );
    // const auto& measure = acquisition->getMeasures().at( iMeasure );
    const auto* acqData = acquisition->getData( 2 + iMeasure );
    memcpy( data, acqData, node.getSize() );
}

HUB_CLOCK * acquisition_getStart( HUB_ACQUISITION* acquisition ) {
    return &acquisition->start();
}

HUB_CLOCK *acquisition_getEnd(HUB_ACQUISITION *acquisition)
{
    return &acquisition->end();
}

void acquisition_to_string( const sensor::Acquisition* acquisition, char* str, int* strLen ) {
    std::stringstream sstr;
    sstr << *acquisition;
    const std::string& stdString = sstr.str();

    *strLen = stdString.size();
#if CPLUSPLUS_VERSION == 20
    memcpy( sensorName, sensorSpec->getSensorName().data(), *strLen + 1 );
#else
    memcpy( str, stdString.c_str(), *strLen + 1 );
#endif
    str[*strLen] = 0;
}

HUB_DOF6 *acquisition_getDof6(HUB_ACQUISITION *acquisition)
{
    hub::sensor::Acquisition * acquisition2 = static_cast<hub::sensor::Acquisition*>(acquisition);
    auto & dof6 = acquisition2->get<hub::format::Dof6&>();
    return reinterpret_cast<HUB_DOF6*>(&dof6);
}


} // namespace native
} // namespace hub
