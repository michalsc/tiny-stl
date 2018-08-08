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
        t_std::string s7(s0.begin(), s0.begin() + 7);

        CHECK( s0 == "Initial string" );
        CHECK( s1.empty() );
        CHECK( s1 == "" );
        CHECK( s2 == "Initial string" );
        CHECK( s3 == "str" );
        CHECK( s4 == "A character sequence" );
        CHECK( s5 == "Another char" );
        CHECK( s6a == "xxxxxxxxxx" );
        CHECK( s6b == "**********" );
        CHECK( s7 == "Initial" );
    }

    SECTION("Testing operator=") {
        t_std::string str1, str2, str3;
        str1 = "Test string: "; // c-string
        str2 = 'x';             // single character
        str3 = str1 + str2;
        CHECK( str1 == "Test string: " );
        CHECK( str2 == "x" );
        CHECK( str3 == "Test string: x" );
    }

    SECTION("Testing iterators") {
        char str_contents[] = "Test string";
        t_std::string str(str_contents);
        int i = 0;

        for (auto it = str.begin(); it != str.end(); ++it, ++i)
        {
            CHECK( str_contents[i] == *it );
        }

        i = strlen(str_contents);

        for (auto it = str.rbegin(); it != str.rend(); ++it, --i)
        {
            CHECK( str_contents[i-1] == *it );
        }

        {
            t_std::string s = "Hello, world";
            auto r = s.rbegin();
            r[7] = 'O'; // replaces 'o' with 'O'
            r += 7;     // iterator now points at 'O'
            t_std::string rev(r, s.rend());

            CHECK( rev == "OlleH" );
        }
    }

    SECTION("Testing capacity functions") {
        t_std::string str("Test string");

        CHECK( str.size() == 11 );
        CHECK( str.length() == 11 );

        str = t_std::string("I like to code in C");
        CHECK( str == "I like to code in C" );

        unsigned sz = str.size();
        str.resize(sz + 2, '+');
        CHECK( str == "I like to code in C++" );

        str.resize(14);
        CHECK( str == "I like to code" );
    }

    SECTION("Element access") {
        char str_contents[] = "Test string";
        t_std::string str(str_contents);

        for (int i = 0; i < str.length(); ++i)
        {
            CHECK( str[i] == str_contents[i] );
            CHECK( str.at(i) == str_contents[i] );
        }
    }

    SECTION("Modifiers") {
        {
            t_std::string name("John");
            t_std::string family("Smith");
            name += " K. "; // c-string
            name += family; // string
            name += '\n'; // character

            CHECK( name == "John K. Smith\n" );
        }

        {
            t_std::string str;
            t_std::string str2 = "Writing ";
            t_std::string str3 = "print 10 and then 5 more";

            // used in the same order as described above:
            str.append(str2);               // "Writing "
            str.append(str3, 6, 3);         // "10 "
            str.append("dots are cool", 5); // "dots "
            str.append("here: ");           // "here: "
            str.append(10u, '.');           // ".........."

            CHECK( str == "Writing 10 dots here: .........." );
        }
        {
            t_std::string str;
            t_std::string base = "The quick brown fox jumps over a lazy dog.";

            // used in the same order as described above:

            str.assign(base);
            CHECK( str == "The quick brown fox jumps over a lazy dog." );

            str.assign(base, 10, 9);
            CHECK( str == "brown fox" );

            str.assign("pangrams are cool", 7);
            CHECK( str == "pangram" );

            str.assign("c-string");
            CHECK( str == "c-string" );

            str.assign(10, '*');
            CHECK( str == "**********" );
        }

        {
            t_std::string str="to be question";
            t_std::string str2="the ";
            t_std::string str3="or not to be";

            // used in the same order as described above:
            str.insert(6, str2);                 // to be (the )question
            CHECK( str == "to be the question" );

            str.insert(6,str3,3,4);             // to be (not )the question
            CHECK( str == "to be not the question" );

            str.insert(10,"that is cool",8);    // to be not (that is )the question
            CHECK( str == "to be not that is the question" );

            str.insert(10,"to be ");            // to be not (to be )that is the question
            CHECK( str == "to be not to be that is the question" );
        }
    }


}