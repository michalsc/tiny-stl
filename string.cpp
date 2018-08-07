#include "t_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ostream>

namespace t_std {

#define MEMF_ANY    1
#define MEMF_CLEAR  2

void * AllocMem(int size, int type)
{
    int *ptr = reinterpret_cast<int *>(malloc(size + 32));
    ptr[0] = size;
    ptr[1] = 0xdeadbeef;
    ptr[2] = 0xdeadbeef;
    ptr[3] = 0xdeadbeef;

    ptr[4 + size/4] = 0xcafebabe;
    ptr[5 + size/4] = 0xcafebabe;
    ptr[6 + size/4] = 0xcafebabe;
    ptr[7 + size/4] = 0xcafebabe;

    if (type & MEMF_CLEAR)
    {
        bzero(&ptr[4], size);
    }

    return &ptr[4];
}

void FreeMem(void *ptr, int size)
{
    unsigned int *p = reinterpret_cast<unsigned int *>(ptr);

    p -= 4;
    if (*p != (unsigned int)size)
        printf("Size mismatch at FreeMem!! %d != %d\n", *p, size);

    if (p[1] != 0xdeadbeef || p[2] != 0xdeadbeef || p[3] != 0xdeadbeef)
    {
        printf("FreeMem(): left wall damaged %08x%08x%08x\n", p[1], p[2], p[3]);
    }
    if (p[4 + size/4] != 0xcafebabe || p[5+size/4] != 0xcafebabe || p[6+size/4] != 0xcafebabe || p[7+size/4] != 0xcafebabe)
    {
        printf("FreeMem(): right wall damaged %08x%08x%08x%08x\n", p[4 + size/4], p[5+size/4], p[6+size/4],p[7+size/4]);
    }
    
    free(p);
}

void CopyMem(const void *src, void *dst, int size)
{
    memcpy(dst, src, size);
}

const char string::_null = 0;

string::string(const char *src) : _buffer(NULL), _capacity(0), _length(0)
{
    if (src)
    {
        int len = strlen(src);

        resize_buffer(len + 1);
        _length = len;
        CopyMem(src, _buffer, len + 1);
    }
}

string::string(const string& str) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(str._length + 1);
    CopyMem(str._buffer, _buffer, str._length + 1);
    _length = str._length;
}

string::string(const string& str, int pos, int len) : _buffer(NULL), _capacity(0), _length(0)
{
    if (len == npos || (pos + len) > str._length)
        len = str._length - pos;

    resize_buffer(len + 1);
    CopyMem(str._buffer + pos, _buffer, len);
    _buffer[len] = 0;
    _length = len;
}

string::string(const char *src, int n) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(n + 1);
    CopyMem(src, _buffer, n);
    _buffer[n] = 0;
    _length = n;
}

string::string(int n, char c) : _buffer(NULL), _capacity(0), _length(0)
{
    resize_buffer(n + 1);
    for (int i=0; i < n; i++)
        _buffer[i] = c;
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
    CopyMem(str._buffer, _buffer, str._length + 1);
    _length = str._length;

    return *this;
}

string& string::operator= (const char* str)
{
    if (str)
    {
        int len = strlen(str);
        resize_buffer(len + 1);
        CopyMem(str, _buffer, len + 1);
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
                void * new_buff = AllocMem(size, MEMF_CLEAR);
                CopyMem(_buffer, new_buff, _capacity);
                FreeMem(_buffer, _capacity);
                _buffer = static_cast<char *>(new_buff);
                _capacity = size;
            }
        }
        else
        {
            // Buffer was not allocated, get it now
            // round up size
            size = (size + 15) & ~15;
            _buffer = static_cast<char *>(AllocMem(size, MEMF_CLEAR));
            
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
            FreeMem(_buffer, _capacity);
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
    
    CopyMem(str._buffer, _buffer + _length, len);
    _length += len - 1;

    return *this;
}

string& string::operator+= (const char *str)
{
    int len = strlen(str) + 1;

    if (_capacity - _length < len)
        resize_buffer(_length + len);
    
    CopyMem(str, _buffer + _length, len);
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

        CopyMem(str._buffer + subpos, _buffer + _length, len + 1);
        _length += len;
    }
    return *this;
}

string& string::append(const char *s, int n)
{
    if (s != NULL)
    {
        int len = strlen(s);
        
        if (len > n)
            len = n;

        if (_length + len >= _capacity)
            resize_buffer(_length + len + 1);
        
        CopyMem(s, _buffer + _length, len);
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
        
        for (int i=0; i < n; i++)
            _buffer[_length + i] = c;
        
        _buffer[_length + n] = 0;
        _length += n;
    }

    return *this;
}

string& string::assign(const string& str)
{
    resize_buffer(str._length + 1);
    CopyMem(str._buffer, _buffer, str._length + 1);
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
        CopyMem(str._buffer + subpos, _buffer, sublen);
        _buffer[sublen] = 0;
        _length = sublen;
    }
    return *this;
}

string& string::assign(const char *s, int n)
{
    if (s)
    {
        int len = strlen(s);
        
        if (len > n)
            len = n;

        resize_buffer(len + 1);
        CopyMem(s, _buffer, len);
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

string operator+(const string &lhs, const string &rhs)
{
    string result;

    result.resize_buffer(lhs._length + rhs._length + 1);
    CopyMem(lhs._buffer, result._buffer, lhs._length);
    CopyMem(rhs._buffer, result._buffer + lhs._length, rhs._length + 1);
    result._length = lhs._length + rhs._length;

    return result;
}

string operator+(const string &lhs, const char *rhs)
{
    if (rhs != NULL)
    {
        string result;

        int len = strlen(rhs);
        result.resize_buffer(lhs._length + len + 1);
        CopyMem(lhs._buffer, result._buffer, lhs._length);
        CopyMem(rhs, result._buffer + lhs._length, len + 1);
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

        int len = strlen(lhs);
        result.resize_buffer(len + rhs._length + 1);
        CopyMem(lhs, result._buffer, len);
        CopyMem(rhs._buffer, result._buffer + len, rhs._length + 1);
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
    CopyMem(lhs._buffer, result._buffer, lhs._length);
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
    CopyMem(rhs._buffer, result._buffer + 1, rhs._length + 1);
    result._length = rhs._length + 1;

    return result;
}

bool operator==(const string &lhs, const string &rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) == 0);
}

bool operator== (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) == 0);
}

bool operator== (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) == 0);
}

bool operator!= (const string& lhs, const string& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) != 0);
}

bool operator!= (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) != 0);
}

bool operator!= (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) != 0);
}

bool operator< (const string& lhs, const string& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) < 0);
}

bool operator< (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) < 0);
}

bool operator< (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) < 0);
}

bool operator<= (const string& lhs, const string& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) <= 0);
}

bool operator<= (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) <= 0);
}

bool operator<= (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) <= 0);
}

bool operator> (const string& lhs, const string& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) > 0);
}

bool operator> (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) > 0);
}

bool operator> (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) > 0);
}

bool operator>= (const string& lhs, const string& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) >= 0);
}

bool operator>= (const char *  lhs, const string& rhs)
{
    return (strcmp(lhs, rhs.c_str()) >= 0);
}

bool operator>= (const string& lhs, const char *  rhs)
{
    return (strcmp(lhs.c_str(), rhs) >= 0);
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

std::ostream &operator<<(std::ostream &os, string const &str)
{
    os << "\"" << str.c_str() << "\"";

    return os;
}

}

