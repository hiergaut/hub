#include "test_common.hpp"
// #include "sensor/test_sensor_utils.hpp"

#include <type_traits>
#include <vector>

#include "core/test_core_common.hpp"
#include "sensor/test_sensor_common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/Measure.hpp>
#include <sensor/Measures.hpp>
#include <sensor/OutputSensor.hpp>
#include <sensor/Resolution.hpp>
#include <sensor/Resolutions.hpp>

template <int... Ns>
class Dim
{
  public:
    static struct {
    } isDim;
    static constexpr auto Size     = sizeof...( Ns );
    static constexpr auto Capacity = ( Ns * ... );
    //    typedef int value_type;
    //    static constexpr size_t size() noexcept { return sizeof...( Ns ); }
};

template <class... Ts>
class Types {
  public:
};


//template <int... Ns>
////using Dim = return template <class... Ts>;
//constexpr auto Fuck() {
//    return Types;
//}


//#define Teste\< args... \> void

template <class Dim, class... Types>
class MatPrivate
{
  public:
//    using type = MatXD<Dim, El...>;
//    typedef MatXD<Dim, El...> type;
};

// template <int N, class... Types>
// class Mat1D : public Mat<Dim<N>, Types...>
//{
//   public:
// };

// template <class... El>
//class Mat


//template <int N, class... El>
//class MatXD<Dim<N>, El...> {
//         public:
//};

template <class... Types>
using Merge = MatPrivate<Dim<1>, Types...>;

template <int N, class... Types>
using Mat1D = MatPrivate<Dim<N>, Types...>;

template <int N, int N2, class... Types>
using Mat2D = MatPrivate<Dim<N, N2>, Types...>;

template <int N, int N2, int N3, class... Types>
using Mat3D = MatPrivate<Dim<N, N2, N3>, Types...>;

template <int N, int N2, int N3, int N4, class... Types>
using Mat4D = MatPrivate<Dim<N, N2, N3, N4>, Types...>;



//template <class Type, int... Ns>
//using Mat = MatXD<Dim<Ns...>, Type>;
//using Mat = MatXD<Dim<Ns...>, Type>;

template <class Type, int... Ns>
class Mat : public MatPrivate<Dim<Ns...>, Type> {
  public:
};

template <class Type>
class Mat<Type, 1> : public MatPrivate<Dim<1>, Type> {
  public:
//    static struct {} is1D;
    void length();
//    void length();
};


template <class Type, int N>
class Mat<Type, N> : public MatPrivate<Dim<N>, Type> {
  public:
//    static struct {} is1D;
    void length();
//    void length();
};

template <class Type, int N, int N2>
class Mat<Type, N, N2> : public MatPrivate<Dim<N, N2>, Type> {
  public:
    void width();
//    static struct {} is2D;
//    void length();
};

template <class Type, int N, int N2, int N3>
class Mat<Type, N, N2, N3> : public MatPrivate<Dim<N, N2, N3>, Type> {
  public:
    void depth();
//    static struct {} is3D;
//    void length();
};


//template <class Type, int N, int N2>
//class Mat<Type, N, N2> {
//  public:
//};

//template <int N, class... El>
//constexpr auto Mat() {
//    return MatXD<Dim<N>, El...>();
//}

//template <int N, int N2, class... El>
//constexpr auto Mat() {
//    return MatXD<Dim<N, N2>, El...>();
//}

//template <int N, int N2, int N3, class... El>
//constexpr auto Mat() {
//    return MatXD<Dim<N, N2, N3>, El...>();
//}

//template <int N, int N2, int N3, int N4, class... El>
//constexpr auto Mat() {
//    return MatXD<Dim<N, N2, N3, N4>, El...>();
//}

// class Mat2D
//{
//   public:
// };

// template <int N, int N2, class... Ts>
// class Matrix2D
//{
//   public:
//     //    using type = Matrix1;
// };

// template <int N, int N2, int N3, class... Ts>
// class Matrix3D
//{
//   public:
//     //    using type = Matrix1;
// };

// template <class T, class T2, class T3, hub::Size_t... Dims>
// class Matrix3 {
//   public:
// };

// template <int N, class... Ts>
// using Matrix = Matrix1D<N, Ts...>;

// template <int N, int N2, class... Ts>
// using Matrix = Matrix2D<N, N2, Ts...>;

// template <class T, hub::Size_t... Dims>
// using Matrix = Matrix1<T, Dims...>;

// template <class T, class T2, hub::Size_t... Dims>
// using Matrix = Matrix2<T, T2>;

// template <class... Args>
// using Matrix = Matrix1<Args...>;

// template <class T, hub::Size_t... Dims>
// constexpr auto Matrix() { return typename Matrix1<T, Dims...>::type(); };
// using Matrix = typename Matrix1<T, Dims...>::type;

// template <class T, class T2, hub::Size_t... Dims>
// using Matrix = typename Matrix2<T, T2, Dims...>::type;

// template <class... Args>
// constexpr auto makeMatrix(Args...) {
//     return Matrix<int, 4, 4>();
// }

// #include <io/Memory.hpp>

