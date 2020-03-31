/*  -*- C++ -*-

    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _TINYSTD_BITS_SUPPORT_H
#define _TINYSTD_BITS_SUPPORT_H

#include <type_traits>
#include <iterator>

#define MEMF_ANY    1
#define MEMF_CLEAR  2

void * AllocMem(int size, int type);
void FreeMem(void *ptr, int size);
void CopyMem(const void *src, void *dst, int size);
void SetMem(void *dst, int size, char fill);
int _strcmp_(const char *s1, const char *s2);
int _strlen_(const char *s1);

#endif // _TINYSTD_BITS_SUPPORT_H
