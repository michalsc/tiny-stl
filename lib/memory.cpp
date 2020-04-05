/*
    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <tinystl/bits/support.h>

#ifndef BUG
#define BUG(...) printf(__VA_ARGS__)
#endif

void * mungwall_malloc(size_t size)
{
    uint32_t *ptr = reinterpret_cast<uint32_t *>(malloc(size + 32));
    ptr[0] = size;
    ptr[1] = 0xdeadbeef;
    ptr[2] = 0xdeadbeef;
    ptr[3] = 0xdeadbeef;

    ptr[4 + size/4] = 0xcafebabe;
    ptr[5 + size/4] = 0xcafebabe;
    ptr[6 + size/4] = 0xcafebabe;
    ptr[7 + size/4] = 0xcafebabe;

    bzero(&ptr[4], size);

    return &ptr[4];
}

void mungwall_free(void *ptr, size_t size)
{
    uint32_t *p = reinterpret_cast<uint32_t *>(ptr);

    p -= 4;
    if (*p != (unsigned int)size) {
#ifndef NDEBUG
        BUG("Size mismatch at mungwall_free!! %d != %d\n", *p, (uint32_t)size);
#endif
    }

    if (p[1] != 0xdeadbeef || p[2] != 0xdeadbeef || p[3] != 0xdeadbeef)
    {
#ifndef NDEBUG
        BUG("mungwall_free(): left wall damaged %08x%08x%08x\n", p[1], p[2], p[3]);
#endif
    }
    if (p[4 + size/4] != 0xcafebabe || p[5+size/4] != 0xcafebabe || p[6+size/4] != 0xcafebabe || p[7+size/4] != 0xcafebabe)
    {
#ifndef NDEBUG
        BUG("mungwall_free(): right wall damaged %08x%08x%08x%08x\n", p[4 + size/4], p[5+size/4], p[6+size/4],p[7+size/4]);
#endif
    }

    free(p);
}