#include <tuple>

//class Integer {
//  public:
//    Integer(int value) {

//    }
//};

TEST_CASE( "InputSensor test" ) {
    //    Dim<1> dim;

//    using trash = Trash<int, typeof(5)>;
//    Test<int, bool>;
//    Test<int, 5>;
//    Test<640, 480>;

//    using MyTypes = Types<int, int>;

//    using test = Teste<640, 480, int>;

//    Types<Dim<1, 2>>;

//    constexpr auto myMat = Mat<640, 480, int>();
//    using MyMat = Mat<640, 480, int>()::type;
//    std::make_tuple(5, 4, int);
//    using MyMat = Mat2D<640, 480, int>;

    using R = unsigned char;
    using G = unsigned char;
    using B = unsigned char;


    using MyMat2 = Mat<int, 640, 480, 3>;
    using MyMat3 = Mat<int, 640>;
    using MyMat4 = Mat2D<640, 480, R, G, B>;
//    using MyMat5 = MatXMatXD<MyMat4, 5>;
//    using MyMat6 = Mat<Mat<int, double>, 5>;
//    using MyMat7 = Mat1D<5, int, double>;
    using MyMat8 = Mat1D<5, Mat<int, 640, 480>, Mat<double, 10>>;
    using MyMat9 = Merge<Mat1D<5, Mat<int, 640, 480>, Mat<double, 10>>>;
//    using MyMat9 = Mat<Merge<Mat<int, 640, 480>, Mat<double, 10>>, 10>;

//       MatXD<5, int>;
//    constexpr auto myMat = Mat<640, 480, int>;
//    using MyMat = Mat<640, 480, int>;
//    myMat::type;
//    using MatType = myMat::


    //    MatrixXD<std::integer_sequence<int, 1>, int>;
    //    using MyMatrix = Matrix<1, int>;
    //    MatrixXD<1, int>;
    //    using sequence = std::integer_sequence<int, 640, 480>;
    //    using MyMatrix = Matrix<double, Dim<640, 480>>;
    //      using MyMatrix = Matrix<double, 640, 480>;
    //    using MyMatrix2 = Matrix<int, double>;
    //    using MyMatrix = Matrix<int, 640, 480>;
    //    using MyMatrix2 = Matrix<double, 10>;
    //    using MyMatrix3 = Matrix<MyMatrix, MyMatrix2>;

        using namespace hub::sensor;
        using Resolution1 = ResolutionT<int, 640, 480>;
    //    using R = unsigned char;
    //    using G = unsigned char;
    //    using B = unsigned char;

//        using Resolution5 = ResolutionT<FormatT<R, G, B>, 640, 480>;
//        static_assert(Resolution5::nByte() == 640 * 480 * 3);
    //    using MyMatrix = Matrix<int, 640, 480>;
    //    using MyMatrix2 = Matrix<double, 10>;

    //    using MyMatrixRGB = Matrix<R, G, B, 640, 480>;

    ////    using MyMatrix12 = Matrix<

    ////    using MyMatrix2 = makeMatrix

//    return;

    ////////////////////////// ResolutionT //////////////////////////////

    using Resolution = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 640, 480>;
    static_assert( Resolution::nResolution == 1 );
    static_assert( Resolution::nByte() == 640 * 480 * 3 );
    using Resolution2 = hub::sensor::ResolutionT<hub::sensor::format::Mat4>;
    static_assert( Resolution2::nResolution == 1 );
    std::cout << "Resolution: " << Resolution() << std::endl;
    std::cout << "Resolution2: " << Resolution2() << std::endl;

//    using Resolutions = hub::sensor::Resolution1DT<20, Resolution, Resolution2>;
//    std::cout << "Resolutions: " << Resolutions() << std::endl;
//    static_assert( Resolutions::nResolution == 1 );
//    static_assert( std::is_same_v<Resolutions::IResolution<0>, Resolution> );
//    static_assert( std::is_same_v<Resolutions::IResolution<1>, Resolution2> );
    //    Resolutions::

    //    using ResolutionsPack = hub::sensor::ResolutionsPackT<Resolutions, 2>;
    //    std::cout << "ResolutionsPack: " << ResolutionsPack() << std::endl;
    //    static_assert(Resolutions::nByte() == 640 * 480 * 3 + 64);
    //    static_assert(ResolutionsPack::nByte() == (640 * 480 * 3 + 64) * 2);

//    using ResolutionPack = hub::sensor::ResolutionT<Resolutions, 2>;
//    std::cout << "ResolutionPack: " << ResolutionPack() << std::endl;
//    static_assert( ResolutionPack::nByte() == ( 640 * 480 * 3 + 64 ) * 2 );

//    using ResolutionPack2 = ResolutionT<FormatT<ResolutionT<double, 640, 480>, ResolutionT<float, 5>>, 100>;
//    using ResolutionPack3 = Resolution1DT<100, ResolutionT<double, 640, 480>, ResolutionT<float, 5>>;
//    using ResolutionPack3 = Resolution1DT<ResolutionPack>;


    return;
//    using ResolutionPack3 = Res1DT<100, ResT<double, 640, 480>, ResT<float, 5>>;
//    using ResolutionPack4 = nd<100, nd<double, 640, 480>, nd<float, 5>>;

//    ResolutionTI<Dims<5>, int> fuck;

    //    auto resolution = Resolutions::get<0>();

    //    static_assert( Resolution::nResolution() == 1 );
    //    auto get = Resolution::get<0>();
    //    static_assert(Resolution::get<0>() == Resolution());
    //    static_assert(Resolution::getResolutionT<0>() == Resolution());

    ////////////////////////// MeasureT //////////////////////////////

    constexpr auto dataSize = 640 * 480 * 3;

    //    constexpr auto dataSize = 640 * 480 * 3 * 2;
    unsigned char datas[640 * 480 * 3 * 2] { 0 };
    //    std::span<hub::Data_t> datasSpan(datas, datas + dataSize);

    //    hub::sensor::MeasureT<Resolution> measureT(datas);
    //    auto measureT = hub::sensor::MeasureT<Resolution>(datas);
    //    auto buffer = hub::Buffer(datas);
    //    auto buffer = hub::Buffer<dataSize>(datas);
    //    static_assert(buffer.size == dataSize);
//    auto datasBuff  = hub::Buffer( datas, dataSize );
//    auto datasBuff2 = hub::Buffer( (const hub::Data_t*)datas, dataSize );
//    assert( datasBuff == datasBuff2 );

    //    return;
    //    datasBuff.data = nullptr;
    //    datasBuff.data[0] = 2;
//    assert( datasBuff == datasBuff2 );

//    std::cout << "datasBuff: " << datasBuff << std::endl;
//    hub::sensor::MeasureT<Resolution> measureT( datasBuff );
    //    hub::sensor::MeasureT<Resolution> measureT{datas, dataSize};
    //    static_assert( measureT.getResolution() == Resolution() );

//    unsigned char data[dataSize] { 0 };
//    unsigned char* data2 = new unsigned char[dataSize];
//    auto buffer          = hub::Buffer( data, dataSize );
//    auto buffer2         = hub::Buffer( data2, dataSize );
//    assert( buffer == buffer2 );
//    //    assert(buffer == datasBuff);

//    //    std::array<unsigned char, dataSize> array;
//    //    decltype(array)::value_type;
//    //    auto type = typeid(std::span<int>::value_type()).hash_code();
//    //    measureT.setData(array);

//    std::memset( data2, 55, dataSize );
//    //    measureT.setData(5, nullptr);
//    //    measureT.setData<data, dataSize>();
//    //    hub::Buffer buffer2(data2, dataSize);
//    measureT.setData( buffer2 );
//    //    measureT.setData(data, dataSize);
//    assert( std::memcmp( datas, data2, dataSize ) == 0 );
//    assert( datasBuff == buffer2 );

    //    std::span span(data);
    //    measureT.setData(span);

//    std::memset( data, 55, dataSize );
    //    std::span span(data);
    //    std::span<hub::Data_t> span2(data2, dataSize);
    //    decltype(span)::value_type;

    //    hub::sensor::MeasuresT<Resolution, Resolution> measuresT(buffer);
//    hub::sensor::MeasureT<Resolution> measure( buffer );

//    hub::sensor::MeasuresT<Resolution> measuresT( buffer );
//    static_assert( measuresT.nResolution == 1 );

    //    hub::sensor::MeasureT<Resolution>(buffer, buffer);

    //    auto & measure1 = measuresT.get<0>();
    //    auto & measure2 = measuresT.get<1>();
    //    auto & measure2 = measuresT.get<1>();

    //    measureT.setData(span);
    //    assert(std::memcmp(datas, data, dataSize) == 0);

    //    measureT.setData(span2);
    //    std::span<unsigned char> span2(data2, data2 + dataSize);

    //    hub::sensor::MeasuresT<Resolution> measureT(datasBuff);

    ////////////////////////// OutputSensorT //////////////////////////////

    return;

//    using OutputSensor = hub::sensor::OutputSensorT<Resolution>;
    //    using Acquisition = OutputSensor::Acquisition;
//    OutputSensor outputSensor;
//    OutputSensor::Acquisition acq;

    //    auto& measure = acq.getMeasure<0>();
    //    measure.setData( nullptr, 10 );
    //    auto & measure2 = acq.getMeasure<1>();

    //    hub::sensor::
}

