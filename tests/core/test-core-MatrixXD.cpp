
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

#include <core/matrix/MatrixXD.hpp>
// #include <core/matrix/MatrixTs.hpp>
// #include <core/Matrix.hpp>

struct RGB8 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct XYZ32F {
    float x;
    float y;
    float z;
};

TEST_CASE( "MatrixXD test" ) {
    TEST_BEGIN()

    constexpr auto width  = 640;
    constexpr auto height = 480;
    using ResRGB          = hub::MatrixXD<RGB8, width, height>;
    using ResPoint        = hub::MatrixXD<XYZ32F, width, height>;

    std::cout << "ResRGB: " << ResRGB() << std::endl;
    std::cout << "ResPoint: " << ResPoint() << std::endl;

    // using Resolution      = hub::MatrixTs<ResRGB, ResPoint>;
    // using Resolution      = hub::MatrixTs<int, double>;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    // using Resolution = ResPoint;
    // using Resolution = ResRGB;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    // ResRGB().getMatrix();
    std::cout << "ResRGB matrix: " << ResRGB().getMatrix() << std::endl;
    std::cout << "ResPoint matrix: " << ResPoint().getMatrix() << std::endl;
    // ResPoint().getMatrix();
    // hub::make_matrix<Resolution>();
    //     std::cout << typeid(int).hash_code() << std::endl;

    static_assert( hub::isMatrix<hub::MatrixXD<int, 640, 480>> );

    // hub::io::Archive archive;

    using namespace hub;

    using MyMatrixXD = MatrixXD<int, 640, 480>;
    static_assert( MyMatrixXD::capacity() == 640 * 480 );
    static_assert( MyMatrixXD::nDim() == 2 );
    static_assert( MyMatrixXD::getDim<0>() == 640 );
    static_assert( MyMatrixXD::getDim<1>() == 480 );
    static_assert( MyMatrixXD::nType() == 1 );
    static_assert( MyMatrixXD::hasType<int>() );
    static_assert( MyMatrixXD::hasType<int, int>() );
    static_assert( !MyMatrixXD::hasType<float>() );
    static_assert( !MyMatrixXD::hasType<bool>() );
    static_assert( std::is_same<MyMatrixXD::getType<0>, int>() );
    static_assert( MyMatrixXD::size() == sizeof( int ) * 640 * 480 );

    std::cout << "MyMatrixXD: " << MyMatrixXD() << std::endl;

    //    MatrixXD<int>;
    //    MatrixXD<int, 1>;
    static_assert( MatrixXD<int, 2>::Capacity == 2 );

    using MatrixInt = MatrixXD<int, 2>;
    static_assert( MatrixInt::Size == 8 );
    //    std::array<int, 2> matrixIntRawData{1, 2};
    //    auto matrixInt_ref = MatrixInt{1, 2};
    using MatrixFloat = MatrixXD<float, 2>;
    static_assert( MatrixFloat::Size == 8 );
    using MatrixDouble = MatrixXD<double, 2>;
    static_assert( MatrixDouble::Size == 16 );

    constexpr Buffer<Data_t, 2> buffer { 1, 2 };
    std::cout << "buffer: " << buffer << std::endl;

    constexpr Buffer<int, 2> buffer2 { 1, 2 };
    std::cout << "buffer2: " << buffer2 << std::endl;

    constexpr MatrixXD<char, 2> matrice4 { { 1, 2 } };
    std::cout << "matrice4: " << matrice4 << std::endl;

    constexpr MatrixXD<int, 2> matrice5 { { 1, 2 } };
    std::cout << "matrice5: " << matrice5 << std::endl;

    char str[] { 'B', 'o', 'u', 'y', 'j', 'o', 'u' };
    std::cout << "str: " << std::to_string( str[0] ) << std::endl;
    std::string string;
    string += str;
    static_assert( std::is_arithmetic<char>() );

    ///////////////////////////////////////////////////////////////////////////////

    using VectorChar = Vector<char, 10>;
    VectorChar vectorChar { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    std::cout << "vectorChar: " << vectorChar << std::endl;
    assert( vectorChar.get<0>() == 1 );
    assert( vectorChar.get<1>() == 2 );

    auto serial = vectorChar.getMatrix();
    std::cout << "serial: " << serial << std::endl;

    assert( serial.hasType<char>() );
    assert( serial.getDims<char>() == std::vector<int> { 10 } );

    ////////////////////////////////
    /// \brief The UserClass class
    ///
    struct UserClass {
        bool a;
        bool b;
        auto toString() const { return std::to_string( a ) + " " + std::to_string( b ); }
        static constexpr auto name() { return "UserClass"; }
    };

    using UserMatrix = MatrixXD<UserClass, 2, 3>;
    //    UserMatrix matrixUser;

    //    MatrixTs<int, UserMatrix> matrixUser;

    ////////////////////////////////

    TEST_END()
}
