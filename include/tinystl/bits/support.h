/*  -*- C++ -*-

    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _TINYSTD_BITS_SUPPORT_H
#define _TINYSTD_BITS_SUPPORT_H

#include <stdint.h>
#include <type_traits>
#include <iterator>

extern "C" {

void * mungwall_malloc(size_t size);
void mungwall_free(void *ptr, size_t size);

}

#endif // _TINYSTD_BITS_SUPPORT_H
