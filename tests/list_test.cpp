#include <tinystl/list>
#include <tinystl/string>
#include "catch.hpp"

// a predicate implemented as a function:
bool single_digit (const int& value) { return (value<10); }

// a predicate implemented as a class:
struct is_odd {
bool operator() (const int& value) { return (value%2)==1; }
};

// comparison, not case sensitive.
bool compare_nocase (tinystd::string& first, tinystd::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])<tolower(second[i])) return true;
    else if (tolower(first[i])>tolower(second[i])) return false;
    ++i;
  }
  return ( first.length() < second.length() );
}

// compare only integral part:
bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

TEST_CASE("tinystl::list class", "[tinystl::list]") {

    SECTION("Constructors") {
        tinystd::list<int> first;                                // empty list of ints
        tinystd::list<int> second (4,100);                       // four ints with value 100
        tinystd::list<int> third (second.begin(),second.end());  // iterating through second
        tinystd::list<int> fourth (third); 

        // the iterator constructor can also be used to construct from arrays:
        int myints[] = {16,2,77,29};
        tinystd::list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

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
        for (tinystd::list<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
        {
            CHECK( myints[i++] == *it );
        }
    }

    SECTION("Assign operator") {
        tinystd::list<int> first (3);      // list of 3 zero-initialized ints
        tinystd::list<int> second (5);     // list of 5 zero-initialized ints

        second = first;
        first = tinystd::list<int>();

        CHECK( first.size() == 0);
        CHECK( second.size() == 3);
    }

    SECTION("Iterators") {
        int myints[] = {75,23,65,42,13};
        tinystd::list<int> mylist (myints,myints+5);
        
        int i=0;
        for (tinystd::list<int>::iterator it = mylist.begin(); it != mylist.end(); ++it)
        {
            CHECK( myints[i++] == *it );
        }

        i=4;
        for (tinystd::list<int>::reverse_iterator it = mylist.rbegin(); it != mylist.rend(); ++it)
        {
            CHECK( myints[i--] == *it );
        }
    }

    SECTION("Capacity") {
        {
            tinystd::list<int> mylist;
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
            tinystd::list<int> myints;
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
            tinystd::list<int> mylist;

            mylist.push_back(77);
            mylist.push_back(22);

            // now front equals 77, and back 22

            mylist.front() -= mylist.back();

            CHECK( mylist.front() == 55 );
        }

        {
            tinystd::list<int> mylist;

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
            tinystd::list<int> mylist;
            tinystd::list<int>::iterator it1,it2;

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
            tinystd::list<int> mylist1, mylist2;
            tinystd::list<int>::iterator it;

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

            int test_list_1[] = { 30, 3, 4, 1, 10, 20 };
            int test_list_2[] = { 2 };
            int i = 0;

            tinystd::string str = "";
            for (it=mylist1.begin(); it!=mylist1.end(); ++it) {
                str += tinystd::to_string(*it);
                str += " ";
            }
            INFO( "list1: " << str.c_str() << " count: " << mylist1.size());
            for (it=mylist1.begin(); it!=mylist1.end(); ++it, ++i) {
                CHECK( test_list_1[i] == *it );
            }

            str = "";
            for (it=mylist2.begin(); it!=mylist2.end(); ++it) {
                str += tinystd::to_string(*it);
                str += " ";
            }
            INFO( "list2: " << str.c_str() << " count: " << mylist2.size());
            i = 0;
            for (it=mylist2.begin(); it!=mylist2.end(); ++it, ++i) {
                CHECK( test_list_2[i] == *it );
            }
        }

        {
            int myints[]= {17,89,7,14};
            tinystd::list<int> mylist (myints,myints+4);

            mylist.remove(89);

            int test_list[] = { 17, 7, 14 };
            int i=0;
            for (auto it=mylist.begin(); it!=mylist.end(); ++it, ++i) {
                CHECK( test_list[i] == *it );
            }
        }

        {
            // See predicates on top of that file

            int myints[]= {15,36,7,17,20,39,4,1};
            tinystd::list<int> mylist (myints,myints+8);   // 15 36 7 17 20 39 4 1

            mylist.remove_if (single_digit);           // 15 36 17 20 39

            mylist.remove_if (is_odd());               // 36 20

            int test_list[] = { 36, 20 };
            int i=0;
            for (auto it=mylist.begin(); it!=mylist.end(); ++it, ++i) {
                CHECK( test_list[i] == *it );
            }
        }

        {
            tinystd::list<double> first, second;

            first.push_back (3.1);
            first.push_back (2.2);
            first.push_back (2.9);

            second.push_back (3.7);
            second.push_back (7.1);
            second.push_back (1.4);

            first.sort();
            second.sort();

            first.merge(second);

            // (second is now empty)

            second.push_back (2.1);

            //first.merge(second, mycomparison);

            double test_list[] = { 1.4, 2.2, 2.9, 2.1, 3.1, 3.7, 7.1 };
            int i=0;

            for (auto it=first.begin(); it != first.end(); ++it, ++i)
            {
                CHECK( test_list[i] == *it );
            }
        }

        {
            tinystd::list<tinystd::string> mylist;
            tinystd::list<tinystd::string>::iterator it;
            mylist.push_back ("one");
            mylist.push_back ("two");
            mylist.push_back ("Three");

            mylist.sort();

            const char *list[] = { "Three", "one", "two" };
            int i=0;
            
            for (it=mylist.begin(); it!=mylist.end(); ++it,++i) {
                CHECK( *it == list[i] );
            }

            mylist.sort(compare_nocase);

            const char *list_2[] = { "one", "Three", "two" };
            i=0;
            
            for (it=mylist.begin(); it!=mylist.end(); ++it,++i) {
                CHECK( *it == list_2[i] );
            }
        }

        {
            tinystd::list<int> mylist;

            for (int i=1; i<10; ++i) mylist.push_back(i);

            mylist.reverse();
            int i=9;
            for (auto it = mylist.begin(); it != mylist.end(); ++it,--i)
            {
                CHECK( *it == i );
            }
        }
    }
}
