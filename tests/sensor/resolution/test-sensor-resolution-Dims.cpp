#include "test_common.hpp"

//#include <vector>

#include "core/test-core-common.hpp"

#include <sensor/resolution/Dims.hpp>

TEST_CASE( "sensor/Dims test" ) {

    InputOutput inputOutput;

    static_assert( !hub::serializable_v<hub::sensor::resolution::Dims> );
    static_assert( !hub::Output::writable_v<hub::sensor::resolution::Dims> );
    static_assert( !hub::Input::readable_v<hub::sensor::resolution::Dims> );

    //////////////////////////// Static Dims

    static_assert( (hub::Size_t)4294967295 == 0xffffffff );
    static_assert( (hub::Size_t)4294967296 == 0 );
    static_assert( (hub::Size_t)4294967297 == 1 );

    constexpr auto dims0D = hub::sensor::resolution::Dims {};
    static_assert( dims0D.size() == 0 );
//    static_assert( dims0D.ns[-1] == 0 ); // out of bound
    static_assert( dims0D.ns[0] == 0 );
    static_assert( dims0D.ns[1] == 0 );
    static_assert( dims0D.ns[2] == 0 );
    static_assert( dims0D.ns[3] == 0 );
    static_assert( dims0D.ns[4] == 0 );
//    static_assert( dims0D.ns[5] == 0 ); // out of bound
    static_assert( dims0D.capacity() == 1 ); // weird

    constexpr auto dims1D = hub::sensor::resolution::Dims { 640 };
    static_assert( dims1D.size() == 1 );
    static_assert( dims1D.ns[0] == 640 );
    static_assert( dims1D.ns[1] == 0 );
    static_assert( dims1D.ns[2] == 0 );
    static_assert( dims1D.ns[3] == 0 );
    static_assert( dims1D.ns[4] == 0 );
    static_assert( dims1D.capacity() == 640 );

//    hub::sensor::resolution::Dims1D dims1D2{dims1D};

    constexpr auto dims2D = hub::sensor::resolution::Dims { 640, 480 };
    static_assert( dims2D.size() == 2 );
    static_assert( dims2D.ns[0] == 640 );
    static_assert( dims2D.ns[1] == 480 );
    static_assert( dims2D.ns[2] == 0 );
    static_assert( dims2D.ns[3] == 0 );
    static_assert( dims2D.ns[4] == 0 );
    static_assert( dims2D.capacity() == 640 * 480 );

    constexpr auto dims3D = hub::sensor::resolution::Dims { 640, 480, 1 };
    static_assert( dims3D.size() == 3 );
    static_assert( dims3D.ns[0] == 640 );
    static_assert( dims3D.ns[1] == 480 );
    static_assert( dims3D.ns[2] == 1 );
    static_assert( dims3D.ns[3] == 0 );
    static_assert( dims3D.ns[4] == 0 );
    static_assert( dims3D.capacity() == 640 * 480 );

    inputOutput.write( dims1D );
    hub::sensor::resolution::Dims dims1D_read;
    inputOutput.read(dims1D_read);
//    const auto dims1D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    CHECK( dims1D == dims1D_read );

    inputOutput.write( dims2D );
    const auto dims2D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    CHECK( dims2D == dims2D_read );

    inputOutput.write( dims3D );
    const auto dims3D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    CHECK( dims3D == dims3D_read );

    //////////////////////////// Template Dims

    using DimsT1D = hub::sensor::resolution::DimsT<5>;
    static_assert( !hub::serializable_v<DimsT1D> );
    static_assert( hub::Output::writable_v<DimsT1D> );
    static_assert( !hub::Input::readable_v<DimsT1D> );
    static_assert( hub::Input::notReadable_v<DimsT1D> );
    static_assert( DimsT1D::size() == 1 );
    static_assert( DimsT1D::n() == 5 );
//    static_assert( DimsT1D::get<-1>() == 5 ); // out of bound
//    static_assert( DimsT1D::get<0>() == 5 );
//    static_assert( DimsT1D::get<1>() == 5 ); // out of bound
    static_assert( DimsT1D::capacity() == 5 );
    inputOutput.write(DimsT1D());
    const auto dimsT1D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    assert(DimsT1D() == dimsT1D_read);

    using DimsT2D = hub::sensor::resolution::DimsT<5, 3>;
    static_assert( !hub::serializable_v<DimsT2D> );
    static_assert( hub::Output::writable_v<DimsT2D> );
    static_assert( !hub::Input::readable_v<DimsT2D> );
    static_assert( hub::Input::notReadable_v<DimsT2D> );
    static_assert( DimsT2D::size() == 2 );
//    static_assert( DimsT2D::n() == 5 ); // not found
    static_assert( DimsT2D::width() == 5 );
    static_assert( DimsT2D::height() == 3 );
//    static_assert( DimsT2D::get<0>() == 5 );
//    static_assert( DimsT2D::get<1>() == 3 );
//    static_assert( DimsT2D::get<2>() == 3 );
    static_assert( DimsT2D::capacity() == 5 * 3 );
    inputOutput.write(DimsT2D());
    const auto dimsT2D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    assert(DimsT2D() == dimsT2D_read);

    using DimsT3D = hub::sensor::resolution::DimsT<5, 3, 2>;
    static_assert( !hub::serializable_v<DimsT3D> );
    static_assert( hub::Output::writable_v<DimsT3D> );
    static_assert( !hub::Input::readable_v<DimsT3D> );
    static_assert( hub::Input::notReadable_v<DimsT3D> );
    static_assert( DimsT3D::size() == 3 );
//    static_assert( DimsT3D::n() == 5 ); // not found
//    static_assert( DimsT3D::width() == 5 );
//    static_assert( DimsT3D::height() == 3 );
    static_assert( DimsT3D::nx() == 5 );
    static_assert( DimsT3D::ny() == 3 );
    static_assert( DimsT3D::nz() == 2 );
//    static_assert( DimsT3D::get<0>() == 5 );
//    static_assert( DimsT3D::get<1>() == 3 );
//    static_assert( DimsT3D::get<2>() == 2 );
//    static_assert( DimsT3D::get<2>() == 3 );
    static_assert( DimsT3D::capacity() == 5 * 3 * 2 );
    inputOutput.write(DimsT3D());
    const auto dimsT3D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    assert(DimsT3D() == dimsT3D_read);

    using DimsT4D = hub::sensor::resolution::DimsT<5, 3, 2, 1>;
    static_assert( !hub::serializable_v<DimsT4D> );
    static_assert( hub::Output::writable_v<DimsT4D> );
    static_assert( !hub::Input::readable_v<DimsT4D> );
    static_assert( hub::Input::notReadable_v<DimsT4D> );
    static_assert( DimsT4D::size() == 4 );
//    static_assert( DimsT4D::n() == 5 ); // not found
//    static_assert( DimsT4D::width() == 5 );
//    static_assert( DimsT4D::height() == 3 );
//    static_assert( DimsT4D::nx() == 5 );
//    static_assert( DimsT4D::ny() == 3 );
//    static_assert( DimsT4D::nz() == 2 );
    static_assert( DimsT4D::n<0>() == 5 );
    static_assert( DimsT4D::n<1>() == 3 );
    static_assert( DimsT4D::n<2>() == 2 );
    static_assert( DimsT4D::n<3>() == 1 );
//    static_assert( DimsT4D::n<4>() == 1 );
//    static_assert( DimsT4D::get<2>() == 3 );
    static_assert( DimsT4D::capacity() == 5 * 3 * 2 * 1 );
    inputOutput.write(DimsT4D());
    const auto dimsT4D_read = inputOutput.get<hub::sensor::resolution::Dims>();
    assert(DimsT4D() == dimsT4D_read);


//    DimsT1D dimsT1D;
//    inputOutput.write(DimsT1D());
//    const auto dimsT1D2_read = inputOutput.get<DimsT1D>();
//    inputOutput.read(dimsT4D()); // not readable

//    DimsT dimsT;
//    static_assert( dimsT.get<0>() == 640 );

//    //        inputOutput.write(dimsT);

//    //        DimsT dimsT;
//    //        inputOutput.read(dimsT);
//    const auto dimsT_read = inputOutput.get<hub::sensor::resolution::Dims>();
//    assert( dimsT == dimsT_read );

//    return;

//    hub::sensor::resolution::Dims dims_read2;
//    inputOutput.read( dims_read2 );

//    using DimsT1 = hub::sensor::DimsT<5>;
//    static_assert( DimsT1::n() == 5 );

//    inputOutput.write( DimsT1() );

//    using DimsT2 = hub::sensor::DimsT<640, 480>;
//    static_assert( DimsT2::width() == 640 );

//    inputOutput.write( DimsT2() );

//    using DimsT3 = hub::sensor::DimsT<640, 480, 1>;
//    static_assert( DimsT3::nx() == 640 );

//    inputOutput.write( DimsT3() );

    //        if (dims_read2.n() == 1) {
    //            dims_read.ns
    //            using DimT_read = hub::sensor::DimsT<dims_read2.ns[0]>();

    //        }

    //        const auto dims_read2 = inputOutput.g
}
