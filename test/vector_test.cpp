/*
    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <tinystl/vector>

TEST_CASE("tinystl::vector class", "[tinystl::vector]") {

    SECTION("Constructors") {
        int i;

        tinystd::vector<int> first;                             // empty vector of ints
        tinystd::vector<int> second (4,100);                    // four ints with value 100
        tinystd::vector<int> third = { 1, 2, 3, 4};             // vecor with initializer
        tinystd::vector<int> fourth (third);
        tinystd::vector<int> fifth (third.begin(), third.end());

        // the iterator constructor can also be used to construct from arrays:
        int myints[] = {16,2,77,29};
        tinystd::vector<int> sixth (myints, myints + sizeof(myints) / sizeof(int) );

        CHECK( first.empty() == true );
        CHECK( second.empty() == false );
        CHECK( third.empty() == false );
        CHECK( fourth.empty() == false );
        CHECK( fifth.empty() == false );
        CHECK( sixth.empty() == false );
        CHECK( first.size() == 0 );
        CHECK( second.size() == 4 );
        CHECK( third.size() == 4 );
        CHECK( fourth.size() == 4 );
        CHECK( fifth.size() == 4 );
        CHECK( sixth.size() == 4 );

        i=1;
        for (tinystd::vector<int>::iterator it = third.begin(); it != third.end(); ++it)
        {
            CHECK( i++ == *it );
        }
        i=1;
        for (tinystd::vector<int>::iterator it = fourth.begin(); it != fourth.end(); ++it)
        {
            CHECK( i++ == *it );
        }
    }
}
