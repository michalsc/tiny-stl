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

    SECTION("Iterators") {
        int myints[] = {75,23,65,42,13};
        t_std::list<int> mylist (myints,myints+5);
        
        int i=0;
        for (t_std::list<int>::iterator it = mylist.begin(); it != mylist.end(); ++it)
        {
            CHECK( myints[i++] == *it );
        }

        i=4;
        for (t_std::list<int>::reverse_iterator it = mylist.rbegin(); it != mylist.rend(); ++it)
        {
            CHECK( myints[i--] == *it );
        }
    }

    SECTION("Capacity") {
        {
            t_std::list<int> mylist;
            int sum (0);

            for (int i=1;i<=10;++i) mylist.push_back(i);

            while (!mylist.empty())
            {
                sum += mylist.front();
                mylist.pop_front();
            }

            CHECK( sum == 55 );
        }

        {
            t_std::list<int> myints;
            CHECK( myints.size() == 0 );

            for (int i=0; i<10; i++) myints.push_back(i);
            CHECK( myints.size() == 10 );

            myints.insert(myints.begin(), 1);
            CHECK( myints.size() == 11 );

            myints.insert(myints.begin(),10,100);
            CHECK( myints.size() == 21 );

            myints.pop_back();
            CHECK( myints.size() == 20 );
        }
    }

    SECTION("Element access") {
        {
            t_std::list<int> mylist;

            mylist.push_back(77);
            mylist.push_back(22);

            // now front equals 77, and back 22

            mylist.front() -= mylist.back();

            CHECK( mylist.front() == 55 );
        }

        {
            t_std::list<int> mylist;

            mylist.push_back(10);

            while (mylist.back() != 0)
            {
                mylist.push_back ( mylist.back() -1 );
            }

            auto it = mylist.begin();
            for (int i=10; i >= 0; ++it, i--)
            {
                CHECK( *it == i );
            }
        }

        {
            t_std::list<int> mylist;
            t_std::list<int>::iterator it1,it2;

            // set some values:
            for (int i=1; i<10; ++i) mylist.push_back(i*10);

                                        // 10 20 30 40 50 60 70 80 90
            it1 = it2 = mylist.begin(); // ^^
            advance (it2,6);            // ^                 ^
            ++it1;                      //    ^              ^

            it1 = mylist.erase (it1);   // 10 30 40 50 60 70 80 90
                                        //    ^           ^

            it2 = mylist.erase (it2);   // 10 30 40 50 60 80 90
                                        //    ^           ^

            ++it1;                      //       ^        ^
            --it2;                      //       ^     ^

            mylist.erase (it1,it2);     // 10 30 60 80 90
                                        //

            int test_array[] = { 10, 30, 60, 80, 90 };
            int i=0;

            for (auto it=mylist.begin(); it != mylist.end(); ++it, ++i)
            {
                CHECK( *it == test_array[i] );
            }
        }
    }

    SECTION("Operations") {
        {
            t_std::list<int> mylist1, mylist2;
            t_std::list<int>::iterator it;

            // set some initial values:
            for (int i=1; i<=4; ++i)
                mylist1.push_back(i);      // mylist1: 1 2 3 4

            for (int i=1; i<=3; ++i)
                mylist2.push_back(i*10);   // mylist2: 10 20 30

            it = mylist1.begin();
            ++it;                         // points to 2

            mylist1.splice (it, mylist2); // mylist1: 1 10 20 30 2 3 4
                                            // mylist2 (empty)
                                            // "it" still points to 2 (the 5th element)
                                          
            mylist2.splice (mylist2.begin(),mylist1, it);
                                            // mylist1: 1 10 20 30 3 4
                                            // mylist2: 2
                                            // "it" is now invalid.
            it = mylist1.begin();
            std::advance(it,3);           // "it" points now to 30

            mylist1.splice ( mylist1.begin(), mylist1, it, mylist1.end());
                                            // mylist1: 30 3 4 1 10 20

            std::cout << "mylist1 contains:";
            for (it=mylist1.begin(); it!=mylist1.end(); ++it)
                std::cout << ' ' << *it;
            std::cout << '\n';

            std::cout << "mylist2 contains:";
            for (it=mylist2.begin(); it!=mylist2.end(); ++it)
                std::cout << ' ' << *it;
            std::cout << '\n';
        }
    }


}