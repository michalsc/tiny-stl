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
#include <tinystl/string>

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

        tinystd::vector<tinystd::string> words1 {"the", "frogurt", "is", "also", "cursed"};
        CHECK( words1.size() == 5 );
        CHECK( words1[0] == "the" );
        CHECK( words1[1] == "frogurt" );
        CHECK( words1[2] == "is" );
        CHECK( words1[3] == "also" );
        CHECK( words1[4] == "cursed" );

        tinystd::vector<tinystd::string> words2(words1.begin(), words1.end());
        CHECK( words2.size() == 5 );
        CHECK( words2[0] == "the" );
        CHECK( words2[1] == "frogurt" );
        CHECK( words2[2] == "is" );
        CHECK( words2[3] == "also" );
        CHECK( words2[4] == "cursed" );

        tinystd::vector<tinystd::string> words3(words1);
        CHECK( words3.size() == 5 );
        CHECK( words3[0] == "the" );
        CHECK( words3[1] == "frogurt" );
        CHECK( words3[2] == "is" );
        CHECK( words3[3] == "also" );
        CHECK( words3[4] == "cursed" );

        tinystd::vector<tinystd::string> words4(5, "Mo");
        for (int i=0; i < 5; i++)
        {
            CHECK( words4[i] == "Mo" );
        }
    }

    SECTION("Operators and assigns") {
        std::vector<int> nums1 {3, 1, 4, 6, 5, 9};
        std::vector<int> nums2;
        std::vector<int> nums3;

        // Initially nums1 is set, others are empty
        CHECK( (nums1.size() == 6 && nums2.size() == 0 && nums3.size() == 0) );

        nums2 = nums1;

        // Copy operator created separate copy of nums1 in nums2. Both are equal size
        CHECK( (nums1.size() == 6 && nums2.size() == 6 && nums3.size() == 0) );

        nums3 = std::move(nums1);

        // Move operator transferred nums1 into nums3, leaving nums1 empty
        CHECK( (nums1.size() == 0 && nums2.size() == 6 && nums3.size() == 6) );


        tinystd::vector<char> characters;
        characters.assign(5, 'a');

        CHECK( characters.size() == 5 );

        for (int i=0; i < 5; i++) {
            CHECK( characters[i] == 'a' );
        }
    }

    SECTION("Element access") {
        tinystd::vector<int> numbers {2, 4, 6, 8};

        CHECK( numbers[1] == 4 );

        numbers[0] = 5;

        CHECK( numbers[0] == 5 );
        CHECK( numbers[1] == 4 );
        CHECK( numbers[2] == 6 );
        CHECK( numbers[3] == 8 );

        CHECK_THROWS_AS( numbers.at(4), std::out_of_range );

        tinystd::vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};

        CHECK( letters.front() == 'o' );
        CHECK( letters.back() == 'f' );
    }

    SECTION("Capacity") {
        tinystd::vector<int> numbers;
        CHECK( numbers.empty() == true );

        numbers.push_back(42);
        CHECK( numbers.empty() == false );

        tinystd::vector<int> nums {1, 3, 5, 7};

        CHECK( nums.size() == 4 );
    }

    SECTION("Iterators") {
        tinystd::vector<int> ints {1, 2, 4, 8, 16};
        tinystd::vector<std::string> fruits {"orange", "apple", "raspberry"};
        tinystd::vector<char> empty;
        tinystd::vector<int> rints(ints.rbegin(), ints.rend());

        // Sums all integers in the vector ints (if any), printing only the result.
        int sum = 0;
        for (auto it = ints.cbegin(); it != ints.cend(); it++)
            sum += *it;

        CHECK( sum == 31 );

        CHECK( *fruits.begin() == "orange" );

        CHECK( empty.begin() == empty.end() );

        for (int i=0; i < ints.size(); i++) {
            CHECK( ints[i] == rints[rints.size() - i - 1] );
        }
    }

    SECTION("Modifiers") {
        tinystd::vector<int> vec(3,100);
        
        CHECK( vec.size() == 3 );
        CHECK( (vec[0] == 100 && vec[1] == 100 && vec[2] == 100) );
    
        auto it = vec.begin();
        it = vec.insert(it, 200);
        
        CHECK( vec.size() == 4 );
        CHECK( vec[0] == 200 );
        CHECK( (vec[1] == 100 && vec[2] == 100 && vec[3] == 100) );

        vec.insert(it,2,300);
        CHECK( vec.size() == 6);
        CHECK( vec[0] == 300 );
        CHECK( vec[1] == 300 );
        CHECK( vec[2] == 200 );

        // "it" no longer valid, get a new one:
        it = vec.begin();
    
        tinystd::vector<int> vec2(2,400);
        vec.insert(it+2, vec2.begin(), vec2.end());

        CHECK( vec.size() == 8);
        CHECK( vec[0] == 300 );
        CHECK( vec[1] == 300 );
        CHECK( vec[2] == 400 );
        CHECK( vec[3] == 400 );
        CHECK( vec[4] == 200 );
        CHECK( vec[5] == 100 );

        int arr[] = { 501,502,503 };
        int test_arr[] = { 501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100 };
        vec.insert(vec.begin(), arr, arr+3);

        CHECK( vec.size() == 11 );
        for (int i=0; i < 11; i++) {
            CHECK( vec[i] == test_arr[i] );
        }
    }
}
