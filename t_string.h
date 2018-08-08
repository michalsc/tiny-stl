#ifndef _T_STRING_H
#define _T_STRING_H

#include <ostream>
#include <iterator>

namespace t_std {

class string {
public:
    // Random access iterator with tiny bit of safety - it cannot go beyond 
    // given boundaries.
    class iterator : public std::iterator<std::random_access_iterator_tag, char> {
        char *p;
    public:
        iterator() : p(nullptr) {};
        iterator(char *ptr) : p(ptr) {};
        iterator(const iterator& it) : p(it.p) {};
        iterator& operator+=(difference_type rhs) { p += rhs; return *this; }
        iterator& operator-=(difference_type rhs) { p -= rhs; return *this; }
        char& operator*() const { return *p; }
        char* operator->() const { return p; }
        char& operator[](difference_type rhs) const { return p[rhs]; }

        iterator& operator++() { ++p; return *this; }
        iterator& operator--() { --p; return *this; }
        iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
        iterator operator--(int) { iterator tmp(*this); operator--(); return tmp; }
        difference_type operator-(const iterator& rhs) const { return (p - rhs.p); }
        iterator operator+(difference_type rhs) const { return iterator(p + rhs); }
        iterator operator-(difference_type rhs) const { return iterator(p - rhs); }
        friend inline iterator operator+(difference_type lhs, const iterator &rhs) { return iterator(lhs + rhs.p); }
        
        bool operator==(const iterator& rhs) const { return p == rhs.p; }
        bool operator!=(const iterator& rhs) const { return p != rhs.p; }
        bool operator>(const iterator& rhs) const { return p > rhs.p; }
        bool operator>=(const iterator &rhs) const { return p >= rhs.p; }
        bool operator<(const iterator &rhs) const { return p < rhs.p; }
        bool operator<=(const iterator &rhs) const { return p <= rhs.p; }
    };
    
    // Random access reverse iterator with tiny bit of safety - it cannot go beyond 
    // given boundaries.
    class reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, char> {
        char *p;
    public:
        reverse_iterator() : p(nullptr) {};
        reverse_iterator(char *ptr) : p(ptr) {};
        reverse_iterator(const reverse_iterator& it) : p(it.p) {};
        iterator& operator+=(difference_type rhs) { p -= rhs; return *this; }
        iterator& operator-=(difference_type rhs) { p += rhs; return *this; }
        char& operator*() const { return *p; }
        char* operator->() const { return p; }
        char& operator[](difference_type rhs) const { return p[-rhs]; }

        iterator& operator++() { --p; return *this; }
        iterator& operator--() { ++p; return *this; }
        iterator operator++(int) { reverse_iterator tmp(*this); operator++(); return tmp; }
        iterator operator--(int) { reverse_iterator tmp(*this); operator--(); return tmp; }
        difference_type operator-(const reverse_iterator &rhs) const { return (rhs.p - p); }
        iterator operator+(difference_type rhs) const { return reverse_iterator(p - rhs); }
        iterator operator-(difference_type rhs) const { return reverse_iterator(p + rhs); }
        friend inline iterator operator+(difference_type lhs, const reverse_iterator &rhs) { return reverse_iterator(rhs.p - lhs); }

        bool operator==(const reverse_iterator& rhs) const { return p==rhs.p; }
        bool operator!=(const reverse_iterator& rhs) const { return p!=rhs.p; }
        bool operator>(const reverse_iterator &rhs) const { return p < rhs.p; }
        bool operator>=(const reverse_iterator &rhs) const { return p <= rhs.p; }
        bool operator<(const reverse_iterator &rhs) const { return p > rhs.p; }
        bool operator<=(const reverse_iterator &rhs) const { return p >= rhs.p; }
    };

    // Constructors
    string(const char *src = "");
    string(const string& str);
    string(const string& str, int pos, int len = npos);
    string(const char *src, int n);
    string(int n, char c);
    string(iterator first, iterator last);
    string(reverse_iterator first, reverse_iterator last);
    string(string&& str);
    ~string();

