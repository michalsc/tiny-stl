#include "t_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace t_std;

#define MEMF_ANY    1
#define MEMF_CLEAR  2

void * AllocMem(int size, int type)
{
    void *ptr = malloc(size);
    if (type & MEMF_CLEAR)
    {
        bzero(ptr, size);
    }
    return ptr;
}

void FreeMem(void *ptr, int size)
{
    (void)size;
    free(ptr);
}

void CopyMem(const void *src, void *dst, int size)
{
    memcpy(dst, src, size);
}

const char string::_null = 0;

string::string(const char *src) : _buffer(NULL), _capacity(0), _length(0)
{
    printf("string::string(const char *src='%s')\n", src);

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
    printf("string::string(const string& str='%s'\n", str._buffer);
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
    resize_buffer(str._capacity);
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
            if (size > (_length + 1))
            {
                void * new_buff = AllocMem(size, MEMF_CLEAR);
                CopyMem(_buffer, new_buff, _capacity);
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
        resize_buffer(_capacity + len);
    
    CopyMem(str._buffer, _buffer + _length, len);
    _length += len - 1;

    return *this;
}

string& string::operator+= (const char *str)
{
    int len = strlen(str) + 1;

    if (_capacity - _length < len)
        resize_buffer(_capacity + len);
    
    CopyMem(str, _buffer + _length, len);
    _length += len - 1;

    return *this;
}

string& string::operator+= (char c)
{
    if (_capacity - _length < 2)
        resize_buffer(_capacity + 2);
    
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
        _length = _length + len;
    }
    return *this;
}

bool operator== (const string& lhs, const string& rhs)
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