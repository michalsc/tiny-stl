#include "t_string.h"
#include "catch.hpp"

TEST_CASE("t_std::string class", "[t_std::string]") {

    SECTION("Constructors are working") {
        t_std::string s0("Initial string");
        // constructors used in the same order as described above:
        t_std::string s1;
        t_std::string s2(s0);
        t_std::string s3(s0, 8, 3);
        t_std::string s4("A character sequence");
        t_std::string s5("Another character sequence", 12);
        t_std::string s6a(10, 'x');
        t_std::string s6b(10, 42);

        CHECK( s0 == "Initial string" );
        CHECK( s1.empty() );
        CHECK( s1 == "" );
        CHECK( s2 == "Initial string" );
        CHECK( s3 == "str" );
        CHECK( s4 == "A character sequence" );
        CHECK( s5 == "Another char" );
        CHECK( s6a == "xxxxxxxxxx" );
        CHECK( s6b == "**********" );
    }
}