//    const int ref_offset    = 5;
//    constexpr int ref_nAcqs = 10;

//    const int ref2_offset    = 0;
//    constexpr int ref_nAcqs2 = 10;

//    std::cout << "ref_acqs" << std::endl;
//    const hub::sensor::Resolution ref_resolution { hub::sensor::format::Y8, 1 };
//    const hub::sensor::Resolutions ref_resolutions { ref_resolution };
//    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", ref_resolutions );
//    std::vector<hub::sensor::Acquisition> ref_acqs;
//    //    const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution
//    //    );
//    const int ref_dataSize = ref_resolution.nByte();
//    unsigned char* data    = new unsigned char[ref_dataSize];
//    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
//        for ( int i = 0; i < ref_dataSize; ++i ) {
//            data[i] = ref_offset + iAcq + 1;
//        }
//        //        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 1 );
//        ref_acqs.emplace_back( ref_resolutions );
//        auto& acq = ref_acqs.back();
//        acq.setStart( ref_offset + iAcq + 1 );
//        acq.setEnd( ref_offset + iAcq + 1 );
//        acq.getMeasures().at( 0 ).setData( data, ref_dataSize );
//        //        ref_acqs.back() << hub::Measure(
//        //            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution
//        //            );
//        std::cout << acq << std::endl;
//    }
//    delete[] data;

