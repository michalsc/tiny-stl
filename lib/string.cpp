/*
    Copyright © 2020 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <string.h>
#include <tinystl/allocator>
#include <tinystl/string>
#include <tinystl/bits/support.h>

namespace tinystd {

const char string::_null = 0;

namespace __internal__ {
    extern const char *__tinystd_version;
    namespace { static const char __attribute__((used)) *ver = __tinystd_version; }
}

namespace {

int _strcmp_(const char *str1, const char *str2)
{
    const char * s1 = str1;
    const char * s2 = str2;
    unsigned char c1;
    unsigned char c2;
    do {
        c1 = *s1++;
        c2 = *s2++;

        if (c1 == 0)
            break;
    } while (c1 == c2);

    return static_cast<unsigned char>(c1) - static_cast<unsigned char>(c2);
}

int _strlen_(const char *str)
{
    int len = 0;
    const char *s = str;

    if (str) {
        while(*str++ != 0);
        len = str - s - 1;
    }

    return len;
}

}

string::string(const char *src) : _buffer(NULL), _capacity(0), _length(0)
{
    if (src)
    {
        int len = _strlen_(src);

        resize_buffer(len + 1);
        _length = len;
        memmove(_buffer, src, len + 1);
    }
}

string::string(const string& str) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(str._length + 1);
    memmove(_buffer, str._buffer, str._length + 1);
    _length = str._length;
}

string::string(const string& str, int pos, int len) : _buffer(NULL), _capacity(0), _length(0)
{
    if (len == npos || (pos + len) > str._length)
        len = str._length - pos;

    resize_buffer(len + 1);
    memmove(_buffer, str._buffer + pos, len);
    _buffer[len] = 0;
    _length = len;
}

string::string(const char *src, int n) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(n + 1);
    memmove(_buffer, src, n);
    _buffer[n] = 0;
    _length = n;
}

string::string(int n, char c) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(n + 1);
    memset(_buffer, c, n);
    _buffer[n] = 0;
    _length = n;
}
string::string(string&& str)
{
    _buffer = str._buffer;
    _capacity = str._capacity;
    _length = str._length;

    str._buffer = NULL;
    str._capacity = 0;
    str._length = 0;
}

string& string::operator= (const string& str)
{
    resize_buffer(str._length + 1);
    memmove(_buffer, str._buffer, str._length + 1);
    _length = str._length;

    return *this;
}

string& string::operator= (const char* str)
{
    if (str)
    {
        int len = _strlen_(str);
        resize_buffer(len + 1);
        memmove(_buffer, str, len + 1);
        _length = len;
    }
    else
        resize_buffer(0);

    return *this;
}

string& string::operator= (char c)
{
    resize_buffer(2);
    _length = 1;
    _buffer[0] = c;
    _buffer[1] = 0;

    return *this;
}

string& string::operator= (string&& str)
{
    if (this != &str)
    {
        resize_buffer(0);

        _buffer = str._buffer;
        _capacity = str._capacity;
        _length = str._length;

        str._buffer = NULL;
        str._capacity = 0;
        str._length = 0;
    }
    return *this;
}

// resize buffer so that it contains at least the terminating character
void string::resize_buffer(int size)
{
    if (size > 0)
    {
        if (_buffer != NULL)
        {
            // round up the size
            size = (size + 15) & ~15;

            // if size is larger than string length, allow to realloc
            if (size > (_capacity))
            {
                char * new_buff = allocator<char>().allocate(size);
                memcpy(new_buff, _buffer, _capacity);
                allocator<char>().deallocate(_buffer, _capacity);
                _buffer = new_buff;
                _capacity = size;
            }
        }
        else
        {
            // Buffer was not allocated, get it now
            // round up size
            size = (size + 15) & ~15;
            _buffer = allocator<char>().allocate(size);

            if (_buffer)
            {
                _capacity = size;
                _length = 0;
            }
        }
    }
    else
    {
        _length = 0;
        if (_buffer != NULL)
        {
            allocator<char>().deallocate(_buffer, _capacity);
            _buffer = NULL;
            _capacity = 0;
        }
    }
}

string::~string()
{
    resize_buffer(0);
}

void string::resize(int n, char c)
{
    if (n < _length)
    {
        _buffer[n] = 0;
        _length = n;
    }
    else
    {
        if (n >= _capacity)
            resize_buffer(n + 1);

        memset(_buffer + _length, c, n - _length);
        _length = n;
    }
}

void string::clear()
{
    if (_buffer && _length > 0)
    {
        memset(_buffer, 0, _length);
        _length = 0;
    }
}

string& string::operator+= (const string& str)
{
    int len = str._length + 1;

    if (_capacity - _length < len)
        resize_buffer(_length + len);

    memmove(_buffer + _length, str._buffer, len);
    _length += len - 1;

    return *this;
}

string& string::operator+= (const char *str)
{
    int len = _strlen_(str) + 1;

    if (_capacity - _length < len)
        resize_buffer(_length + len);

    memmove(_buffer + _length, str, len);
    _length += len - 1;

    return *this;
}

string& string::operator+= (char c)
{
    if (_capacity - _length < 2)
        resize_buffer(_length + 2);

    _buffer[_length++] = c;
    _buffer[_length] = 0;

    return *this;
}

string& string::append(const string &str, int subpos, int sublen)
{
    if (subpos < str._length)
    {
        int len = sublen;

        if (len > str._length - subpos)
            len = str._length - subpos;

        if (_capacity <= (_length + len))
            resize_buffer(_length + len + 1);

        memmove(_buffer + _length, str._buffer + subpos, len + 1);
        _length += len;
    }
    return *this;
}

string& string::append(const char *s, int n)
{
    if (s != NULL)
    {
        int len = _strlen_(s);

        if (len > n)
            len = n;

        if (_length + len >= _capacity)
            resize_buffer(_length + len + 1);

        memmove(_buffer + _length, s, len);
        _length += len;
    }
    return *this;
}

string& string::append(int n, char c)
{
    if (n > 0)
    {
        if (_length + n >= _capacity)
            resize_buffer(_length + n + 1);

        memset(_buffer + _length, c, n);

        _buffer[_length + n] = 0;
        _length += n;
    }

    return *this;
}

string& string::assign(const string& str)
{
    resize_buffer(str._length + 1);
    memmove(_buffer, str._buffer, str._length + 1);
    _length = str._length;

    return *this;
}

string& string::assign(const string& str, int subpos, int sublen)
{
    if (subpos < str._length)   // else out of range exception!
    {
        if (sublen + subpos > str._length)
            sublen = str._length - subpos;

        resize_buffer(sublen + 1);
        memmove(_buffer, str._buffer + subpos, sublen);
        _buffer[sublen] = 0;
        _length = sublen;
    }
    return *this;
}

string& string::assign(const char *s, int n)
{
    if (s)
    {
        int len = _strlen_(s);

        if (len > n)
            len = n;

        resize_buffer(len + 1);
        memmove(_buffer, s, len);
        _buffer[len] = 0;
        _length = len;
    }

    return *this;
}

string& string::assign(int n, char c)
{
    resize_buffer(n + 1);
    memset(_buffer, c, n);
    _buffer[n] = 0;
    _length = n;

    return *this;
}

string& string::assign(string&& str)
{
    resize_buffer(0);

    _buffer = str._buffer;
    _capacity = str._capacity;
    _length = str._length;

    str._buffer = NULL;
    str._capacity = 0;
    str._length = 0;

    return *this;
}

string& string::insert(int pos, const string& str)
{
    if (pos > _length)
        pos = _length;

    if (_length + str._length + 1 > _capacity)
        resize_buffer(_length + str._length + 1);

    memmove(_buffer + pos + str._length, _buffer + pos, _length - pos + 1);
    memmove(_buffer + pos, str._buffer, str._length);

    _length += str._length;

    return *this;
}

string& string::insert(int pos, const string& str, int subpos, int sublen)
{
    if (subpos < str._length)
    {
        if (sublen > subpos + str._length)
            sublen = str._length - subpos;

        if (pos > _length)
            pos = _length;

        if (_length + sublen + 1 > _capacity)
            resize_buffer(_length + sublen + 1);

        memmove(_buffer + pos + sublen, _buffer + pos, _length - pos + 1);
        memmove(_buffer + pos, str._buffer + subpos, sublen);

        _length += sublen;
    }

    return *this;
}

string& string::insert(int pos, const char* s)
{
    if (s)
    {
        if (pos > _length)
            pos = _length;

        int len = _strlen_(s);

        if (_length + len + 1 > _capacity)
            resize_buffer(_length + len + 1);

        memmove(_buffer + pos + len, _buffer + pos, _length - pos + 1);
        memmove(_buffer + pos, s, len);

        _length += len;
    }

    return *this;
}

string& string::insert(int pos, const char* s, int n)
{
    if (s)
    {
        if (pos > _length)
            pos = _length;

        int len = _strlen_(s);

        if (len > n)
            len = n;

        if (_length + len + 1 > _capacity)
            resize_buffer(_length + len + 1);

        memmove(_buffer + pos + len, _buffer + pos, _length - pos + 1);
        memmove(_buffer + pos, s, len);

        _length += len;
    }

    return *this;
}

string& string::insert(int pos, int n, char c)
{
    if (n)
    {
        if (pos > _length)
            pos = _length;

        int len = n;

        if (_length + len + 1 > _capacity)
            resize_buffer(_length + len + 1);

        memmove(_buffer + pos + len, _buffer + pos, _length - pos + 1);
        memset(_buffer + pos, c, len);

        _length += len;
    }

    return *this;
}

string::iterator string::insert(string::const_iterator p, char c)
{
    return insert(p, 1, c);
}

string::iterator string::insert(string::const_iterator p, int n, char c)
{
    int pos = p - const_iterator(_buffer);

    if (n)
    {

        if (pos > _length)
            pos = _length;

        int len = n;

        if (_length + len + 1 > _capacity)
            resize_buffer(_length + len + 1);

        memmove(_buffer + pos + len, _buffer + pos, _length - pos + 1);
        memset(_buffer + pos, c, len);

        _length += len;
    }

    return iterator(_buffer + pos);
}

string& string::erase(int pos, int len)
{
    if (pos < _length)
    {
        if (len == npos || pos + len > _length)
            len = _length - pos;

        memmove(_buffer + pos, _buffer + pos + len, _length - pos - len);
        _length-=len;
        _buffer[_length] = 0;
    }

    return *this;
}

string::iterator string::erase(string::const_iterator p)
{
    int pos = p - const_iterator(_buffer);

    memmove(_buffer + pos, _buffer + pos + 1, _length - pos + 1);

    --_length;

    return iterator(_buffer + pos);
}

string::iterator string::erase(string::const_iterator first, string::const_iterator last)
{
    int pos = first - const_iterator(_buffer);
    int len = last - first;

    erase(pos, len);

    return iterator(_buffer + pos);
}

int string::copy(char *c, int len, int pos)
{
    if (pos < _length)
    {
        if (pos + len > _length)
            len = _length - pos;

        memmove(c, _buffer + pos, len);

        return len;
    }
    return 0;
}

string operator+(const string &lhs, const string &rhs)
{
    string result;

    result.resize_buffer(lhs._length + rhs._length + 1);
    memmove(result._buffer, lhs._buffer, lhs._length);
    memmove(result._buffer + lhs._length, rhs._buffer, rhs._length + 1);
    result._length = lhs._length + rhs._length;

    return result;
}

string operator+(const string &lhs, const char *rhs)
{
    if (rhs != NULL)
    {
        string result;

        int len = _strlen_(rhs);
        result.resize_buffer(lhs._length + len + 1);
        memmove(result._buffer, lhs._buffer, lhs._length);
        memmove(result._buffer + lhs._length, rhs, len + 1);
        result._length = lhs._length + len;

        return result;
    }
    else
        return lhs;
}

string operator+(const char *lhs, const string &rhs)
{
    if (lhs != NULL)
    {
        string result;

        int len = _strlen_(lhs);
        result.resize_buffer(len + rhs._length + 1);
        memmove(result._buffer, lhs, len);
        memmove(result._buffer + len, rhs._buffer, rhs._length + 1);
        result._length = len + rhs._length;

        return result;
    }
    else
        return rhs;
}

string operator+(const string &lhs, char rhs)
{
    string result;

    result.resize_buffer(lhs._length + 2);
    memmove(result._buffer, lhs._buffer, lhs._length);
    result._buffer[lhs._length] = rhs;
    result._buffer[lhs._length + 1] = 0;
    result._length = lhs._length + 1;

    return result;
}

string operator+(char lhs, const string &rhs)
{
    string result;

    result.resize_buffer(rhs._length + 2);
    result._buffer[0] = lhs;
    memmove(result._buffer + 1, rhs._buffer, rhs._length + 1);
    result._length = rhs._length + 1;

    return result;
}

bool operator==(const string &lhs, const string &rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) == 0);
}

bool operator== (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) == 0);
}

bool operator== (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) == 0);
}

bool operator!= (const string& lhs, const string& rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) != 0);
}

bool operator!= (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) != 0);
}

bool operator!= (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) != 0);
}

bool operator< (const string& lhs, const string& rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) < 0);
}

bool operator< (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) < 0);
}

bool operator< (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) < 0);
}

bool operator<= (const string& lhs, const string& rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) <= 0);
}

bool operator<= (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) <= 0);
}

bool operator<= (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) <= 0);
}

bool operator> (const string& lhs, const string& rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) > 0);
}

bool operator> (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) > 0);
}

bool operator> (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) > 0);
}

bool operator>= (const string& lhs, const string& rhs)
{
    return (_strcmp_(lhs.c_str(), rhs.c_str()) >= 0);
}

bool operator>= (const char *  lhs, const string& rhs)
{
    return (_strcmp_(lhs, rhs.c_str()) >= 0);
}

bool operator>= (const string& lhs, const char *  rhs)
{
    return (_strcmp_(lhs.c_str(), rhs) >= 0);
}

void string::swap(string& str)
{
    char *_buff = _buffer;
    int _cap = _capacity;
    int _len = _length;

    _buffer = str._buffer;
    _capacity = str._capacity;
    _length = str._length;

    str._buffer = _buff;
    str._capacity = _cap;
    str._length = _len;
}

string to_string(int val)
{
    string str(25, ' ');
    bool minus = (val < 0) ? true : false;
    string::reverse_iterator it;

    if (minus)
        val = -val;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    if (minus) {
        ++it;
        *it = '-';
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

string to_string(long val)
{
    string str(25, ' ');
    bool minus = (val < 0) ? true : false;
    string::reverse_iterator it;

    if (minus)
        val = -val;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    if (minus) {
        ++it;
        *it = '-';
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

string to_string(long long val)
{
    string str(25, ' ');
    bool minus = (val < 0) ? true : false;
    string::reverse_iterator it;

    if (minus)
        val = -val;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    if (minus) {
        ++it;
        *it = '-';
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

string to_string(unsigned long long val)
{
    string str(25, ' ');
    string::reverse_iterator it;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

string to_string(unsigned long val)
{
    string str(25, ' ');
    string::reverse_iterator it;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

string to_string(unsigned val)
{
    string str(25, ' ');
    string::reverse_iterator it;

    for (it = str.rbegin(); it != str.rend(); ++it)
    {
        *it = '0' + val % 10;
        val /= 10;
        if (val == 0)
            break;
    }

    ++it;
    int cnt = str.rend() - it;

    if (cnt > 0)
        str.erase(0, cnt);

    return str;
}

}
