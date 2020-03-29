/*
    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdio.h>
#include <iostream>

#include <tinystd/string>
#include <tinystd/list>

static const char __attribute__((used)) version[] = "\0$VER: MiniStd " VERSION_STRING;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    tinystd::list<int> list(10, 0);
    tinystd::list<tinystd::string> list2;
    
    list.test();
    list.push_front(10);
    list.test();
    list.push_back(20);
    list.test();

    for (auto it=list.begin(); it != list.end(); ++it)
    {
        printf("%d ", *it);
    }
    printf("\n");

    for (auto it = list.rbegin(); it != list.rend(); ++it)
    {
        printf("%d ", *it);
    }
    printf("\n");
    list.clear();

    printf("%ld\n", list.max_size());
    printf("%ld\n", list2.max_size());

    tinystd::string str = "Hello";
    tinystd::string s0("Initial string");

    // constructors used in the same order as described above:
    tinystd::string s1;
    tinystd::string s2(s0);
    tinystd::string s3(s0, 8, 3);
    tinystd::string s4("A character sequence");
    tinystd::string s5("Another character sequence", 12);
    tinystd::string s6a(10, 'x');
    tinystd::string s6b(10, 42);

    printf("MiniStd\n");
    printf("str.length = %d\n", str.length());
    printf("str.capacity = %d\n", str.capacity());

    printf("s0: '%s'\n", s0.c_str());
    printf("s1: '%s'\n", s1.c_str());
    printf("s2: '%s'\n", s2.c_str());
    printf("s3: '%s'\n", s3.c_str());
    printf("s4: '%s'\n", s4.c_str());
    printf("s5: '%s'\n", s5.c_str());
    printf("s6a: '%s'\n", s6a.c_str());
    printf("s6b: '%s'\n", s6b.c_str());

    for (auto it = s0.begin(); it != s0.end(); ++it)
        printf("%c ", *it);
    printf("\n");

    for (auto it = s0.rbegin(); it != s0.rend(); ++it)
        printf("%c ", *it);
    printf("\n");

    tinystd::string sx = tinystd::to_string(((unsigned long long)0xffffffffffffffffULL));
    printf("'%s'\n", sx.c_str());

    return 0;
}