//    std::cout << std::endl;

//    //////////////////////

//    std::cout << "ref2_acqs" << std::endl;
//    const hub::sensor::Resolution ref_resolution2 { hub::sensor::format::DOF6, 1 };
//    const hub::sensor::Resolutions ref_resolutions2 { ref_resolution2 };
//    const hub::sensor::SensorSpec ref_sensorSpec2 { "sensorName2", ref_resolutions2 };
//    std::vector<hub::sensor::Acquisition> ref_acqs2;
//    //    const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize(
//    ref_resolution2
//    //    );
//    const int ref_dataSize2 = ref_resolution2.nByte();
//    unsigned char* data2    = new unsigned char[ref_dataSize2];
//    //    float* data2Float       = (float*)data2;
//    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
//        for ( int i = 0; i < ref_dataSize2; ++i ) {
//            data2[i] = ref2_offset + iAcq + 1;
//            //            data2Float[i] = ref2_offset + iAcq + 1;
//        }
//        //        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
//        ref_acqs2.emplace_back( ref_resolutions2 );
//        auto& acq = ref_acqs2.back();
//        acq.setStart( ref2_offset + iAcq + 1 );
//        acq.setEnd( ref2_offset + iAcq + 1 );
//        acq.getMeasures().at( 0 ).setData( data2, ref_dataSize2 );
//        //        ref_acqs2.back() << hub::Measure(
//        //            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2,
//        //            ref_resolution2 );
//        std::cout << acq << std::endl;
//    }
//    delete[] data2;

//    std::cout << std::endl;

//    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    static_assert( !hub::Input::readable_v<hub::sensor::Acquisition> );
//    static_assert( !hub::Input::readable_v<hub::sensor::Format> );
//    static_assert( !hub::Input::readable_v<hub::sensor::Measure> );
//    static_assert( !hub::Input::readable_v<hub::sensor::Measures> );
//    static_assert( !hub::Input::readable_v<hub::sensor::Resolution> );
//    static_assert( !hub::Input::readable_v<hub::sensor::Resolutions> );
//    static_assert( !hub::Input::readable_v<hub::sensor::SensorSpec> );

//    static_assert( !hub::Input::getable_v<hub::sensor::Acquisition> );
//    static_assert( !hub::Input::getable_v<hub::sensor::Format> );
//    static_assert( !hub::Input::getable_v<hub::sensor::Measure> );
//    static_assert( !hub::Input::getable_v<hub::sensor::Measures> );
//    static_assert( !hub::Input::getable_v<hub::sensor::Resolution> );
//    static_assert( !hub::Input::getable_v<hub::sensor::Resolutions> );
//    static_assert( !hub::Input::getable_v<hub::sensor::SensorSpec> );

//    static_assert( !hub::serializable_v<hub::sensor::Acquisition> );
//    static_assert( !hub::serializable_v<hub::sensor::Format> );
//    static_assert( !hub::serializable_v<hub::sensor::Measure> );
//    static_assert( !hub::serializable_v<hub::sensor::Measures> );
//    static_assert( hub::serializable_v<hub::sensor::Resolution> );
//    static_assert( !hub::serializable_v<hub::sensor::Resolutions> );
//    static_assert( !hub::serializable_v<hub::sensor::SensorSpec> );

//    //    static_assert(hub::Input::getable_v<hub::sensor::Resolution>);
//    //    static_assert(! isInput_v<hub::sensor::Resolution>);
//    //    static_assert(isInput_v<hub::Input>);
//    //    static_assert(isInput_v<InputOutput>);

//    {
//        InputOutput inputOutput;

//        int ref_int = 5;
//        inputOutput.write( ref_int );
////        inputOutput.write( ref_int );
//        int read_int;
//        inputOutput.read( read_int );
//        assert( read_int == ref_int );
////        const auto read_int2 = inputOutput.get<int>();
////        assert( read_int2 == ref_int );

////        inputOutput.write( 4.0 );
////        auto d = inputOutput.get<double>();
////        assert( d == 4.0 );

//        constexpr hub::sensor::Format format = hub::sensor::format::RGB8;
//        assert( format == hub::sensor::format::RGB8 );
//        inputOutput.write( format );
//        std::cout << "start reading" << std::endl;
//        hub::sensor::Format format_read;
//        inputOutput.read( format_read );
//        assert( format == format_read );

////        inputOutput.write( format );
////        auto format_read2 = inputOutput.get<hub::sensor::Format>();
////        assert( format == format_read2 );

