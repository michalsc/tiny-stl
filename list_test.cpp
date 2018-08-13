#include "t_list.h"
#include "catch.hpp"

TEST_CASE("t_std::list class", "[t_std::list]") {

    SECTION("Constructors") {
        t_std::list<int> first;                                // empty list of ints
        t_std::list<int> second (4,100);                       // four ints with value 100
        t_std::list<int> third (second.begin(),second.end());  // iterating through second
        t_std::list<int> fourth (third); 

        // the iterator constructor can also be used to construct from arrays:
        int myints[] = {16,2,77,29};
        t_std::list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

        CHECK( first.empty() == true );
        CHECK( second.empty() == false );
        CHECK( third.empty() == false );
        CHECK( fourth.empty() == false );
        CHECK( fifth.empty() == false );
        CHECK( first.size() == 0 );
        CHECK( second.size() == 4 );
        CHECK( third.size() == 4 );
        CHECK( fourth.size() == 4 );
        CHECK( fifth.size() == 4 );

        int i=0;
        for (t_std::list<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
        {
            CHECK( myints[i++] == *it );
        }
    }

    SECTION("Assign operator") {
        t_std::list<int> first (3);      // list of 3 zero-initialized ints
        t_std::list<int> second (5);     // list of 5 zero-initialized ints

        second = first;
        first = t_std::list<int>();

        CHECK( first.size() == 0);
        CHECK( second.size() == 3);
    }

}