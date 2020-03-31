/*
    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <tinystl/string>
#include "catch.hpp"

TEST_CASE("tinystl::string class", "[tinystl::string]") {

    SECTION("Constructors are working") {
        tinystd::string s0("Initial string");
        // constructors used in the same order as described above:
        tinystd::string s1;
        tinystd::string s2(s0);
        tinystd::string s3(s0, 8, 3);
        tinystd::string s4("A character sequence");
        tinystd::string s5("Another character sequence", 12);
        tinystd::string s6a(10, 'x');
        tinystd::string s6b(10, 42);
        tinystd::string s7(s0.begin(), s0.begin() + 7);

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
        tinystd::string str1, str2, str3;
        str1 = "Test string: "; // c-string
        str2 = 'x';             // single character
        str3 = str1 + str2;
        CHECK( str1 == "Test string: " );
        CHECK( str2 == "x" );
        CHECK( str3 == "Test string: x" );
    }

    SECTION("Testing iterators") {
        char str_contents[] = "Test string";
        tinystd::string str(str_contents);
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
            tinystd::string s = "Hello, world";
            auto r = s.rbegin();
            r[7] = 'O'; // replaces 'o' with 'O'
            r += 7;     // iterator now points at 'O'
            tinystd::string rev(r, s.rend());

            CHECK( rev == "OlleH" );
        }
    }

    SECTION("Testing capacity functions") {
        tinystd::string str("Test string");

        CHECK( str.size() == 11 );
        CHECK( str.length() == 11 );

        str = tinystd::string("I like to code in C");
        CHECK( str == "I like to code in C" );

        unsigned sz = str.size();
        str.resize(sz + 2, '+');
        CHECK( str == "I like to code in C++" );

        str.resize(14);
        CHECK( str == "I like to code" );
    }

    SECTION("Element access") {
        char str_contents[] = "Test string";
        tinystd::string str(str_contents);

        for (int i = 0; i < str.length(); ++i)
        {
            CHECK( str[i] == str_contents[i] );
            CHECK( str.at(i) == str_contents[i] );
        }
    }

    SECTION("Modifiers") {
        {
            tinystd::string name("John");
            tinystd::string family("Smith");
            name += " K. "; // c-string
            name += family; // string
            name += '\n'; // character

            CHECK( name == "John K. Smith\n" );
        }

        {
            tinystd::string str;
            tinystd::string str2 = "Writing ";
            tinystd::string str3 = "print 10 and then 5 more";

            // used in the same order as described above:
            str.append(str2);               // "Writing "
            str.append(str3, 6, 3);         // "10 "
            str.append("dots are cool", 5); // "dots "
            str.append("here: ");           // "here: "
            str.append(10u, '.');           // ".........."
            str.append(str3.begin()+8,str3.end());  // " and then 5 more"
            str.append(5,0x2E);

            CHECK( str == "Writing 10 dots here: .......... and then 5 more....." );
        }
        {
            tinystd::string str;
            tinystd::string base = "The quick brown fox jumps over a lazy dog.";

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

            str.assign(base.begin()+16,base.end()-12);
            CHECK( str == "fox jumps over" );
        }

        {
            tinystd::string str="to be question";
            tinystd::string str2="the ";
            tinystd::string str3="or not to be";
            tinystd::string::iterator it;

            // used in the same order as described above:
            str.insert(6, str2);                 // to be (the )question
            CHECK( str == "to be the question" );

            str.insert(6,str3,3,4);             // to be (not )the question
            CHECK( str == "to be not the question" );

            str.insert(10,"that is cool",8);    // to be not (that is )the question
            CHECK( str == "to be not that is the question" );

            str.insert(10,"to be ");            // to be not (to be )that is the question
            CHECK( str == "to be not to be that is the question" );

            str.insert(15,1,':');               // to be not to be(:) that is the question
            CHECK( str == "to be not to be: that is the question" );

            it = str.insert(str.begin()+5,','); // to be(,) not to be: that is the question
            CHECK ( str == "to be, not to be: that is the question" );

            str.insert (str.end(),3,'.');
            CHECK ( str == "to be, not to be: that is the question..." );

            str.insert (it+2,str3.begin(),str3.begin()+3); // (or )
            CHECK ( str == "to be, or not to be: that is the question..." );
        }

        {
            tinystd::string str ("This is an example sentence.");

                                                    // "This is an example sentence."
            str.erase (10,8);                       //            ^^^^^^^^
            CHECK( str == "This is an sentence." );
                                                    // "This is an sentence."
            str.erase (str.begin()+9);              //           ^
            CHECK( str == "This is a sentence." );
                                                    // "This is a sentence."
            str.erase (str.begin()+5, str.end()-9); //       ^^^^^
            CHECK( str == "This sentence." );
                                                   // "This sentence."
        }

        {
            tinystd::string base="this is a test string.";
            tinystd::string str2="n example";
            tinystd::string str3="sample phrase";
            tinystd::string str4="useful.";

            // replace signatures used in the same order as described above:

            // Using positions:                 0123456789*123456789*12345
            tinystd::string str=base;         // "this is a test string."
            str.replace(9,5,str2);          // "this is an example string." (1)
            CHECK( str == "this is an example string." );
            str.replace(19,6,str3,7,6);     // "this is an example phrase." (2)
            CHECK( str == "this is an example phrase." );
            str.replace(8,10,"just a");     // "this is just a phrase."     (3)
            CHECK( str == "this is just a phrase." );
            str.replace(8,6,"a shorty",7);  // "this is a short phrase."    (4)
            CHECK( str == "this is a short phrase." );
            str.replace(22,1,3,'!');        // "this is a short phrase!!!"  (5)
            CHECK( str == "this is a short phrase!!!" );

            // Using iterators:                                               0123456789*123456789*
            str.replace(str.begin(),str.end()-3,str3);                    // "sample phrase!!!"      (1)
            CHECK( str == "sample phrase!!!" );
            str.replace(str.begin(),str.begin()+6,"replace");             // "replace phrase!!!"     (3)
            CHECK( str == "replace phrase!!!" );
            str.replace(str.begin()+8,str.begin()+14,"is coolness",7);    // "replace is cool!!!"    (4)
            CHECK( str == "replace is cool!!!" );
            str.replace(str.begin()+12,str.end()-4,4,'o');                // "replace is cooool!!!"  (5)
            CHECK( str == "replace is cooool!!!" );
            str.replace(str.begin()+11,str.end(),str4.begin(),str4.end());// "replace is useful."    (6)
            CHECK( str == "replace is useful." );
        }
    }


}