////        constexpr auto dims2 = std::vector<int>{1, 2, 3};
////        const hub::sensor::Resolution::NDim& nDim = { 1, 2, 3 };
////        inputOutput.write( nDim );
////        std::cout << "start reading" << std::endl;
////        hub::sensor::Resolution::NDim dims_read;
////        inputOutput.read( dims_read );
////        assert( nDim == dims_read );

////        inputOutput.write( nDim );
////        auto dims_read2 = inputOutput.get<hub::sensor::Resolution::NDim>();
////        assert( nDim == dims_read2 );

////        static_assert(hub::Input::serializable_v<hub::sensor::Resolution>);
//        static_assert(! hub::serializable_v<hub::sensor::Resolution>);
//        static_assert(! hub::Output::writable_v<hub::sensor::Resolution>);
//        static_assert(! hub::Input::readable_v<hub::sensor::Resolution>);

//        constexpr auto dims0D = hub::sensor::resolution::NDim{};
//        static_assert(dims0D.n() == 0);
//        static_assert(dims0D.capacity() == 1);
//        static_assert(dims0D.ns[0] == 0);

//        constexpr auto dims1D = hub::sensor::resolution::NDim{640};
////        static_assert(dims1D.n() == 640);
////        dims1D.width();
////        dims1D.height();
////        dims1D.nx();
////        static_assert(dims1D.height() == 480);
//        static_assert(dims1D.n() == 1);
////        static_assert(dims1D.get<0>() == 640);
////        static_assert(dims1D.get(1) == 480);
////        static_assert(dims1D.n() == 640);
//        static_assert(dims1D.capacity() == 640);
////        dims1D.width();

//        constexpr auto dims2D = hub::sensor::resolution::NDim{640, 480};
////        static_assert(dims2D.width() == 640);
////        static_assert(dims2D.height() == 480);
//        static_assert(dims2D.n() == 2);
//        static_assert(dims2D.ns[0] == 640);
////        static_assert(dims2D.get<1>() == 480);
////        static_assert(dims2D.get<2>() == 0);
////        static_assert(dims2D.get<3>() == 0);
////        static_assert(dims2D.get<4>() == 0);
////        static_assert(dims2D.width() == 640);
////        static_assert(dims2D.height() == 480);
//        static_assert(dims2D.capacity() == 640 * 480);
////        dims2D.width();

//        static_assert((hub::Size_t)4294967295 == 0xffffffff);
//        static_assert((hub::Size_t)4294967296 == 0);
//        static_assert((hub::Size_t)4294967297 == 1);
//        constexpr auto dims3D = hub::sensor::resolution::NDim{640, 480, 1};
////        static_assert(dims2D.width() == 640);
////        static_assert(dims2D.height() == 480);
////        static_assert(dims3D.size() == 3);
////        static_assert(dims3D.get<0>() == 640);
//        static_assert(dims3D.ns[1] == 480);
////        static_assert(dims3D.get<1>() == 480);
////        static_assert(dims3D.get<2>() == 1);
////        static_assert(dims3D.get<3>() == 0);
////        static_assert(dims3D.get<4>() == 0);
////        static_assert(dims3D.get<5>() == 0);
////        static_assert(dims3D.nx() == 640);
////        static_assert(dims3D.ny() == 480);
////        static_assert(dims3D.nz() == 1);
////        static_assert(dims3D.capacity() == 640 * 480);
////        dims3D.width();

//        std::cout << "nDim:" << std::endl;

//        inputOutput.write(dims3D);
////        hub::sensor::resolution::NDim dims_read;
////        inputOutput.read(dims_read);
//        const auto dims_read = inputOutput.get<hub::sensor::resolution::NDim>();
//        assert(dims3D == dims_read);
////        static_assert(dims_read == dims3D);

////        hub::sensor::DimsT<640, 480, 1> dimsT;
//        using DimsT = hub::sensor::DimsT<640, 480, 1>;
//        hub::sensor::write(DimsT(), inputOutput);
////        inputOutput.write(DimsT());

//        DimsT dimsT;
//        static_assert(dimsT.get<0>() == 640);

////        inputOutput.write(dimsT);

////        DimsT dimsT;
////        inputOutput.read(dimsT);
//        const auto dimsT_read = inputOutput.get<hub::sensor::resolution::NDim>();
//        assert(dimsT == dimsT_read);

//        return;

//        hub::sensor::resolution::NDim dims_read2;
//        inputOutput.read(dims_read2);

//        using DimsT1 = hub::sensor::DimsT<5>;
//        static_assert(DimsT1::n() == 5);

//        inputOutput.write(DimsT1());

//        using DimsT2 = hub::sensor::DimsT<640, 480>;
//        static_assert(DimsT2::width() == 640);

//        inputOutput.write(DimsT2());

//        using DimsT3 = hub::sensor::DimsT<640, 480, 1>;
//        static_assert(DimsT3::nx() == 640);

//        inputOutput.write(DimsT3());

////        if (dims_read2.n() == 1) {
////            dims_read.ns
////            using DimT_read = hub::sensor::DimsT<dims_read2.ns[0]>();