    // Assignment operators
    string& operator= (const string& str);
    string& operator= (const char* str);
    string& operator= (char c);
    string& operator= (string&& str);

    // Iterators
    iterator begin() { return iterator(_buffer); }
    iterator end() { return iterator(_buffer + _length); }
    reverse_iterator rbegin() { return reverse_iterator(_buffer + _length - 1); }
    reverse_iterator rend() { return reverse_iterator(_buffer - 1); }

    // Capacity
    int size() { return _length; }
    int length() { return _length; }
    int max_size() { return 0x7fffffff; }
    void resize(int n, char c=0);
    int capacity() { return _capacity; }
    void reserve(int n = 0) { if (n > (_length + 1)) resize_buffer(n + 1); }
    void clear();
    bool empty() const { return (_length == 0); }

    // Element access
    char& operator[] (int pos) { if (pos >= 0 && pos < _length) return _buffer[pos]; else return (char&)_null; }
    char& at(int pos) { if (pos >= 0 && pos < _length) return _buffer[pos]; else return (char&)_null; }
    
    // Modifiers
    string& operator+= (const string& str);
    string& operator+= (const char* s);
    string& operator+= (char c);
    string& append(const string& str) { return (*this += str); }
    string& append(const string& str, int subpos, int sublen);
    string& append(const char* s) { return (*this += s); }
    string& append(const char* s, int n);
    string& append(int n, char c);
    void push_back(char c) { *this += c; }
    string& assign(const string& str);
    string& assign(const string& str, int subpos, int sublen);
    string& assign(const char *s) { return (*this = s); }
    string& assign(const char *s, int n);
    string& assign(int n, char c);
    string& assign(string&& str);
    string& insert(int pos, const string& str);
    string& insert(int pos, const string& str, int subpos, int sublen);
    string& insert(int pos, const char* s);
    string& insert(int pos, const char* s, int n);
    //erase
    //replace
    void swap(string& str);

    // String operations
    const char * c_str() const { if (_length) return _buffer; else return &_null; }
    const char * data() const { if (_length) return _buffer; else return &_null; }
    
    // Member constants
    static const int npos = -1;

private:
    static const char _null;
    char *_buffer;
    int _capacity;
    int _length;
    void resize_buffer(int size);

    friend string operator+(const string &lhs, const string &rhs);
    friend string operator+(const string &lhs, const char *rhs);
    friend string operator+(const char *lhs, const string &rhs);
    friend string operator+(const string &lhs, char rhs);
    friend string operator+(char lhs, const string &rhs);
};

string operator+ (const string& lhs, const string& rhs);
string operator+ (const string& lhs, const char *  rhs);
string operator+ (const char *  lhs, const string& rhs);
string operator+ (const string& lhs, char          rhs);
string operator+ (char          lhs, const string& rhs);

bool operator== (const string& lhs, const string& rhs);
bool operator== (const char*   lhs, const string& rhs);
bool operator== (const string& lhs, const char*   rhs);
bool operator!= (const string& lhs, const string& rhs);
bool operator!= (const char*   lhs, const string& rhs);
bool operator!= (const string& lhs, const char*   rhs);
bool operator<  (const string& lhs, const string& rhs);
bool operator<  (const char*   lhs, const string& rhs);
bool operator<  (const string& lhs, const char*   rhs);
bool operator<= (const string& lhs, const string& rhs);
bool operator<= (const char*   lhs, const string& rhs);
bool operator<= (const string& lhs, const char*   rhs);
bool operator>  (const string& lhs, const string& rhs);
bool operator>  (const char*   lhs, const string& rhs);
bool operator>  (const string& lhs, const char*   rhs);
bool operator>= (const string& lhs, const string& rhs);
bool operator>= (const char*   lhs, const string& rhs);
bool operator>= (const string& lhs, const char*   rhs);

void swap(string& s1, string& s2);

// To be removed when used as standalone
std::ostream &operator<<(std::ostream &os, string const &str);

}

#endif // _T_STRING_H
