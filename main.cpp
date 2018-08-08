#include <stdio.h>
#include <iostream>

#include "t_string.h"

static const char __attribute__((used)) version[] = "\0$VER: MiniStd " VERSION_STRING;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    t_std::string str = "Hello";
    t_std::string s0("Initial string");

    // constructors used in the same order as described above:
    t_std::string s1;
    t_std::string s2(s0);
    t_std::string s3(s0, 8, 3);
    t_std::string s4("A character sequence");
    t_std::string s5("Another character sequence", 12);
    t_std::string s6a(10, 'x');
    t_std::string s6b(10, 42);

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

    return 0;
}