////        }

////        const auto dims_read2 = inputOutput.g

//        return;

//        constexpr hub::sensor::Resolution2 resolution2{hub::sensor::format::RGB8,
//        hub::sensor::nDim::Full_HD}; std::cout << resolution2 << std::endl;

////        constexpr hub::sensor::Resolution2 resolution3{{3, "RGB", true}, {640, 480}};
//        constexpr hub::sensor::Resolution2 resolution3 = hub::sensor::resolution::RGB8_Full_HD;
//        std::cout << resolution3 << std::endl;

////        dims_read.

////        using DimsT1 = hub::sensor::DimsT<640, 480, 1>;
////        static_assert(DimsT1::size() == 3);

////        constexpr auto dimsT1 = dims1D.getDimsT();

////            make_dimsT(dims1D);
////        constexpr auto dimsT1D = make_dimsT(dims1D);
////        constexpr auto dimsT2D = (dims1D.size() == 1) ?(1) :(2);
////        hub::sensor::DimsT<1>();
////        hub::sensor::DimsT<1, 2>();
////        constexpr auto dimsT3D = (dims1D.size() == 1) ?(hub::sensor::DimsT<1>())
///:(hub::sensor::DimsT<1, 2>());

////        } else if (dims1D.size() == 2) {
////        }
////        static_assert(dimsT1D.size() == 1);

////        constexpr std::tuple<int, int, int> tuple3{1, 2, 3};
////        static_assert(std::get<0, int>(tuple3) == 1);
////        static_assert(std::get<1, int>(tuple3) == 2);
////        static_assert(std::get<2, int>(tuple3) == 3);

////        InputOutputConstExpr<
////        constexpr InputExpr inputExpr{0, 1, 2, 3, 4, true, dims1D, dims2D, dims3D, 5};
////        static_assert(inputOutput.get<0, hub::sensor::resolution::NDim>() == dims3D);
////        static_assert(inputExpr.get<int>() == 0);
////        static_assert(inputExpr.get<bool>() == true);
////        static_assert(inputExpr.get<1>() == dims1D);
////        static_assert(inputExpr.get<2>() == dims2D);
////        static_assert(inputExpr.get<3>() == dims3D);

////        static_assert(inputExpr.get<0, hub::sensor::resolution::NDim>() == dims1D);
////        static_assert(inputExpr.get<1, hub::sensor::resolution::NDim>() == dims2D);
////        static_assert(inputExpr.get<2, hub::sensor::resolution::NDim>() == dims3D);

////        constexpr int i = 0;
////        for (constexpr i = 0; i < 5; ++i) {
////            constexpr auto get0 = inputExpr.get<i>();
////        }
////        for (const auto & x : tuple3) {

////        }
////        constexpr auto get0 = inputExpr.get<0>();
////        constexpr auto get1 = inputExpr.get<

//        return;

////        constexpr auto nDim = hub::sensor::resolution::NDim{640};
////        static_assert(nDim.size() == 1);
////        static_assert(nDim.get(1) == 480);
////        nDim.n();
////        nDim.width();
////        nDim.nx();

////        constexpr hub::sensor::ResolutionT res {format, 640, 480};
////        constexpr hub::sensor::ResolutionT<format, 1, 2, 3> resolution;

////        constexpr hub::sensor::ResolutionT resolution {format, 1, 2, 3};
////        static_assert(resolution.nDim.n() == 3);
////        static_assert(resolution.format == format);
////        std::cout << resolution.nByte() << std::endl;

////        static_assert(resolution.nDim. == 3);
////        hub::sensor::Resolution resolution { format, 1, 2, 3 };
////        inputOutput.write( resolution );
////        std::cout << "start reading" << std::endl;
////        hub::sensor::Resolution resolution_read;
////        hub::sensor::ResolutionT<format, 1, 2, 3> resolution_read;
////        inputOutput.read( resolution_read );

////        constexpr auto resolution_maked = hub::sensor::make_resolution(format, 1, 2);
////        static_assert(resolution_maked == resolution);

////        inputOutput.write( resolution );
////        auto resolution_read2 = inputOutput.get<hub::sensor::Resolution>();
////        assert( resolution == resolution_read2 );

//        return;

//        std::tuple<int, bool> tuple{2, true};
//        inputOutput.write(tuple);
//        std::tuple<int, bool> tuple2;
//        inputOutput.read(tuple2);
//        assert(tuple == tuple2);

//        //        return;

////        const std::string sensorName = "sensorName";
////        inputOutput.write( sensorName );
////        inputOutput.write( sensorName );
////        std::cout << "start reading" << std::endl;
////        std::string sensorName_read;
////        inputOutput.read( sensorName_read );
////        assert( sensorName == sensorName_read );
////        auto sensorName_read2 = inputOutput.get<std::string>();
////        assert( sensorName == sensorName_read2 );

////        inputOutput.write( ref_resolutions );
////        std::cout << "start reading" << std::endl;
////        hub::sensor::Resolutions resolutions_read;
////        inputOutput.read( resolutions_read );
////        assert( ref_resolutions == resolutions_read );

