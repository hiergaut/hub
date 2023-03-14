
#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <data/UserData.hpp>

TEST_CASE( "UserData test" ) {

    unsigned char data[28] { 0 };
    const auto& resolution = hub::Resolution { { 1 }, hub::Format::DOF6 };
    hub::data::Measure measure( data, 28, resolution );

    hub::data::UserData userData( "userData", 0 );
    std::cout << userData << std::endl;

    const hub::data::Measure& measure5 = userData;
    hub::data::UserData userData2( measure5 );

    std::cout << userData2 << std::endl;
}