////        inputOutput.write( ref_resolutions );
////        auto resolutions_read2 = inputOutput.get<hub::sensor::Resolutions>();
////        assert( ref_resolutions == resolutions_read2 );

////        hub::sensor::SensorSpec::MetaData metaData;
////        metaData["a"] = 5;
////        inputOutput.write( metaData );
////        std::cout << "start reading" << std::endl;
////        hub::sensor::SensorSpec::MetaData metaData_read;
////        inputOutput.read( metaData_read );
////        assert( metaData == metaData_read );

////        inputOutput.write( metaData );
////        auto metaData_read2 = inputOutput.get<hub::sensor::SensorSpec::MetaData>();
////        assert( metaData == metaData_read2 );

////        inputOutput.write( ref_sensorSpec );
////        hub::sensor::SensorSpec ref_sensorSpec_read =
/// inputOutput.get<hub::sensor::SensorSpec>(); /        //                inputOutput.read(
/// ref_sensorSpec_read ); /        assert( ref_sensorSpec == ref_sensorSpec_read );

//        //        //        InputOutput inputOutput2;

//        //        //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
//        //        //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

//        //        assert( inputOutput.isEnd() );
//        //        hub::sensor::OutputSensor outputSensor( inputOutput, ref_sensorSpec );
//        //        assert( !inputOutput.isEnd() );
//        //        assert( outputSensor.getSpec() == ref_sensorSpec );
//        //        //            hub::sensor::OutputSensor outputSensor2( inputOutput2,
//        //        ref_sensorSpec2 );
//        //        //            assert(outputSensor2.getSpec() == ref_sensorSpec2);

//        //        assert( !inputOutput.isEnd() );
//        //        hub::sensor::InputSensor inputSensor( inputOutput );
//        //        //        std::cout << "ref_sensorSpec: " << ref_sensorSpec << std::endl;
//        //        assert( inputSensor.getSpec() == ref_sensorSpec );
//        //        //        std::cout << "inputSensor.getSpec(): " << inputSensor.getSpec() <<
//        //        std::endl; assert( inputOutput.isEnd() );
//        //        //        assert( inputSensor.getSpec() == ref_sensorSpec );

//        //        for ( const auto& acq : ref_acqs ) {
//        //            outputSensor << acq;
//        //            std::cout << "write acq : " << acq << std::endl;
//        //        }

//        //        auto acq_read = inputSensor.acq();
//        //        while ( !inputOutput.isEnd() ) {
//        //            inputSensor >> acq_read;
//        //            std::cout << "read acq : " << acq_read << std::endl;
//        //        }

//        //            for ( const auto& acq2 : ref_acqs2 ) {
//        //                outputSensor2 << acq2;
//        //                std::cout << "write acq2 : " << acq2 << std::endl;
//        //            }

//        //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
//        //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );

//        //        hub::sensor::InputSensor inputSensor2( inputOutput2 );
//        //            assert(inputSensor2.getSpec() == ref_sensorSpec);

//        //        auto acq = inputOutput.get<hub::sensor::Acquisition>();

//        //        auto acq2_read = inputSensor2.acq();
//        //        while ( !inputOutput2.isEnd() ) {
//        //            inputSensor2 >> acq2_read;
//        //            std::cout << "read acq2 : " << acq2_read << std::endl;
//        //        }

//        //        hub::sensor::Acquisition acq;
//        //        std::cout << "sync acqs" << std::endl;
//        //        std::vector<hub::sensor::Acquisition> sync_acqs;
//        //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

//        //            inputSensor >> inputSensor2 >> acq;
//        //            std::cout << acq << std::endl;
//        //            sync_acqs.push_back( std::move( acq ) );
//        //        }

//        //        std::cout << std::endl;

//        //        assert( sync_acqs.size() == ref_sync_acqs.size() );
//        //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
//        //            const auto& acq2 = sync_acqs.at( i );

//        //            CHECK( acq2 == ref_sync_acqs.at( i ) );
//        //        }
//    }

//    //    ///////////////////////////////// TESTING EACH INPUT IMPLEMENTS

//    //    std::cout << "----------------------------------------- Memory "
//    //                 "----------------------------------------"
//    //                 "---------------------"
//    //              << std::endl;

//    //    {
//    //        std::vector<char> buff;
//    //        hub::output::OutputMemory<decltype( buff )> outputMemory( buff );
//    //        hub::input::InputMemory<decltype( buff )> inputMemory( buff );
//    //        std::vector<char> buff2;
//    //        hub::output::OutputMemory<decltype( buff2 )> outputMemory2( buff2 );
//    //        hub::input::InputMemory<decltype( buff2 )> inputMemory2( buff2 );

//    //        checkSynchronize( std::move( outputMemory ),
//    //                          ref_sensorSpec,
//    //                          ref_acqs,
//    //                          std::move( outputMemory2 ),
//    //                          ref_sensorSpec2,
//    //                          ref_acqs2,
//    //                          std::move( inputMemory ),
//    //                          std::move( inputMemory2 ),
//    //                          ref_sync_acqs );
//    //    }

//    //    std::cout
//    //        << "----------------------------------------- File
//    //        ----------------------------------------"
//    //           "---------------------"
//    //        << std::endl;

//    //    {
//    //        {
//    //            auto outputFile  = hub::output::OutputFile( "filepath.txt" );
//    //            auto outputFile2 = hub::output::OutputFile( "filepath2.txt" );

//    //            hub::sensor::OutputSensor outputSensor( ref_sensorSpec, std::move( outputFile )
//    );
//    //            hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, std::move( outputFile2
//    )
//    //            );

//    //            for ( const auto& acq : ref_acqs ) {
//    //                outputSensor << acq;
//    //            }
//    //            for ( const auto& acq2 : ref_acqs2 ) {
//    //                outputSensor2 << acq2;
//    //            }
//    //        }

//    //        auto inputFile  = hub::input::InputFile( "filepath.txt" );
//    //        auto inputFile2 = hub::input::InputFile( "filepath2.txt" );
//    //        hub::sensor::InputSensor inputSensor( std::move( inputFile ) );
//    //        hub::sensor::InputSensor inputSensor2( std::move( inputFile2 ) );

//    //        hub::sensor::Acquisition acq;
//    //        std::cout << "sync acqs" << std::endl;
//    //        std::vector<hub::sensor::Acquisition> sync_acqs;
//    //        while ( !inputSensor.getInput().isEnd() && !inputSensor2.getInput().isEnd() ) {

//    //            inputSensor >> inputSensor2 >> acq;
//    //            std::cout << acq << std::endl;
//    //            sync_acqs.push_back( std::move( acq ) );
//    //        }

//    //        std::cout << std::endl;

//    //        assert( sync_acqs.size() == ref_sync_acqs.size() );
//    //        for ( int i = 0; i < sync_acqs.size(); ++i ) {
//    //            const auto& acq2 = sync_acqs.at( i );

//    //            CHECK( acq2 == ref_sync_acqs.at( i ) );
//    //        }
//    //    }

//    //    std::cout << "----------------------------------------- Stream "
//    //                 "----------------------------------------"
//    //                 "---------------------"
//    //              << std::endl;

//    // #ifdef HUB_BUILD_SERVER
//    //     auto outputSensor  = hub::sensor::OutputSensor(ref_sensorSpec,
//    hub::output::OutputStream(
//    //     FILE_NAME )); auto outputSensor2 = hub::sensor::OutputSensor(ref_sensorSpec2,
//    //     hub::output::OutputStream( FILE_NAME + "2" )); auto inputSensor   =
//    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME )); auto inputSensor2  =
//    //     hub::sensor::InputSensor(hub::input::InputStream( FILE_NAME + "2" ));

//    //        checkSynchronize( outputSensor.getOutput() ,
//    //                          ref_acqs,
//    //                          outputSensor2.getOutput() ,
//    //                          ref_acqs2,
//    //                          ref_sync_acqs,
//    //                          false,
//    //                          inputSensor.getInput(),
//    //                          inputSensor2.getInput()
//    //                      );
//    // #else
//    //    {
//    //        auto outputStream  = hub::output::OutputStream( FILE_NAME );
//    //        auto outputStream2 = hub::output::OutputStream( FILE_NAME "2" );
//    //        auto inputStream   = hub::input::InputStream( FILE_NAME );
//    //        auto inputStream2  = hub::input::InputStream( FILE_NAME "2" );

//    //        checkSynchronize( std::move( outputStream ),
//    //                          ref_sensorSpec,
//    //                          ref_acqs,
//    //                          std::move( outputStream2 ),
//    //                          ref_sensorSpec2,
//    //                          ref_acqs2,
//    //                          std::move( inputStream ),
//    //                          std::move( inputStream2 ),
//    //                          ref_sync_acqs,
//    //                          true );
//    //    }
//    // #endif

//    ///////////////////////////////////////////////////////////////////////////////////////////
//}

////    std::vector<char> buff;

////    const auto resolution = hub::sensor::Resolution { hub::sensor::format::BGR8, 1 };
////    const hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );
////    unsigned char data[3] { 1, 2, 3 };
////    hub::sensor::OutputSensor outputSensor( sensorSpec, hub::io::Memory<decltype( buff )>( buff
///) /    );

////    auto acq = std::move( hub::sensor::Acquisition { 0, 1 } << hub::Measure { data, 3,
/// resolution /    } ); outputSensor << acq; outputSensor << acq; outputSensor << acq;

////    hub::sensor::InputSensor inputSensor { hub::io::Memory<decltype( buff )>( buff ) };

////    hub::sensor::Acquisition acq2;
////    inputSensor >> acq2;
////    CHECK( acq == acq2 );

////    const auto& input = inputSensor.getInput();
////    CHECK( !input.isEnd() );

////    auto acqs = inputSensor.getAllAcquisitions();
////    CHECK( acqs.size() == 2 );
////    CHECK( acqs[0] == acq );
////    CHECK( acqs[1] == acq );
////    CHECK( input.isEnd() );
